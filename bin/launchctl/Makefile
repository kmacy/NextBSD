.include <bsd.own.mk>

PROG= launchctl
MAN=

BASE_INCLUDE= -I${.CURDIR}/../../include/apple

CFLAGS+= ${BASE_INCLUDE} -fblocks
CFLAGS+= -I${.CURDIR}/../../lib/liblaunch
CFLAGS+= -I${.CURDIR}/../../include/mach
LDFLAGS+= -L${.CURDIR}/../../lib/liblaunch -llaunch
LDFLAGS+= -L${.CURDIR}/../../lib/libmach -lmach
LDFLAGS+= -L${.CURDIR}/../../lib/libdispatch -ldispatch
LDFLAGS+= -L${.CURDIR}/../../lib/libosxsupport -losxsupport
LDFLAGS+= -L/usr/local/lib -lBlocksRuntime -pthread
DPADD+= ${LIBJANSSON}
LDADD+= -lutil
LDADD+= -ljansson
SRCS= launchctl.c

.include <bsd.prog.mk>
