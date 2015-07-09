#!/bin/sh
#
# Removed dependency from /etc/rc.

hostname_start()
{
	# If we are not inside a jail, set the host name if it is not already set.
	# If we are inside a jail, set the host name even if it is already set,
	# but first check if it is permitted.
	#
	if [ `$SYSCTL_N security.jail.jailed` -eq 1 ]; then
		if [ `$SYSCTL_N security.jail.set_hostname_allowed` -eq 0 ]; then
			return
		fi
	elif [ -n "`/bin/hostname -s`" ]; then
		return
	else
		# If we're not in a jail and rc.conf doesn't specify a
		# hostname, see if we can get one from kenv.
		#
		if [ -z "${hostname}" -a \
		    -n "`/bin/kenv dhcp.host-name 2> /dev/null`" ]; then
			hostname=`/bin/kenv dhcp.host-name`
		fi
	fi

	/bin/hostname ${hostname}
	echo "Setting hostname: `hostname`."
}

hostname_start
exit 0

