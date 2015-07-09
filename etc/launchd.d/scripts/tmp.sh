#!/bin/sh
#
# Removed dependency from /etc/rc.

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/tmp.pid"
touch $pidfile

# If we do not have a writable /tmp, create a memory
# filesystem for /tmp.  If /tmp is a symlink (e.g. to /var/tmp,
# then it should already be writable).
#
case "${tmpmfs}" in
[Yy][Ee][Ss])
	mount_md ${tmpsize} /tmp "${tmpmfs_flags}"
	chmod 01777 /tmp
	;;
[Nn][Oo])
	;;
*)
	if (/bin/mkdir -p /tmp/.diskless 2> /dev/null); then
		rmdir /tmp/.diskless
	else
		if [ -h /tmp ]; then
			echo "*** /tmp is a symlink to a non-writable area!"
			echo "dropping into shell, ^D to continue anyway."
			/bin/sh
		else
			mount_md ${tmpsize} /tmp "${tmpmfs_flags}"
			chmod 01777 /tmp
		fi
	fi
	;;
esac
