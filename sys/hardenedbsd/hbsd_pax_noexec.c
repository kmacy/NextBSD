/*-
 * Copyright (c) 2006 Elad Efrat <elad@NetBSD.org>
 * Copyright (c) 2013-2014, by Oliver Pinter <oliver.pinter@hardenedbsd.org>
 * Copyright (c) 2014, by Shawn Webb <lattera at gmail.com>
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

#include "opt_pax.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/imgact.h>
#include <sys/stat.h>
#include <sys/proc.h>
#include <sys/pax.h>
#include <sys/sysctl.h>
#include <sys/queue.h>
#include <sys/libkern.h>
#include <sys/jail.h>

#include <sys/mman.h>
#include <sys/libkern.h>
#include <sys/exec.h>
#include <sys/kthread.h>

#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_param.h>
#include <vm/vm_extern.h>

FEATURE(pax_pageexec, "PAX PAGEEXEC hardening");
FEATURE(pax_mprotect, "PAX MPROTECT hardening");

#ifdef PAX_HARDENING
static int pax_pageexec_status = PAX_FEATURE_OPTOUT;
static int pax_mprotect_status = PAX_FEATURE_OPTOUT;
#else /* !PAX_HARDENING */
static int pax_pageexec_status = PAX_FEATURE_OPTIN;
static int pax_mprotect_status = PAX_FEATURE_OPTIN;
#endif /* PAX_HARDENING */

TUNABLE_INT("hardening.pax.pageexec.status", &pax_pageexec_status);
TUNABLE_INT("hardening.pax.mprotect.status", &pax_mprotect_status);

#ifdef PAX_SYSCTLS
SYSCTL_DECL(_hardening_pax);

/*
 * sysctls
 */
static int sysctl_pax_pageexec_status(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_mprotect_status(SYSCTL_HANDLER_ARGS);

SYSCTL_NODE(_hardening_pax, OID_AUTO, pageexec, CTLFLAG_RD, 0,
    "Remove WX pages from user-space.");

SYSCTL_PROC(_hardening_pax_pageexec, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_pageexec_status, "I",
    "Restrictions status. "
    "0 - disabled, "
    "1 - opt-in,  "
    "2 - opt-out, "
    "3 - force enabled");

static int
sysctl_pax_pageexec_status(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = pr->pr_hardening.hr_pax_pageexec_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch (val) {
	case PAX_FEATURE_DISABLED:
		printf("PAX MPROTECT depend on PAGEEXEC!\n");
		if (pr == &prison0)
			pax_mprotect_status = val;

		pr->pr_hardening.hr_pax_mprotect_status = val;
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		if (pr == &prison0)
			pax_pageexec_status = val;

		pr->pr_hardening.hr_pax_pageexec_status = val;
		break;
	default:
		return (EINVAL);
	}

	return (0);
}

SYSCTL_NODE(_hardening_pax, OID_AUTO, mprotect, CTLFLAG_RD, 0,
    "MPROTECT hardening - enforce W^X.");

SYSCTL_PROC(_hardening_pax_mprotect, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_mprotect_status, "I",
    "Restrictions status. "
    "0 - disabled, "
    "1 - opt-in,  "
    "2 - opt-out, "
    "3 - force enabled");

static int
sysctl_pax_mprotect_status(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = pr->pr_hardening.hr_pax_mprotect_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch (val) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		if (pr == &prison0)
			pax_mprotect_status = val;

		pr->pr_hardening.hr_pax_mprotect_status = val;
		break;
	default:
		return (EINVAL);
	}

	return (0);
}
#endif /* PAX_SYSCTLS */


/*
 * PaX PAGEEXEC functions
 */

static void
pax_noexec_sysinit(void)
{

	switch (pax_pageexec_status) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		break;
	default:
		printf("[PAX PAGEEXEC] WARNING, invalid PAX settings in loader.conf!"
		    " (hardening.pax.pageexec.status = %d)\n", pax_pageexec_status);
		pax_pageexec_status = PAX_FEATURE_FORCE_ENABLED;
		break;
	}
	printf("[PAX PAGEEXEC] status: %s\n", pax_status_str[pax_pageexec_status]);

	switch (pax_mprotect_status) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		break;
	default:
		printf("[PAX MPROTECT] WARNING, invalid PAX settings in loader.conf!"
		    " (hardening.pax.mprotect.status = %d)\n", pax_mprotect_status);
		pax_mprotect_status = PAX_FEATURE_FORCE_ENABLED;
		break;
	}
	printf("[PAX MPROTECT] status: %s\n", pax_status_str[pax_mprotect_status]);
}
SYSINIT(pax_noexec, SI_SUB_PAX, SI_ORDER_SECOND, pax_noexec_sysinit, NULL);

