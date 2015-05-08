#!/bin/sh -
#
# Removed dependency from /etc/rc.

ramdisk_start()
{
	for unit in $ramdisk_units; do
		eval mdoptions=\$ramdisk_${unit}_config
		if [ "$mdoptions" = "${mdoptions##-t}" ]; then
			echo "Type not specified for md$unit"
			continue
		fi
		eval fsoptions=\$ramdisk_${unit}_newfs

		mdconfig -a $mdoptions -u $unit
		newfs $fsoptions /dev/md$unit
	done
}

ramdisk_stop()
{
	for unit in $ramdisk_units
	do
		if [ -c /dev/md$unit ]; then
			umount -f /dev/md$unit > /dev/null 2>&1
			mdconfig -d -u $unit
		fi
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ramdisk.pid"
touch $pidfile
ramdisk_start
exit 0
