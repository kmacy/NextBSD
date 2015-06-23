#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach/mach.h>
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

int main(int argc, char *argv[])
{
	kern_return_t kr;
	mach_port_t bport, port, reply_port;
	struct msg_recv message;
	int delay = 0;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <first message> <second message> [<delay>]\n", argv[0]);
		exit(1);
	}

	if (argc == 4)
		delay = atoi(argv[3]);

	task_get_special_port(mach_task_self(), TASK_BOOTSTRAP_PORT, &bport);
	printf("bootstrap port: %d\n", bport);

	kr = bootstrap_look_up(bootstrap_port, "mach.service-test", &port);
	if (kr != KERN_SUCCESS) {
		fprintf(stderr, "bootstrap_look_up: kr=%d\n", kr);
		exit(1);
	}

	printf("service port: %d\n", port);

	kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &reply_port);
	if (kr != KERN_SUCCESS) {
		fprintf(stderr, "mach_port_allocate: kr=%d\n", kr);
		exit(1);
	}

	kr = mach_port_insert_right(mach_task_self(), reply_port, reply_port, MACH_MSG_TYPE_MAKE_SEND);
	if (kr != KERN_SUCCESS) {
		fprintf(stderr, "mach_port_insert_right: kr=%d\n", kr);
		exit(1);
	}

	message.hdr.msgh_local_port = reply_port;
	message.hdr.msgh_remote_port = port;
	message.hdr.msgh_size = sizeof(struct msg_send);
	message.hdr.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND);
	strcpy(message.body, argv[1]);

	printf("message size: %d\n", message.hdr.msgh_size);

	kr = mach_msg((mach_msg_header_t *)&message, MACH_SEND_MSG | MACH_RCV_MSG,
	    sizeof(struct msg_send), sizeof(struct msg_recv), reply_port,
	    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

	if (kr != KERN_SUCCESS)
		printf("first mach_msg_send failure: kr=%d\n", kr);

	if (delay)
		sleep(delay);

	message.hdr.msgh_local_port = reply_port;
	message.hdr.msgh_remote_port = port;
	message.hdr.msgh_size = sizeof(struct msg_send);
	message.hdr.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND);
	strcpy(message.body, argv[2]);

	kr = mach_msg((mach_msg_header_t *)&message, MACH_SEND_MSG | MACH_RCV_MSG,
	    sizeof(struct msg_send), sizeof(struct msg_recv), reply_port,
	    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

	if (kr != KERN_SUCCESS)
		printf("second mach_msg_send failure: kr=%d\n", kr);
}
