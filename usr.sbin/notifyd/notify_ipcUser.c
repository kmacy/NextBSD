/*
 * IDENTIFICATION:
 * stub generated Tue Apr 28 09:01:37 2015
 * with a MiG generated Mon Apr 27 08:45:27 CEST 2015 by root@testvm
 * OPTIONS: 
 */
#define	__MIG_check__Reply__notify_ipc_subsystem__ 1

#include "notify_ipc.h"
/* LINTLIBRARY */


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
#ifndef	__MachMsgErrorWithTimeout
#define	__MachMsgErrorWithTimeout(_R_) { \
	switch (_R_) { \
	case MACH_SEND_INVALID_REPLY: \
	case MACH_RCV_INVALID_NAME: \
	case MACH_RCV_IN_SET: \
	case MACH_RCV_PORT_DIED: \
	case MACH_RCV_PORT_CHANGED: \
	case MACH_SEND_INVALID_MEMORY: \
	case MACH_SEND_INVALID_RIGHT: \
	case MACH_SEND_INVALID_TYPE: \
	case MACH_SEND_MSG_TOO_SMALL: \
	case MACH_SEND_INVALID_RT_OOL_SIZE: \
	case MACH_RCV_TIMED_OUT: \
		mig_dealloc_reply_port(InP->Head.msgh_reply_port); \
	} \
}
#endif	/* __MachMsgErrorWithTimeout */

#ifndef	__MachMsgErrorWithoutTimeout
#define	__MachMsgErrorWithoutTimeout(_R_) { \
	switch (_R_) { \
	case MACH_SEND_INVALID_REPLY: \
	case MACH_RCV_INVALID_NAME: \
	case MACH_RCV_IN_SET: \
	case MACH_RCV_PORT_DIED: \
	case MACH_RCV_PORT_CHANGED: \
	case MACH_SEND_INVALID_MEMORY: \
	case MACH_SEND_INVALID_RIGHT: \
	case MACH_SEND_INVALID_TYPE: \
	case MACH_SEND_MSG_TOO_SMALL: \
	case MACH_SEND_INVALID_RT_OOL_SIZE: \
		mig_dealloc_reply_port(InP->Head.msgh_reply_port); \
	} \
}
#endif	/* __MachMsgErrorWithoutTimeout */

#ifndef	__DeclareSendRpc
#define	__DeclareSendRpc(_NUM_, _NAME_)
#endif	/* __DeclareSendRpc */

#ifndef	__BeforeSendRpc
#define	__BeforeSendRpc(_NUM_, _NAME_)
#endif	/* __BeforeSendRpc */

#ifndef	__AfterSendRpc
#define	__AfterSendRpc(_NUM_, _NAME_)
#endif	/* __AfterSendRpc */

#ifndef	__DeclareSendSimple
#define	__DeclareSendSimple(_NUM_, _NAME_)
#endif	/* __DeclareSendSimple */

#ifndef	__BeforeSendSimple
#define	__BeforeSendSimple(_NUM_, _NAME_)
#endif	/* __BeforeSendSimple */

#ifndef	__AfterSendSimple
#define	__AfterSendSimple(_NUM_, _NAME_)
#endif	/* __AfterSendSimple */

#define msgh_request_port	msgh_remote_port
#define msgh_reply_port		msgh_local_port



#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_post_t__defined)
#define __MIG_check__Reply___notify_server_post_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_post_t(__Reply___notify_server_post_t *Out0P)
{

	typedef __Reply___notify_server_post_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945768) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_post_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_post */
mig_external kern_return_t _notify_server_post
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_post_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_post_t__defined */

	__DeclareSendRpc(78945668, "_notify_server_post")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945668;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945668, "_notify_server_post")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945668, "_notify_server_post")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_post_t__defined)
	check_result = __MIG_check__Reply___notify_server_post_t((__Reply___notify_server_post_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_post_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_plain_t__defined)
#define __MIG_check__Reply___notify_server_register_plain_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_plain_t(__Reply___notify_server_register_plain_t *Out0P)
{

	typedef __Reply___notify_server_register_plain_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945769) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_plain_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_plain */
mig_external kern_return_t _notify_server_register_plain
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_plain_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_plain_t__defined */

	__DeclareSendRpc(78945669, "_notify_server_register_plain")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945669;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945669, "_notify_server_register_plain")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945669, "_notify_server_register_plain")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_plain_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_plain_t((__Reply___notify_server_register_plain_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_plain_t__defined) */

	*token = Out0P->token;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_check_t__defined)
