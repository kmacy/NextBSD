/*-
 * Copyright (c) 2006 Elad Efrat <elad@NetBSD.org>
 * Copyright (c) 2013-2015, by Oliver Pinter <oliver.pinter@hardenedbsd.org>
 * Copyright (c) 2014-2015, by Shawn Webb <shawn.webb@hardenedbsd.org>
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
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/sx.h>
#include <sys/sysent.h>
#include <sys/stat.h>
#include <sys/proc.h>
#include <sys/elf_common.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <sys/vnode.h>
#include <sys/queue.h>
#include <sys/libkern.h>
#include <sys/jail.h>

#include <sys/mman.h>
#include <sys/libkern.h>
#include <sys/exec.h>
#include <sys/kthread.h>

#include <sys/syslimits.h>
#include <sys/param.h>

#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_extern.h>

#include <machine/elf.h>

#include <sys/pax.h>

static int pax_validate_flags(uint32_t flags);
static int pax_check_conflicting_modes(uint32_t mode);

/*
 * Enforce and check HardenedBSD constraints
 */

#ifndef INVARIANTS
#ifndef PAX_INSECURE_MODE
#error "HardenedBSD required enabled INVARIANTS in kernel config... If you really know what you're doing you can add `options PAX_INSECURE_MODE` to the kernel config"
#endif
#endif

SYSCTL_NODE(_hardening, OID_AUTO, pax, CTLFLAG_RD, 0,
    "PaX (exploit mitigation) features.");

static int HardenedBSD_version = __HardenedBSD_version;
SYSCTL_INT(_hardening, OID_AUTO, version, CTLFLAG_RD|CTLFLAG_CAPRD,
    &HardenedBSD_version, 0, "HardenedBSD version");

const char *pax_status_str[] = {
	[PAX_FEATURE_DISABLED] = "disabled",
	[PAX_FEATURE_OPTIN] = "opt-in",
	[PAX_FEATURE_OPTOUT] = "opt-out",
	[PAX_FEATURE_FORCE_ENABLED] = "force enabled",
	[PAX_FEATURE_UNKNOWN_STATUS] = "UNKNOWN -> changed to \"force enabled\""
};

const char *pax_status_simple_str[] = {
	[PAX_FEATURE_SIMPLE_DISABLED] = "disabled",
	[PAX_FEATURE_SIMPLE_ENABLED] = "enabled"
};

/*
 * @brief Get the current process prison.
 *
 * @param p		The current process pointer.
 *
 * @return		prion0's address if failed or kernel process
 * 			the actual process' prison's address else
 *
 */
struct prison *
pax_get_prison(struct proc *p)
{

	/* p can be NULL with kernel threads, so use prison0. */
	if (p == NULL || p->p_ucred == NULL)
		return (&prison0);

#if 0
	PROC_LOCK_ASSERT(p, MA_OWNED);
#endif

	return (p->p_ucred->cr_prison);
}

struct prison *
pax_get_prison_td(struct thread *td)
{

	if (td == NULL || td->td_ucred == NULL)
		return (&prison0);

	return (td->td_ucred->cr_prison);
}

/*
 * @brief Get the current PaX status from process.
 *
 * @param p		The controlled process pointer.
 * @param flags		Where to write the current state.
 *
 * @return		none
 */
void
pax_get_flags(struct proc *p, uint32_t *flags)
{

	*flags = p->p_pax;
}

void
pax_get_flags_td(struct thread *td, uint32_t *flags)
{

	*flags = td->td_pax;
}

static int
pax_validate_flags(uint32_t flags)
{

	if ((flags & ~PAX_NOTE_ALL) != 0)
		return (1);

	return (0);
}

static int
pax_check_conflicting_modes(uint32_t mode)
{

	if (((mode & PAX_NOTE_ALL_ENABLED) & ((mode & PAX_NOTE_ALL_DISABLED) >> 1)) != 0)
		return (1);

	return (0);
}

