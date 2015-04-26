#include <sys/types.h>
#include <sys/errno.h>
#include <mach/mach.h>
#include <xpc/launchd.h>
#include "xpc_internal.h"
#include <machine/atomic.h>
#include <assert.h>
#include <syslog.h>
#include <stdarg.h>


void
fail_log(const char *exp)
{
	syslog(LOG_ERR, "%s", exp);
	sleep(1);
	printf("%s", exp);
	abort();
}

static void nvlist_add_prim(nvlist_t *nv, const char *key, xpc_object_t xobj);

__private_extern__ int
nvlist_exists_object(const nvlist_t *nv, const char *key)
{

	return (nvlist_exists_type(nv, key, NV_TYPE_PTR));
}

__private_extern__ void
nvlist_add_object(nvlist_t *nv, const char *key, xpc_object_t xobj)
{

	nvlist_add_number_type(nv, key, (uint64_t)xobj, NV_TYPE_PTR);
}

__private_extern__ xpc_object_t
nvlist_get_object(const nvlist_t *nv, const char *key)
{

	return ((xpc_object_t)nvlist_get_number(nv, key));
}

__private_extern__ xpc_object_t
nvlist_move_object(const nvlist_t *nv, const char *key)
{

	return ((xpc_object_t)nvlist_get_number(nv, key));
}

__private_extern__ size_t
nvcount(const nvlist_t *nv)
{
	void *cookiep;
	const char *key;
	size_t count;
	int type;

	count = 0;
	cookiep = NULL;
	while ((key = nvlist_next(nv, &type, &cookiep)) != NULL)
		count++;
	return (count);
}

static void
xpc_nvlist_add_nvlist_type(nvlist_t *nv, const char *key, nvlist_t *nvval, int type)
{
	void *cookiep;
	const char *ikey;
	int itype;
	xpc_object_t tmp;

	cookiep = NULL;
	while ((ikey = nvlist_next(nvval, &itype, &cookiep)) != NULL) {
		if (itype == NV_TYPE_PTR) {
			tmp = nvlist_get_object(nvval, ikey);
			nvlist_add_prim(nvval, ikey, tmp);
		}
	}
	nvlist_add_nvlist_type(nv, key, nvval, type);
}

static void
nvlist_add_prim(nvlist_t *nv, const char *key, xpc_object_t xobj)
{
	struct xpc_object *xo;

	xo = xobj;
	switch (xo->xo_xpc_type) {
	case _XPC_TYPE_BOOL:
		nvlist_add_bool(nv, key, xo->xo_bool);
		break;
	case _XPC_TYPE_ENDPOINT:
		nvlist_add_number_type(nv, key, xo->xo_uint, NV_TYPE_ENDPOINT);
		break;
	case _XPC_TYPE_INT64:
		nvlist_add_number_type(nv, key, xo->xo_uint, NV_TYPE_INT64);
		break;
	case _XPC_TYPE_UINT64:
		nvlist_add_number_type(nv, key, xo->xo_uint, NV_TYPE_UINT64);
		break;
	case _XPC_TYPE_STRING:
		nvlist_add_string(nv, key, xo->xo_str);
		break;
	case _XPC_TYPE_DICTIONARY:
		xpc_nvlist_add_nvlist_type(nv, key, xo->xo_nv, NV_TYPE_NVLIST_DICTIONARY);
		break;
	case _XPC_TYPE_ARRAY:
		xpc_nvlist_add_nvlist_type(nv, key, xo->xo_nv, NV_TYPE_NVLIST_ARRAY);
		break;
	default:
		fail_log("unsupported serialization type\n");
#if 0
	printf("unsupported serialization type %u\n", xo->xo_xpc_type);
	abort();
#endif
	}
}
static void
xpc_nvlist_destroy(nvlist_t *nv)
{
	void *cookiep;
	const char *key;
	xpc_object_t tmp;
	int type;

	cookiep = NULL;
	while ((key = nvlist_next(nv, &type, &cookiep)) != NULL) {
		if (type == NV_TYPE_PTR) {
			tmp = nvlist_get_object(nv, key);
			xpc_release(tmp);
		}
	}
	nvlist_destroy(nv);
}

static void *
xpc_nvlist_pack(const nvlist_t *nv, void *buf, size_t *size)
{
	nvlist_t *clone;
	void *cookiep, *packed;
	const char *key;
	xpc_object_t tmp;
	int type;

	if ((clone = nvlist_clone(nv)) == NULL)
		return (NULL);

	cookiep = NULL;
	while ((key = nvlist_next(clone, &type, &cookiep)) != NULL) {
		if (type == NV_TYPE_PTR) {
			tmp = nvlist_get_object(clone, key);
			nvlist_add_prim(clone, key, tmp);
		}
	}
	packed = nvlist_pack_buffer(clone, buf, size);
	nvlist_destroy(clone);
	return (packed);
}

