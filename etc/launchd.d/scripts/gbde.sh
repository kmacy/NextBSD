#!/bin/sh
#
# Removed dependency from /etc/rc

find_gbde_devices()
{
	case "${gbde_devices-auto}" in
	[Aa][Uu][Tt][Oo])
		gbde_devices=""
		;;
	*)
		return 0
		;;
	esac

	case "$1" in
	start)
		fstab="/etc/fstab"
		;;
	stop)
		fstab=$(mktemp /tmp/mtab.XXXXXX)
		mount -p >${fstab}
		;;
	esac

	#
	# We can't use "mount -p | while ..." because when a shell loop
	# is the target of a pipe it executes in a subshell, and so can't
	# modify variables in the script.
	#
	while read device mountpt type options dump pass; do
		case "$device" in
		*.bde)
			# Ignore swap devices
			case "$type" in
			swap)
				continue
				;;
			esac

			gbde_devices="${gbde_devices} ${device}"
		esac
	done <${fstab}

	case "$1" in
	stop)
		rm -f ${fstab}
		;;
	esac

	return 0
}

gbde_start()
{
	for device in $gbde_devices; do
		parent=${device%.bde}
		parent=${parent#/dev/}
		parent_=`ltr ${parent} '/' '_'`
		eval "lock=\${gbde_lock_${parent_}-\"${gbde_lockdir}/${parent_}.lock\"}"
		if [ -e "/dev/${parent}" -a ! -e "/dev/${parent}.bde" ]; then
			echo "Configuring Disk Encryption for ${parent}."

			count=1
			while [ ${count} -le ${gbde_attach_attempts} ]; do
				if [ -e "${lock}" ]; then
					gbde attach ${parent} -l ${lock}
				else
					gbde attach ${parent}
				fi
				if [ -e "/dev/${parent}.bde" ]; then
					break
				fi
				echo "Attach failed; attempt ${count} of ${gbde_attach_attempts}."
				count=$((${count} + 1))
			done
		fi
	done
}

gbde_stop()
{
	for device in $gbde_devices; do
		parent=${device%.bde}
		parent=${parent#/dev/}
		if [ -e "/dev/${parent}.bde" ]; then
			umount "/dev/${parent}.bde" 2>/dev/null
			gbde detach "${parent}"
		fi
	done
}

## start here ##
# used to emulate "requires/provide" functionality
pidfile="/var/run/gbde.pid"
touch $pidfile

gbde_start
exit 0
