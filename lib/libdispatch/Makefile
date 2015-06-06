.include <bsd.init.mk>

LIB= dispatch

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/resolver

CFLAGS+=	-I${.CURDIR}/../../apsl/include -I${.CURDIR}/os -I${.CURDIR}/private
CFLAGS+=	-I${.CURDIR} -I${.CURDIR}/../../include -I${.CURDIR}/../../sys 
CFLAGS+=	-I. -I${.CURDIR}/include -fblocks
CFLAGS+=	-D__APPLE__ -DDISPATCH_USE_DTRACE=0
CFLAGS+=	-DDISPATCH_USE_SIMPLE_ASL=0 -DUSE_OBJC=0 -D__BLOCKS__=1

CFLAGS+=	-g -O0

include/machine: ${.CURDIR}/Makefile
	mkdir -p include
	ln -sf ../../sys/${MACHINE_CPUARCH}/include include/machine

protocolServer.c protocolUser.c protocol.h protocolServer.h: include/machine ${.CURDIR}/src/protocol.defs 
	mig ${CFLAGS}  -user protocolUser.c \
	-server protocolServer.c -header protocol.h \
	-sheader protocolServer.h ${.CURDIR}/src/protocol.defs


SRCS =  			\
	protocolUser.c \
	protocolServer.c \
	allocator.c		\
	apply.c			\
	benchmark.c		\
	data.c			\
	init.c			\
	introspection.c	\
	io.c			\
	object.c		\
	once.c			\
	queue.c			\
	semaphore.c		\
	source.c		\
	time.c			\
	transform.c		\
	voucher.c

SRCS+=	resolver.c
SRCS+=	object.m

CLEANFILES+= *.o *.So *.po *~ protocol.h protocolServer.c protocolServer.h protocolUser.c \
	*.so* *.a
.include <bsd.lib.mk>
