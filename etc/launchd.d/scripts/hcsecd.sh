#!/bin/sh
#
# Removed dependency from /etc/rc.

hcsecd_prestart()
{
	if ! kldstat -q -m ng_btsocket ; then
		if kldload ng_btsocket > /dev/null 2>&1 ; then
			echo 'ng_btsocket module loaded'
		else
			echo 'ng_btsocket module failed to load'
			return 1
		fi
	fi

	return 0
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/hcsecd.pid"
touch $pidfile
config="${hcsecd_config:-/etc/bluetooth/${name}.conf}"
command_args="-f ${config}"

hcsecd_prestart
exec hcsecd -d ${command_args}
exit 0
