#!/bin/sh
#
# Add additional swap files
#
# Moved to launchd-friendly format

name="addswap"
start_cmd="addswap_start"
stop_cmd=":"

addswap_start()
{
	case ${swapfile} in
	[Nn][Oo] | '')
		;;
	*)
		if [ -w "${swapfile}" ]; then
			echo "Adding ${swapfile} as additional swap"
			mdev=`mdconfig -a -t vnode -f ${swapfile}` && swapon /dev/${mdev}
		fi
		;;
	esac
}

# used to emulate "requires/provide" functionality
pidfile="/var/run/addswap.pid"
touch $pidfile

swapfile="$1"
addswap_start

exit 0
