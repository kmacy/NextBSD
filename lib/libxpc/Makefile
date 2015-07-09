.include <bsd.init.mk>

LIB= xpc
NO_WERROR= yes

.PATH: ${.CURDIR}/xpc

BASE_INCLUDE= -I${.CURDIR}/../../include/apple
BASE_INCLUDE+= -I${.CURDIR}/../libnv
BASE_INCLUDE+= -I${.CURDIR} -I${.CURDIR}/../../sbin/launchd -I.
BASE_INCLUDE+= -I${.CURDIR}/../../include -I${.CURDIR}/../../sys
CFLAGS+= ${BASE_INCLUDE} -fblocks -g -O0

SRCS=	xpc_dictionary.c \
	xpc_misc.c \
	xpc_type.c \
	xpc_array.c \
	xpc_connection.c \
	subr_nvlist.c \
	subr_nvpair.c

INCSDIR= ${INCLUDEDIR}/xpc
INCS=	activity.h \
	base.h \
	connection.h \
	debug.h \
	endpoint.h \
	launchd.h \
	xpc.h

.include <bsd.lib.mk>
