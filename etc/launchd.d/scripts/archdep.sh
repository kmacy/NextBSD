#!/bin/sh
#
# Removed dependency from /etc/rc.

# should we print out unaligned access warnings?
#
unaligned_warnings()
{
	sysctl machdep.unaligned_print=0
}

# Alpha OSF/1 binary emulation
#
osf1_compat()
{
	echo -n ' OSF/1'
	if ! kldstat -v | grep osf1_ecoff > /dev/null; then
		kldload osf1 > /dev/null 2>&1
	fi
}

# SCO binary emulation
#
ibcs2_compat()
{
	echo -n ' ibcs2'
	kldload ibcs2 > /dev/null 2>&1
	case ${ibcs2_loaders} in
	[Nn][Oo])
		;;
	*)
		for i in ${ibcs2_loaders}; do
			kldload ibcs2_$i > /dev/null 2>&1
		done
		;;
	esac
}

archdep_start()
{
	_arch=`${SYSCTL_N} hw.machine_arch`
	echo -n "Initial $_arch initialization:"
	case $_arch in
	i386)
		ibcs2_compat
		;;
	alpha)
		osf1_compat
		unaligned_warnings
		;;
	esac
	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/archdep.pid"
touch $pidfile

archdep_start

exit 0

