.include <src.opts.mk>
PROG= bootstrap-kqueue-client-test
SRCS= bootstrap-kqueue-client.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple
LDADD+= -lSystem

.include <bsd.prog.mk>
