#!/bin/sh
#
# Removed dependency from /etc/rc.

#
# Support for legacy /etc/rc.early script
#
if [ -r /etc/rc.early ]; then
	. /etc/rc.early
fi

# used to emulate "requires/provide" functionality
pidfile="/var/run/early.pid"
touch $pidfile

exit 0
