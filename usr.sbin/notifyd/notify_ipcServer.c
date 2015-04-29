/*
 * IDENTIFICATION:
 * stub generated Tue Apr 28 09:01:37 2015
 * with a MiG generated Mon Apr 27 08:45:27 CEST 2015 by root@testvm
 * OPTIONS: 
 */

/* Module notify_ipc */

#define	__MIG_check__Request__notify_ipc_subsystem__ 1

#include <sys/cdefs.h>
#include <sys/types.h>
#ifdef _KERNEL
#include <sys/mach/ndr.h>
#include <sys/mach/kern_return.h>
#include <sys/mach/notify.h>
#include <sys/mach/mach_types.h>
#include <sys/mach/message.h>
#include <sys/mach/mig_errors.h>
#else /* !_KERNEL */
#include <string.h>
#include <mach/ndr.h>
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/notify.h>
#include <mach/mach_types.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#endif /*_KERNEL */

#include <sys/mach/std_types.h>
#include <sys/mach/mig.h>
#include <sys/mach/mig.h>
#include <sys/mach/mach_types.h>
#include <sys/types.h>
#include "notify_ipc_types.h"

#ifndef	mig_internal
#define	mig_internal	static __inline__
#endif	/* mig_internal */

#ifndef	mig_external
#define mig_external
#endif	/* mig_external */

#if	!defined(__MigTypeCheck) && defined(TypeCheck)
#define	__MigTypeCheck		TypeCheck	/* Legacy setting */
#endif	/* !defined(__MigTypeCheck) */

#if	!defined(__MigKernelSpecificCode) && defined(_MIG_KERNEL_SPECIFIC_CODE_)
#define	__MigKernelSpecificCode	_MIG_KERNEL_SPECIFIC_CODE_	/* Legacy setting */
#endif	/* !defined(__MigKernelSpecificCode) */

#ifndef	LimitCheck
#define	LimitCheck 0
#endif	/* LimitCheck */

#ifndef	min
#define	min(a,b)  ( ((a) < (b))? (a): (b) )
#endif	/* min */

#if !defined(_WALIGN_)
#define _WALIGN_(x) (((x) + 7) & ~7)
#endif /* !defined(_WALIGN_) */

#if !defined(_WALIGNSZ_)
#define _WALIGNSZ_(x) _WALIGN_(sizeof(x))
#endif /* !defined(_WALIGNSZ_) */

#ifndef	UseStaticTemplates
#define	UseStaticTemplates	1
#endif	/* UseStaticTemplates */

#define _WALIGN_(x) (((x) + 7) & ~7)
#define _WALIGNSZ_(x) _WALIGN_(sizeof(x))
#ifndef	__DeclareRcvRpc
#define	__DeclareRcvRpc(_NUM_, _NAME_)
#endif	/* __DeclareRcvRpc */

#ifndef	__BeforeRcvRpc
#define	__BeforeRcvRpc(_NUM_, _NAME_)
#endif	/* __BeforeRcvRpc */

#ifndef	__AfterRcvRpc
#define	__AfterRcvRpc(_NUM_, _NAME_)
#endif	/* __AfterRcvRpc */

#ifndef	__DeclareRcvSimple
#define	__DeclareRcvSimple(_NUM_, _NAME_)
#endif	/* __DeclareRcvSimple */

#ifndef	__BeforeRcvSimple
#define	__BeforeRcvSimple(_NUM_, _NAME_)
#endif	/* __BeforeRcvSimple */

#ifndef	__AfterRcvSimple
#define	__AfterRcvSimple(_NUM_, _NAME_)
#endif	/* __AfterRcvSimple */

#define novalue void
#define msgh_request_port	msgh_local_port
#define MACH_MSGH_BITS_REQUEST(bits)	MACH_MSGH_BITS_LOCAL(bits)
#define msgh_reply_port		msgh_remote_port
#define MACH_MSGH_BITS_REPLY(bits)	MACH_MSGH_BITS_REMOTE(bits)

#define MIG_RETURN_ERROR(X, code)	{\
				((mig_reply_error_t *)X)->RetCode = code;\
				((mig_reply_error_t *)X)->NDR = NDR_record;\
				return;\
				}

/* typedefs for all requests */

#ifndef __Request__notify_ipc_subsystem__defined
#define __Request__notify_ipc_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_post_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_register_plain_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_register_check_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int sig;
	} __Request___notify_server_register_signal_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t fileport;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int ntoken;
	} __Request___notify_server_register_file_descriptor_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int ntoken;
	} __Request___notify_server_register_mach_port_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int user;
		int group;
	} __Request___notify_server_set_owner_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_get_owner_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int mode;
	} __Request___notify_server_set_access_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_get_access_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_release_name_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_cancel_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_check_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_get_state_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		uint64_t state;
	} __Request___notify_server_set_state_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_type_number_t pathCnt;
		int flags;
	} __Request___notify_server_monitor_file_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_suspend_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_resume_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int pid;
	} __Request___notify_server_suspend_pid_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int pid;
	} __Request___notify_server_resume_pid_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_simple_post_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_post_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
	} __Request___notify_server_post_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
	} __Request___notify_server_post_4_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
	} __Request___notify_server_register_plain_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
	} __Request___notify_server_register_check_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		int sig;
	} __Request___notify_server_register_signal_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t fileport;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
	} __Request___notify_server_register_file_descriptor_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
	} __Request___notify_server_register_mach_port_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_cancel_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
	} __Request___notify_server_get_state_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
	} __Request___notify_server_get_state_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
		uint64_t state;
	} __Request___notify_server_set_state_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		uint64_t state;
	} __Request___notify_server_set_state_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_type_number_t pathCnt;
		int flags;
	} __Request___notify_server_monitor_file_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		uint32_t reg_type;
		int sig;
		int prev_slot;
		uint64_t prev_state;
		uint64_t prev_time;
		mach_msg_type_number_t pathCnt;
		int path_flags;
	} __Request___notify_server_regenerate_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif
#endif /* !__Request__notify_ipc_subsystem__defined */

/* typedefs for all replies */

#ifndef __Reply__notify_ipc_subsystem__defined
#define __Reply__notify_ipc_subsystem__defined

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_post_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int token;
		int status;
	} __Reply___notify_server_register_plain_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int size;
		int slot;
		int token;
		int status;
	} __Reply___notify_server_register_check_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int token;
		int status;
	} __Reply___notify_server_register_signal_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int token;
		int status;
	} __Reply___notify_server_register_file_descriptor_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int token;
		int status;
	} __Reply___notify_server_register_mach_port_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_set_owner_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int user;
		int group;
		int status;
	} __Reply___notify_server_get_owner_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_set_access_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int mode;
		int status;
	} __Reply___notify_server_get_access_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_release_name_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_cancel_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int check;
		int status;
	} __Reply___notify_server_check_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uint64_t state;
		int status;
	} __Reply___notify_server_get_state_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_set_state_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_monitor_file_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_suspend_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_resume_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_suspend_pid_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int status;
	} __Reply___notify_server_resume_pid_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_simple_post_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uint64_t name_id;
		int status;
	} __Reply___notify_server_post_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_post_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_post_4_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_register_plain_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int size;
		int slot;
		uint64_t name_id;
		int status;
	} __Reply___notify_server_register_check_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_register_signal_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_register_file_descriptor_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_register_mach_port_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_cancel_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uint64_t state;
		int status;
	} __Reply___notify_server_get_state_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uint64_t state;
		uint64_t nid;
		int status;
	} __Reply___notify_server_get_state_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_set_state_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		uint64_t nid;
		int status;
	} __Reply___notify_server_set_state_3_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
	} __Reply___notify_server_monitor_file_2_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		NDR_record_t NDR;
		kern_return_t RetCode;
		int new_slot;
		uint64_t new_name_id;
		int status;
	} __Reply___notify_server_regenerate_t;
