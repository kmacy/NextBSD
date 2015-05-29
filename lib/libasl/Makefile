# $FreeBSD$

LIB=	asl
SHLIB_MAJOR=	1
SHLIBDIR?= /usr/lib
WARNS=6

SRCS=	asl.c asl_ipcServer.c asl_ipcUser.c asl_core.c asl_file.c asl_msg.c asl_object.c asl_string.c syslog.c \
	asl_client.c asl_fd.c asl_legacy1.c asl_msg_list.c asl_store.c asl_util.c \
	asl_common.c asl_memory.c

MAN=	asl.3 syslog.3

CLEANFILES+= asl_core.h asl_legacy1.h asl_msg_list.h asl_private.h asl_string.h \
	asl_client.h asl_file.h asl_msg.h asl_object.h asl_store.h

asl_ipc.h asl_ipcServer.c asl_ipcUser.c: asl_ipc.defs 
	mig ${MIG_FLAGS} -sheader asl_ipc.h ${.CURDIR}/asl_ipc.defs

asl.c: asl_ipc.h asl_ipcServer.c

INCS=	asl.h
A= ${.CURDIR}/${MACHINE_CPUARCH}
H= ${.CURDIR}/../../include/mach
DEFS=${.CURDIR}/../../sys/compat/mach/defs


.PATH: ${.CURDIR}/mach
.PATH: ${A}
.PATH: ${A}/sys

.for _dir in ${.CURDIR}/../.. ${.CURDIR}/../../sys ${.CURDIR}/../../.. /sys /usr/src/sys
.if !defined(SYSDIR) && exists(${_dir}/kern/)
SYSDIR= ${_dir}
.endif
.endfor
.if !defined(SYSDIR) || !exists(${SYSDIR}/kern/)
.error "can't find kernel source tree"
.endif

DEFINES= -D__APPLE__ -fblocks -DLIBC_NO_LIBCRASHREPORTERCLIENT -DPRIVATE
CFLAGS+= -I${.CURDIR}/../../include -I. -I${A} -I${.CURDIR} -I./include -fblocks ${DEFINES}
CFLAGS+= -I${SYSDIR} -I${.CURDIR}/../../apsl/include -I${.CURDIR}/../liblaunch -I${.CURDIR} -I${.CURDIR}/../libdispatch -I${.CURDIR}/../libxpc
CFLAGS+= -I${.CURDIR}/../libosxsupport -I${.CURDIR}/../libnotify
CFLAGS+= -fPIC -DPRIVATE -D__MigTypeCheck=1 -DLIBSYSCALL_INTERFACE=1

.include <bsd.lib.mk>
