.include <bsd.init.mk>

LIB=	xpc
WARNS?=	6

BASE_INCLUDE= -I${.CURDIR}/../../apsl/include
BASE_INCLUDE+= -I${.CURDIR}/../libnv
BASE_INCLUDE+= -I${.CURDIR} -I${.CURDIR}/../../sbin/launchd -I.
BASE_INCLUDE+= -I${.CURDIR}/../../include -I${.CURDIR}/../../sys
CFLAGS+= ${BASE_INCLUDE} -fblocks -g -O0

SRCS = xpc_dictionary.c xpc_misc.c xpc_type.c xpc_array.c xpc_connection.c

NO_WERROR=yes

.include <bsd.lib.mk>
