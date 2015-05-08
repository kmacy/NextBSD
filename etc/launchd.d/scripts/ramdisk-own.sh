#!/bin/sh -
#
# Removed dependency from /etc/rc.

ramdisk_own_start()
{
	for unit in $ramdisk_units; do
		device="/dev/md$unit"
		dir=`mount | grep $device | cut -d' ' -f3`

		eval owner=\$ramdisk_${unit}_owner
		eval perms=\$ramdisk_${unit}_perms

		[ "X$owner" != "X" ] && chown -f "$owner" $device $dir
		[ "X$perms" != "X" ] && chmod -f "$perms" /dev/md$unit $dir
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ramdisk-own.pid"
touch $pidfile
ramdisk_own_start
exit 0
