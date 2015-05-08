#!/bin/sh
#
# Removed dependency from /etc/rc.

ipfilter_loaded()
{
	if ! kldstat -v | grep "ipfilter$" > /dev/null 2>&1; then
		return 1
	else
		return 0
	fi
}

ipfilter_prestart()
{
	# load ipfilter kernel module if needed
	if ! ipfilter_loaded; then
		if kldload ipl; then
			info 'IP-filter module loaded.'
		else
			err 1 'IP-filter module failed to load.'
		fi
	fi

	# check for ipfilter rules
	if [ ! -r "${ipfilter_rules}" ] && [ ! -r "${ipv6_ipfilter_rules}" ]
	then
		echo 'IP-filter: NO IPF RULES'
		return 1
	fi
	return 0
}

ipfilter_start()
{
	echo "Enabling ipfilter."
	if [ `sysctl -n net.inet.ipf.fr_running` -le 0 ]; then
		${ipfilter_program:-/sbin/ipf} -E
	fi
	${ipfilter_program:-/sbin/ipf} -Fa
	if [ -r "${ipfilter_rules}" ]; then
		${ipfilter_program:-/sbin/ipf} \
		    -f "${ipfilter_rules}" ${ipfilter_flags}
	fi
	${ipfilter_program:-/sbin/ipf} -6 -Fa
	if [ -r "${ipv6_ipfilter_rules}" ]; then
		${ipfilter_program:-/sbin/ipf} -6 \
		    -f "${ipv6_ipfilter_rules}" ${ipfilter_flags}
	fi
}

ipfilter_stop()
{
	# XXX - The ipf -D command is not effective for 'lkm's
	if [ `sysctl -n net.inet.ipf.fr_running` -eq 1 ]; then
		echo "Saving firewall state tables"
		${ipfs_program:-/sbin/ipfs} -W ${ipfs_flags}
		echo "Disabling ipfilter."
		${ipfilter_program:-/sbin/ipf} -D
	fi
}

ipfilter_reload()
{
	echo "Reloading ipfilter rules."

	${ipfilter_program:-/sbin/ipf} -I -Fa
	if [ -r "${ipfilter_rules}" ]; then
		${ipfilter_program:-/sbin/ipf} -I \
		    -f "${ipfilter_rules}" ${ipfilter_flags}
	fi
	${ipfilter_program:-/sbin/ipf} -I -6 -Fa
	if [ -r "${ipv6_ipfilter_rules}" ]; then
		${ipfilter_program:-/sbin/ipf} -I -6 \
		    -f "${ipv6_ipfilter_rules}" ${ipfilter_flags}
	fi
	${ipfilter_program:-/sbin/ipf} -s

}

ipfilter_resync()
{
	# Don't resync if ipfilter is not loaded
	if ! ipfilter_loaded; then
		 return
	fi
	${ipfilter_program:-/sbin/ipf} -y ${ipfilter_flags}
}

ipfilter_status()
{
	${ipfilter_program:-/sbin/ipf} -V
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ipfilter.pid"
touch $pidfile
ipfilter_prestart
ipfilter_start
exit 0
