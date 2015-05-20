/*
 * Copyright 2014-2015 iXsystems, Inc.
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <errno.h>
#include <mach/mach.h>
#include <servers/bootstrap.h>
#include <xpc/xpc.h>
#include <machine/atomic.h>
#include <Block.h>
#include "xpc_internal.h"

#define XPC_CONNECTION_NEXT_ID(conn) (atomic_fetchadd_int(&conn->xc_last_id, 1))

static void xpc_connection_recv_message();
static void xpc_send(xpc_connection_t xconn, xpc_object_t message, uint64_t id);

xpc_connection_t
xpc_connection_create(const char *name, dispatch_queue_t targetq)
{
	kern_return_t kr;
	char *qname;
	struct xpc_connection *conn;

	if ((conn = malloc(sizeof(struct xpc_connection))) == NULL) {
		errno = ENOMEM;
		return (NULL);
	}

	memset(conn, 0, sizeof(struct xpc_connection));
	conn->xc_last_id = 1;
	TAILQ_INIT(&conn->xc_peers);
	TAILQ_INIT(&conn->xc_pending);

	/* Create connection queue */
	asprintf(&qname, "com.ixsystems.xpc.connection.%p", conn);
	conn->xc_queue = dispatch_queue_create(qname, NULL);

	/* Queue is initially suspended */
	dispatch_suspend(conn->xc_queue);

	/* Create local port */
	kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
	    &conn->xc_local_port);
	if (kr != KERN_SUCCESS) {
		errno = EPERM;
		return (NULL);
	}

	kr = mach_port_insert_right(mach_task_self(), conn->xc_local_port,
	    conn->xc_local_port, MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS) {
		errno = EPERM;
		return (NULL);
	}

	return (conn);
}

xpc_connection_t
xpc_connection_create_mach_service(const char *name, dispatch_queue_t targetq,
    uint64_t flags)
{
	kern_return_t kr;
	struct xpc_connection *conn;

	conn = xpc_connection_create(name, targetq);
	if (conn == NULL)
		return (NULL);

	conn->xc_flags = flags;

	if (flags & XPC_CONNECTION_MACH_SERVICE_LISTENER) {
		kr = bootstrap_check_in(bootstrap_port, name,
		    &conn->xc_local_port);
		if (kr != KERN_SUCCESS) {
			errno = EBUSY;
			free(conn);
			return (NULL);
		}

		return (conn);	
	}

	/* Look up named mach service */
	kr = bootstrap_look_up(bootstrap_port, name, &conn->xc_remote_port);
	if (kr != KERN_SUCCESS) {
		errno = ENOENT;
		free(conn);
		return (NULL);
	}

	return (conn);
}

void
xpc_connection_set_target_queue(xpc_connection_t connection,
    dispatch_queue_t targetq)
{

}

void
xpc_connection_set_event_handler(xpc_connection_t xconn,
    xpc_handler_t handler)
{
	struct xpc_connection *conn;

	debugf("connection=%p", xconn);
	conn = xconn;
	conn->xc_handler = (xpc_handler_t)Block_copy(handler);
}

void
xpc_connection_suspend(xpc_connection_t xconn)
{
	struct xpc_connection *conn;

	conn = xconn;
	dispatch_suspend(conn->xc_recv_source);
}

void
xpc_connection_resume(xpc_connection_t xconn)
{
	struct xpc_connection *conn;

	debugf("connection=%p", xconn);
	conn = xconn;

	/* Create dispatch source for top-level connection */
	if (conn->xc_parent == NULL) {
		conn->xc_recv_source = dispatch_source_create(
		    DISPATCH_SOURCE_TYPE_MACH_RECV, conn->xc_local_port, 0,
		    dispatch_get_main_queue());
		dispatch_set_context(conn->xc_recv_source, conn);
		dispatch_source_set_event_handler_f(conn->xc_recv_source,
		    xpc_connection_recv_message);
		dispatch_resume(conn->xc_recv_source);
	}

	dispatch_resume(conn->xc_queue);
}

void
xpc_connection_send_message(xpc_connection_t xconn,
    xpc_object_t message)
{
	struct xpc_connection *conn;
	uint64_t id;

	conn = xconn;
	id = xpc_dictionary_get_uint64(message, XPC_SEQID);

	if (id == 0)
		id = XPC_CONNECTION_NEXT_ID(conn);

	dispatch_async(conn->xc_queue, ^{
		xpc_send(conn, message, id);
	});
}

void
xpc_connection_send_message_with_reply(xpc_connection_t xconn,
    xpc_object_t message, dispatch_queue_t targetq, xpc_handler_t handler)
{
	struct xpc_connection *conn;
	struct xpc_pending_call *call;

	conn = xconn;
	call = malloc(sizeof(struct xpc_pending_call));
	call->xp_id = XPC_CONNECTION_NEXT_ID(conn);
	call->xp_handler = handler;
	TAILQ_INSERT_TAIL(&conn->xc_pending, call, xp_link);

	dispatch_async(conn->xc_queue, ^{
		xpc_send(conn, message, call->xp_id);
	});

}

xpc_object_t
xpc_connection_send_message_with_reply_sync(xpc_connection_t conn,
    xpc_object_t message)
{

}

void
xpc_main(xpc_connection_handler_t handler)
{

	dispatch_main();
}

static void
xpc_send(xpc_connection_t xconn, xpc_object_t message, uint64_t id)
{
	struct xpc_connection *conn;
	kern_return_t kr;

	conn = xconn;
	kr = xpc_pipe_send(message, conn->xc_remote_port,
	    conn->xc_local_port, id);	
}

static void
xpc_connection_recv_message(void *context)
{
	struct xpc_pending_call *call;
	struct xpc_connection *conn, *peer;
	xpc_object_t result;
	mach_port_t remote;
	kern_return_t kr;
	uint64_t id;

	debugf("connection=%p", context);

	conn = context;
	kr = xpc_pipe_receive(conn->xc_local_port, &remote, &result, &id);
	if (kr != KERN_SUCCESS)
		return;

	debugf("message=%p, id=%d, remote=<%d>", result, id, remote);

	if (conn->xc_flags & XPC_CONNECTION_MACH_SERVICE_LISTENER) {
		TAILQ_FOREACH(peer, &conn->xc_peers, xc_link) {
			if (remote == peer->xc_remote_port) {
				dispatch_async(peer->xc_queue, ^{
					peer->xc_handler(result);
				});
				return;
			}
		}

		debugf("new peer on port <%u>", remote);

		/* New peer */
		peer = xpc_connection_create(NULL, NULL);
		peer->xc_parent = conn;
		peer->xc_remote_port = remote;
		TAILQ_INSERT_TAIL(&conn->xc_peers, peer, xc_link);

		dispatch_async(conn->xc_queue, ^{
			conn->xc_handler(peer);
		});

		dispatch_async(peer->xc_queue, ^{
			peer->xc_handler(result);
		});
	} else {
		TAILQ_FOREACH(call, &conn->xc_pending, xp_link) {
			if (call->xp_id == id) {
				dispatch_async(conn->xc_queue, ^{
					call->xp_handler(result);
					TAILQ_REMOVE(&conn->xc_pending, call,
					    xp_link);
					free(call);
				});
				return;
			}
		}

		if (conn->xc_handler) {
			dispatch_async(conn->xc_queue, ^{
				conn->xc_handler(result);
			});
		}
	}
}
