#	From: @(#)Makefile	8.3 (Berkeley) 1/7/94
# $FreeBSD$

.include <src.opts.mk>

# XXX MISSING:		deroff diction graph learn plot
#			spell spline struct xsend
# XXX Use GNU versions: diff ld patch
# Moved to secure: bdes
#

SUBDIR=	alias \
	apply \
	asa \
	aslutil \
	awk \
	banner \
	basename \
	brandelf \
	bsdiff \
	bzip2 \
	bzip2recover \
	cap_mkdb \
	chat \
	chpass \
	cksum \
	cmp \
	col \
	colldef \
	colrm \
	column \
	comm \
	compress \
	cpuset \
	csplit \
	ctlstat \
	cut \
	dirname \
	dpv \
	du \
	elf2aout \
	elfdump \
	enigma \
	env \
	expand \
	false \
	fetch \
	find \
	fmt \
	fold \
	fstat \
	fsync \
	gcore \
	gencat \
	getconf \
	getent \
	getopt \
	grep \
	gzip \
	head \
	hexdump \
	id \
	ident \
	ipcrm \
	ipcs \
	join \
	jot \
	keylogin \
	keylogout \
	killall \
	ktrace \
	ktrdump \
	lam \
	lastcomm \
	ldd \
	leave \
	less \
	lessecho \
	lesskey \
	limits \
	locale \
	lock \
	lockf \
	logger \
	login \
	logins \
	logname \
	look \
	lorder \
	lsvfs \
	lzmainfo \
	m4 \
	${_mach_tests} \
	mandoc \
	mesg \
	migcom \
	minigzip \
	ministat \
	mkdep \
	mkfifo \
	mkimg \
	mklocale \
	mktemp \
	mkulzma \
	mkuzip \
	mt \
	ncal \
	netstat \
	newgrp \
	nfsstat \
	nice \
	nl \
	numactl \
	nohup \
	notifyutil \
	opieinfo \
	opiekey \
	opiepasswd \
	pagesize \
	passwd \
	paste \
	patch \
	pathchk \
	perror \
	pr \
	printenv \
	printf \
	procstat \
	protect \
	rctl \
	renice \
	rev \
	revoke \
	rpcinfo \
	rs \
	rup \
	rusers \
	rwall \
	script \
	sed \
	send-pr \
	seq \
	shar \
	showmount \
	sockstat \
	soelim \
	sort \
	split \
	stat \
	stdbuf \
	su \
	systat \
	tabs \
	tail \
	tar \
	tcopy \
	tee \
	time \
	timeout \
	tip \
	top \
	touch \
	tput \
	tr \
	true \
	truncate \
	tset \
	tsort \
	tty \
	uname \
	unexpand \
	uniq \
	unzip \
	units \
	unvis \
	uudecode \
	uuencode \
	vis \
	vmstat \
	w \
	wall \
	wc \
	what \
	whereis \
	which \
	whois \
	write \
	xargs \
	xinstall \
	xo \
	${_xlint} \
	${_xstr} \
	xz \
	xzdec \
	yes

# NB: keep these sorted by MK_* knobs

SUBDIR.${MK_AT}+=	at
SUBDIR.${MK_ATM}+=	atm
SUBDIR.${MK_BLUETOOTH}+=	bluetooth
SUBDIR.${MK_BSD_CPIO}+=	cpio
SUBDIR.${MK_CALENDAR}+=	calendar
SUBDIR.${MK_CLANG}+=	clang
SUBDIR.${MK_EE}+=	ee
SUBDIR.${MK_FILE}+=	file
SUBDIR.${MK_FINGER}+=	finger
SUBDIR.${MK_FTP}+=	ftp
SUBDIR.${MK_GAMES}+=	caesar
SUBDIR.${MK_GAMES}+=	factor
SUBDIR.${MK_GAMES}+=	fortune
SUBDIR.${MK_GAMES}+=	grdc
SUBDIR.${MK_GAMES}+=	morse
SUBDIR.${MK_GAMES}+=	number
SUBDIR.${MK_GAMES}+=	pom
SUBDIR.${MK_GAMES}+=	primes
SUBDIR.${MK_GAMES}+=	random
SUBDIR.${MK_GPL_DTC}+=	dtc
SUBDIR.${MK_GROFF}+=	vgrind
SUBDIR.${MK_HESIOD}+=	hesinfo
SUBDIR.${MK_ICONV}+=	iconv
SUBDIR.${MK_ICONV}+=	mkcsmapper
SUBDIR.${MK_ICONV}+=	mkesdb
SUBDIR.${MK_ISCSI}+=	iscsictl
SUBDIR.${MK_KDUMP}+=	kdump
SUBDIR.${MK_KDUMP}+=	truss
SUBDIR.${MK_KERBEROS_SUPPORT}+=	compile_et
SUBDIR.${MK_LDNS_UTILS}+=	drill
SUBDIR.${MK_LDNS_UTILS}+=	host
SUBDIR.${MK_LOCATE}+=	locate
# XXX msgs?
.if ${MK_MAIL} != "no"
SUBDIR+=	biff
SUBDIR+=	from
SUBDIR+=	mail
SUBDIR+=	msgs
.endif