#define __MIG_check__Reply___notify_server_register_check_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_check_t(__Reply___notify_server_register_check_t *Out0P)
{

	typedef __Reply___notify_server_register_check_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945770) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_check_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_check */
mig_external kern_return_t _notify_server_register_check
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *size,
	int *slot,
	int *token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_check_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_check_t__defined */

	__DeclareSendRpc(78945670, "_notify_server_register_check")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945670;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945670, "_notify_server_register_check")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945670, "_notify_server_register_check")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_check_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_check_t((__Reply___notify_server_register_check_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_check_t__defined) */

	*size = Out0P->size;

	*slot = Out0P->slot;

	*token = Out0P->token;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_signal_t__defined)
#define __MIG_check__Reply___notify_server_register_signal_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_signal_t(__Reply___notify_server_register_signal_t *Out0P)
{

	typedef __Reply___notify_server_register_signal_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945771) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_signal_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_signal */
mig_external kern_return_t _notify_server_register_signal
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int sig,
	int *token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_signal_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_signal_t__defined */

	__DeclareSendRpc(78945671, "_notify_server_register_signal")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->sig = sig;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945671;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945671, "_notify_server_register_signal")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945671, "_notify_server_register_signal")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_signal_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_signal_t((__Reply___notify_server_register_signal_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_signal_t__defined) */

	*token = Out0P->token;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_file_descriptor_t__defined)
#define __MIG_check__Reply___notify_server_register_file_descriptor_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_file_descriptor_t(__Reply___notify_server_register_file_descriptor_t *Out0P)
{

	typedef __Reply___notify_server_register_file_descriptor_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945772) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_file_descriptor_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_file_descriptor */
mig_external kern_return_t _notify_server_register_file_descriptor
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	mach_port_t fileport,
	int ntoken,
	int *token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_file_descriptor_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_file_descriptor_t__defined */

	__DeclareSendRpc(78945672, "_notify_server_register_file_descriptor")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_port_descriptor_t fileportTemplate = {
		/* name = */		MACH_PORT_NULL,
		/* pad1 = */		0,
		/* pad2 = */		0,
		/* disp = */		17,
		/* type = */		MACH_MSG_PORT_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 2;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->fileport = fileportTemplate;
	InP->fileport.name = fileport;
#else	/* UseStaticTemplates */
	InP->fileport.name = fileport;
	InP->fileport.disposition = 17;
	InP->fileport.type = MACH_MSG_PORT_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->ntoken = ntoken;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945672;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945672, "_notify_server_register_file_descriptor")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945672, "_notify_server_register_file_descriptor")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_file_descriptor_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_file_descriptor_t((__Reply___notify_server_register_file_descriptor_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_file_descriptor_t__defined) */

	*token = Out0P->token;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_mach_port_t__defined)
#define __MIG_check__Reply___notify_server_register_mach_port_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_mach_port_t(__Reply___notify_server_register_mach_port_t *Out0P)
{

	typedef __Reply___notify_server_register_mach_port_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945773) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_mach_port_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_mach_port */
mig_external kern_return_t _notify_server_register_mach_port
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	mach_port_t port,
	int ntoken,
	int *token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_mach_port_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_mach_port_t__defined */

	__DeclareSendRpc(78945673, "_notify_server_register_mach_port")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_port_descriptor_t portTemplate = {
		/* name = */		MACH_PORT_NULL,
		/* pad1 = */		0,
		/* pad2 = */		0,
		/* disp = */		17,
		/* type = */		MACH_MSG_PORT_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 2;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->port = portTemplate;
	InP->port.name = port;
#else	/* UseStaticTemplates */
	InP->port.name = port;
	InP->port.disposition = 17;
	InP->port.type = MACH_MSG_PORT_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->ntoken = ntoken;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945673;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945673, "_notify_server_register_mach_port")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945673, "_notify_server_register_mach_port")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_mach_port_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_mach_port_t((__Reply___notify_server_register_mach_port_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_mach_port_t__defined) */

	*token = Out0P->token;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_set_owner_t__defined)
#define __MIG_check__Reply___notify_server_set_owner_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_set_owner_t(__Reply___notify_server_set_owner_t *Out0P)
{

	typedef __Reply___notify_server_set_owner_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945774) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_set_owner_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_owner */