#ifdef  __MigPackStructs
#pragma pack()
#endif
#endif /* !__Reply__notify_ipc_subsystem__defined */


/* union of all replies */

#ifndef __ReplyUnion___notify_ipc_subsystem__defined
#define __ReplyUnion___notify_ipc_subsystem__defined
union __ReplyUnion___notify_ipc_subsystem {
	__Reply___notify_server_post_t Reply__notify_server_post;
	__Reply___notify_server_register_plain_t Reply__notify_server_register_plain;
	__Reply___notify_server_register_check_t Reply__notify_server_register_check;
	__Reply___notify_server_register_signal_t Reply__notify_server_register_signal;
	__Reply___notify_server_register_file_descriptor_t Reply__notify_server_register_file_descriptor;
	__Reply___notify_server_register_mach_port_t Reply__notify_server_register_mach_port;
	__Reply___notify_server_set_owner_t Reply__notify_server_set_owner;
	__Reply___notify_server_get_owner_t Reply__notify_server_get_owner;
	__Reply___notify_server_set_access_t Reply__notify_server_set_access;
	__Reply___notify_server_get_access_t Reply__notify_server_get_access;
	__Reply___notify_server_release_name_t Reply__notify_server_release_name;
	__Reply___notify_server_cancel_t Reply__notify_server_cancel;
	__Reply___notify_server_check_t Reply__notify_server_check;
	__Reply___notify_server_get_state_t Reply__notify_server_get_state;
	__Reply___notify_server_set_state_t Reply__notify_server_set_state;
	__Reply___notify_server_monitor_file_t Reply__notify_server_monitor_file;
	__Reply___notify_server_suspend_t Reply__notify_server_suspend;
	__Reply___notify_server_resume_t Reply__notify_server_resume;
	__Reply___notify_server_suspend_pid_t Reply__notify_server_suspend_pid;
	__Reply___notify_server_resume_pid_t Reply__notify_server_resume_pid;
	__Reply___notify_server_simple_post_t Reply__notify_server_simple_post;
	__Reply___notify_server_post_2_t Reply__notify_server_post_2;
	__Reply___notify_server_post_3_t Reply__notify_server_post_3;
	__Reply___notify_server_post_4_t Reply__notify_server_post_4;
	__Reply___notify_server_register_plain_2_t Reply__notify_server_register_plain_2;
	__Reply___notify_server_register_check_2_t Reply__notify_server_register_check_2;
	__Reply___notify_server_register_signal_2_t Reply__notify_server_register_signal_2;
	__Reply___notify_server_register_file_descriptor_2_t Reply__notify_server_register_file_descriptor_2;
	__Reply___notify_server_register_mach_port_2_t Reply__notify_server_register_mach_port_2;
	__Reply___notify_server_cancel_2_t Reply__notify_server_cancel_2;
	__Reply___notify_server_get_state_2_t Reply__notify_server_get_state_2;
	__Reply___notify_server_get_state_3_t Reply__notify_server_get_state_3;
	__Reply___notify_server_set_state_2_t Reply__notify_server_set_state_2;
	__Reply___notify_server_set_state_3_t Reply__notify_server_set_state_3;
	__Reply___notify_server_monitor_file_2_t Reply__notify_server_monitor_file_2;
	__Reply___notify_server_regenerate_t Reply__notify_server_regenerate;
};
#endif /* __RequestUnion___notify_ipc_subsystem__defined */
/* Forward Declarations */


mig_internal novalue _X_notify_server_post
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_plain
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_check
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_signal
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_file_descriptor
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_mach_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_set_owner
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_get_owner
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_set_access
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_get_access
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_release_name
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_cancel
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_check
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_get_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_set_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_monitor_file
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_suspend
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_resume
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_suspend_pid
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_resume_pid
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_simple_post
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_post_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_post_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_post_4
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_plain_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_check_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_signal_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_file_descriptor_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_register_mach_port_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_cancel_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_get_state_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_get_state_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_set_state_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_set_state_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_monitor_file_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);

mig_internal novalue _X_notify_server_regenerate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP);


#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_post_t__defined)
#define __MIG_check__Request___notify_server_post_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_post_t(__attribute__((__unused__)) __Request___notify_server_post_t *In0P)
{

	typedef __Request___notify_server_post_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_post_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_post */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_post
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *status;
	audit_token_t audit;
{ return __notify_server_post(server, name, nameCnt, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_post */
mig_internal novalue _X_notify_server_post
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_post_t __Request;
	typedef __Reply___notify_server_post_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_post_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_post_t__defined */

	__DeclareRcvRpc(78945668, "_notify_server_post")
	__BeforeRcvRpc(78945668, "_notify_server_post")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_post_t__defined)
	check_result = __MIG_check__Request___notify_server_post_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_post_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_post(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945668, "_notify_server_post")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_plain_t__defined)
#define __MIG_check__Request___notify_server_register_plain_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_plain_t(__attribute__((__unused__)) __Request___notify_server_register_plain_t *In0P)
{

	typedef __Request___notify_server_register_plain_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_plain_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_plain */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_plain
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *token;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_plain(server, name, nameCnt, token, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_plain */
mig_internal novalue _X_notify_server_register_plain
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_plain_t __Request;
	typedef __Reply___notify_server_register_plain_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_plain_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_plain_t__defined */

	__DeclareRcvRpc(78945669, "_notify_server_register_plain")
	__BeforeRcvRpc(78945669, "_notify_server_register_plain")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_plain_t__defined)
	check_result = __MIG_check__Request___notify_server_register_plain_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_plain_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_plain(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945669, "_notify_server_register_plain")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_check_t__defined)
#define __MIG_check__Request___notify_server_register_check_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_check_t(__attribute__((__unused__)) __Request___notify_server_register_check_t *In0P)
{

	typedef __Request___notify_server_register_check_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_check_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_check */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_check
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, size, slot, token, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *size;
	int *slot;
	int *token;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_check(server, name, nameCnt, size, slot, token, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *size,
	int *slot,
	int *token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_check */
mig_internal novalue _X_notify_server_register_check
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_check_t __Request;
	typedef __Reply___notify_server_register_check_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_check_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_check_t__defined */

	__DeclareRcvRpc(78945670, "_notify_server_register_check")
	__BeforeRcvRpc(78945670, "_notify_server_register_check")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_check_t__defined)
	check_result = __MIG_check__Request___notify_server_register_check_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_check_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_check(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->size, &OutP->slot, &OutP->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945670, "_notify_server_register_check")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_signal_t__defined)
#define __MIG_check__Request___notify_server_register_signal_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_signal_t(__attribute__((__unused__)) __Request___notify_server_register_signal_t *In0P)
{

	typedef __Request___notify_server_register_signal_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_signal_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_signal */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_signal
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, sig, token, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int sig;
	int *token;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_signal(server, name, nameCnt, sig, token, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int sig,
	int *token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_signal */
mig_internal novalue _X_notify_server_register_signal
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int sig;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_signal_t __Request;
	typedef __Reply___notify_server_register_signal_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_signal_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_signal_t__defined */

	__DeclareRcvRpc(78945671, "_notify_server_register_signal")
	__BeforeRcvRpc(78945671, "_notify_server_register_signal")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_signal_t__defined)
	check_result = __MIG_check__Request___notify_server_register_signal_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_signal_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_signal(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->sig, &OutP->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945671, "_notify_server_register_signal")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_file_descriptor_t__defined)
#define __MIG_check__Request___notify_server_register_file_descriptor_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_file_descriptor_t(__attribute__((__unused__)) __Request___notify_server_register_file_descriptor_t *In0P)
{

	typedef __Request___notify_server_register_file_descriptor_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 2) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->fileport.type != MACH_MSG_PORT_DESCRIPTOR ||
	    In0P->fileport.disposition != 17)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_file_descriptor_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_file_descriptor */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_file_descriptor
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, fileport, ntoken, token, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	mach_port_t fileport;
	int ntoken;
	int *token;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_file_descriptor(server, name, nameCnt, fileport, ntoken, token, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	mach_port_t fileport,
	int ntoken,
	int *token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_file_descriptor */
mig_internal novalue _X_notify_server_register_file_descriptor
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t fileport;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int ntoken;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_file_descriptor_t __Request;
	typedef __Reply___notify_server_register_file_descriptor_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_file_descriptor_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_file_descriptor_t__defined */

	__DeclareRcvRpc(78945672, "_notify_server_register_file_descriptor")
	__BeforeRcvRpc(78945672, "_notify_server_register_file_descriptor")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_file_descriptor_t__defined)
	check_result = __MIG_check__Request___notify_server_register_file_descriptor_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_file_descriptor_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_file_descriptor(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->fileport.name, In0P->ntoken, &OutP->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945672, "_notify_server_register_file_descriptor")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_mach_port_t__defined)
