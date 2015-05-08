#!/bin/sh
#
# Removed dependency from /etc/rc.

savecore_prestart()
{
	#	${DUMPDIR} should be a directory or a symbolic link
	#	to the crash directory if core dumps are to be saved.
	#
	DUMPDIR="${dumpdir:-/var/crash}"

	# Quit if we have no dump device
	case ${dumpdev} in
	[Nn][Oo] | '')
		echo 'No dump device. Quitting.'
		return 1
		;;
	[Aa][Uu][Tt][Oo])
		dumpdev=`/bin/realpath /dev/dumpdev`
		;;
	esac

	# If there is no crash directory set it now
	case ${dumpdir} in
	'')
		dumpdir='/var/crash'
		;;
	[Nn][Oo])
		dumpdir='NO'
		;;
	esac

	if [ ! -c "${dumpdev}" ]; then
		echo "Dump device does not exist.  Savecore not run."
		return 1
	fi

	if [ ! -d "${dumpdir}" ]; then
		echo "Dump directory does not exist.  Savecore not run."
		return 1
	fi
	return 0
}

savecore_start()
{
	echo "Checking for core dump on ${dumpdev}..."
	savecore ${savecore_flags} ${DUMPDIR} ${dumpdev}
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/savecore.pid"
touch $pidfile
savecore_prestart
savecore_start
exit 0