mig_external kern_return_t _notify_server_set_owner
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int user,
	int group,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_set_owner_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_set_owner_t__defined */

	__DeclareSendRpc(78945674, "_notify_server_set_owner")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->user = user;

	InP->group = group;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945674;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945674, "_notify_server_set_owner")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945674, "_notify_server_set_owner")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_set_owner_t__defined)
	check_result = __MIG_check__Reply___notify_server_set_owner_t((__Reply___notify_server_set_owner_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_set_owner_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_get_owner_t__defined)
#define __MIG_check__Reply___notify_server_get_owner_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_get_owner_t(__Reply___notify_server_get_owner_t *Out0P)
{

	typedef __Reply___notify_server_get_owner_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945775) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_get_owner_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_owner */
mig_external kern_return_t _notify_server_get_owner
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *user,
	int *group,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_get_owner_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_get_owner_t__defined */

	__DeclareSendRpc(78945675, "_notify_server_get_owner")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945675;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945675, "_notify_server_get_owner")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945675, "_notify_server_get_owner")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_get_owner_t__defined)
	check_result = __MIG_check__Reply___notify_server_get_owner_t((__Reply___notify_server_get_owner_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_get_owner_t__defined) */

	*user = Out0P->user;

	*group = Out0P->group;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_set_access_t__defined)
#define __MIG_check__Reply___notify_server_set_access_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_set_access_t(__Reply___notify_server_set_access_t *Out0P)
{

	typedef __Reply___notify_server_set_access_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945776) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_set_access_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_access */
mig_external kern_return_t _notify_server_set_access
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int mode,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_set_access_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_set_access_t__defined */

	__DeclareSendRpc(78945676, "_notify_server_set_access")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->mode = mode;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945676;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945676, "_notify_server_set_access")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945676, "_notify_server_set_access")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_set_access_t__defined)
	check_result = __MIG_check__Reply___notify_server_set_access_t((__Reply___notify_server_set_access_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_set_access_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_get_access_t__defined)
#define __MIG_check__Reply___notify_server_get_access_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_get_access_t(__Reply___notify_server_get_access_t *Out0P)
{

	typedef __Reply___notify_server_get_access_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945777) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_get_access_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_access */
mig_external kern_return_t _notify_server_get_access
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *mode,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_get_access_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_get_access_t__defined */

	__DeclareSendRpc(78945677, "_notify_server_get_access")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945677;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945677, "_notify_server_get_access")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945677, "_notify_server_get_access")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_get_access_t__defined)
	check_result = __MIG_check__Reply___notify_server_get_access_t((__Reply___notify_server_get_access_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_get_access_t__defined) */

	*mode = Out0P->mode;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_release_name_t__defined)
#define __MIG_check__Reply___notify_server_release_name_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_release_name_t(__Reply___notify_server_release_name_t *Out0P)
{

	typedef __Reply___notify_server_release_name_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945778) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_release_name_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_release_name */
mig_external kern_return_t _notify_server_release_name
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_release_name_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_release_name_t__defined */

	__DeclareSendRpc(78945678, "_notify_server_release_name")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945678;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945678, "_notify_server_release_name")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945678, "_notify_server_release_name")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_release_name_t__defined)
	check_result = __MIG_check__Reply___notify_server_release_name_t((__Reply___notify_server_release_name_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_release_name_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_cancel_t__defined)
#define __MIG_check__Reply___notify_server_cancel_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_cancel_t(__Reply___notify_server_cancel_t *Out0P)
{

	typedef __Reply___notify_server_cancel_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945779) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_cancel_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_cancel */
mig_external kern_return_t _notify_server_cancel
(
	mach_port_t server,
	int token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_cancel_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_cancel_t__defined */

	__DeclareSendRpc(78945679, "_notify_server_cancel")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945679;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945679, "_notify_server_cancel")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945679, "_notify_server_cancel")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_cancel_t__defined)
	check_result = __MIG_check__Reply___notify_server_cancel_t((__Reply___notify_server_cancel_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_cancel_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_check_t__defined)
#define __MIG_check__Reply___notify_server_check_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_check_t(__Reply___notify_server_check_t *Out0P)
{

	typedef __Reply___notify_server_check_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945780) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_check_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_check */
mig_external kern_return_t _notify_server_check
(
	mach_port_t server,
	int token,
	int *check,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_check_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_check_t__defined */

	__DeclareSendRpc(78945680, "_notify_server_check")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945680;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945680, "_notify_server_check")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945680, "_notify_server_check")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_check_t__defined)
	check_result = __MIG_check__Reply___notify_server_check_t((__Reply___notify_server_check_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_check_t__defined) */

	*check = Out0P->check;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_get_state_t__defined)
