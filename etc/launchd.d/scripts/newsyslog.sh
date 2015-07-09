#!/bin/sh
#
# newsyslog wrapper for launchd

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/newsyslog.pid"
touch $pidfile

exec /usr/sbin/newsyslog -C

exit 0
