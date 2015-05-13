.include <bsd.init.mk>

LIB=	launch

.PATH: .
.PATH: ../../sbin/launchd



BASE_INCLUDE= -I${.CURDIR}/../../apsl/include
BASE_INCLUDE+= -I${.CURDIR} -I${.CURDIR}/../../sbin/launchd -I.
BASE_INCLUDE+= -I${.CURDIR}/../../include -I${.CURDIR}/../../sys
DEFINES= -DTARGET_CPU_X86=1 -D__APPLE__ -fblocks -DLIBC_NO_LIBCRASHREPORTERCLIENT -DPRIVATE
MIG_FLAGS= ${BASE_INCLUDE} 	${DEFINES}
CFLAGS+= ${MIG_FLAGS} -D__MigTypeCheck -g -O0
CFLAGS+= -I${.CURDIR}/../../contrib/openbsm

helper.h launchd_helperUser.c launchd_helperServer.c helperServer.h: helper.defs machine
	mig ${MIG_FLAGS} -header helper.h -sheader helperServer.h ${.CURDIR}/helper.defs
job.h jobUser.c jobServer.h: job.defs machine
	mig ${MIG_FLAGS} -sheader jobServer.h ${.CURDIR}/job.defs

SRCS=  jobUser.c launchd_helperUser.c launchd_helperServer.c
SRCS+= libbootstrap.c liblaunch.c libvproc.c

machine:
	ln -s ${.CURDIR}/../../sys/${MACHINE_CPUARCH}/include machine

libvproc.c: job.h

CLEANFILES+= machine *User.c *Server.c job.h helper.h *Server.h launchd_helper.h *~

.include <bsd.lib.mk>
