/*-
 * Copyright (c) 2006 Elad Efrat <elad@NetBSD.org>
 * Copyright (c) 2013-2014, by Oliver Pinter <oliver.pinter@hardenedbsd.org>
 * Copyright (c) 2014, by Shawn Webb <shawn.webb@hardenedbsd.org>
 * Copyright (c) 2014, by Danilo Egea Gondolfo <danilo at FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "opt_compat.h"
#include "opt_pax.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/imgact.h>
#include <sys/imgact_elf.h>
#include <sys/sysent.h>
#include <sys/stat.h>
#include <sys/proc.h>
#include <sys/elf_common.h>
#include <sys/mount.h>
#include <sys/pax.h>
#include <sys/sysctl.h>
#include <sys/vnode.h>
#include <sys/queue.h>
#include <sys/libkern.h>
#include <sys/jail.h>
#include <sys/priv.h>
#include <sys/fnv_hash.h>
#include <sys/vnode.h>

#include <sys/mman.h>
#include <sys/libkern.h>
#include <sys/exec.h>
#include <sys/kthread.h>

#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_extern.h>

#include <machine/elf.h>

#define PAX_SEGVGUARD_EXPIRY		(2 * 60)
#define PAX_SEGVGUARD_SUSPENSION	(10 * 60)
#define PAX_SEGVGUARD_MAXCRASHES	5


FEATURE(segvguard, "Segmentation fault protection.");

static int pax_segvguard_status = PAX_FEATURE_OPTIN;
static int pax_segvguard_expiry = PAX_SEGVGUARD_EXPIRY;
static int pax_segvguard_suspension = PAX_SEGVGUARD_SUSPENSION;
static int pax_segvguard_maxcrashes = PAX_SEGVGUARD_MAXCRASHES;


struct pax_segvguard_entry {
	uid_t se_uid;
	ino_t se_inode;
	char se_mntpoint[MNAMELEN];

	size_t se_ncrashes;
	sbintime_t se_expiry;
	sbintime_t se_suspended;
	LIST_ENTRY(pax_segvguard_entry) se_entry;
};

struct pax_segvguard_key {
	uid_t se_uid;
	ino_t se_inode;
	char se_mntpoint[MNAMELEN];
};

struct pax_segvguard_entryhead {
	struct pax_segvguard_entry *lh_first;
	struct mtx bucket_mtx;
};

static struct pax_segvguard_entryhead *pax_segvguard_hashtbl;
static int pax_segvguard_hashsize = 512;

#define PAX_SEGVGUARD_HASHVAL(x) \
	fnv_32_buf((&(x)), sizeof(x), FNV1_32_INIT)

#define PAX_SEGVGUARD_HASH(x) \
	(&pax_segvguard_hashtbl[PAX_SEGVGUARD_HASHVAL(x) % pax_segvguard_hashsize])

#define PAX_SEGVGUARD_KEY(x) \
	((struct pax_segvguard_key *) x)

#define PAX_SEGVGUARD_LOCK_INIT(bucket) \
	mtx_init(&(bucket)->bucket_mtx, "segvguard mutex", NULL, MTX_DEF)

#define PAX_SEGVGUARD_LOCK(bucket) \
	mtx_lock(&bucket->bucket_mtx)

#define PAX_SEGVGUARD_UNLOCK(bucket) \
	mtx_unlock(&bucket->bucket_mtx)


MALLOC_DECLARE(M_PAX);
MALLOC_DEFINE(M_PAX, "pax_segvguard", "PaX segvguard memory");

TUNABLE_INT("hardening.pax.segvguard.status", &pax_segvguard_status);
TUNABLE_INT("hardening.pax.segvguard.expiry_timeout", &pax_segvguard_expiry);
TUNABLE_INT("hardening.pax.segvguard.suspend_timeout", &pax_segvguard_suspension);
TUNABLE_INT("hardening.pax.segvguard.max_crashes", &pax_segvguard_maxcrashes);

#ifdef PAX_SYSCTLS
static int sysctl_pax_segvguard_status(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_segvguard_expiry(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_segvguard_suspension(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_segvguard_maxcrashes(SYSCTL_HANDLER_ARGS);

SYSCTL_DECL(_hardening_pax);
SYSCTL_NODE(_hardening_pax, OID_AUTO, segvguard, CTLFLAG_RD, 0, "PaX segvguard");

SYSCTL_PROC(_hardening_pax_segvguard, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_segvguard_status, "I",
    "Guard status. "
    "0 - disabled, "
    "1 - opt-in,  "
    "2 - opt-out, "
    "3 - force enabled");

SYSCTL_PROC(_hardening_pax_segvguard, OID_AUTO, expiry_timeout,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_segvguard_expiry, "I",
    "Entry expiry timeout (in seconds).");

SYSCTL_PROC(_hardening_pax_segvguard, OID_AUTO, suspend_timeout,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_segvguard_suspension, "I",
    "Entry suspension timeout (in seconds).");

SYSCTL_PROC(_hardening_pax_segvguard, OID_AUTO, max_crashes,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_segvguard_maxcrashes, "I",
    "Max number of crashes before expiry.");
#endif

#ifdef PAX_SYSCTLS
static int
sysctl_pax_segvguard_status(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;
	struct prison *pr;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_segvguard_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch (val) {
	case    PAX_FEATURE_DISABLED:
	case    PAX_FEATURE_OPTIN:
	case    PAX_FEATURE_OPTOUT:
	case    PAX_FEATURE_FORCE_ENABLED:
		if (pr == &prison0)
			pax_segvguard_status = val;
		pr->pr_hardening.hr_pax_segvguard_status = val;
		break;
	default:
		return (EINVAL);
	}

	return (0);
}

static int
sysctl_pax_segvguard_expiry(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;
	struct prison *pr;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_segvguard_expiry;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	if (pr == &prison0)
		pax_segvguard_expiry = val;

	pr->pr_hardening.hr_pax_segvguard_expiry = val;

	return (0);
}

static int
sysctl_pax_segvguard_suspension(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;
	struct prison *pr;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_segvguard_suspension;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	if (pr == &prison0)
		pax_segvguard_suspension = val;

	pr->pr_hardening.hr_pax_segvguard_suspension = val;

	return (0);
}

static int
sysctl_pax_segvguard_maxcrashes(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;
	struct prison *pr;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_segvguard_maxcrashes;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	if (pr == &prison0)
		pax_segvguard_maxcrashes = val;

	pr->pr_hardening.hr_pax_segvguard_maxcrashes = val;

	return (0);
}
#endif

void
pax_segvguard_init_prison(struct prison *pr)
{
	struct prison *pr_p;

	if (pr == &prison0) {
		/* prison0 has no parent, use globals */
		pr->pr_hardening.hr_pax_segvguard_status =
		    pax_segvguard_status;
		pr->pr_hardening.hr_pax_segvguard_expiry =
		    pax_segvguard_expiry;
		pr->pr_hardening.hr_pax_segvguard_suspension =
		    pax_segvguard_suspension;
		pr->pr_hardening.hr_pax_segvguard_maxcrashes =
		    pax_segvguard_maxcrashes;
	} else {
		KASSERT(pr->pr_parent != NULL,
		   ("%s: pr->pr_parent == NULL", __func__));
		pr_p = pr->pr_parent;

		pr->pr_hardening.hr_pax_segvguard_status =
		    pr_p->pr_hardening.hr_pax_segvguard_status;
		pr->pr_hardening.hr_pax_segvguard_expiry =
		    pr_p->pr_hardening.hr_pax_segvguard_expiry;
		pr->pr_hardening.hr_pax_segvguard_suspension =
		    pr_p->pr_hardening.hr_pax_segvguard_suspension;
		pr->pr_hardening.hr_pax_segvguard_maxcrashes =
		    pr_p->pr_hardening.hr_pax_segvguard_maxcrashes;
	}
}

