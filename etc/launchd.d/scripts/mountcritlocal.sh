#!/bin/sh
#
# Removed dependency from /etc/rc.

mountcritlocal_start()
{
	# Set up the list of network filesystem types for which mounting
	# should be delayed until after network initialization.
	case ${extra_netfs_types} in
	[Nn][Oo])
		;;
	*)
		netfs_types="${netfs_types} ${extra_netfs_types}"
		;;
	esac

	# Mount everything except nfs filesystems.
	mount_excludes='no'
	for i in ${netfs_types}; do
		fstype=${i%:*}
		mount_excludes="${mount_excludes}${fstype},"
	done
	mount_excludes=${mount_excludes%,}
	mount -a -t ${mount_excludes}

	case $? in
	0)
		;;
	*)
		echo 'Mounting /etc/fstab filesystems failed,' \
		    ' startup aborted'
		kill -QUIT $$
		;;
	esac
}

# start here
# used to emulate "requires/provide" functionality
mountcritlocal_start
exit 0
