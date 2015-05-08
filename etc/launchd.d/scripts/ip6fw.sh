#!/bin/sh
#
# Removed dependency from /etc/rc.subr.

ip6fw_prestart()
{
	# Load IPv6 firewall module, if not already loaded
	if ! ${SYSCTL} net.inet6.ip6.fw.enable > /dev/null 2>&1; then
		kldload ip6fw && {
			echo 'Kernel IPv6 firewall module loaded.'
			return 0
		}
		echo 'IPv6 firewall kernel module failed to load.'
		return 1
	fi
}

ip6fw_start()
{
	# Specify default rules file if none provided
	if [ -z "${ipv6_firewall_script}" ]; then
		ipv6_firewall_script=/etc/rc.firewall6
	fi

	# Load rules
	#
	if [ -r "${ipv6_firewall_script}" ]; then
		. "${ipv6_firewall_script}"
		echo 'IPv6 Firewall rules loaded.'
	elif [ "`ip6fw l 65535`" = "65535 deny ipv6 from any to any" ]; then
		echo 'IPv6 firewall rules have not been loaded. Default' \
		    ' to DENY all access.'
	fi

	# Enable firewall logging
	#
	echo 'IPv6 Firewall logging=YES'
	sysctl net.inet6.ip6.fw.verbose=1 >/dev/null

	# Enable the firewall
	#
	${SYSCTL_W} net.inet6.ip6.fw.enable=1
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ip6fw.pid"
touch $pidfile
ip6fw_prestart
ip6fw_start
exit 0
