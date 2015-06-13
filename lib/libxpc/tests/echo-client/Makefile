.include <src.opts.mk>
PROG= xpc-echo-client
SRCS= xpc-echo-client.c
BINDIR?= /usr/bin

NO_WERROR= yes
MK_MAN=no
CFLAGS+= -g -D__APPLE__ -fblocks
CFLAGS+= -I. -I../../../../include -I../../../../apsl/include -I../../../../sys/sys -I../../../../sys
LDADD+= -lmach -lBlocksRuntime -ldispatch -losxsupport -llaunch -lnv -lsbuf -lxpc -pthread

.include <bsd.prog.mk>
