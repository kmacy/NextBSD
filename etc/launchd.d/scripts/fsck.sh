#!/bin/sh -x
#
# Removed dependency from /etc/rc.

stop_boot()
{
	#	Terminate the process (which may include the parent /etc/rc)
	#	if booting directly to multiuser mode.
	#
	if [ "$autoboot" = yes ]; then
		kill -TERM $$
	fi
	exit 1
}

fsck_start()
{
	if [ "$autoboot" = no ]; then
		echo "Fast boot: skipping disk checks."
	elif [ ! -r /etc/fstab ]; then
		echo "Warning! No /etc/fstab: skipping disk checks."
	elif [ "$autoboot" = yes ]; then
					# During fsck ignore SIGQUIT
		trap : 3

		echo "Starting file system checks now:"
		fsck -F -p

		case $? in
		0)
			;;
		2)
			stop_boot
			;;
		4)
			echo "Rebooting..."
			reboot
			echo "Reboot failed; help!"
			stop_boot
			;;
		8)
			echo "File system preen failed, trying fsck -y."
			fsck -y
			case $? in
			0)
				;;
			*)
			echo "Automatic file system check failed; help!"
				stop_boot
				;;
			esac
			;;
		12)
			echo "Boot interrupted."
			stop_boot
			;;
		130)
			stop_boot
			;;
		*)
			echo "Unknown error; help!"
			stop_boot
			;;
		esac
	fi
}

# start here
autoboot="yes"

fsck_start
exit 0
