#!/bin/sh
#
# Remove dependency from /etc/rc.

encswap_attach()
{
	while read device mountpoint type options rest ; do
		case ":${device}:${type}:${options}" in
		:#*)
			continue
			;;
		*.bde:swap:sw)
			passphrase=`dd if=/dev/random count=1 2>/dev/null | md5 -q`
			device="${device%.bde}"
			gbde init "${device}" -P "${passphrase}" || return 1
			gbde attach "${device}" -p "${passphrase}" || return 1
			;;
		*.eli:swap:sw)
			device="${device%.eli}"
			geli onetime ${geli_swap_flags} "${device}" || return 1
			;;
		esac
	done < /etc/fstab
}

encswap_detach()
{
	while read device mountpoint type options rest ; do
		case ":${device}:${type}:${options}" in
		:#*)
			continue
			;;
		*.bde:swap:sw)
			device="${device%.bde}"
			gbde detach "${device}"
			;;
		*.eli:swap:sw)
			# Nothing here, because geli swap devices should be
			# created with the auto-detach-on-last-close option.
			;;
		esac
	done < /etc/fstab
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/encswap.pid"
touch $pidfile

encswap_attach
exit 0
