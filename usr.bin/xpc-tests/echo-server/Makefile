.include <src.opts.mk>
PROG= xpc-echo-server
SRCS= xpc-echo-server.c
BINDIR?= /usr/bin

NO_WERROR= yes
MK_MAN=no
CFLAGS+= -g -D__APPLE__ -fblocks
CFLAGS+= -I${.CURDIR}/../../../include/apple -I${.CURDIR}/../../../include
CFLAGS+= -I${.CURDIR}/../../../sys
LDADD+= -lmach -lBlocksRuntime -losxsupport -llaunch -ldispatch -lbsm -lnv -lxpc -pthread -lsbuf

.include <bsd.prog.mk>
