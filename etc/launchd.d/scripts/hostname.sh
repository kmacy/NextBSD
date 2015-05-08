#!/bin/sh
#
# Removed dependency from /etc/rc.

CONFIGDB="/usr/share/solidbase/data/configdb"
SQLITE_CMD="/sbin/bsdsqlite3"
CONFIG_TABLE="host_conf"

hostname_start()
{
	# If we are not inside a jail, set the host name if it is not already set.
	# If we are inside a jail, set the host name even if it is already set,
	# but first check if it is permitted.
	#
	if [ `sysctl -n security.jail.jailed` -eq 1 ]; then
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
	if [ -x /usr/bin/logger ]; then
		logger "launchd: Setting hostname: `hostname`."
	fi
}

# get_hostname
#        Determine hostname from sqlitedb
#
get_hostname_sql()
{
	hostname=`${SQLITE_CMD} ${CONFIGDB} 'select value from '${CONFIG_TABLE}' where key="hostname"'`
}

get_hostname_xml()
{
	hostname=`/sbin/launch_xml -get pfsense.system.hostname`
	hostname=$hostname.`/sbin/launch_xml -get pfsense.system.domain`
}

# start here
# used to emulate "requires/provide" functionality

pidfile="/var/run/hostname.pid"
touch $pidfile
get_hostname_xml
/bin/hostname $hostname
hostname_start
exit 0