uint32_t
pax_segvguard_setup_flags(struct image_params *imgp, uint32_t mode)
{
	struct prison *pr;
	uint32_t flags, status;

	flags = 0;
	status = 0;

	pr = pax_get_prison(imgp->proc);
	status = pr->pr_hardening.hr_pax_segvguard_status;

	if (status == PAX_FEATURE_DISABLED) {
		flags &= ~PAX_NOTE_SEGVGUARD;
		flags |= PAX_NOTE_NOSEGVGUARD;

		return (flags);
	}

	if (status == PAX_FEATURE_FORCE_ENABLED) {
		flags |= PAX_NOTE_SEGVGUARD;
		flags &= ~PAX_NOTE_NOSEGVGUARD;

		return (flags);
	}

	if (status == PAX_FEATURE_OPTIN) {
		if (mode & PAX_NOTE_SEGVGUARD) {
			flags |= PAX_NOTE_SEGVGUARD;
			flags &= ~PAX_NOTE_NOSEGVGUARD;
		} else {
			flags &= ~PAX_NOTE_SEGVGUARD;
			flags |= PAX_NOTE_NOSEGVGUARD;
		}

		return (flags);
	}

	if (status == PAX_FEATURE_OPTOUT) {
		if (mode & PAX_NOTE_NOSEGVGUARD) {
			flags &= ~PAX_NOTE_SEGVGUARD;
			flags |= PAX_NOTE_NOSEGVGUARD;
		} else {
			flags |= PAX_NOTE_SEGVGUARD;
			flags &= ~PAX_NOTE_NOSEGVGUARD;
		}

		return (flags);
	}

	/*
	 * unknown status, force segvguard
	 */
	flags |= PAX_NOTE_SEGVGUARD;
	flags &= ~PAX_NOTE_NOSEGVGUARD;

	return (flags);
}

