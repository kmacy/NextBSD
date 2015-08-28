# $FreeBSD$

LIB=	ohash
SRCS=	ohash.c
INTERNALLIB=

.if defined(NO_SHARED)
.if ${NO_SHARED} == "no" || ${NO_SHARED} == "NO"
CFLAGS+= -fPIC
.endif
.else
CFLAGS+= -fPIC
.endif

WARNS=	3

.include <bsd.lib.mk>