#define __MIG_check__Request___notify_server_register_mach_port_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_mach_port_t(__attribute__((__unused__)) __Request___notify_server_register_mach_port_t *In0P)
{

	typedef __Request___notify_server_register_mach_port_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 2) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->port.type != MACH_MSG_PORT_DESCRIPTOR ||
	    In0P->port.disposition != 17)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_mach_port_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_mach_port */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_mach_port
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, port, ntoken, token, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	mach_port_t port;
	int ntoken;
	int *token;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_mach_port(server, name, nameCnt, port, ntoken, token, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	mach_port_t port,
	int ntoken,
	int *token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_mach_port */
mig_internal novalue _X_notify_server_register_mach_port
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int ntoken;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_mach_port_t __Request;
	typedef __Reply___notify_server_register_mach_port_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_mach_port_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_mach_port_t__defined */

	__DeclareRcvRpc(78945673, "_notify_server_register_mach_port")
	__BeforeRcvRpc(78945673, "_notify_server_register_mach_port")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_mach_port_t__defined)
	check_result = __MIG_check__Request___notify_server_register_mach_port_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_mach_port_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_mach_port(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->port.name, In0P->ntoken, &OutP->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945673, "_notify_server_register_mach_port")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_set_owner_t__defined)
#define __MIG_check__Request___notify_server_set_owner_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_set_owner_t(__attribute__((__unused__)) __Request___notify_server_set_owner_t *In0P)
{

	typedef __Request___notify_server_set_owner_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_set_owner_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_owner */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_set_owner
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, user, group, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int user;
	int group;
	int *status;
	audit_token_t audit;
{ return __notify_server_set_owner(server, name, nameCnt, user, group, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int user,
	int group,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_set_owner */
mig_internal novalue _X_notify_server_set_owner
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int user;
		int group;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_set_owner_t __Request;
	typedef __Reply___notify_server_set_owner_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_set_owner_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_set_owner_t__defined */

	__DeclareRcvRpc(78945674, "_notify_server_set_owner")
	__BeforeRcvRpc(78945674, "_notify_server_set_owner")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_set_owner_t__defined)
	check_result = __MIG_check__Request___notify_server_set_owner_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_set_owner_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_set_owner(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->user, In0P->group, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945674, "_notify_server_set_owner")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_get_owner_t__defined)
#define __MIG_check__Request___notify_server_get_owner_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_get_owner_t(__attribute__((__unused__)) __Request___notify_server_get_owner_t *In0P)
{

	typedef __Request___notify_server_get_owner_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_get_owner_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_owner */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_get_owner
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, user, group, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *user;
	int *group;
	int *status;
	audit_token_t audit;
{ return __notify_server_get_owner(server, name, nameCnt, user, group, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *user,
	int *group,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_get_owner */
mig_internal novalue _X_notify_server_get_owner
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_get_owner_t __Request;
	typedef __Reply___notify_server_get_owner_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_get_owner_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_get_owner_t__defined */

	__DeclareRcvRpc(78945675, "_notify_server_get_owner")
	__BeforeRcvRpc(78945675, "_notify_server_get_owner")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_get_owner_t__defined)
	check_result = __MIG_check__Request___notify_server_get_owner_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_get_owner_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_get_owner(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->user, &OutP->group, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945675, "_notify_server_get_owner")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_set_access_t__defined)
#define __MIG_check__Request___notify_server_set_access_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_set_access_t(__attribute__((__unused__)) __Request___notify_server_set_access_t *In0P)
{

	typedef __Request___notify_server_set_access_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_set_access_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_access */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_set_access
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, mode, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int mode;
	int *status;
	audit_token_t audit;
{ return __notify_server_set_access(server, name, nameCnt, mode, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int mode,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_set_access */
mig_internal novalue _X_notify_server_set_access
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int mode;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_set_access_t __Request;
	typedef __Reply___notify_server_set_access_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_set_access_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_set_access_t__defined */

	__DeclareRcvRpc(78945676, "_notify_server_set_access")
	__BeforeRcvRpc(78945676, "_notify_server_set_access")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_set_access_t__defined)
	check_result = __MIG_check__Request___notify_server_set_access_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_set_access_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_set_access(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->mode, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945676, "_notify_server_set_access")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_get_access_t__defined)
#define __MIG_check__Request___notify_server_get_access_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_get_access_t(__attribute__((__unused__)) __Request___notify_server_get_access_t *In0P)
{

	typedef __Request___notify_server_get_access_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_get_access_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_access */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_get_access
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, mode, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *mode;
	int *status;
	audit_token_t audit;
{ return __notify_server_get_access(server, name, nameCnt, mode, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *mode,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_get_access */
mig_internal novalue _X_notify_server_get_access
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_get_access_t __Request;
	typedef __Reply___notify_server_get_access_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_get_access_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_get_access_t__defined */

	__DeclareRcvRpc(78945677, "_notify_server_get_access")
	__BeforeRcvRpc(78945677, "_notify_server_get_access")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_get_access_t__defined)
	check_result = __MIG_check__Request___notify_server_get_access_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_get_access_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_get_access(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->mode, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945677, "_notify_server_get_access")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_release_name_t__defined)
#define __MIG_check__Request___notify_server_release_name_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_release_name_t(__attribute__((__unused__)) __Request___notify_server_release_name_t *In0P)
{

	typedef __Request___notify_server_release_name_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_release_name_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_release_name */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_release_name
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int *status;
	audit_token_t audit;
{ return __notify_server_release_name(server, name, nameCnt, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_release_name */
mig_internal novalue _X_notify_server_release_name
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_release_name_t __Request;
	typedef __Reply___notify_server_release_name_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_release_name_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_release_name_t__defined */

	__DeclareRcvRpc(78945678, "_notify_server_release_name")
	__BeforeRcvRpc(78945678, "_notify_server_release_name")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_release_name_t__defined)
	check_result = __MIG_check__Request___notify_server_release_name_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_release_name_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_release_name(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945678, "_notify_server_release_name")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_cancel_t__defined)
#define __MIG_check__Request___notify_server_cancel_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_cancel_t(__attribute__((__unused__)) __Request___notify_server_cancel_t *In0P)
{

	typedef __Request___notify_server_cancel_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_cancel_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_cancel */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_cancel
#if	defined(LINTLIBRARY)
    (server, token, status, audit)
	mach_port_t server;
	int token;
	int *status;
	audit_token_t audit;
{ return __notify_server_cancel(server, token, status, audit); }
#else
(
	mach_port_t server,
	int token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_cancel */
mig_internal novalue _X_notify_server_cancel
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_cancel_t __Request;
	typedef __Reply___notify_server_cancel_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_cancel_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_cancel_t__defined */

	__DeclareRcvRpc(78945679, "_notify_server_cancel")
	__BeforeRcvRpc(78945679, "_notify_server_cancel")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_cancel_t__defined)
	check_result = __MIG_check__Request___notify_server_cancel_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_cancel_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_cancel(In0P->Head.msgh_request_port, In0P->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945679, "_notify_server_cancel")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_check_t__defined)
#define __MIG_check__Request___notify_server_check_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_check_t(__attribute__((__unused__)) __Request___notify_server_check_t *In0P)
{

	typedef __Request___notify_server_check_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_check_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_check */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_check
#if	defined(LINTLIBRARY)
    (server, token, check, status, audit)
	mach_port_t server;
	int token;
	int *check;
	int *status;
	audit_token_t audit;
{ return __notify_server_check(server, token, check, status, audit); }
#else
(
	mach_port_t server,
	int token,
	int *check,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_check */
mig_internal novalue _X_notify_server_check
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_check_t __Request;
	typedef __Reply___notify_server_check_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_check_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_check_t__defined */

	__DeclareRcvRpc(78945680, "_notify_server_check")
	__BeforeRcvRpc(78945680, "_notify_server_check")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_check_t__defined)
	check_result = __MIG_check__Request___notify_server_check_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_check_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_check(In0P->Head.msgh_request_port, In0P->token, &OutP->check, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945680, "_notify_server_check")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_get_state_t__defined)
#define __MIG_check__Request___notify_server_get_state_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_get_state_t(__attribute__((__unused__)) __Request___notify_server_get_state_t *In0P)
{

	typedef __Request___notify_server_get_state_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_get_state_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_get_state
#if	defined(LINTLIBRARY)
    (server, token, state, status, audit)
	mach_port_t server;
	int token;
	uint64_t *state;
	int *status;
	audit_token_t audit;
{ return __notify_server_get_state(server, token, state, status, audit); }
#else
(
	mach_port_t server,
	int token,
	uint64_t *state,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_get_state */
mig_internal novalue _X_notify_server_get_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_get_state_t __Request;
	typedef __Reply___notify_server_get_state_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_get_state_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_get_state_t__defined */

	__DeclareRcvRpc(78945681, "_notify_server_get_state")
	__BeforeRcvRpc(78945681, "_notify_server_get_state")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_get_state_t__defined)
	check_result = __MIG_check__Request___notify_server_get_state_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_get_state_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_get_state(In0P->Head.msgh_request_port, In0P->token, &OutP->state, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945681, "_notify_server_get_state")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_set_state_t__defined)
#define __MIG_check__Request___notify_server_set_state_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_set_state_t(__attribute__((__unused__)) __Request___notify_server_set_state_t *In0P)
{

	typedef __Request___notify_server_set_state_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_set_state_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_state */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_set_state
#if	defined(LINTLIBRARY)
    (server, token, state, status, audit)
	mach_port_t server;
	int token;
	uint64_t state;
	int *status;
	audit_token_t audit;
{ return __notify_server_set_state(server, token, state, status, audit); }
#else
(
	mach_port_t server,
	int token,
	uint64_t state,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_set_state */
mig_internal novalue _X_notify_server_set_state
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		uint64_t state;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_set_state_t __Request;
	typedef __Reply___notify_server_set_state_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_set_state_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_set_state_t__defined */

	__DeclareRcvRpc(78945682, "_notify_server_set_state")
	__BeforeRcvRpc(78945682, "_notify_server_set_state")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_set_state_t__defined)
	check_result = __MIG_check__Request___notify_server_set_state_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_set_state_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_set_state(In0P->Head.msgh_request_port, In0P->token, In0P->state, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945682, "_notify_server_set_state")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_monitor_file_t__defined)
#define __MIG_check__Request___notify_server_monitor_file_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_monitor_file_t(__attribute__((__unused__)) __Request___notify_server_monitor_file_t *In0P)
{

	typedef __Request___notify_server_monitor_file_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->path.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->path.size != In0P->pathCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_monitor_file_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_monitor_file */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_monitor_file
#if	defined(LINTLIBRARY)
    (server, token, path, pathCnt, flags, status, audit)
	mach_port_t server;
	int token;
	caddr_t path;
	mach_msg_type_number_t pathCnt;
	int flags;
	int *status;
	audit_token_t audit;
{ return __notify_server_monitor_file(server, token, path, pathCnt, flags, status, audit); }
#else
(
	mach_port_t server,
	int token,
	caddr_t path,
	mach_msg_type_number_t pathCnt,
	int flags,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_monitor_file */
mig_internal novalue _X_notify_server_monitor_file
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_type_number_t pathCnt;
		int flags;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_monitor_file_t __Request;
	typedef __Reply___notify_server_monitor_file_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_monitor_file_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_monitor_file_t__defined */

	__DeclareRcvRpc(78945685, "_notify_server_monitor_file")
	__BeforeRcvRpc(78945685, "_notify_server_monitor_file")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_monitor_file_t__defined)
	check_result = __MIG_check__Request___notify_server_monitor_file_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_monitor_file_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_monitor_file(In0P->Head.msgh_request_port, In0P->token, (caddr_t)(In0P->path.address), In0P->pathCnt, In0P->flags, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945685, "_notify_server_monitor_file")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_suspend_t__defined)
#define __MIG_check__Request___notify_server_suspend_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_suspend_t(__attribute__((__unused__)) __Request___notify_server_suspend_t *In0P)
{

	typedef __Request___notify_server_suspend_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_suspend_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_suspend */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_suspend
#if	defined(LINTLIBRARY)
    (server, token, status, audit)
	mach_port_t server;
	int token;
	int *status;
	audit_token_t audit;
{ return __notify_server_suspend(server, token, status, audit); }
#else
(
	mach_port_t server,
	int token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_suspend */
mig_internal novalue _X_notify_server_suspend
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_suspend_t __Request;
	typedef __Reply___notify_server_suspend_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_suspend_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_suspend_t__defined */

	__DeclareRcvRpc(78945686, "_notify_server_suspend")
	__BeforeRcvRpc(78945686, "_notify_server_suspend")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_suspend_t__defined)
	check_result = __MIG_check__Request___notify_server_suspend_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_suspend_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_suspend(In0P->Head.msgh_request_port, In0P->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945686, "_notify_server_suspend")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_resume_t__defined)
#define __MIG_check__Request___notify_server_resume_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_resume_t(__attribute__((__unused__)) __Request___notify_server_resume_t *In0P)
{

	typedef __Request___notify_server_resume_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_resume_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_resume */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_resume
#if	defined(LINTLIBRARY)
    (server, token, status, audit)
	mach_port_t server;
	int token;
	int *status;
	audit_token_t audit;
{ return __notify_server_resume(server, token, status, audit); }
#else
(
	mach_port_t server,
	int token,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_resume */
mig_internal novalue _X_notify_server_resume
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_resume_t __Request;
	typedef __Reply___notify_server_resume_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_resume_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_resume_t__defined */

	__DeclareRcvRpc(78945687, "_notify_server_resume")
	__BeforeRcvRpc(78945687, "_notify_server_resume")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_resume_t__defined)
	check_result = __MIG_check__Request___notify_server_resume_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_resume_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_resume(In0P->Head.msgh_request_port, In0P->token, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945687, "_notify_server_resume")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_suspend_pid_t__defined)
#define __MIG_check__Request___notify_server_suspend_pid_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_suspend_pid_t(__attribute__((__unused__)) __Request___notify_server_suspend_pid_t *In0P)
{

	typedef __Request___notify_server_suspend_pid_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_suspend_pid_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_suspend_pid */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_suspend_pid
#if	defined(LINTLIBRARY)
    (server, pid, status, audit)
	mach_port_t server;
	int pid;
	int *status;
	audit_token_t audit;
{ return __notify_server_suspend_pid(server, pid, status, audit); }
#else
(
	mach_port_t server,
	int pid,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_suspend_pid */
mig_internal novalue _X_notify_server_suspend_pid
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int pid;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_suspend_pid_t __Request;
	typedef __Reply___notify_server_suspend_pid_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_suspend_pid_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_suspend_pid_t__defined */

	__DeclareRcvRpc(78945688, "_notify_server_suspend_pid")
	__BeforeRcvRpc(78945688, "_notify_server_suspend_pid")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_suspend_pid_t__defined)
	check_result = __MIG_check__Request___notify_server_suspend_pid_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_suspend_pid_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_suspend_pid(In0P->Head.msgh_request_port, In0P->pid, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945688, "_notify_server_suspend_pid")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_resume_pid_t__defined)
#define __MIG_check__Request___notify_server_resume_pid_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_resume_pid_t(__attribute__((__unused__)) __Request___notify_server_resume_pid_t *In0P)
{

	typedef __Request___notify_server_resume_pid_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_resume_pid_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_resume_pid */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_resume_pid
#if	defined(LINTLIBRARY)
    (server, pid, status, audit)
	mach_port_t server;
	int pid;
	int *status;
	audit_token_t audit;
{ return __notify_server_resume_pid(server, pid, status, audit); }
#else
(
	mach_port_t server,
	int pid,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_resume_pid */
mig_internal novalue _X_notify_server_resume_pid
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int pid;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_resume_pid_t __Request;
	typedef __Reply___notify_server_resume_pid_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_resume_pid_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_resume_pid_t__defined */

	__DeclareRcvRpc(78945689, "_notify_server_resume_pid")
	__BeforeRcvRpc(78945689, "_notify_server_resume_pid")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_resume_pid_t__defined)
	check_result = __MIG_check__Request___notify_server_resume_pid_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_resume_pid_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_resume_pid(In0P->Head.msgh_request_port, In0P->pid, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945689, "_notify_server_resume_pid")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_simple_post_t__defined)
#define __MIG_check__Request___notify_server_simple_post_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_simple_post_t(__attribute__((__unused__)) __Request___notify_server_simple_post_t *In0P)
{

	typedef __Request___notify_server_simple_post_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_simple_post_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_simple_post */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_simple_post
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	audit_token_t audit;
{ return __notify_server_simple_post(server, name, nameCnt, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_simple_post */
mig_internal novalue _X_notify_server_simple_post
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_simple_post_t __Request;
	typedef __Reply___notify_server_simple_post_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_simple_post_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_simple_post_t__defined */

	__DeclareRcvSimple(78945690, "_notify_server_simple_post")
	__BeforeRcvSimple(78945690, "_notify_server_simple_post")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_simple_post_t__defined)
	check_result = __MIG_check__Request___notify_server_simple_post_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_simple_post_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_simple_post(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, TrailerP->msgh_audit);
	__AfterRcvSimple(78945690, "_notify_server_simple_post")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_post_2_t__defined)
#define __MIG_check__Request___notify_server_post_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_post_2_t(__attribute__((__unused__)) __Request___notify_server_post_2_t *In0P)
{

	typedef __Request___notify_server_post_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_post_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_post_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_post_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, name_id, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	uint64_t *name_id;
	int *status;
	audit_token_t audit;
{ return __notify_server_post_2(server, name, nameCnt, name_id, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	uint64_t *name_id,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_post_2 */
mig_internal novalue _X_notify_server_post_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_post_2_t __Request;
	typedef __Reply___notify_server_post_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_post_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_post_2_t__defined */

	__DeclareRcvRpc(78945691, "_notify_server_post_2")
	__BeforeRcvRpc(78945691, "_notify_server_post_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_post_2_t__defined)
	check_result = __MIG_check__Request___notify_server_post_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_post_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_post_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, &OutP->name_id, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945691, "_notify_server_post_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_post_3_t__defined)
#define __MIG_check__Request___notify_server_post_3_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_post_3_t(__attribute__((__unused__)) __Request___notify_server_post_3_t *In0P)
{

	typedef __Request___notify_server_post_3_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_post_3_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_post_3 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_post_3
#if	defined(LINTLIBRARY)
    (server, name_id, audit)
	mach_port_t server;
	uint64_t name_id;
	audit_token_t audit;
{ return __notify_server_post_3(server, name_id, audit); }
#else
(
	mach_port_t server,
	uint64_t name_id,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_post_3 */
mig_internal novalue _X_notify_server_post_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_post_3_t __Request;
	typedef __Reply___notify_server_post_3_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_post_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_post_3_t__defined */

	__DeclareRcvSimple(78945692, "_notify_server_post_3")
	__BeforeRcvSimple(78945692, "_notify_server_post_3")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_post_3_t__defined)
	check_result = __MIG_check__Request___notify_server_post_3_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_post_3_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_post_3(In0P->Head.msgh_request_port, In0P->name_id, TrailerP->msgh_audit);
	__AfterRcvSimple(78945692, "_notify_server_post_3")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_post_4_t__defined)
#define __MIG_check__Request___notify_server_post_4_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_post_4_t(__attribute__((__unused__)) __Request___notify_server_post_4_t *In0P)
{

	typedef __Request___notify_server_post_4_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_post_4_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_post_4 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_post_4
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	audit_token_t audit;
{ return __notify_server_post_4(server, name, nameCnt, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_post_4 */
mig_internal novalue _X_notify_server_post_4
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_post_4_t __Request;
	typedef __Reply___notify_server_post_4_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_post_4_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_post_4_t__defined */

	__DeclareRcvSimple(78945693, "_notify_server_post_4")
	__BeforeRcvSimple(78945693, "_notify_server_post_4")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_post_4_t__defined)
	check_result = __MIG_check__Request___notify_server_post_4_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_post_4_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_post_4(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, TrailerP->msgh_audit);
	__AfterRcvSimple(78945693, "_notify_server_post_4")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_plain_2_t__defined)
#define __MIG_check__Request___notify_server_register_plain_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_plain_2_t(__attribute__((__unused__)) __Request___notify_server_register_plain_2_t *In0P)
{

	typedef __Request___notify_server_register_plain_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_plain_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_register_plain_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_plain_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	audit_token_t audit;
{ return __notify_server_register_plain_2(server, name, nameCnt, token, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_register_plain_2 */
mig_internal novalue _X_notify_server_register_plain_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_plain_2_t __Request;
	typedef __Reply___notify_server_register_plain_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_plain_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_plain_2_t__defined */

	__DeclareRcvSimple(78945694, "_notify_server_register_plain_2")
	__BeforeRcvSimple(78945694, "_notify_server_register_plain_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_plain_2_t__defined)
	check_result = __MIG_check__Request___notify_server_register_plain_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_plain_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_plain_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, TrailerP->msgh_audit);
	__AfterRcvSimple(78945694, "_notify_server_register_plain_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_check_2_t__defined)
#define __MIG_check__Request___notify_server_register_check_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_check_2_t(__attribute__((__unused__)) __Request___notify_server_register_check_2_t *In0P)
{

	typedef __Request___notify_server_register_check_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_check_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_check_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_check_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, size, slot, name_id, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	int *size;
	int *slot;
	uint64_t *name_id;
	int *status;
	audit_token_t audit;
{ return __notify_server_register_check_2(server, name, nameCnt, token, size, slot, name_id, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	int *size,
	int *slot,
	uint64_t *name_id,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_register_check_2 */
mig_internal novalue _X_notify_server_register_check_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_check_2_t __Request;
	typedef __Reply___notify_server_register_check_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_check_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_check_2_t__defined */

	__DeclareRcvRpc(78945695, "_notify_server_register_check_2")
	__BeforeRcvRpc(78945695, "_notify_server_register_check_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_check_2_t__defined)
	check_result = __MIG_check__Request___notify_server_register_check_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_check_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_check_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, &OutP->size, &OutP->slot, &OutP->name_id, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945695, "_notify_server_register_check_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_signal_2_t__defined)
#define __MIG_check__Request___notify_server_register_signal_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_signal_2_t(__attribute__((__unused__)) __Request___notify_server_register_signal_2_t *In0P)
{

	typedef __Request___notify_server_register_signal_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_signal_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_register_signal_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_signal_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, sig, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	int sig;
	audit_token_t audit;
{ return __notify_server_register_signal_2(server, name, nameCnt, token, sig, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	int sig,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_register_signal_2 */
mig_internal novalue _X_notify_server_register_signal_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		int sig;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_signal_2_t __Request;
	typedef __Reply___notify_server_register_signal_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_signal_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_signal_2_t__defined */

	__DeclareRcvSimple(78945696, "_notify_server_register_signal_2")
	__BeforeRcvSimple(78945696, "_notify_server_register_signal_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_signal_2_t__defined)
	check_result = __MIG_check__Request___notify_server_register_signal_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_signal_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_signal_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, In0P->sig, TrailerP->msgh_audit);
	__AfterRcvSimple(78945696, "_notify_server_register_signal_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_file_descriptor_2_t__defined)
#define __MIG_check__Request___notify_server_register_file_descriptor_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_file_descriptor_2_t(__attribute__((__unused__)) __Request___notify_server_register_file_descriptor_2_t *In0P)
{

	typedef __Request___notify_server_register_file_descriptor_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 2) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->fileport.type != MACH_MSG_PORT_DESCRIPTOR ||
	    In0P->fileport.disposition != 17)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_file_descriptor_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_register_file_descriptor_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_file_descriptor_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, fileport, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	mach_port_t fileport;
	audit_token_t audit;
{ return __notify_server_register_file_descriptor_2(server, name, nameCnt, token, fileport, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	mach_port_t fileport,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_register_file_descriptor_2 */
mig_internal novalue _X_notify_server_register_file_descriptor_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t fileport;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_file_descriptor_2_t __Request;
	typedef __Reply___notify_server_register_file_descriptor_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_file_descriptor_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_file_descriptor_2_t__defined */

	__DeclareRcvSimple(78945697, "_notify_server_register_file_descriptor_2")
	__BeforeRcvSimple(78945697, "_notify_server_register_file_descriptor_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_file_descriptor_2_t__defined)
	check_result = __MIG_check__Request___notify_server_register_file_descriptor_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_file_descriptor_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_file_descriptor_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, In0P->fileport.name, TrailerP->msgh_audit);
	__AfterRcvSimple(78945697, "_notify_server_register_file_descriptor_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_register_mach_port_2_t__defined)
#define __MIG_check__Request___notify_server_register_mach_port_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_register_mach_port_2_t(__attribute__((__unused__)) __Request___notify_server_register_mach_port_2_t *In0P)
{

	typedef __Request___notify_server_register_mach_port_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 2) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->port.type != MACH_MSG_PORT_DESCRIPTOR ||
	    In0P->port.disposition != 17)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_register_mach_port_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_register_mach_port_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_register_mach_port_2
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, port, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	mach_port_t port;
	audit_token_t audit;
{ return __notify_server_register_mach_port_2(server, name, nameCnt, token, port, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	mach_port_t port,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_register_mach_port_2 */
mig_internal novalue _X_notify_server_register_mach_port_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_register_mach_port_2_t __Request;
	typedef __Reply___notify_server_register_mach_port_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_register_mach_port_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_register_mach_port_2_t__defined */

	__DeclareRcvSimple(78945698, "_notify_server_register_mach_port_2")
	__BeforeRcvSimple(78945698, "_notify_server_register_mach_port_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_register_mach_port_2_t__defined)
	check_result = __MIG_check__Request___notify_server_register_mach_port_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_register_mach_port_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_register_mach_port_2(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, In0P->port.name, TrailerP->msgh_audit);
	__AfterRcvSimple(78945698, "_notify_server_register_mach_port_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_cancel_2_t__defined)
#define __MIG_check__Request___notify_server_cancel_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_cancel_2_t(__attribute__((__unused__)) __Request___notify_server_cancel_2_t *In0P)
{

	typedef __Request___notify_server_cancel_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_cancel_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_cancel_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_cancel_2
#if	defined(LINTLIBRARY)
    (server, token, audit)
	mach_port_t server;
	int token;
	audit_token_t audit;
{ return __notify_server_cancel_2(server, token, audit); }
#else
(
	mach_port_t server,
	int token,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_cancel_2 */
mig_internal novalue _X_notify_server_cancel_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_cancel_2_t __Request;
	typedef __Reply___notify_server_cancel_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_cancel_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_cancel_2_t__defined */

	__DeclareRcvSimple(78945699, "_notify_server_cancel_2")
	__BeforeRcvSimple(78945699, "_notify_server_cancel_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_cancel_2_t__defined)
	check_result = __MIG_check__Request___notify_server_cancel_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_cancel_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_cancel_2(In0P->Head.msgh_request_port, In0P->token, TrailerP->msgh_audit);
	__AfterRcvSimple(78945699, "_notify_server_cancel_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_get_state_2_t__defined)
#define __MIG_check__Request___notify_server_get_state_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_get_state_2_t(__attribute__((__unused__)) __Request___notify_server_get_state_2_t *In0P)
{

	typedef __Request___notify_server_get_state_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_get_state_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_get_state_2
#if	defined(LINTLIBRARY)
    (server, name_id, state, status, audit)
	mach_port_t server;
	uint64_t name_id;
	uint64_t *state;
	int *status;
	audit_token_t audit;
{ return __notify_server_get_state_2(server, name_id, state, status, audit); }
#else
(
	mach_port_t server,
	uint64_t name_id,
	uint64_t *state,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_get_state_2 */
mig_internal novalue _X_notify_server_get_state_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_get_state_2_t __Request;
	typedef __Reply___notify_server_get_state_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_get_state_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_get_state_2_t__defined */

	__DeclareRcvRpc(78945700, "_notify_server_get_state_2")
	__BeforeRcvRpc(78945700, "_notify_server_get_state_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_get_state_2_t__defined)
	check_result = __MIG_check__Request___notify_server_get_state_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_get_state_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_get_state_2(In0P->Head.msgh_request_port, In0P->name_id, &OutP->state, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945700, "_notify_server_get_state_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_get_state_3_t__defined)
#define __MIG_check__Request___notify_server_get_state_3_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_get_state_3_t(__attribute__((__unused__)) __Request___notify_server_get_state_3_t *In0P)
{

	typedef __Request___notify_server_get_state_3_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_get_state_3_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state_3 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_get_state_3
#if	defined(LINTLIBRARY)
    (server, token, state, nid, status, audit)
	mach_port_t server;
	int token;
	uint64_t *state;
	uint64_t *nid;
	int *status;
	audit_token_t audit;
{ return __notify_server_get_state_3(server, token, state, nid, status, audit); }
#else
(
	mach_port_t server,
	int token,
	uint64_t *state,
	uint64_t *nid,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_get_state_3 */
mig_internal novalue _X_notify_server_get_state_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_get_state_3_t __Request;
	typedef __Reply___notify_server_get_state_3_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_get_state_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_get_state_3_t__defined */

	__DeclareRcvRpc(78945701, "_notify_server_get_state_3")
	__BeforeRcvRpc(78945701, "_notify_server_get_state_3")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_get_state_3_t__defined)
	check_result = __MIG_check__Request___notify_server_get_state_3_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_get_state_3_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_get_state_3(In0P->Head.msgh_request_port, In0P->token, &OutP->state, &OutP->nid, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945701, "_notify_server_get_state_3")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_set_state_2_t__defined)
#define __MIG_check__Request___notify_server_set_state_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_set_state_2_t(__attribute__((__unused__)) __Request___notify_server_set_state_2_t *In0P)
{

	typedef __Request___notify_server_set_state_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_set_state_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_set_state_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_set_state_2
#if	defined(LINTLIBRARY)
    (server, name_id, state, audit)
	mach_port_t server;
	uint64_t name_id;
	uint64_t state;
	audit_token_t audit;
{ return __notify_server_set_state_2(server, name_id, state, audit); }
#else
(
	mach_port_t server,
	uint64_t name_id,
	uint64_t state,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_set_state_2 */
mig_internal novalue _X_notify_server_set_state_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		uint64_t name_id;
		uint64_t state;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_set_state_2_t __Request;
	typedef __Reply___notify_server_set_state_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_set_state_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_set_state_2_t__defined */

	__DeclareRcvSimple(78945702, "_notify_server_set_state_2")
	__BeforeRcvSimple(78945702, "_notify_server_set_state_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_set_state_2_t__defined)
	check_result = __MIG_check__Request___notify_server_set_state_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_set_state_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_set_state_2(In0P->Head.msgh_request_port, In0P->name_id, In0P->state, TrailerP->msgh_audit);
	__AfterRcvSimple(78945702, "_notify_server_set_state_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_set_state_3_t__defined)
#define __MIG_check__Request___notify_server_set_state_3_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_set_state_3_t(__attribute__((__unused__)) __Request___notify_server_set_state_3_t *In0P)
{

	typedef __Request___notify_server_set_state_3_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 0) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_set_state_3_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_state_3 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_set_state_3
#if	defined(LINTLIBRARY)
    (server, token, state, nid, status, audit)
	mach_port_t server;
	int token;
	uint64_t state;
	uint64_t *nid;
	int *status;
	audit_token_t audit;
{ return __notify_server_set_state_3(server, token, state, nid, status, audit); }
#else
(
	mach_port_t server,
	int token,
	uint64_t state,
	uint64_t *nid,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_set_state_3 */
mig_internal novalue _X_notify_server_set_state_3
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		uint64_t state;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_set_state_3_t __Request;
	typedef __Reply___notify_server_set_state_3_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_set_state_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_set_state_3_t__defined */

	__DeclareRcvRpc(78945703, "_notify_server_set_state_3")
	__BeforeRcvRpc(78945703, "_notify_server_set_state_3")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_set_state_3_t__defined)
	check_result = __MIG_check__Request___notify_server_set_state_3_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_set_state_3_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_set_state_3(In0P->Head.msgh_request_port, In0P->token, In0P->state, &OutP->nid, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945703, "_notify_server_set_state_3")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_monitor_file_2_t__defined)
#define __MIG_check__Request___notify_server_monitor_file_2_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_monitor_file_2_t(__attribute__((__unused__)) __Request___notify_server_monitor_file_2_t *In0P)
{

	typedef __Request___notify_server_monitor_file_2_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 1) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->path.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->path.size != In0P->pathCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_monitor_file_2_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* SimpleRoutine _notify_server_monitor_file_2 */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_monitor_file_2
#if	defined(LINTLIBRARY)
    (server, token, path, pathCnt, flags, audit)
	mach_port_t server;
	int token;
	caddr_t path;
	mach_msg_type_number_t pathCnt;
	int flags;
	audit_token_t audit;
{ return __notify_server_monitor_file_2(server, token, path, pathCnt, flags, audit); }
#else
(
	mach_port_t server,
	int token,
	caddr_t path,
	mach_msg_type_number_t pathCnt,
	int flags,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* SimpleRoutine _notify_server_monitor_file_2 */
mig_internal novalue _X_notify_server_monitor_file_2
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		int token;
		mach_msg_type_number_t pathCnt;
		int flags;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_monitor_file_2_t __Request;
	typedef __Reply___notify_server_monitor_file_2_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_monitor_file_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_monitor_file_2_t__defined */

	__DeclareRcvSimple(78945704, "_notify_server_monitor_file_2")
	__BeforeRcvSimple(78945704, "_notify_server_monitor_file_2")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_monitor_file_2_t__defined)
	check_result = __MIG_check__Request___notify_server_monitor_file_2_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_monitor_file_2_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_monitor_file_2(In0P->Head.msgh_request_port, In0P->token, (caddr_t)(In0P->path.address), In0P->pathCnt, In0P->flags, TrailerP->msgh_audit);
	__AfterRcvSimple(78945704, "_notify_server_monitor_file_2")
}

#if ( __MigTypeCheck )
#if __MIG_check__Request__notify_ipc_subsystem__
#if !defined(__MIG_check__Request___notify_server_regenerate_t__defined)
#define __MIG_check__Request___notify_server_regenerate_t__defined

mig_internal kern_return_t __MIG_check__Request___notify_server_regenerate_t(__attribute__((__unused__)) __Request___notify_server_regenerate_t *In0P)
{

	typedef __Request___notify_server_regenerate_t __Request;
#if	__MigTypeCheck
	if (!(In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    (In0P->msgh_body.msgh_descriptor_count != 3) ||
	    (In0P->Head.msgh_size != (mach_msg_size_t)sizeof(__Request)))
		return MIG_BAD_ARGUMENTS;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->name.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->port.type != MACH_MSG_PORT_DESCRIPTOR ||
	    In0P->port.disposition != 17)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if	__MigTypeCheck
	if (In0P->path.type != MACH_MSG_OOL_DESCRIPTOR)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

#if __MigTypeCheck
	if (In0P->name.size != In0P->nameCnt)
		return MIG_TYPE_ERROR;
	if (In0P->path.size != In0P->pathCnt)
		return MIG_TYPE_ERROR;
#endif	/* __MigTypeCheck */

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Request___notify_server_regenerate_t__defined) */
#endif /* __MIG_check__Request__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_regenerate */
#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
kern_return_t __notify_server_regenerate
#if	defined(LINTLIBRARY)
    (server, name, nameCnt, token, reg_type, port, sig, prev_slot, prev_state, prev_time, path, pathCnt, path_flags, new_slot, new_name_id, status, audit)
	mach_port_t server;
	caddr_t name;
	mach_msg_type_number_t nameCnt;
	int token;
	uint32_t reg_type;
	mach_port_t port;
	int sig;
	int prev_slot;
	uint64_t prev_state;
	uint64_t prev_time;
	caddr_t path;
	mach_msg_type_number_t pathCnt;
	int path_flags;
	int *new_slot;
	uint64_t *new_name_id;
	int *status;
	audit_token_t audit;
{ return __notify_server_regenerate(server, name, nameCnt, token, reg_type, port, sig, prev_slot, prev_state, prev_time, path, pathCnt, path_flags, new_slot, new_name_id, status, audit); }
#else
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	uint32_t reg_type,
	mach_port_t port,
	int sig,
	int prev_slot,
	uint64_t prev_state,
	uint64_t prev_time,
	caddr_t path,
	mach_msg_type_number_t pathCnt,
	int path_flags,
	int *new_slot,
	uint64_t *new_name_id,
	int *status,
	audit_token_t audit
);
#endif	/* defined(LINTLIBRARY) */

/* Routine _notify_server_regenerate */
mig_internal novalue _X_notify_server_regenerate
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{

#ifdef  __MigPackStructs
#pragma pack(4)
#endif
	typedef struct {
		mach_msg_header_t Head;
		/* start of the kernel processed data */
		mach_msg_body_t msgh_body;
		mach_msg_ool_descriptor_t name;
		mach_msg_port_descriptor_t port;
		mach_msg_ool_descriptor_t path;
		/* end of the kernel processed data */
		NDR_record_t NDR;
		mach_msg_type_number_t nameCnt;
		int token;
		uint32_t reg_type;
		int sig;
		int prev_slot;
		uint64_t prev_state;
		uint64_t prev_time;
		mach_msg_type_number_t pathCnt;
		int path_flags;
		mach_msg_max_trailer_t trailer;
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	typedef __Request___notify_server_regenerate_t __Request;
	typedef __Reply___notify_server_regenerate_t Reply;

	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	Request *In0P = (Request *) InHeadP;
	Reply *OutP = (Reply *) OutHeadP;
	mach_msg_max_trailer_t *TrailerP;
#if	__MigTypeCheck
	unsigned int trailer_size;
#endif	/* __MigTypeCheck */
#ifdef	__MIG_check__Request___notify_server_regenerate_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Request___notify_server_regenerate_t__defined */

	__DeclareRcvRpc(78945705, "_notify_server_regenerate")
	__BeforeRcvRpc(78945705, "_notify_server_regenerate")
/* RetCArg=0x0 rtSimpleRequest=0 */

#if	defined(__MIG_check__Request___notify_server_regenerate_t__defined)
	check_result = __MIG_check__Request___notify_server_regenerate_t((__Request *)In0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ MIG_RETURN_ERROR(OutP, check_result); }
#endif	/* defined(__MIG_check__Request___notify_server_regenerate_t__defined) */

	TrailerP = (mach_msg_max_trailer_t *)((vm_offset_t)In0P +
		round_msg(In0P->Head.msgh_size));
	if (TrailerP->msgh_trailer_type != MACH_MSG_TRAILER_FORMAT_0)
		{ MIG_RETURN_ERROR(In0P, MIG_TRAILER_ERROR); }
#if	__MigTypeCheck
	trailer_size = TrailerP->msgh_trailer_size -
		(mach_msg_size_t)(sizeof(mach_msg_trailer_type_t) - sizeof(mach_msg_trailer_size_t));
#endif	/* __MigTypeCheck */
#if	__MigTypeCheck
	if (trailer_size < (mach_msg_size_t)sizeof(audit_token_t))
		{ MIG_RETURN_ERROR(OutP, MIG_TRAILER_ERROR); }
	trailer_size -= (mach_msg_size_t)sizeof(audit_token_t);
#endif	/* __MigTypeCheck */
	OutP->RetCode = __notify_server_regenerate(In0P->Head.msgh_request_port, (caddr_t)(In0P->name.address), In0P->nameCnt, In0P->token, In0P->reg_type, In0P->port.name, In0P->sig, In0P->prev_slot, In0P->prev_state, In0P->prev_time, (caddr_t)(In0P->path.address), In0P->pathCnt, In0P->path_flags, &OutP->new_slot, &OutP->new_name_id, &OutP->status, TrailerP->msgh_audit);
	if (OutP->RetCode != KERN_SUCCESS) {
		MIG_RETURN_ERROR(OutP, OutP->RetCode);
	}

	OutP->NDR = NDR_record;


	OutP->Head.msgh_size = (sizeof(Reply));
	__AfterRcvRpc(78945705, "_notify_server_regenerate")
}


#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
boolean_t notify_ipc_server(
		mach_msg_header_t *InHeadP,
		mach_msg_header_t *OutHeadP);

#ifdef	mig_external
mig_external
#else
extern
#endif	/* mig_external */
mig_routine_t notify_ipc_server_routine(
		mach_msg_header_t *InHeadP);


/* Description of this subsystem, for use in direct RPC */
const struct _notify_ipc_subsystem {
	mig_server_routine_t 	server;	/* Server routine */
	mach_msg_id_t	start;	/* Min routine number */
	mach_msg_id_t	end;	/* Max routine number + 1 */
	unsigned int	maxsize;	/* Max msg size */
	vm_address_t	reserved;	/* Reserved */
	struct routine_descriptor	/*Array of routine descriptors */
		routine[38];
} _notify_ipc_subsystem = {
	notify_ipc_server_routine,
	78945668,
	78945706,
	(mach_msg_size_t)sizeof(union __ReplyUnion___notify_ipc_subsystem),
	(vm_address_t)0,
	{
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_post, 12, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_post_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_plain, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_plain_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_check, 15, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_check_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_signal, 14, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_signal_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_file_descriptor, 15, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_file_descriptor_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_mach_port, 15, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_mach_port_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_set_owner, 14, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_set_owner_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_get_owner, 14, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_get_owner_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_set_access, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_set_access_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_get_access, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_get_access_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_release_name, 12, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_release_name_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_cancel, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_cancel_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_check, 12, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_check_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_get_state, 12, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_get_state_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_set_state, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_set_state_t) },
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_monitor_file, 14, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_monitor_file_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_suspend, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_suspend_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_resume, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_resume_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_suspend_pid, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_suspend_pid_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_resume_pid, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_resume_pid_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_simple_post, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_simple_post_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_post_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_post_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_post_3, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_post_3_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_post_4, 11, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_post_4_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_plain_2, 12, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_plain_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_check_2, 16, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_check_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_signal_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_signal_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_file_descriptor_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_file_descriptor_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_register_mach_port_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_register_mach_port_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_cancel_2, 10, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_cancel_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_get_state_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_get_state_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_get_state_3, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_get_state_3_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_set_state_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_set_state_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_set_state_3, 14, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_set_state_3_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_monitor_file_2, 13, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_monitor_file_2_t) },
          { (mig_impl_routine_t) 0,
            (mig_stub_routine_t) _X_notify_server_regenerate, 26, 0, (routine_arg_descriptor_t)0, (mach_msg_size_t)sizeof(__Reply___notify_server_regenerate_t) },
	}
};

mig_external boolean_t notify_ipc_server
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	register mig_routine_t routine;

	OutHeadP->msgh_bits = MACH_MSGH_BITS(MACH_MSGH_BITS_REPLY(InHeadP->msgh_bits), 0);
	OutHeadP->msgh_remote_port = InHeadP->msgh_reply_port;
	/* Minimal size: routine() will update it if different */
	OutHeadP->msgh_size = (mach_msg_size_t)sizeof(mig_reply_error_t);
	OutHeadP->msgh_local_port = MACH_PORT_NULL;
	OutHeadP->msgh_id = InHeadP->msgh_id + 100;

	if ((InHeadP->msgh_id > 78945705) || (InHeadP->msgh_id < 78945668) ||
	    ((routine = _notify_ipc_subsystem.routine[InHeadP->msgh_id - 78945668].stub_routine) == 0)) {
		((mig_reply_error_t *)OutHeadP)->NDR = NDR_record;
		((mig_reply_error_t *)OutHeadP)->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InHeadP, OutHeadP);
	return TRUE;
}

mig_external mig_routine_t notify_ipc_server_routine
	(mach_msg_header_t *InHeadP)
{
	register int msgh_id;

	msgh_id = InHeadP->msgh_id - 78945668;

	if ((msgh_id > 37) || (msgh_id < 0))
		return 0;

	return _notify_ipc_subsystem.routine[msgh_id].stub_routine;
}
