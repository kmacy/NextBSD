#!/bin/sh
#
# Removed dependency from /etc/rc.

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/network_ipv6.pid"
touch $pidfile

echo -n 'Additional TCP options:'
case ${log_in_vain} in
[Nn][Oo] | '')
	log_in_vain=0
	;;
[Yy][Ee][Ss])
	log_in_vain=1
	;;
[0-9]*)
	;;
*)
	echo " invalid log_in_vain setting: ${log_in_vain}"
	log_in_vain=0
	;;
esac

[ "${log_in_vain}" -ne 0 ] && (
	echo -n " log_in_vain=${log_in_vain}"
	sysctl net.inet.tcp.log_in_vain="${log_in_vain}" >/dev/null
	sysctl net.inet.udp.log_in_vain="${log_in_vain}" >/dev/null
)
echo '.'