.if ${MK_MAKE} != "no"
SUBDIR+=	bmake
.endif

.if ${MK_MAN_UTILS} != "no"
SUBDIR+=	catman
.if ${MK_MANDOCDB} == "no"
SUBDIR+=	makewhatis
.endif
SUBDIR+=	man
.endif

.if ${MK_NETCAT} != "no"
SUBDIR+=	nc
.endif

.if ${MK_NIS} != "no"
SUBDIR+=	ypcat
SUBDIR+=	ypmatch
SUBDIR+=	ypwhich
.endif

.if ${MK_OPENSSH} != "no"
SUBDIR+=	ssh-copy-id
.endif

.if ${MK_OPENSSL} != "no"
SUBDIR+=	bc
SUBDIR+=	chkey
SUBDIR+=	dc
SUBDIR+=	newkey
.endif

.if ${MK_QUOTAS} != "no"
SUBDIR+=	quota
.endif

.if ${MK_RCMDS} != "no"
SUBDIR+=	rlogin
SUBDIR+=	rsh
SUBDIR+=	ruptime
SUBDIR+=	rwho
.endif

.if ${MK_TALK} != "no"
SUBDIR+=	talk
.endif

.if ${MK_TELNET} != "no"
SUBDIR+=	telnet
.endif

.if ${MACHINE_CPUARCH} == "amd64"
#_xpc_tests=     xpc-tests
_mach_tests=	mach-tests
.endif

.if ${MK_TESTS} != "no"
SUBDIR+=	tests
.endif

.if ${MK_TEXTPROC} != "no"
SUBDIR+=	checknr
SUBDIR+=	colcrt
SUBDIR+=	ul
.endif

.if ${MK_TFTP} != "no"
SUBDIR+=	tftp
.endif

.if ${MK_TOOLCHAIN} != "no"
SUBDIR+=	addr2line
SUBDIR+=	ar
SUBDIR+=	c89
SUBDIR+=	c99
SUBDIR+=	ctags
SUBDIR+=	cxxfilt
SUBDIR+=	elfcopy
SUBDIR+=	file2c
.if ${MACHINE_ARCH} != "aarch64" # ARM64TODO gprof does not build
SUBDIR.${MK_TOOLCHAIN}+=	gprof
.endif
SUBDIR.${MK_TOOLCHAIN}+=	indent
SUBDIR.${MK_TOOLCHAIN}+=	lex
SUBDIR.${MK_TOOLCHAIN}+=	mkstr
SUBDIR.${MK_TOOLCHAIN}+=	nm
SUBDIR.${MK_TOOLCHAIN}+=	readelf
SUBDIR.${MK_TOOLCHAIN}+=	rpcgen
SUBDIR.${MK_TOOLCHAIN}+=	unifdef
SUBDIR.${MK_TOOLCHAIN}+=	size
SUBDIR.${MK_TOOLCHAIN}+=	strings
.if ${MACHINE_ARCH} != "aarch64" # ARM64TODO xlint does not build
SUBDIR.${MK_TOOLCHAIN}+=	xlint
.endif
SUBDIR.${MK_TOOLCHAIN}+=	xstr
SUBDIR.${MK_TOOLCHAIN}+=	yacc
SUBDIR.${MK_VI}+=	vi
SUBDIR.${MK_VT}+=	vtfontcvt
SUBDIR.${MK_USB}+=	usbhidaction
SUBDIR.${MK_USB}+=	usbhidctl
SUBDIR.${MK_UTMPX}+=	last
SUBDIR.${MK_UTMPX}+=	users
SUBDIR.${MK_UTMPX}+=	who
SUBDIR.${MK_SVN}+=	svn
SUBDIR.${MK_SVNLITE}+=	svn

.include <bsd.arch.inc.mk>

SUBDIR:=	${SUBDIR:O:u}

SUBDIR_PARALLEL=

.include <bsd.subdir.mk>
