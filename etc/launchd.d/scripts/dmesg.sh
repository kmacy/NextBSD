#!/bin/sh
#
# Removed dependency from /etc/rc.

dmesg_file="/var/run/dmesg.boot"

do_dmesg()
{
	rm -f ${dmesg_file}
	( umask 022 ; /sbin/dmesg $rc_flags > ${dmesg_file} )
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/dmesg.pid"
touch $pidfile

do_dmesg
exit 0
