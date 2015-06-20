.include <src.opts.mk>
PROG= kqueue-tests
SRCS= kqueue-tests.c

MK_MAN=no
CFLAGS+= -D__APPLE__ -I${.CURDIR}/../../../include/apple
LDADD+= -lmach -pthread

.include <bsd.prog.mk>
