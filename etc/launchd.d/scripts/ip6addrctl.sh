#!/bin/sh
#
# Removed dependency from /etc/rc.

ip6addrctl_prefer_ipv6()
{
	ip6addrctl flush >/dev/null 2>&1
	ip6addrctl add ::1/128		50	0
	ip6addrctl add ::/0		40	1
	ip6addrctl add 2002::/16	30	2
	ip6addrctl add ::/96		20	3
	ip6addrctl add ::ffff:0:0/96	10	4
	ip6addrctl
}

ip6addrctl_prefer_ipv4()
{
	ip6addrctl flush >/dev/null 2>&1
	ip6addrctl add ::ffff:0:0/96	50	0
	ip6addrctl add ::1/128		40	1
	ip6addrctl add ::/0		30	2
	ip6addrctl add 2002::/16	20	3
	ip6addrctl add ::/96		10	4
	ip6addrctl
}

ip6addrctl_start()
{
	if ifconfig lo0 inet6 >/dev/null 2>&1; then
		# We have IPv6 support in kernel.

		# install the policy of the address selection algorithm.
		if [ -f /etc/ip6addrctl.conf ]; then
			ip6addrctl flush >/dev/null 2>&1
			ip6addrctl install /etc/ip6addrctl.conf
			ip6addrctl
		else
			ip6addrctl_prefer_ipv6
		fi
	fi
}

ip6addrctl_stop()
{
	if ifconfig lo0 inet6 >/dev/null 2>&1; then
		# We have IPv6 support in kernel.
		ip6addrctl flush >/dev/null 2>&1
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ip6addrctl.pid"
touch $pidfile
ip6addrctl_start
exit 0
