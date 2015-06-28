.include <src.opts.mk>
PROG= bootstrap-inline-receive-server-test
SRCS= bootstrap-inline-receive-server.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple
LDADD+= -lSystem

.include <bsd.prog.mk>