#define __MIG_check__Reply___notify_server_get_state_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_get_state_t(__Reply___notify_server_get_state_t *Out0P)
{

	typedef __Reply___notify_server_get_state_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945781) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_get_state_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state */
mig_external kern_return_t _notify_server_get_state
(
	mach_port_t server,
	int token,
	uint64_t *state,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_get_state_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_get_state_t__defined */

	__DeclareSendRpc(78945681, "_notify_server_get_state")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945681;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945681, "_notify_server_get_state")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945681, "_notify_server_get_state")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_get_state_t__defined)
	check_result = __MIG_check__Reply___notify_server_get_state_t((__Reply___notify_server_get_state_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_get_state_t__defined) */

	*state = Out0P->state;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_set_state_t__defined)
#define __MIG_check__Reply___notify_server_set_state_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_set_state_t(__Reply___notify_server_set_state_t *Out0P)
{

	typedef __Reply___notify_server_set_state_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945782) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_set_state_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_state */
mig_external kern_return_t _notify_server_set_state
(
	mach_port_t server,
	int token,
	uint64_t state,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_set_state_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_set_state_t__defined */

	__DeclareSendRpc(78945682, "_notify_server_set_state")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->state = state;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945682;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945682, "_notify_server_set_state")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945682, "_notify_server_set_state")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_set_state_t__defined)
	check_result = __MIG_check__Reply___notify_server_set_state_t((__Reply___notify_server_set_state_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_set_state_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_monitor_file_t__defined)
#define __MIG_check__Reply___notify_server_monitor_file_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_monitor_file_t(__Reply___notify_server_monitor_file_t *Out0P)
{

	typedef __Reply___notify_server_monitor_file_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945785) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_monitor_file_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_monitor_file */
mig_external kern_return_t _notify_server_monitor_file
(
	mach_port_t server,
	int token,
	caddr_t path,
	mach_msg_type_number_t pathCnt,
	int flags,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_monitor_file_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_monitor_file_t__defined */

	__DeclareSendRpc(78945685, "_notify_server_monitor_file")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t pathTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->path = pathTemplate;
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
#else	/* UseStaticTemplates */
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
	InP->path.deallocate =  FALSE;
	InP->path.copy = MACH_MSG_VIRTUAL_COPY;
	InP->path.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->token = token;

	InP->pathCnt = pathCnt;

	InP->flags = flags;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945685;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945685, "_notify_server_monitor_file")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945685, "_notify_server_monitor_file")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_monitor_file_t__defined)
	check_result = __MIG_check__Reply___notify_server_monitor_file_t((__Reply___notify_server_monitor_file_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_monitor_file_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_suspend_t__defined)
#define __MIG_check__Reply___notify_server_suspend_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_suspend_t(__Reply___notify_server_suspend_t *Out0P)
{

	typedef __Reply___notify_server_suspend_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945786) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_suspend_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_suspend */
mig_external kern_return_t _notify_server_suspend
(
	mach_port_t server,
	int token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_suspend_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_suspend_t__defined */

	__DeclareSendRpc(78945686, "_notify_server_suspend")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945686;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945686, "_notify_server_suspend")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945686, "_notify_server_suspend")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_suspend_t__defined)
	check_result = __MIG_check__Reply___notify_server_suspend_t((__Reply___notify_server_suspend_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_suspend_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_resume_t__defined)
#define __MIG_check__Reply___notify_server_resume_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_resume_t(__Reply___notify_server_resume_t *Out0P)
{

	typedef __Reply___notify_server_resume_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945787) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_resume_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_resume */
mig_external kern_return_t _notify_server_resume
(
	mach_port_t server,
	int token,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_resume_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_resume_t__defined */

	__DeclareSendRpc(78945687, "_notify_server_resume")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945687;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945687, "_notify_server_resume")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945687, "_notify_server_resume")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_resume_t__defined)
	check_result = __MIG_check__Reply___notify_server_resume_t((__Reply___notify_server_resume_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_resume_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_suspend_pid_t__defined)
#define __MIG_check__Reply___notify_server_suspend_pid_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_suspend_pid_t(__Reply___notify_server_suspend_pid_t *Out0P)
{

	typedef __Reply___notify_server_suspend_pid_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945788) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_suspend_pid_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_suspend_pid */
mig_external kern_return_t _notify_server_suspend_pid
(
	mach_port_t server,
	int pid,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_suspend_pid_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_suspend_pid_t__defined */

	__DeclareSendRpc(78945688, "_notify_server_suspend_pid")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->pid = pid;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945688;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945688, "_notify_server_suspend_pid")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945688, "_notify_server_suspend_pid")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_suspend_pid_t__defined)
	check_result = __MIG_check__Reply___notify_server_suspend_pid_t((__Reply___notify_server_suspend_pid_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_suspend_pid_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_resume_pid_t__defined)
#define __MIG_check__Reply___notify_server_resume_pid_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_resume_pid_t(__Reply___notify_server_resume_pid_t *Out0P)
{

	typedef __Reply___notify_server_resume_pid_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945789) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_resume_pid_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_resume_pid */
mig_external kern_return_t _notify_server_resume_pid
(
	mach_port_t server,
	int pid,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_resume_pid_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_resume_pid_t__defined */

	__DeclareSendRpc(78945689, "_notify_server_resume_pid")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->pid = pid;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945689;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945689, "_notify_server_resume_pid")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945689, "_notify_server_resume_pid")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_resume_pid_t__defined)
	check_result = __MIG_check__Reply___notify_server_resume_pid_t((__Reply___notify_server_resume_pid_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_resume_pid_t__defined) */

	*status = Out0P->status;

	return KERN_SUCCESS;
}

