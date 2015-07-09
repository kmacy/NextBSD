#!/bin/sh
#
# Removed dependency from /etc/rc.

ipnat_precmd()
{
	# Make sure ipfilter is loaded before continuing
	if ! ${SYSCTL} net.inet.ipf.fr_pass >/dev/null 2>&1; then
		if kldload ipl; then
			echo 'IP-filter module loaded.'
		else
			echo 'IP-filter module failed to load.'
		fi
	fi
	return 0
}

ipnat_start()
{
	if [ ! -f ${ipnat_rules} ]; then
		echo 'NO IPNAT RULES'
		return 0
	fi
	echo "Installing NAT rules."
	${ipnat_program} -CF -f ${ipnat_rules} ${ipnat_flags}
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ipnat.pid"
touch $pidfile
ipnat_precmd
ipnat_start
exit 0
