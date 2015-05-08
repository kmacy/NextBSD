#!/bin/sh
#
# Removed dependency from /etc/rc.

nisdomain_start()
{
	# Set the domainname if we're using NIS
	#
	case ${nisdomainname} in
	[Nn][Oo]|'')
		;;
	*)
		domainname ${nisdomainname}
		echo "Setting NIS domain: `/bin/domainname`."
		;;
	esac
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/nisdomain.pid"
touch $pidfile
nisdomain_start
exit 0
