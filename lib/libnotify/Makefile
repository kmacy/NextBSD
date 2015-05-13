.include <bsd.init.mk>

LIB=	notify
WARNS=	6

.PATH: .
.PATH: ../../sbin/launchd

BASE_INCLUDE= -I${.CURDIR}/../../apsl/include
BASE_INCLUDE+= -I${.CURDIR} -I. -I${.CURDIR}/../liblaunch -I${.CURDIR}/../libosxsupport
BASE_INCLUDE+= -I${.CURDIR}/../../include -I${.CURDIR}/../../sys
DEFINES= -D__APPLE__ -fblocks -DLIBC_NO_LIBCRASHREPORTERCLIENT -DPRIVATE
MIG_FLAGS= ${BASE_INCLUDE} 	${DEFINES}
CFLAGS+= ${MIG_FLAGS} -D__MigTypeCheck 

notify_ipc.h notify_ipcUser.c: notify_ipc.defs
	mig ${MIG_FLAGS} ${.CURDIR}/notify_ipc.defs

notify_client.c: notify_ipc.h

SRCS= libnotify.c notify_client.c table.c notify_ipcUser.c

machine:
	ln -s ${.CURDIR}/../../sys/${MACHINE_CPUARCH}/include machine

CLEANFILES+= *User.c *Server.c notify_ipc.h

.include <bsd.lib.mk>
