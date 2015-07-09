#!/bin/sh
#
# amd wrapper for launchd

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/amd.pid"
touch $pidfile

exec /usr/sbin/amd

exit 0
