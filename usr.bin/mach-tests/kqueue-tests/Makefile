.include <src.opts.mk>
PROG= kqueue-tests
SRCS= kqueue-tests.c
BINDIR?= /usr/bin

MK_MAN=no
CFLAGS+= -D__APPLE__
LDADD+= -lmach -pthread

.include <bsd.prog.mk>
