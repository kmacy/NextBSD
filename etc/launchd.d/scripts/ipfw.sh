#!/bin/sh
#
# Removed dependency from /etc/rc.

ipfw_precmd()
{
	if ! ${SYSCTL} net.inet.ip.fw.enable > /dev/null 2>&1; then
		if ! kldload ipfw; then
			echo "unable to load firewall module."
			return 1
		fi
	fi

	return 0
}

ipfw_start()
{
	# set the firewall rules script if none was specified
	[ -z "${firewall_script}" ] && firewall_script=/etc/rc.firewall

	if [ -r "${firewall_script}" ]; then
		echo -n 'Starting divert daemons:'
		if [ -f /etc/rc.d/natd ] ; then
			/etc/rc.d/natd start
		fi
		. "${firewall_script}"
		echo -n 'Firewall rules loaded'
	elif [ "`ipfw list 65535`" = "65535 deny ip from any to any" ]; then
		echo 'Warning: kernel has firewall functionality, but' \
		    ' firewall rules are not enabled.'
		echo '           All ip services are disabled.'
	fi
	echo '.'

	# Firewall logging
	#
	echo 'Firewall logging enabled'
	sysctl net.inet.ip.fw.verbose=1 >/dev/null

	# Enable the firewall
	#
	${SYSCTL_W} net.inet.ip.fw.enable=1
}

ipfw_stop()
{
	# Disable the firewall
	#
	${SYSCTL_W} net.inet.ip.fw.enable=0
	if [ -f /etc/rc.d/natd ] ; then
		/etc/rc.d/natd stop
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ipfw.pid"
touch $pidfile
ipfw_precmd
ipfw_start
exit 0