void
pax_noexec_init_prison(struct prison *pr)
{
	struct prison *pr_p;

	CTR2(KTR_PAX, "%s: Setting prison %s PaX variables\n",
	    __func__, pr->pr_name);

	if (pr == &prison0) {
		/* prison0 has no parent, use globals */
		pr->pr_hardening.hr_pax_pageexec_status = pax_pageexec_status;
		pr->pr_hardening.hr_pax_mprotect_status = pax_mprotect_status;
	} else {
		KASSERT(pr->pr_parent != NULL,
		   ("%s: pr->pr_parent == NULL", __func__));
		pr_p = pr->pr_parent;

		pr->pr_hardening.hr_pax_pageexec_status =
		    pr_p->pr_hardening.hr_pax_pageexec_status;
		pr->pr_hardening.hr_pax_mprotect_status =
		    pr_p->pr_hardening.hr_pax_mprotect_status;
	}
}

u_int
pax_pageexec_setup_flags(struct image_params *imgp, u_int mode)
{
	struct prison *pr;
	u_int flags, status;

	flags = 0;
	status = 0;

	pr = pax_get_prison(imgp->proc);
	status = pr->pr_hardening.hr_pax_pageexec_status;

	if (status == PAX_FEATURE_DISABLED) {
		flags &= ~PAX_NOTE_PAGEEXEC;
		flags |= PAX_NOTE_NOPAGEEXEC;

		return (flags);
	}

	if (status == PAX_FEATURE_FORCE_ENABLED) {
		flags |= PAX_NOTE_PAGEEXEC;
		flags &= ~PAX_NOTE_NOPAGEEXEC;

		return (flags);
	}

	if (status == PAX_FEATURE_OPTIN) {
		if (mode & PAX_NOTE_PAGEEXEC) {
			flags |= PAX_NOTE_PAGEEXEC;
			flags &= ~PAX_NOTE_NOPAGEEXEC;
		} else {
			flags &= ~PAX_NOTE_PAGEEXEC;
			flags |= PAX_NOTE_NOPAGEEXEC;
		}

		return (flags);
	}

	if (status == PAX_FEATURE_OPTOUT) {
		if (mode & PAX_NOTE_NOPAGEEXEC) {
			flags &= ~PAX_NOTE_PAGEEXEC;
			flags |= PAX_NOTE_NOPAGEEXEC;
		} else {
			flags |= PAX_NOTE_PAGEEXEC;
			flags &= ~PAX_NOTE_NOPAGEEXEC;
		}

		return (flags);
	}

	/*
	 * unknown status, force PAGEEXEC
	 */
	flags |= PAX_NOTE_PAGEEXEC;
	flags &= ~PAX_NOTE_NOPAGEEXEC;

	return (flags);
}

/*
 * PAGEEXEC
 */

int
pax_pageexec_active(struct proc *p)
{
	u_int flags;

	pax_get_flags(p, &flags);

	CTR3(KTR_PAX, "%s: pid = %d p_pax = %x",
	    __func__, p->p_pid, flags);

	if ((flags & PAX_NOTE_PAGEEXEC) == PAX_NOTE_PAGEEXEC)
		return (true);

	if ((flags & PAX_NOTE_NOPAGEEXEC) == PAX_NOTE_NOPAGEEXEC)
		return (false);

	return (true);
}

void
pax_pageexec(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot)
{

	if (!pax_pageexec_active(p)) {
		return;
	}

	CTR3(KTR_PAX, "%s: pid = %d prot = %x",
	    __func__, p->p_pid, *prot);

	if ((*prot & (VM_PROT_WRITE|VM_PROT_EXECUTE)) != VM_PROT_EXECUTE) {
		*prot &= ~VM_PROT_EXECUTE;
	} else {
		*prot &= ~VM_PROT_WRITE;
	}
}

/*
 * MPROTECT
 */

int
pax_mprotect_active(struct proc *p)
{
	u_int flags;

	pax_get_flags(p, &flags);

	CTR3(KTR_PAX, "%s: pid = %d p_pax = %x",
	    __func__, p->p_pid, flags);

	if ((flags & PAX_NOTE_MPROTECT) == PAX_NOTE_MPROTECT)
		return (true);

	if ((flags & PAX_NOTE_NOMPROTECT) == PAX_NOTE_NOMPROTECT)
		return (false);

	return (true);
}

