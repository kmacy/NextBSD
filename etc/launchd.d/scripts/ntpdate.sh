#!/bin/sh
#
# Removed dependency from /etc/rc.

ntpdate_start()
{
	if [ -z "$ntpdate_hosts" -a -f /etc/ntp.conf ]; then
		ntpdate_hosts=`awk '
			/^server[ \t]*127.127/      {next}
			/^(server|peer)/            {print $2}
		' </etc/ntp.conf`
	fi
	if [ -n "$ntpdate_hosts" -o -n "$rc_flags" ]; then
		echo "Setting date via ntp."
		${ntpdate_program:-ntpdate} $rc_flags $ntpdate_hosts
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ntpdate.pid"
touch $pidfile
ntpdate_program="/usr/sbin/ntpdate"
ntpdate_start
exit 0
