/*-
 * Copyright (c) 2006 Elad Efrat <elad@NetBSD.org>
 * Copyright (c) 2013-2015, by Oliver Pinter <oliver.pinter@hardenedbsd.org>
 * Copyright (c) 2014-2015 by Shawn Webb <shawn.webb@hardenedbsd.org>
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

#ifndef	_SYS_PAX_H
#define	_SYS_PAX_H

#define	__HardenedBSD_version	33

#if defined(_KERNEL) || defined(_WANT_PRISON)
struct hardening_features {
	int	 hr_pax_aslr_status;		/* (p) PaX ASLR enabled */
	int	 hr_pax_aslr_mmap_len;		/* (p) Number of bits randomized with mmap */
	int	 hr_pax_aslr_stack_len;		/* (p) Number of bits randomized with stack */
	int	 hr_pax_aslr_exec_len;		/* (p) Number of bits randomized with the execbase */
	int	 hr_pax_aslr_vdso_len;		/* (p) Number of bits randomized with the VDSO */
	int	 hr_pax_aslr_compat_status;	/* (p) PaX ASLR enabled (compat32) */
	int	 hr_pax_aslr_compat_mmap_len;	/* (p) Number of bits randomized with mmap (compat32) */
	int	 hr_pax_aslr_compat_stack_len;	/* (p) Number of bits randomized with stack (compat32) */
	int	 hr_pax_aslr_compat_exec_len;	/* (p) Number of bits randomized with the execbase (compat32) */
	int	 hr_pax_aslr_compat_vdso_len;	/* (p) Number of bits randomized with the VDSO (compat32) */
	int	 hr_pax_segvguard_status;       /* (p) PaX segvguard enabled */
	int	 hr_pax_segvguard_expiry;       /* (p) Number of seconds to expire an entry */
	int	 hr_pax_segvguard_suspension;   /* (p) Number of seconds to suspend an application */
	int	 hr_pax_segvguard_maxcrashes;   /* (p) Maximum number of crashes before suspending application */
	int	 hr_pax_pageexec_status;	/* (p) Remove WX pages from user-space */
	int	 hr_pax_mprotect_status;	/* (p) Enforce W^X mappings */
	int	 hr_pax_disallow_map32bit_status;	/* (p) MAP_32BIT protection (amd64 only) */
	int	 hr_pax_procfs_harden;		/* (p) Harden procfs */
	int	 hr_pax_ptrace_hardening_status;	/* (p) Disallow unprivileged ptrace */
	gid_t	 hr_pax_ptrace_hardening_gid;	/* (p) Allowed ptrace users group */
};
#endif

#ifdef _KERNEL

#include <vm/vm.h>

struct image_params;
struct prison;
struct thread;
struct proc;
struct vnode;
struct vm_offset_t;

/*
 * used in sysctl handler
 */
#define	PAX_FEATURE_DISABLED		0
#define	PAX_FEATURE_OPTIN		1
#define	PAX_FEATURE_OPTOUT		2
#define	PAX_FEATURE_FORCE_ENABLED	3
#define	PAX_FEATURE_UNKNOWN_STATUS	4

extern const char *pax_status_str[];

#define PAX_FEATURE_SIMPLE_DISABLED	0
#define PAX_FEATURE_SIMPLE_ENABLED	1

extern const char *pax_status_simple_str[];

/*
 * generic pax functions
 */
int pax_elf(struct image_params *, uint32_t);
void pax_get_flags(struct proc *p, uint32_t *flags);
void pax_get_flags_td(struct thread *td, uint32_t *flags);
struct prison *pax_get_prison(struct proc *p);
struct prison *pax_get_prison_td(struct thread *td);
void pax_init_prison(struct prison *pr);

/*
 * ASLR related functions
 */
int pax_aslr_active(struct proc *p);
#ifdef PAX_ASLR
void pax_aslr_init_prison(struct prison *pr);
void pax_aslr_init_prison32(struct prison *pr);
void pax_aslr_init_vmspace(struct proc *p);
void pax_aslr_init_vmspace32(struct proc *p);
#else
#define	pax_aslr_init_prison(pr)	do {} while (0)
#define	pax_aslr_init_prison32(pr)	do {} while (0)
#define	pax_aslr_init_vmspace		NULL
#define	pax_aslr_init_vmspace32		NULL
#endif
void pax_aslr_init(struct image_params *imgp);
void pax_aslr_execbase(struct proc *p, u_long *et_dyn_addrp);
void pax_aslr_mmap(struct proc *p, vm_offset_t *addr, 
    vm_offset_t orig_addr, int flags);
void pax_aslr_mmap_map_32bit(struct proc *p, vm_offset_t *addr, 
    vm_offset_t orig_addr, int flags);
void pax_aslr_rtld(struct proc *p, u_long *addr);
uint32_t pax_aslr_setup_flags(struct image_params *imgp, uint32_t mode);
void pax_aslr_stack(struct proc *p, vm_offset_t *addr);
void pax_aslr_stack_with_gap(struct proc *p, vm_offset_t *addr);
void pax_aslr_vdso(struct proc *p, vm_offset_t *addr);
uint32_t pax_disallow_map32bit_setup_flags(struct image_params *imgp, uint32_t mode);

/*
 * Log related functions
 */

#define	PAX_LOG_DEFAULT		0x00000000
#define	PAX_LOG_SKIP_DETAILS	0x00000001
#define	PAX_LOG_NO_NEWLINE	0x00000002
#define	PAX_LOG_P_COMM		0x00000004
#define	PAX_LOG_NO_P_PAX	0x00000008
#define	PAX_LOG_NO_INDENT	0x00000010