u_int
pax_mprotect_setup_flags(struct image_params *imgp, u_int mode)
{
	struct prison *pr;
	u_int flags, status;

	flags = 0;
	status = 0;

	pr = pax_get_prison(imgp->proc);
	status = pr->pr_hardening.hr_pax_mprotect_status;

	if (status == PAX_FEATURE_DISABLED) {
		flags &= ~PAX_NOTE_MPROTECT;
		flags |= PAX_NOTE_NOMPROTECT;

		return (flags);
	}

	if (status == PAX_FEATURE_FORCE_ENABLED) {
		flags |= PAX_NOTE_MPROTECT;
		flags &= ~PAX_NOTE_NOMPROTECT;

		return (flags);
	}

	if (status == PAX_FEATURE_OPTIN) {
		if (mode & PAX_NOTE_MPROTECT) {
			flags |= PAX_NOTE_MPROTECT;
			flags &= ~PAX_NOTE_NOMPROTECT;
		} else {
			flags &= ~PAX_NOTE_MPROTECT;
			flags |= PAX_NOTE_NOMPROTECT;
		}

		return (flags);
	}

	if (status == PAX_FEATURE_OPTOUT) {
		if (mode & PAX_NOTE_NOMPROTECT) {
			flags &= ~PAX_NOTE_MPROTECT;
			flags |= PAX_NOTE_NOMPROTECT;
		} else {
			flags |= PAX_NOTE_MPROTECT;
			flags &= ~PAX_NOTE_NOMPROTECT;
		}

		return (flags);
	}

	/*
	 * unknown status, force MPROTECT
	 */
	flags |= PAX_NOTE_MPROTECT;
	flags &= ~PAX_NOTE_NOMPROTECT;

	return (flags);
}

void
pax_mprotect(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot)
{

	if (!pax_mprotect_active(p)) {
		return;
	}

	CTR3(KTR_PAX, "%s: pid = %d maxprot = %x",
	    __func__, p->p_pid, *maxprot);

	if ((*maxprot & (VM_PROT_WRITE|VM_PROT_EXECUTE)) != VM_PROT_EXECUTE) {
		*maxprot &= ~VM_PROT_EXECUTE;
	} else {
		*maxprot &= ~VM_PROT_WRITE;
	}
}

int
pax_mprotect_enforce(struct proc *p, vm_prot_t old_prot, vm_prot_t new_prot)
{
	if (!pax_mprotect_active(p))
		return (0);

	if ((new_prot & VM_PROT_EXECUTE) == VM_PROT_EXECUTE &&
	    ((old_prot & VM_PROT_EXECUTE) != VM_PROT_EXECUTE)) {

		return (KERN_PROTECTION_FAILURE);
	}

	return (0);
}


/*
 * @brief Removes VM_PROT_EXECUTE from prot and maxprot.
 *
 * Mainly used to remove exec protection from data, stack, and other sections.
 *
 * @param p		The controlled vmspace's process proc pointer.
 * @param prot
 * @param maxprot
 *
 * @return		none
 */
void
pax_noexec_nx(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot)
{

	CTR4(KTR_PAX, "%s: before - pid = %d prot = %x maxprot = %x",
	    __func__, p->p_pid, *prot, *maxprot);

	if (!pax_pageexec_active(p)) {
		*prot &= ~VM_PROT_EXECUTE;

		if (!pax_mprotect_active(p)) {
			*maxprot &= ~VM_PROT_EXECUTE;
		}
	}

	CTR4(KTR_PAX, "%s: after - pid = %d prot = %x maxprot = %x",
	    __func__, p->p_pid, *prot, *maxprot);
}

/*
 * @brief Removes VM_PROT_WRITE from prot and maxprot.
 *
 * Mainly used to remove write protection from TEXT sections.
 *
 * @param p		The controlled vmspace's process proc pointer.
 * @param prot
 * @param maxprot
 *
 * @return		none
 */
void
pax_noexec_nw(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot)
{

	CTR4(KTR_PAX, "%s: before - pid = %d prot = %x maxprot = %x",
	    __func__, p->p_pid, *prot, *maxprot);

	if (!pax_pageexec_active(p)) {
		*prot &= ~VM_PROT_WRITE;

		if (!pax_mprotect_active(p)) {
			*maxprot &= ~VM_PROT_WRITE;
		}
	}

	CTR4(KTR_PAX, "%s: after - pid = %d prot = %x maxprot = %x",
	    __func__, p->p_pid, *prot, *maxprot);
}