int
pax_segvguard_update_flags_if_setuid(struct image_params *imgp, struct vnode *vn)
{
	int ret;
	struct prison *pr;
	u_int status;

	ret = 0;

	pr = pax_get_prison(imgp->proc);
	status = pr->pr_hardening.hr_pax_segvguard_status;

	if (status == PAX_FEATURE_OPTIN) {
		uint32_t flags;
		struct vattr vap;

		flags = imgp->proc->p_pax;

		/* lock? */
		ret = VOP_GETATTR(vn, &vap, imgp->proc->p_ucred);
		if (ret != 0) {
			flags |= PAX_NOTE_SEGVGUARD;
			flags &= ~PAX_NOTE_NOSEGVGUARD;
			/*
			 * XXXOP: alert the user:
			 *  pax_log_log
			 *  pax_log_ulog
			 */

			imgp->proc->p_pax = flags;

			return (ret);
		}

		CTR3(KTR_PAX, "%s: pid = %d p_pax = %x - before update",
		    __func__, imgp->proc->p_pid, flags);

		if ((vap.va_mode & (S_ISUID | S_ISGID)) != 0) {
			flags |= PAX_NOTE_SEGVGUARD;
			flags &= ~PAX_NOTE_NOSEGVGUARD;

			imgp->proc->p_pax = flags;

			CTR3(KTR_PAX, "%s: pid = %d p_pax = %x - after update",
			    __func__, imgp->proc->p_pid, flags);

			return (ret);
		}
	}

	return (ret);
}


static bool
pax_segvguard_active(struct proc *proc)
{
	uint32_t flags;

	if (proc == NULL)
		return (true);

	pax_get_flags(proc, &flags);

	CTR3(KTR_PAX, "%s: pid = %d p_pax = %x",
	    __func__, proc->p_pid, flags);

	if ((flags & PAX_NOTE_SEGVGUARD) == PAX_NOTE_SEGVGUARD)
		return (true);

	if ((flags & PAX_NOTE_NOSEGVGUARD) == PAX_NOTE_NOSEGVGUARD)
		return (false);

	return (true);
}

static struct pax_segvguard_entry *
pax_segvguard_add(struct thread *td, struct vnode *vn, sbintime_t sbt)
{
	struct pax_segvguard_entry *v;
	struct pax_segvguard_key *key;
	struct prison *pr;
	struct stat sb;
	int error;

	error = vn_stat(vn, &sb, td->td_ucred, NOCRED, curthread);
	if (error != 0) {
		pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
		    "%s:%d stat error. Bailing.", __func__, __LINE__);

		return (NULL);
	}

	pr = pax_get_prison_td(td);

	v = malloc(sizeof(struct pax_segvguard_entry), M_PAX, M_NOWAIT);
	if (v == NULL)
		return (NULL);

	v->se_inode = sb.st_ino;
	strncpy(v->se_mntpoint, vn->v_mount->mnt_stat.f_mntonname, MNAMELEN);

	v->se_uid = td->td_ucred->cr_ruid;
	v->se_ncrashes = 1;
	v->se_expiry = sbt + pr->pr_hardening.hr_pax_segvguard_expiry * SBT_1S;
	v->se_suspended = 0;

	key = PAX_SEGVGUARD_KEY(v);
	PAX_SEGVGUARD_LOCK(PAX_SEGVGUARD_HASH(*key));
	LIST_INSERT_HEAD(PAX_SEGVGUARD_HASH(*key), v, se_entry);
	PAX_SEGVGUARD_UNLOCK(PAX_SEGVGUARD_HASH(*key));

	return (v);
}

static struct pax_segvguard_entry *
pax_segvguard_lookup(struct thread *td, struct vnode *vn)
{
	struct pax_segvguard_entry *v;
	struct pax_segvguard_key sk;
	struct stat sb;
	int error;

	error = vn_stat(vn, &sb, td->td_ucred, NOCRED, curthread);
	if (error != 0) {
		pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
		    "%s:%d stat error. Bailing.", __func__, __LINE__);

		return (NULL);
	}

	sk.se_inode = sb.st_ino;
	strncpy(sk.se_mntpoint, vn->v_mount->mnt_stat.f_mntonname, MNAMELEN);
	sk.se_uid = td->td_ucred->cr_ruid;

	PAX_SEGVGUARD_LOCK(PAX_SEGVGUARD_HASH(sk));
	LIST_FOREACH(v, PAX_SEGVGUARD_HASH(sk), se_entry) {
		if (v->se_inode == sb.st_ino &&
		    !strncmp(sk.se_mntpoint, v->se_mntpoint, MNAMELEN) &&
		    td->td_ucred->cr_ruid == v->se_uid) {
			PAX_SEGVGUARD_UNLOCK(PAX_SEGVGUARD_HASH(sk));

			return (v);
		}
	}
	PAX_SEGVGUARD_UNLOCK(PAX_SEGVGUARD_HASH(sk));

	return (NULL);
}

