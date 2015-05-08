#!/bin/sh
#
# Removed dependency from /etc/rc.

# Make a bounds file for msgs(1) if there isn't one already
#
if [ -d /var/msgs -a ! -f /var/msgs/bounds -a ! -L /var/msgs/bounds ]; then
	echo 0 > /var/msgs/bounds
fi

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/msgs.pid"
touch $pidfile