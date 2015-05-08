#!/bin/sh
#
# Removed dependency from /etc/rc.

bgfsck_start ()
{
	bgfsck_msg='Starting background file system checks'
	if [ ${background_fsck_delay:=0} -gt 0 ]; then
		bgfsck_msg="${bgfsck_msg} in ${background_fsck_delay} seconds"
	fi
	echo "${bgfsck_msg}."

	(sleep ${background_fsck_delay}; nice -4 fsck -B -p) 2>&1 | \
	    logger -p daemon.notice -t fsck
}

# start here
# used to emulate "requires/provide" functionality
background_fsck_delay=60

bgfsck_start

exit 0
