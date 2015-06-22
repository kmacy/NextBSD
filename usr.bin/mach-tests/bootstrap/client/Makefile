.include <src.opts.mk>
PROG= bootstrap-client-test
SRCS= bootstrap-client.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple
LDADD+= -lSystem

.include <bsd.prog.mk>