/* SimpleRoutine _notify_server_simple_post */
mig_external kern_return_t _notify_server_simple_post
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_simple_post_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_simple_post_t__defined */

	__DeclareSendSimple(78945690, "_notify_server_simple_post")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945690;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945690, "_notify_server_simple_post")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945690, "_notify_server_simple_post")
	return msg_result;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_post_2_t__defined)
#define __MIG_check__Reply___notify_server_post_2_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_post_2_t(__Reply___notify_server_post_2_t *Out0P)
{

	typedef __Reply___notify_server_post_2_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945791) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_post_2_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_post_2 */
mig_external kern_return_t _notify_server_post_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	uint64_t *name_id,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_post_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_post_2_t__defined */

	__DeclareSendRpc(78945691, "_notify_server_post_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945691;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945691, "_notify_server_post_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945691, "_notify_server_post_2")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_post_2_t__defined)
	check_result = __MIG_check__Reply___notify_server_post_2_t((__Reply___notify_server_post_2_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_post_2_t__defined) */

	*name_id = Out0P->name_id;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

/* SimpleRoutine _notify_server_post_3 */
mig_external kern_return_t _notify_server_post_3
(
	mach_port_t server,
	uint64_t name_id
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_post_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_post_3_t__defined */

	__DeclareSendSimple(78945692, "_notify_server_post_3")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->name_id = name_id;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945692;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945692, "_notify_server_post_3")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945692, "_notify_server_post_3")
	return msg_result;
}

/* SimpleRoutine _notify_server_post_4 */
mig_external kern_return_t _notify_server_post_4
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_post_4_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_post_4_t__defined */

	__DeclareSendSimple(78945693, "_notify_server_post_4")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945693;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945693, "_notify_server_post_4")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945693, "_notify_server_post_4")
	return msg_result;
}

/* SimpleRoutine _notify_server_register_plain_2 */
mig_external kern_return_t _notify_server_register_plain_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_plain_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_plain_2_t__defined */

	__DeclareSendSimple(78945694, "_notify_server_register_plain_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945694;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945694, "_notify_server_register_plain_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945694, "_notify_server_register_plain_2")
	return msg_result;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_register_check_2_t__defined)
#define __MIG_check__Reply___notify_server_register_check_2_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_register_check_2_t(__Reply___notify_server_register_check_2_t *Out0P)
{

	typedef __Reply___notify_server_register_check_2_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945795) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_register_check_2_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_register_check_2 */
mig_external kern_return_t _notify_server_register_check_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	int *size,
	int *slot,
	uint64_t *name_id,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_check_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_check_2_t__defined */

	__DeclareSendRpc(78945695, "_notify_server_register_check_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945695;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945695, "_notify_server_register_check_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945695, "_notify_server_register_check_2")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_register_check_2_t__defined)
	check_result = __MIG_check__Reply___notify_server_register_check_2_t((__Reply___notify_server_register_check_2_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_register_check_2_t__defined) */

	*size = Out0P->size;

	*slot = Out0P->slot;

	*name_id = Out0P->name_id;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

/* SimpleRoutine _notify_server_register_signal_2 */
mig_external kern_return_t _notify_server_register_signal_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	int sig
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_signal_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_signal_2_t__defined */

	__DeclareSendSimple(78945696, "_notify_server_register_signal_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->sig = sig;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945696;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945696, "_notify_server_register_signal_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945696, "_notify_server_register_signal_2")
	return msg_result;
}

