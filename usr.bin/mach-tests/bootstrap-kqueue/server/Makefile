.include <src.opts.mk>
PROG= bootstrap-kqueue-server-test
SRCS= bootstrap-kqueue-server.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple
LDADD+= -lSystem

.include <bsd.prog.mk>
