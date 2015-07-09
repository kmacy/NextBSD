#!/bin/sh
#
# Removed dependency from /etc/rc.

# Last chance to set sysctl variables that failed the first time.
#
/etc/rc.d/sysctl lastload

securelevel_start()
{
	if [ ${kern_securelevel} -ge 0 ]; then
		echo 'Raising kernel security level: '
		${SYSCTL_W} kern.securelevel=${kern_securelevel}
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/securelevel.pid"
touch $pidfile
securelevel_start
exit 0