/* SimpleRoutine _notify_server_register_file_descriptor_2 */
mig_external kern_return_t _notify_server_register_file_descriptor_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	mach_port_t fileport
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_file_descriptor_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_file_descriptor_2_t__defined */

	__DeclareSendSimple(78945697, "_notify_server_register_file_descriptor_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_port_descriptor_t fileportTemplate = {
		/* name = */		MACH_PORT_NULL,
		/* pad1 = */		0,
		/* pad2 = */		0,
		/* disp = */		17,
		/* type = */		MACH_MSG_PORT_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 2;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->fileport = fileportTemplate;
	InP->fileport.name = fileport;
#else	/* UseStaticTemplates */
	InP->fileport.name = fileport;
	InP->fileport.disposition = 17;
	InP->fileport.type = MACH_MSG_PORT_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945697;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945697, "_notify_server_register_file_descriptor_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945697, "_notify_server_register_file_descriptor_2")
	return msg_result;
}

/* SimpleRoutine _notify_server_register_mach_port_2 */
mig_external kern_return_t _notify_server_register_mach_port_2
(
	mach_port_t server,
	caddr_t name,
	mach_msg_type_number_t nameCnt,
	int token,
	mach_port_t port
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_register_mach_port_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_register_mach_port_2_t__defined */

	__DeclareSendSimple(78945698, "_notify_server_register_mach_port_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_port_descriptor_t portTemplate = {
		/* name = */		MACH_PORT_NULL,
		/* pad1 = */		0,
		/* pad2 = */		0,
		/* disp = */		17,
		/* type = */		MACH_MSG_PORT_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 2;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->port = portTemplate;
	InP->port.name = port;
#else	/* UseStaticTemplates */
	InP->port.name = port;
	InP->port.disposition = 17;
	InP->port.type = MACH_MSG_PORT_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945698;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945698, "_notify_server_register_mach_port_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945698, "_notify_server_register_mach_port_2")
	return msg_result;
}

/* SimpleRoutine _notify_server_cancel_2 */
mig_external kern_return_t _notify_server_cancel_2
(
	mach_port_t server,
	int token
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_cancel_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_cancel_2_t__defined */

	__DeclareSendSimple(78945699, "_notify_server_cancel_2")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945699;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945699, "_notify_server_cancel_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945699, "_notify_server_cancel_2")
	return msg_result;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_get_state_2_t__defined)
#define __MIG_check__Reply___notify_server_get_state_2_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_get_state_2_t(__Reply___notify_server_get_state_2_t *Out0P)
{

	typedef __Reply___notify_server_get_state_2_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945800) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_get_state_2_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state_2 */
mig_external kern_return_t _notify_server_get_state_2
(
	mach_port_t server,
	uint64_t name_id,
	uint64_t *state,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_get_state_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_get_state_2_t__defined */

	__DeclareSendRpc(78945700, "_notify_server_get_state_2")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->name_id = name_id;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945700;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945700, "_notify_server_get_state_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945700, "_notify_server_get_state_2")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_get_state_2_t__defined)
	check_result = __MIG_check__Reply___notify_server_get_state_2_t((__Reply___notify_server_get_state_2_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_get_state_2_t__defined) */

	*state = Out0P->state;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_get_state_3_t__defined)
#define __MIG_check__Reply___notify_server_get_state_3_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_get_state_3_t(__Reply___notify_server_get_state_3_t *Out0P)
{

	typedef __Reply___notify_server_get_state_3_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945801) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_get_state_3_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_get_state_3 */
mig_external kern_return_t _notify_server_get_state_3
(
	mach_port_t server,
	int token,
	uint64_t *state,
	uint64_t *nid,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_get_state_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_get_state_3_t__defined */

	__DeclareSendRpc(78945701, "_notify_server_get_state_3")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945701;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945701, "_notify_server_get_state_3")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945701, "_notify_server_get_state_3")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_get_state_3_t__defined)
	check_result = __MIG_check__Reply___notify_server_get_state_3_t((__Reply___notify_server_get_state_3_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_get_state_3_t__defined) */

	*state = Out0P->state;

	*nid = Out0P->nid;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

/* SimpleRoutine _notify_server_set_state_2 */
mig_external kern_return_t _notify_server_set_state_2
(
	mach_port_t server,
	uint64_t name_id,
	uint64_t state
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_set_state_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_set_state_2_t__defined */

	__DeclareSendSimple(78945702, "_notify_server_set_state_2")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->name_id = name_id;

	InP->state = state;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945702;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945702, "_notify_server_set_state_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945702, "_notify_server_set_state_2")
	return msg_result;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_set_state_3_t__defined)
#define __MIG_check__Reply___notify_server_set_state_3_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_set_state_3_t(__Reply___notify_server_set_state_3_t *Out0P)
{

	typedef __Reply___notify_server_set_state_3_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945803) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_set_state_3_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_set_state_3 */
mig_external kern_return_t _notify_server_set_state_3
(
	mach_port_t server,
	int token,
	uint64_t state,
	uint64_t *nid,
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_set_state_3_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_set_state_3_t__defined */

	__DeclareSendRpc(78945703, "_notify_server_set_state_3")

	InP->msgh_body.msgh_descriptor_count = 0;
	InP->NDR = NDR_record;

	InP->token = token;

	InP->state = state;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945703;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945703, "_notify_server_set_state_3")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945703, "_notify_server_set_state_3")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_set_state_3_t__defined)
	check_result = __MIG_check__Reply___notify_server_set_state_3_t((__Reply___notify_server_set_state_3_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_set_state_3_t__defined) */

	*nid = Out0P->nid;

	*status = Out0P->status;

	return KERN_SUCCESS;
}

