#!/bin/sh
#
# Removed dependency from /etc/rc.
# TODO: incomplete - need to work on /etc/network.subr

dhclient_start()
{
	# prevent unnecessicary restarts
	# XXX: should use a pidfile
	if [ -x /usr/bin/pgrep ]; then
		pids=`/usr/bin/pgrep -f "dhclient: $ifn(\$| .*)"`
		if [ -n "$pids" ]; then
			exit 0
		fi
	fi

	${dhclient_program} ${rc_flags} $ifn
}

dhclient_stop()
{
	ifconfig $ifn down	# cause dhclient to die
}

dhclient_program="/sbin/dhclient"
ifn="$2"

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/dhclient.pid"
touch $pidfile

dhclient_start
exit 0
