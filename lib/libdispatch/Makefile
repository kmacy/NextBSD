.include <bsd.init.mk>

LIB= dispatch

.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/dispatch
.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/resolver

CFLAGS+=	-I${.CURDIR}/../../apsl/include -I${.CURDIR}/os -I${.CURDIR}/private
CFLAGS+=	-I${.CURDIR} -I${MACHINE_INCLUDES} -I${.CURDIR}/../../include -I${.CURDIR}/../../sys 
CFLAGS+=	-I. -I${.CURDIR}/include -fblocks
CFLAGS+=	-D__APPLE__ -DDISPATCH_USE_DTRACE=0
CFLAGS+=	-DDISPATCH_USE_SIMPLE_ASL=0 -DUSE_OBJC=0 -D__BLOCKS__=1

protocolServer.c protocolUser.c protocol.h protocolServer.h: ${.CURDIR}/src/protocol.defs 
	mig ${CFLAGS}  -user protocolUser.c \
	-server protocolServer.c -header protocol.h \
	-sheader protocolServer.h ${.CURDIR}/src/protocol.defs

SRCS =  protocolUser.c \
	protocolServer.c \
	allocator.c \
	apply.c	\
	benchmark.c \
	data.c \
	init.c \
	introspection.c	\
	io.c \
	object.c \
	once.c \
	queue.c \
	semaphore.c \
	source.c \
	time.c \
	transform.c \
	voucher.c \
	resolver.c \
	object.m

INCSDIR= ${INCLUDEDIR}/dispatch
INCS=	base.h \
	block.h \
	data.h \
	dispatch.h \
	group.h \
	introspection.h \
	io.h \
	object.h \
	once.h \
	queue.h \
	semaphore.h \
	source.h \
	time.h

CLEANFILES+= protocol.h protocolServer.c protocolServer.h protocolUser.c

.include <bsd.lib.mk>