void
pax_segvguard_remove(struct thread *td, struct vnode *vn)
{
	struct pax_segvguard_entry *v;
	struct pax_segvguard_key *key;

	v = pax_segvguard_lookup(td, vn);

	if (v != NULL) {
		key = PAX_SEGVGUARD_KEY(v);
		PAX_SEGVGUARD_LOCK(PAX_SEGVGUARD_HASH(*key));
		LIST_REMOVE(v, se_entry);
		PAX_SEGVGUARD_UNLOCK(PAX_SEGVGUARD_HASH(*key));
		free(v, M_PAX);
	}

}

int
pax_segvguard_segfault(struct thread *td, const char *name)
{
	struct pax_segvguard_entry *se;
	struct prison *pr;
	struct vnode *v;
	sbintime_t sbt;

	v = td->td_proc->p_textvp;
	if (v == NULL)
		return (EFAULT);

	pr = pax_get_prison_td(td);

	if (pax_segvguard_active(td->td_proc) == false)
		return (0);

	sbt = sbinuptime();

	se = pax_segvguard_lookup(td, v);

	/*
	 * If a program we don't know about crashed, we need to create a new entry for it
	 */
	if (se == NULL) {
		pax_segvguard_add(td, v, sbt);
	} else {
		if (se->se_expiry < sbt && se->se_suspended <= sbt) {
			pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
			    "[%s (%d)] Suspension expired.", name, td->td_proc->p_pid);
			se->se_ncrashes = 1;
			se->se_expiry = sbt + pr->pr_hardening.hr_pax_segvguard_expiry * SBT_1S;
			se->se_suspended = 0;

			return (0);
		}

		se->se_ncrashes++;

		if (se->se_ncrashes >= pax_segvguard_maxcrashes) {
			pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
			    "[%s (%d)] Suspending execution for %d seconds after %zu crashes.",
			    name, td->td_proc->p_pid,
			    pax_segvguard_suspension, se->se_ncrashes);
			se->se_suspended = sbt + pr->pr_hardening.hr_pax_segvguard_suspension * SBT_1S;
			se->se_ncrashes = 0;
			se->se_expiry = 0;
		}
	}

	return (0);
}

int
pax_segvguard_check(struct thread *td, struct vnode *v, const char *name)
{
	struct pax_segvguard_entry *se;
	sbintime_t sbt;

	if (v == NULL)
		return (EFAULT);

	if (pax_segvguard_active(td->td_proc) == false)
		return (0);

	sbt = sbinuptime();

	se = pax_segvguard_lookup(td, v);

	if (se != NULL) {
		if (se->se_expiry < sbt && se->se_suspended <= sbt) {
			pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
			    "[%s (%d)] Suspension expired.",
			    name, td->td_proc->p_pid);
			pax_segvguard_remove(td, v);

			return (0);
		}

		if (se->se_suspended > sbt) {
			pax_log_segvguard(td->td_proc, PAX_LOG_DEFAULT,
			    "[%s (%d)] Preventing execution due to repeated segfaults.",
			    name, td->td_proc->p_pid);

			return (EPERM);
		}
	}

	return (0);
}

static void
pax_segvguard_sysinit(void)
{
	int i;

	switch (pax_segvguard_status) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		break;
	default:
		printf("[PAX SEGVGUARD] WARNING, invalid PAX settings in loader.conf!"
		    " (pax_segvguard_status = %d)\n", pax_segvguard_status);
		pax_segvguard_status = PAX_FEATURE_FORCE_ENABLED;
		break;
	}
	printf("[PAX SEGVGUARD] status: %s\n", pax_status_str[pax_segvguard_status]);
	printf("[PAX SEGVGUARD] expiry: %d sec\n", pax_segvguard_expiry);
	printf("[PAX SEGVGUARD] suspension: %d sec\n", pax_segvguard_suspension);
	printf("[PAX SEGVGUARD] maxcrahes: %d\n", pax_segvguard_maxcrashes);

	pax_segvguard_hashtbl =
		malloc(pax_segvguard_hashsize * sizeof(struct pax_segvguard_entryhead),
		M_PAX, M_WAITOK | M_ZERO);

	for(i = 0; i < pax_segvguard_hashsize; i++)
		PAX_SEGVGUARD_LOCK_INIT(&pax_segvguard_hashtbl[i]);
}

SYSINIT(pax_segvguard_init, SI_SUB_PAX, SI_ORDER_ANY, pax_segvguard_sysinit, NULL);
