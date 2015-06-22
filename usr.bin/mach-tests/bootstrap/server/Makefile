.include <src.opts.mk>
PROG= bootstrap-server-test
SRCS= bootstrap-server.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple
LDADD+= -lSystem

.include <bsd.prog.mk>
