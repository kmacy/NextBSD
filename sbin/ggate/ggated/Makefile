# $FreeBSD$

.PATH: ${.CURDIR}/../shared

PROG=	ggated
MAN=	ggated.8
SRCS=	ggated.c ggate.c

LIBADD=	pthread

CFLAGS+= -I${.CURDIR}/../shared

NO_WCAST_ALIGN=
.include <bsd.prog.mk>
