.include <src.opts.mk>
PROG= bootstrap-portset-fiddling-server-test
SRCS= bootstrap-portset-fiddling-server.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS= -I${.CURDIR}/../../../include/apple -fblocks
LDADD+= -lSystem

.include <bsd.prog.mk>