/* SimpleRoutine _notify_server_monitor_file_2 */
mig_external kern_return_t _notify_server_monitor_file_2
(
	mach_port_t server,
	int token,
	caddr_t path,
	mach_msg_type_number_t pathCnt,
	int flags
)
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
	} Request;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
	} Mess;

	Request *InP = &Mess.In;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_monitor_file_2_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_monitor_file_2_t__defined */

	__DeclareSendSimple(78945704, "_notify_server_monitor_file_2")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t pathTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 1;
#if	UseStaticTemplates
	InP->path = pathTemplate;
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
#else	/* UseStaticTemplates */
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
	InP->path.deallocate =  FALSE;
	InP->path.copy = MACH_MSG_VIRTUAL_COPY;
	InP->path.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->token = token;

	InP->pathCnt = pathCnt;

	InP->flags = flags;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, 0);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = MACH_PORT_NULL;
	InP->Head.msgh_id = 78945704;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendSimple(78945704, "_notify_server_monitor_file_2")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendSimple(78945704, "_notify_server_monitor_file_2")
	return msg_result;
}

#if ( __MigTypeCheck )
#if __MIG_check__Reply__notify_ipc_subsystem__
#if !defined(__MIG_check__Reply___notify_server_regenerate_t__defined)
#define __MIG_check__Reply___notify_server_regenerate_t__defined

mig_internal kern_return_t __MIG_check__Reply___notify_server_regenerate_t(__Reply___notify_server_regenerate_t *Out0P)
{

	typedef __Reply___notify_server_regenerate_t __Reply;
#if	__MigTypeCheck
	unsigned int msgh_size;
#endif	/* __MigTypeCheck */
	if (Out0P->Head.msgh_id != 78945805) {
	    if (Out0P->Head.msgh_id == MACH_NOTIFY_SEND_ONCE)
		{ return MIG_SERVER_DIED; }
	    else
		{ return MIG_REPLY_MISMATCH; }
	}

#if	__MigTypeCheck
	msgh_size = Out0P->Head.msgh_size;

	if ((Out0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX) ||
	    ((msgh_size != sizeof(__Reply)) &&
	     (msgh_size != sizeof(mig_reply_error_t) ||
	      Out0P->RetCode == KERN_SUCCESS)))
		{ return MIG_TYPE_ERROR ; }
#endif	/* __MigTypeCheck */

	if (Out0P->RetCode != KERN_SUCCESS) {
		return ((mig_reply_error_t *)Out0P)->RetCode;
	}

	return MACH_MSG_SUCCESS;
}
#endif /* !defined(__MIG_check__Reply___notify_server_regenerate_t__defined) */
#endif /* __MIG_check__Reply__notify_ipc_subsystem__ */
#endif /* ( __MigTypeCheck ) */


/* Routine _notify_server_regenerate */
mig_external kern_return_t _notify_server_regenerate
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
	int *status
)
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
	} Request;
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
		mach_msg_trailer_t trailer;
	} Reply;
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
	} __Reply;
