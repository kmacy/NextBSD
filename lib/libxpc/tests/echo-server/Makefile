.include <src.opts.mk>
PROG= xpc-echo-server
SRCS= xpc_echo_server.c

MK_MAN=no
CFLAGS+= -g -D__APPLE__ -fblocks
CFLAGS+= -I. -I../../../../include -I../../../../apsl/include -I../../../../sys/sys -I../../../../sys
LDADD+= -lmach -lBlocksRuntime -losxsupport -llaunch -ldispatch -lnv -lxpc -pthread -lsbuf

.include <bsd.prog.mk>
