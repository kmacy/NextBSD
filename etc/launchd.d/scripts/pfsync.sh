#!/bin/sh
#
# Removed dependency from /etc/rc.

pfsync_prestart()
{
	case "$pfsync_syncdev" in
	'')
		echo "pfsync_syncdev is not set."
		return 1
		;;
	esac

	# load pf kernel module if needed
	if ! kldstat -q -m pf ; then
		if kldload pf ; then
			echo "pf module loaded."
		else
			echo "pf module failed to load."
			return 1
		fi
	fi

	return 0
}

pfsync_start()
{
	echo "Enabling pfsync."
	ifconfig pfsync0 syncdev $pfsync_syncdev $pfsync_ifconfig up
}

pfsync_stop()
{
	echo "Disabling pfsync."
	ifconfig pfsync0 -syncdev down
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/pfsync.pid"
touch $pidfile
pfsync_prestart
pfsync_start
exit 0