#ifdef  __MigPackStructs
#pragma pack()
#endif
	/*
	 * typedef struct {
	 * 	mach_msg_header_t Head;
	 * 	NDR_record_t NDR;
	 * 	kern_return_t RetCode;
	 * } mig_reply_error_t;
	 */

	union {
		Request In;
		Reply Out;
	} Mess;

	Request *InP = &Mess.In;
	Reply *Out0P = &Mess.Out;

	mach_msg_return_t msg_result;

#ifdef	__MIG_check__Reply___notify_server_regenerate_t__defined
	kern_return_t check_result;
#endif	/* __MIG_check__Reply___notify_server_regenerate_t__defined */

	__DeclareSendRpc(78945705, "_notify_server_regenerate")

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t nameTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_port_descriptor_t portTemplate = {
		/* name = */		MACH_PORT_NULL,
		/* pad1 = */		0,
		/* pad2 = */		0,
		/* disp = */		20,
		/* type = */		MACH_MSG_PORT_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	const static mach_msg_ool_descriptor_t pathTemplate = {
		/* addr = */		(void *)0,
		/* size = */		0,
		/* deal = */		FALSE,
		/* copy = */		MACH_MSG_VIRTUAL_COPY,
		/* pad2 = */		0,
		/* type = */		MACH_MSG_OOL_DESCRIPTOR,
	};
#endif	/* UseStaticTemplates */

	InP->msgh_body.msgh_descriptor_count = 3;
#if	UseStaticTemplates
	InP->name = nameTemplate;
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
#else	/* UseStaticTemplates */
	InP->name.address = (void *)(name);
	InP->name.size = nameCnt;
	InP->name.deallocate =  FALSE;
	InP->name.copy = MACH_MSG_VIRTUAL_COPY;
	InP->name.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->port = portTemplate;
	InP->port.name = port;
#else	/* UseStaticTemplates */
	InP->port.name = port;
	InP->port.disposition = 20;
	InP->port.type = MACH_MSG_PORT_DESCRIPTOR;
#endif	/* UseStaticTemplates */

#if	UseStaticTemplates
	InP->path = pathTemplate;
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
#else	/* UseStaticTemplates */
	InP->path.address = (void *)(path);
	InP->path.size = pathCnt;
	InP->path.deallocate =  FALSE;
	InP->path.copy = MACH_MSG_VIRTUAL_COPY;
	InP->path.type = MACH_MSG_OOL_DESCRIPTOR;
#endif	/* UseStaticTemplates */

	InP->NDR = NDR_record;

	InP->nameCnt = nameCnt;

	InP->token = token;

	InP->reg_type = reg_type;

	InP->sig = sig;

	InP->prev_slot = prev_slot;

	InP->prev_state = prev_state;

	InP->prev_time = prev_time;

	InP->pathCnt = pathCnt;

	InP->path_flags = path_flags;

	InP->Head.msgh_bits = MACH_MSGH_BITS_COMPLEX|
		MACH_MSGH_BITS(19, MACH_MSG_TYPE_MAKE_SEND_ONCE);
	/* msgh_size passed as argument */
	InP->Head.msgh_request_port = server;
	InP->Head.msgh_reply_port = mig_get_reply_port();
	InP->Head.msgh_id = 78945705;
	
/* BEGIN VOUCHER CODE */

#ifdef USING_VOUCHERS
	if (voucher_mach_msg_set != NULL) {
		voucher_mach_msg_set(&InP->Head);
	}
#endif // USING_VOUCHERS
	
/* END VOUCHER CODE */

	__BeforeSendRpc(78945705, "_notify_server_regenerate")
	msg_result = mach_msg(&InP->Head, MACH_SEND_MSG|MACH_RCV_MSG|MACH_MSG_OPTION_NONE, sizeof(Request), (mach_msg_size_t)sizeof(Reply), InP->Head.msgh_reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
	__AfterSendRpc(78945705, "_notify_server_regenerate")
	if (msg_result != MACH_MSG_SUCCESS) {
		__MachMsgErrorWithoutTimeout(msg_result);
		{ return msg_result; }
	}


#if	defined(__MIG_check__Reply___notify_server_regenerate_t__defined)
	check_result = __MIG_check__Reply___notify_server_regenerate_t((__Reply___notify_server_regenerate_t *)Out0P);
	if (check_result != MACH_MSG_SUCCESS)
		{ return check_result; }
#endif	/* defined(__MIG_check__Reply___notify_server_regenerate_t__defined) */

	*new_slot = Out0P->new_slot;

	*new_name_id = Out0P->new_name_id;

	*status = Out0P->status;

	return KERN_SUCCESS;
}