static void
xpc_object_destroy(struct xpc_object *xo)
{
	
	if (xo->xo_nv != NULL)
		xpc_nvlist_destroy(xo->xo_nv);
	free(xo);
}

xpc_object_t
xpc_retain(xpc_object_t obj)
{
	struct xpc_object *xo;

	xo = obj;
	atomic_add_int(&xo->xo_refcnt, 1);
	return (obj);
}

void
xpc_release(xpc_object_t obj)
{
	struct xpc_object *xo;

	xo = obj;
	if (atomic_fetchadd_int(&xo->xo_refcnt, -1) > 1)
		return;
	xpc_object_destroy(xo);
}

static const char *xpc_errors[] = {
	"No Error Found",
	"No Memory",
	"Invalid Argument",
	"No Such Process"
};


const char *
xpc_strerror(int error)
{

	if (error > EXMAX || error < 0)
		return "BAD ERROR";
	return (xpc_errors[error]);
}

char *
xpc_copy_description(xpc_object_t obj __unused)
{

	return (strdup("figure it out yourself!"));
}

struct _launch_data {
	uint64_t type;
	union {
		struct {
			union {
				launch_data_t *_array;
				char *string;
				void *opaque;
				int64_t __junk;
			};
			union {
				uint64_t _array_cnt;
				uint64_t string_len;
				uint64_t opaque_size;
			};
		};
		int64_t fd;
		uint64_t  mp;
		uint64_t err;
		int64_t number;
		uint64_t boolean; /* We'd use 'bool' but this struct needs to be used under Rosetta, and sizeof(bool) is different between PowerPC and Intel */
		double float_num;
	};
};

static uint8_t ld_to_xpc_type[] = {
	_XPC_TYPE_INVALID,
	_XPC_TYPE_DICTIONARY,
	_XPC_TYPE_ARRAY,
	_XPC_TYPE_FD,
	_XPC_TYPE_UINT64,
	_XPC_TYPE_DOUBLE,
	_XPC_TYPE_BOOL,
	_XPC_TYPE_STRING,
	_XPC_TYPE_DATA,
	_XPC_TYPE_ERROR,
	_XPC_TYPE_ENDPOINT
};
	
xpc_object_t
ld2xpc(launch_data_t ld)
{
	struct xpc_object *xo;
	xpc_u val;


	if (ld->type > LAUNCH_DATA_MACHPORT)
		return (NULL);
	if (ld->type == LAUNCH_DATA_STRING || ld->type == LAUNCH_DATA_OPAQUE) {
		val.str = malloc(ld->string_len);
		memcpy(__DECONST(void *, val.str), ld->string, ld->string_len);
		xo = _xpc_prim_create(ld_to_xpc_type[ld->type], val, ld->string_len);
	} else if (ld->type == LAUNCH_DATA_BOOL) {
		val.b = (bool)ld->boolean;
		xo = _xpc_prim_create(ld_to_xpc_type[ld->type], val, 0);
	} else if (ld->type == LAUNCH_DATA_ARRAY) {
		xo = xpc_array_create(NULL, 0);
		for (uint64_t i = 0; i < ld->_array_cnt; i++)
			xpc_array_append_value(xo, ld2xpc(ld->_array[i]));
	} else {
		val.ui = ld->mp;
		xo = _xpc_prim_create(ld_to_xpc_type[ld->type], val, ld->string_len);	
	}
	return (xo);
}

xpc_object_t
xpc_copy_entitlement_for_token(const char *key __unused, audit_token_t *token __unused)
{
	xpc_u val;

	val.b = true;
	return (_xpc_prim_create(_XPC_TYPE_BOOL, val,0));
}

struct xpc_message {
	mach_msg_header_t header;
	char data[0];
	mach_msg_trailer_t trailer;
};
#define MAX_RECV 8192
#define XPC_RECV_SIZE MAX_RECV-sizeof(mach_msg_header_t)-sizeof(mach_msg_trailer_t)
struct xpc_recv_message {
	mach_msg_header_t header;
	char data[XPC_RECV_SIZE];
	mach_msg_trailer_t trailer;
};

