/*-
 * Copyright (c) 2014, by David Carlier <devnexen at gmail.com>
 * Copyright (c) 2014, by Oliver Pinter <oliver.pinter@hardenedbsd.org>
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
#include <sys/mount.h>
#include <sys/systm.h>
#include <sys/jail.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/imgact.h>
#include <sys/libkern.h>
#include <sys/limits.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/pax.h>
#include <sys/queue.h>
#include <sys/sbuf.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/syslimits.h>
#include <sys/ucred.h>

#include <machine/stdarg.h>

static int sysctl_ptrace_hardening_status(SYSCTL_HANDLER_ARGS);
#ifdef PAX_PTRACE_HARDENING_GRP
static int sysctl_ptrace_hardening_gid(SYSCTL_HANDLER_ARGS);
#endif

#ifdef PAX_HARDENING
int pax_ptrace_hardening_status = PAX_FEATURE_SIMPLE_ENABLED;
#else
int pax_ptrace_hardening_status = PAX_FEATURE_SIMPLE_DISABLED;
#endif
#ifdef PAX_PTRACE_HARDENING_GRP
gid_t pax_ptrace_hardening_gid = PAX_PTRACE_HARDENING_GRP;
#endif

TUNABLE_INT("hardening.ptrace_hardening.status",
    &pax_ptrace_hardening_status);
#ifdef PAX_PTRACE_HARDENING_GRP
TUNABLE_INT("hardening.ptrace_hardening.gid",
    &pax_ptrace_hardening_gid);
#endif


/*
 * sysctls
 */
#ifdef PAX_SYSCTLS
SYSCTL_NODE(_hardening, OID_AUTO, ptrace_hardening, CTLFLAG_RD, 0,
    "PTrace settings.");

SYSCTL_PROC(_hardening_ptrace_hardening, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_ptrace_hardening_status, "I",
    "Restrictions status. "
    "0 - disabled, "
    "1 - enabled");

#ifdef PAX_PTRACE_HARDENING_GRP
SYSCTL_PROC(_hardening_ptrace_hardening, OID_AUTO, gid,
    CTLTYPE_ULONG|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_ptrace_hardening_gid, "LU",
    "Allowed gid");
#endif

int
sysctl_ptrace_hardening_status(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_ptrace_hardening_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch(val) {
	case    PAX_FEATURE_SIMPLE_DISABLED:
	case    PAX_FEATURE_SIMPLE_ENABLED:
		if (pr == &prison0)
			pax_ptrace_hardening_status = val;

		pr->pr_hardening.hr_pax_ptrace_hardening_status = val;
		break;
	default:
		return (EINVAL);
	}

	return (0);
}

#ifdef PAX_PTRACE_HARDENING_GRP
int
sysctl_ptrace_hardening_gid(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err;
	long val;

	pr = pax_get_prison_td(req->td);

	val = pr->pr_hardening.hr_pax_ptrace_hardening_gid;
	err = sysctl_handle_long(oidp, &val, sizeof(long), req);
	if (err || (req->newptr == NULL))
		return (err);

	if (val < 0 || val > GID_MAX)
		return (EINVAL);

	if (pr == &prison0)
		pax_ptrace_hardening_gid = val;

	 pr->pr_hardening.hr_pax_ptrace_hardening_gid = val;

	return (0);
}
#endif
#endif /* PAX_SYSCTLS */

static void
pax_ptrace_hardening_sysinit(void)
{

	switch (pax_ptrace_hardening_status) {
	case PAX_FEATURE_SIMPLE_DISABLED:
	case PAX_FEATURE_SIMPLE_ENABLED:
		break;
	default:
		printf("[PAX HARDENING] WARNING, invalid settings in loader.conf!"
		    " (hardening.ptrace_hardening.status= %d)\n",
		    pax_ptrace_hardening_status);
		pax_ptrace_hardening_status = PAX_FEATURE_SIMPLE_ENABLED;
	}
	printf("[PAX HARDENING] ptrace hardening status: %s\n",
	    pax_status_simple_str[pax_ptrace_hardening_status]);

#ifdef PAX_PTRACE_HARDENING_GRP
	if (pax_ptrace_hardening_gid < 0 ||
	    pax_ptrace_hardening_gid > GID_MAX) {
		panic("[PAX HARDENING] ptrace hardening\n"
		    "hardening.ptrace_hardening.gid not in range!\n");
	}
	printf("[PAX HARDENING] ptrace hardening allowed gid : %d\n",
	    pax_ptrace_hardening_gid);
#endif
}
SYSINIT(pax_ptrace_hardening, SI_SUB_PAX, SI_ORDER_THIRD, pax_ptrace_hardening_sysinit, NULL);

void
pax_ptrace_hardening_init_prison(struct prison *pr)
{
	struct prison *pr_p;

	CTR2(KTR_PAX, "%s: Setting prison %s PaX variables\n",
	    __func__, pr->pr_name);

	if (pr == &prison0) {
		/* prison0 has no parent, use globals */
		pr->pr_hardening.hr_pax_ptrace_hardening_status =
		    pax_ptrace_hardening_status;
		pr->pr_hardening.hr_pax_ptrace_hardening_gid =
		    pax_ptrace_hardening_gid;
	} else {
		KASSERT(pr->pr_parent != NULL,
		   ("%s: pr->pr_parent == NULL", __func__));
		pr_p = pr->pr_parent;

		pr->pr_hardening.hr_pax_ptrace_hardening_status =
		    pr_p->pr_hardening.hr_pax_ptrace_hardening_status;
		pr->pr_hardening.hr_pax_ptrace_hardening_gid =
		    pr_p->pr_hardening.hr_pax_ptrace_hardening_gid;
	}
}

static inline int
pax_ptrace_allowed(struct prison *pr, struct ucred *cred)
{
	uid_t uid;
	gid_t allowed_gid;

	// XXXOP: convert the uid check to priv_check(...)
	uid = cred->cr_ruid;
	allowed_gid = pr->pr_hardening.hr_pax_ptrace_hardening_gid;
#ifdef PAX_PTRACE_HARDENING_GRP
	if ((uid != 0) &&
	    (groupmember(allowed_gid, cred) == 0))
		return (EPERM);
#else
	if (uid != 0)
		return (EPERM);
#endif
	return (0);
}

int
pax_ptrace_hardening(struct thread *td)
{
	struct prison *pr;
	int err;

	pr = pax_get_prison_td(td);

	if (pr->pr_hardening.hr_pax_ptrace_hardening_status ==
	    PAX_FEATURE_SIMPLE_DISABLED)
		return (0);

	err = pax_ptrace_allowed(pr, td->td_ucred);
	if (err != 0) {
		pax_log_ptrace_hardening(td->td_proc, PAX_LOG_DEFAULT,
		    "forbidden ptrace call attempt from %d user",
		    td->td_ucred->cr_ruid);

		return (err);
	}

	return (0);
}


