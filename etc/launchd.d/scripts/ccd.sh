#!/bin/sh
#
# Remove /etc/rc dependency

ccd_start()
{
	if [ -f /etc/ccd.conf ]; then
		echo "Configuring CCD devices."
		ccdconfig -C
	fi
}

# start here
pid_file="/var/run/ccd.pid"
touch ${pid_file}

ccd_start
exit 0