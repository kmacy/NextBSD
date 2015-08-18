/*-
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include "opt_pax.h"
#include "opt_ddb.h"

#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/imgact.h>
#include <sys/pax.h>
#include <sys/proc.h>
#include <sys/sbuf.h>
#include <sys/jail.h>
#include <machine/stdarg.h>

#ifdef DDB
#include <ddb/ddb.h>
#endif

static void pax_log_log(struct proc *p, struct thread *td, uint64_t flags,
    const char *prefix, const char *fmt, va_list ap);
static void pax_log_ulog(const char *prefix, const char *fmt, va_list ap);

#define PAX_LOG_FEATURES_STRING		\
		    "\020"		\
		    "\001PAGEEXEC"	\
		    "\002NOPAGEEXEC"	\
		    "\003MPROTECT"	\
		    "\004NOMPROTECT"	\
		    "\005SEGVGUARD"	\
		    "\006NOSEGVGUARD"	\
		    "\007ASLR"		\
		    "\010NOASLR"	\
		    "\011SHLIBRANDOM"	\
		    "\012NOSHLIBRANDOM"		\
		    "\013<f10>"		\
		    "\014<f11>"		\
		    "\015<f12>"		\
		    "\016<f13>"		\
		    "\017<f14>"		\
		    "\020<f15>"		\
		    "\021<f16>"		\
		    "\022<f17>"		\
		    "\023<f18>"		\
		    "\024<f19>"		\
		    "\025<f20>"		\
		    "\026<f21>"		\
		    "\027<f22>"		\
		    "\030<f23>"		\
		    "\031<f24>"		\
		    "\032<f25>"		\
		    "\033<f26>"		\
		    "\034<f27>"		\
		    "\035<f28>"		\
		    "\036<f29>"		\
		    "\037<f30>"		\
		    "\040<f31>"

#define __HARDENING_LOG_TEMPLATE(MAIN, SUBJECT, prefix, name)		\
void									\
prefix##_log_##name(struct proc *p, uint64_t flags,			\
    const char* fmt, ...)						\
{									\
	const char *prefix = "["#MAIN" "#SUBJECT"]";			\
	va_list args;							\
									\
	if (hardening_log_log == 0)					\
		return;							\
									\
	va_start(args, fmt);						\
	pax_log_log(p, NULL, flags, prefix, fmt, args);			\
	va_end(args);							\
}									\
									\
void									\
prefix##_ulog_##name(const char* fmt, ...)				\
{									\
	const char *prefix = "["#MAIN" "#SUBJECT"]";			\
	va_list args;							\
									\
	if (hardening_log_ulog == 0)					\
		return;							\
									\
	va_start(args, fmt);						\
	pax_log_ulog(prefix, fmt, args);				\
	va_end(args);							\
}

static int sysctl_hardening_log_log(SYSCTL_HANDLER_ARGS);
static int sysctl_hardening_log_ulog(SYSCTL_HANDLER_ARGS);

static int hardening_log_log = PAX_FEATURE_SIMPLE_ENABLED;
static int hardening_log_ulog = PAX_FEATURE_SIMPLE_DISABLED;

TUNABLE_INT("hardening.log.log", &hardening_log_log);
TUNABLE_INT("hardening.log.ulog", &hardening_log_ulog);

#ifdef PAX_SYSCTLS
SYSCTL_NODE(_hardening, OID_AUTO, log, CTLFLAG_RD, 0,
    "Hardening related logging facility.");

SYSCTL_PROC(_hardening_log, OID_AUTO, log,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_hardening_log_log, "I",
    "log to syslog "
    "0 - disabled, "
    "1 - enabled ");

SYSCTL_PROC(_hardening_log, OID_AUTO, ulog,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_hardening_log_ulog, "I",
    "log to user terminal"
    "0 - disabled, "
    "1 - enabled ");
#endif


static void
hardening_log_sysinit(void)
{
	switch (hardening_log_log) {
	case PAX_FEATURE_SIMPLE_DISABLED:
	case PAX_FEATURE_SIMPLE_ENABLED:
		break;
	default:
		printf("[PAX LOG] WARNING, invalid settings in loader.conf!"
		    " (hardening.log.log = %d)\n", hardening_log_log);
		hardening_log_log = PAX_FEATURE_SIMPLE_ENABLED;
	}
	printf("[PAX LOG] logging to system: %s\n",
	    pax_status_simple_str[hardening_log_log]);

	switch (hardening_log_ulog) {
	case PAX_FEATURE_SIMPLE_DISABLED:
	case PAX_FEATURE_SIMPLE_ENABLED:
		break;
	default:
		printf("[PAX LOG] WARNING, invalid settings in loader.conf!"
		    " (hardening.log.ulog = %d)\n", hardening_log_ulog);
		hardening_log_ulog = PAX_FEATURE_SIMPLE_ENABLED;
	}
	printf("[PAX LOG] logging to user: %s\n",
	    pax_status_simple_str[hardening_log_ulog]);
}
SYSINIT(hardening_log, SI_SUB_PAX, SI_ORDER_SECOND, hardening_log_sysinit, NULL);

#ifdef PAX_SYSCTLS
static int
sysctl_hardening_log_log(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;

	val = hardening_log_log;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	switch (val) {
	case	PAX_FEATURE_SIMPLE_DISABLED :
	case	PAX_FEATURE_SIMPLE_ENABLED :
		break;
	default:
		return (EINVAL);

	}

	hardening_log_log = val;

	return (0);
}

static int
sysctl_hardening_log_ulog(SYSCTL_HANDLER_ARGS)
{
	int err;
	int val;

	val = hardening_log_ulog;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	switch (val) {
	case	PAX_FEATURE_SIMPLE_DISABLED :
	case	PAX_FEATURE_SIMPLE_ENABLED :
		break;
	default:
		return (EINVAL);

	}

	hardening_log_ulog = val;

	return (0);
}
#endif

static void
_pax_log_prefix(struct sbuf *sb, uint64_t flags, const char *prefix)
{

	sbuf_printf(sb, "%s ", prefix);
}

static void
_pax_log_indent(struct sbuf *sb, uint64_t flags)
{

	if ((flags & PAX_LOG_NO_INDENT) != PAX_LOG_NO_INDENT)
		sbuf_printf(sb, "\n -> ");
}

static void
_pax_log_proc_details(struct sbuf *sb, uint64_t flags, struct proc *p)
{

	if (p != NULL) {
		if ((flags & PAX_LOG_P_COMM) == PAX_LOG_P_COMM)
			sbuf_printf(sb, "p_comm: %s ", p->p_comm);

		sbuf_printf(sb, "pid: %d ", p->p_pid);
		sbuf_printf(sb, "ppid: %d ", p->p_pptr->p_pid);

		if ((flags & PAX_LOG_NO_P_PAX) != PAX_LOG_NO_P_PAX)
			sbuf_printf(sb, "p_pax: 0x%b ", p->p_pax, PAX_LOG_FEATURES_STRING);
	}
}

static void
_pax_log_thread_details(struct sbuf *sb, uint64_t flags, struct thread *td)
{

	if (td != NULL) {
		sbuf_printf(sb, "tid: %d ", td->td_tid);
	}
}

static void
_pax_log_details_end(struct sbuf *sb)
{

	sbuf_printf(sb, "\n");
}

static void
_pax_log_imgp_details(struct sbuf *sb, uint64_t flags, struct image_params *imgp)
{

	if (imgp != NULL && imgp->args != NULL)
		if (imgp->args->fname != NULL)
			sbuf_printf(sb, "fname: %s ",
			    imgp->args->fname);
}


static void
pax_log_log(struct proc *p, struct thread *td, uint64_t flags,
    const char *prefix, const char *fmt, va_list ap)
{
	struct sbuf *sb;

	sb = sbuf_new_auto();
	if (sb == NULL)
		panic("%s: Could not allocate memory", __func__);

	_pax_log_prefix(sb, flags, prefix);

	sbuf_vprintf(sb, fmt, ap);
	if ((flags & PAX_LOG_SKIP_DETAILS) != PAX_LOG_SKIP_DETAILS) {
		_pax_log_indent(sb, flags);
		_pax_log_proc_details(sb, flags, p);
		_pax_log_thread_details(sb, flags, td);
		_pax_log_details_end(sb);
	}

	if (sbuf_finish(sb) != 0)
		panic("%s: Could not generate message", __func__);

	printf("%s", sbuf_data(sb));
	sbuf_delete(sb);
}

static void
pax_log_ulog(const char *prefix, const char *fmt, va_list ap)
{
	struct sbuf *sb;

	sb = sbuf_new_auto();
	if (sb == NULL)
		panic("%s: Could not allocate memory", __func__);

	if (prefix != NULL)
		sbuf_printf(sb, "%s ", prefix);
	sbuf_vprintf(sb, fmt, ap);
	if (sbuf_finish(sb) != 0)
		panic("%s: Could not generate message", __func__);

	hbsd_uprintf("%s", sbuf_data(sb));				\
	sbuf_delete(sb);
}

void
pax_printf_flags(struct proc *p, uint64_t flags)
{

	if (p != NULL) {
		printf("pax flags: 0x%b%c", p->p_pax, PAX_LOG_FEATURES_STRING,
		    ((flags & PAX_LOG_NO_NEWLINE) == PAX_LOG_NO_NEWLINE) ?
		    ' ' : '\n');
	}
}

void
pax_printf_flags_td(struct thread *td, uint64_t flags)
{

	if (td != NULL) {
		printf("pax flags: 0x%b%c", td->td_pax, PAX_LOG_FEATURES_STRING,
		    ((flags & PAX_LOG_NO_NEWLINE) == PAX_LOG_NO_NEWLINE) ?
		    ' ' : '\n');
	}
}

#ifdef DDB
void
pax_db_printf_flags(struct proc *p, uint64_t flags)
{

	if (p != NULL) {
		db_printf(" pax flags: 0x%b%c", p->p_pax, PAX_LOG_FEATURES_STRING,
		    ((flags & PAX_LOG_NO_NEWLINE) == PAX_LOG_NO_NEWLINE) ?
		    ' ' : '\n');
	}
}

void
pax_db_printf_flags_td(struct thread *td, uint64_t flags)
{

	if (td != NULL) {
		db_printf(" pax flags: 0x%b%c", td->td_pax, PAX_LOG_FEATURES_STRING,
		    ((flags & PAX_LOG_NO_NEWLINE) == PAX_LOG_NO_NEWLINE) ?
		    ' ' : '\n');
	}
}
#endif

__HARDENING_LOG_TEMPLATE(PAX, INTERNAL, pax, internal);
__HARDENING_LOG_TEMPLATE(PAX, ASLR, pax, aslr);
__HARDENING_LOG_TEMPLATE(PAX, PAGEEXEC, pax, pageexec);
__HARDENING_LOG_TEMPLATE(PAX, MPROTECT, pax, mprotect);
__HARDENING_LOG_TEMPLATE(PAX, SEGVGUARD, pax, segvguard);
__HARDENING_LOG_TEMPLATE(PAX, PTRACE_HARDENING, pax, ptrace_hardening);


void
pax_log_internal_imgp(struct image_params *imgp, uint64_t flags, const char* fmt, ...)
{
	const char *prefix = "[PAX INTERNAL]";
	struct sbuf *sb;
	va_list args;

	KASSERT(imgp != NULL, ("%s: imgp == NULL", __func__));

	if (hardening_log_log == 0)
		return;

	sb = sbuf_new_auto();
	if (sb == NULL)
		panic("%s: Could not allocate memory", __func__);

	_pax_log_prefix(sb, flags, prefix);

	va_start(args, fmt);
	sbuf_vprintf(sb, fmt, args);
	va_end(args);

	if ((flags & PAX_LOG_SKIP_DETAILS) != PAX_LOG_SKIP_DETAILS) {
		_pax_log_indent(sb, flags);
		_pax_log_imgp_details(sb, flags, imgp);
		_pax_log_indent(sb, flags);
		_pax_log_proc_details(sb, flags, imgp->proc);
		_pax_log_details_end(sb);
	}

	if (sbuf_finish(sb) != 0)
		panic("%s: Could not generate message", __func__);

	printf("%s", sbuf_data(sb));
	sbuf_delete(sb);
}
