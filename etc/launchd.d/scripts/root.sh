#!/bin/sh
#
# Removed dependency from /etc/rc.

root_start()
{
	# root normally must be read/write, but if this is a BOOTP NFS
	# diskless boot it does not have to be.
	#
	case ${root_rw_mount} in
	[Nn][Oo] | '')
		;;
	*)
		if ! mount -uw /; then
			echo 'Mounting root filesystem rw failed, startup aborted'
			/bin/kill -QUIT $$
		fi
		;;
	esac

	umount -a >/dev/null 2>&1

	# If we booted a special kernel remove the record
	# so we will boot the default kernel next time.
	/sbin/nextboot -D
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/root.pid"
touch $pidfile
root_start
exit 0