/*
 * @bried Initialize the new process PaX state
 *
 * @param imgp		Executable image's structure.
 * @param mode		Requested mode.
 *
 * @return		ENOEXEC on fail
 * 			0 on success
 */
int
pax_elf(struct image_params *imgp, uint32_t mode)
{
	uint32_t flags;

	if (pax_validate_flags(mode) != 0) {
		pax_log_internal_imgp(imgp, PAX_LOG_DEFAULT,
		    "unknown paxflags: %x", mode);
		pax_ulog_internal("unknown paxflags: %x\n", mode);

		return (ENOEXEC);
	}

	if (pax_check_conflicting_modes(mode) != 0) {
		/*
		 * indicate flags inconsistencies in dmesg and in user terminal
		 */
		pax_log_internal_imgp(imgp, PAX_LOG_DEFAULT,
		    "inconsistent paxflags: %x", mode);
		pax_ulog_internal("inconsistent paxflags: %x\n", mode);

		return (ENOEXEC);
	}

	flags = 0;

#ifdef PAX_ASLR
	flags |= pax_aslr_setup_flags(imgp, mode);
	flags |= pax_disallow_map32bit_setup_flags(imgp, mode);
#endif

#ifdef PAX_NOEXEC
	flags |= pax_pageexec_setup_flags(imgp, mode);
	flags |= pax_mprotect_setup_flags(imgp, mode);
#endif

#ifdef PAX_SEGVGUARD
	flags |= pax_segvguard_setup_flags(imgp, mode);
#endif

#ifdef PAX_HARDENING
	flags |= pax_hardening_setup_flags(imgp, mode);
#endif

	CTR3(KTR_PAX, "%s : flags = %x mode = %x",
	    __func__, flags, mode);

	/*
	 * Recheck the flags after the parsing: prevent broken setups.
	 */
	if (pax_validate_flags(flags) != 0) {
		pax_log_internal_imgp(imgp, PAX_LOG_DEFAULT,
		    "unknown paxflags after the setup: %x", flags);
		pax_ulog_internal("unknown paxflags after the setup: %x\n", flags);

		return (ENOEXEC);
	}

	/*
	 * Recheck the flags after the parsing: prevent conflicting setups.
	 * This check should be always false.
	 */
	if (pax_check_conflicting_modes(flags) != 0) {
		/*
		 * indicate flags inconsistencies in dmesg and in user terminal
		 */
		pax_log_internal_imgp(imgp, PAX_LOG_DEFAULT,
		    "inconsistent paxflags after the setup: %x", flags);
		pax_ulog_internal("inconsistent paxflags after the setup: %x\n", flags);

		return (ENOEXEC);
	}

	imgp->proc->p_pax = flags;

	/*
	 * if we enable/disable features with secadm, print out a warning
	 */
	if (mode != 0) {
		pax_log_internal_imgp(imgp, PAX_LOG_DEFAULT,
		   "the process has non-default settings");
	}

	return (0);
}


/*
 * @brief Print out PaX settings on boot time, and validate some of them.
 *
 * @return		none
 */
static void
pax_sysinit(void)
{

	printf("PAX: initialize and check PaX and HardenedBSD features.\n");
}
SYSINIT(pax, SI_SUB_PAX, SI_ORDER_FIRST, pax_sysinit, NULL);

/*
 * @brief Initialize prison's state.
 *
 * The prison0 state initialized with global state.
 * The child prisons state initialized with it's parent's state.
 *
 * @param pr		Initializable prison's pointer.
 *
 * @return		none
 */
void
pax_init_prison(struct prison *pr)
{

	CTR2(KTR_PAX, "%s: Setting prison %s PaX variables\n",
	    __func__, pr->pr_name);

	pax_aslr_init_prison(pr);
	pax_hardening_init_prison(pr);
	pax_noexec_init_prison(pr);
	pax_segvguard_init_prison(pr);
	pax_ptrace_hardening_init_prison(pr);

#ifdef COMPAT_FREEBSD32
	pax_aslr_init_prison32(pr);
#endif
}