#define XPC_RPORT "XPC remote port"
int
xpc_pipe_routine_reply(xpc_object_t xobj)
{
	struct xpc_object *xo;
	size_t size, msg_size;
	struct xpc_message *message;
	kern_return_t kr;
	int err;

	xo = xobj;
	assert(xo->xo_xpc_type == _XPC_TYPE_DICTIONARY);
	size = nvlist_size(xo->xo_nv);
	msg_size = size + sizeof(mach_msg_header_t);
	if ((message = malloc(msg_size)) == NULL)
		return (ENOMEM);
	if (xpc_nvlist_pack(xo->xo_nv, &message->data, &size) == NULL)
		return (EINVAL);

	message->header.msgh_size = msg_size;
	message->header.msgh_remote_port = xpc_dictionary_copy_mach_send(xobj, XPC_RPORT);
	message->header.msgh_local_port = MACH_PORT_NULL;
	kr = mach_msg_send(&message->header);
	if (kr != KERN_SUCCESS)
		err = (kr == KERN_INVALID_TASK) ? EPIPE : EINVAL;
	else
		err = 0;
	free(message);
	return (err);
}


#define LOG(...)	\
	do {            \
	syslog(LOG_ERR, "%s:%u: ", __FILE__, __LINE__);	\
	syslog(LOG_ERR, __VA_ARGS__);					\
	sleep(1); \
	} while (0)

int
xpc_pipe_try_receive(mach_port_t portset, xpc_object_t *requestobj, mach_port_t *rcvport,
	boolean_t (*demux)(mach_msg_header_t *, mach_msg_header_t *), mach_msg_size_t msgsize __unused,
	int flags __unused)
{
	struct xpc_recv_message message;
	mach_msg_header_t *request;
	kern_return_t kr;
	mig_reply_error_t response;
	mach_msg_trailer_t *tr;
	int data_size;
	struct xpc_object *xo;
	audit_token_t *auditp;
	xpc_u val;

	request = &message.header;
	/* should be size - but what about arbitrary XPC data? */
	request->msgh_size = MAX_RECV;
	request->msgh_local_port = portset;
	kr = mach_msg(request, MACH_RCV_MSG |
				  MACH_RCV_TRAILER_TYPE(MACH_MSG_TRAILER_FORMAT_0) |
				  MACH_RCV_TRAILER_ELEMENTS(MACH_RCV_TRAILER_AUDIT),
				  0, request->msgh_size, request->msgh_local_port,
				  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

	if (kr != 0)
		LOG("mach_msg_receive returned %d\n", kr);
	*rcvport = request->msgh_remote_port;
	if (demux(request, (mach_msg_header_t *)&response)) {
		(void)mach_msg_send((mach_msg_header_t *)&response);
		/*  can't do anything with the return code
		* just tell the caller this has been handled
		*/
		return (TRUE);
	}
	LOG("demux returned false\n");
	data_size = request->msgh_size;
	LOG("unpacking data_size=%d", data_size);
	val.nv = nvlist_unpack(&message.data, data_size);
	LOG("unpacked to %p", val.nv);
	/* is padding for alignment enforced in the kernel?*/
	tr = (mach_msg_trailer_t *)(uintptr_t)(((uint8_t *)request) + sizeof(request) + data_size);
	xo = _xpc_prim_create_flags(_XPC_TYPE_DICTIONARY, val, nvcount(val.nv), _XPC_FROM_WIRE);
	LOG("created xpc_object %p", xo);
	switch(tr->msgh_trailer_type) {
	case MACH_RCV_TRAILER_AUDIT:
	case MACH_RCV_TRAILER_CTX:
	case MACH_RCV_TRAILER_LABELS:
		auditp = &((mach_msg_audit_trailer_t *)tr)->msgh_audit;
	default:
		auditp = NULL;
	}
	if (auditp) {
		xo->xo_audit_token = malloc(sizeof(*auditp));
		memcpy(xo->xo_audit_token, auditp, sizeof(*auditp));
	}
	xpc_dictionary_set_mach_send(xo, XPC_RPORT, request->msgh_remote_port);
	*requestobj = xo;
	return (0);
}

int
xpc_call_wakeup(mach_port_t rport, int retcode)
{
	mig_reply_error_t msg;
	int err;
	kern_return_t kr;

	msg.Head.msgh_remote_port = rport;
	msg.RetCode = retcode;
	kr = mach_msg_send(&msg.Head);
	if (kr != KERN_SUCCESS)
		err = (kr == KERN_INVALID_TASK) ? EPIPE : EINVAL;
	else
		err = 0;

	return (err);
}