void pax_printf_flags(struct proc *p, uint64_t flags);
void pax_printf_flags_td(struct thread *td, uint64_t flags);
void pax_db_printf_flags(struct proc *p, uint64_t flags);
void pax_db_printf_flags_td(struct thread *td, uint64_t flags);
int hbsd_uprintf(const char *fmt, ...) __printflike(1, 2);
void pax_log_internal(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_log_internal_imgp(struct image_params *imgp, uint64_t flags, const char* fmt, ...) __printflike(3, 4);
void pax_ulog_internal(const char *fmt, ...) __printflike(1, 2);
void pax_log_aslr(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_ulog_aslr(const char *fmt, ...) __printflike(1, 2);
void pax_log_pageexec(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_ulog_pageexec(const char *fmt, ...) __printflike(1, 2);
void pax_log_mprotect(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_ulog_mprotect(const char *fmt, ...) __printflike(1, 2);
void pax_log_segvguard(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_ulog_segvguard(const char *fmt, ...) __printflike(1, 2);
void pax_log_ptrace_hardening(struct proc *, uint64_t flags, const char *fmt, ...) __printflike(3, 4);
void pax_ulog_ptrace_hardening(const char *fmt, ...) __printflike(1, 2);


/*
 * SegvGuard related functions
 */
#ifdef PAX_SEGVGUARD
void pax_segvguard_init_prison(struct prison *pr);
#else
#define	pax_segvguard_init_prison(pr)	do {} while (0)
#endif
int pax_segvguard_check(struct thread *, struct vnode *, const char *);
int pax_segvguard_segfault(struct thread *, const char *);
void pax_segvguard_remove(struct thread *td, struct vnode *vn);
uint32_t pax_segvguard_setup_flags(struct image_params *imgp, uint32_t mode);
int pax_segvguard_update_flags_if_setuid(struct image_params *imgp,
    struct vnode *vn);

/*
 * PAX PAGEEXEC and MPROTECT hardening
 */
#ifdef PAX_NOEXEC
void pax_noexec_init_prison(struct prison *pr);
#else
#define	pax_noexec_init_prison(pr)	do {} while (0)
#endif
void pax_noexec_nw(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot);
void pax_noexec_nx(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot);
int pax_pageexec_active(struct proc *p);
int pax_mprotect_active(struct proc *p);
u_int pax_pageexec_setup_flags(struct image_params *imgp, u_int mode);
u_int pax_mprotect_setup_flags(struct image_params *imgp, u_int mode);
void pax_pageexec(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot);
void pax_mprotect(struct proc *p, vm_prot_t *prot, vm_prot_t *maxprot);
int pax_mprotect_enforce(struct proc *p, vm_prot_t old_prot, vm_prot_t new_prot);

/*
 * Hardening related functions
 */
#ifdef PAX_HARDENING
void pax_hardening_init_prison(struct prison *pr);
#else
#define	pax_hardening_init_prison(pr)	do {} while (0)
#endif
int pax_disallow_map32bit_active(struct thread *td, int flags);
int pax_mprotect_exec_harden(struct thread *td);
int pax_procfs_harden(struct thread *td);
uint32_t pax_hardening_setup_flags(struct image_params *imgp,
    uint32_t mode);

/*
 * ptrace hardening related functions
 */
#if defined(PAX_PTRACE_HARDENING) || defined(PAX_PTRACE_HARDENING_GRP)
void pax_ptrace_hardening_init_prison(struct prison *pr);
#else
#define	pax_ptrace_hardening_init_prison(pr)	do {} while (0)
#endif
int pax_ptrace_hardening(struct thread *td);

#endif /* _KERNEL */

#define	PAX_NOTE_PAGEEXEC	0x00000001
#define	PAX_NOTE_NOPAGEEXEC	0x00000002
#define	PAX_NOTE_MPROTECT	0x00000004
#define	PAX_NOTE_NOMPROTECT	0x00000008
#define	PAX_NOTE_SEGVGUARD	0x00000010
#define	PAX_NOTE_NOSEGVGUARD	0x00000020
#define	PAX_NOTE_ASLR		0x00000040
#define	PAX_NOTE_NOASLR		0x00000080
#define	PAX_NOTE_SHLIBRANDOM	0x00000100
#define	PAX_NOTE_NOSHLIBRANDOM	0x00000200
#define	PAX_NOTE_DISALLOWMAP32BIT	0x00000400
#define	PAX_NOTE_NODISALLOWMAP32BIT	0x00000800

#define	PAX_NOTE_RESERVED0	0x40000000
#define	PAX_NOTE_FINALIZED	0x80000000

#define PAX_NOTE_ALL_ENABLED	\
    (PAX_NOTE_PAGEEXEC | PAX_NOTE_MPROTECT | PAX_NOTE_SEGVGUARD | \
    PAX_NOTE_ASLR | PAX_NOTE_SHLIBRANDOM | PAX_NOTE_DISALLOWMAP32BIT)
#define PAX_NOTE_ALL_DISABLED	\
    (PAX_NOTE_NOPAGEEXEC | PAX_NOTE_NOMPROTECT | \
    PAX_NOTE_NOSEGVGUARD | PAX_NOTE_NOASLR | PAX_NOTE_NOSHLIBRANDOM | \
    PAX_NOTE_NODISALLOWMAP32BIT)
#define PAX_NOTE_ALL	(PAX_NOTE_ALL_ENABLED | PAX_NOTE_ALL_DISABLED)

#endif /* !_SYS_PAX_H */
