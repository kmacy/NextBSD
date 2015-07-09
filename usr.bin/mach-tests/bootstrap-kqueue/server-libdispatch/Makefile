.include <src.opts.mk>
PROG= bootstrap-libdispatch-server-test
SRCS= bootstrap-libdispatch-server.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple -fblocks
LDADD+= -lSystem

.include <bsd.prog.mk>
