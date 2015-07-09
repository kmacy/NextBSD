#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <dispatch/dispatch.h>
#include <servers/bootstrap.h>

struct msg_send
{
	mach_msg_header_t hdr;
	char body[256];
};

struct msg_recv
{
	mach_msg_header_t hdr;
	char body[256];
	mach_msg_trailer_t trailer;
};

int main()
{
	__block kern_return_t kr;
	mach_port_t bport, port;
	dispatch_source_t source;

	task_get_special_port(mach_task_self(), TASK_BOOTSTRAP_PORT, &bport);
	syslog(LOG_ERR, "bootstrap port: %d", bport);

	kr = bootstrap_check_in(bootstrap_port, "mach.service-test", &port);
	if (kr != KERN_SUCCESS) {
		syslog(LOG_ERR, "bootstrap_check_in: kr=%d", kr);
		exit(1);
	}

	syslog(LOG_ERR, "service port: %d", port);

	source = dispatch_source_create(
	    DISPATCH_SOURCE_TYPE_MACH_RECV, port, 0,
	    dispatch_get_main_queue());

	dispatch_source_set_event_handler(source, ^{
		struct msg_recv message;
		struct msg_send reply;

		message.hdr.msgh_local_port = port;
		message.hdr.msgh_size = sizeof(struct msg_recv);

		kr = mach_msg_receive((mach_msg_header_t *)&message);
		if (kr != KERN_SUCCESS)
			syslog(LOG_ERR, "mach_msg_receive failure: kr=%d", kr);
		else
			syslog(LOG_ERR, "received message on port %d: body=%s", message.hdr.msgh_remote_port, message.body);

		memset(&reply, 0, sizeof(struct msg_send));
		sprintf(&reply.body[0], "hello buddy");
		reply.hdr.msgh_local_port = MACH_PORT_NULL;
		reply.hdr.msgh_remote_port = message.hdr.msgh_remote_port;
		reply.hdr.msgh_size = sizeof(struct msg_send);
		reply.hdr.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
		kr = mach_msg_send((mach_msg_header_t *)&reply);
		if (kr != KERN_SUCCESS)
			syslog(LOG_ERR, "mach_msg_send failure: kr=%d", kr);
	});

	dispatch_resume(source);
	dispatch_main();
}

