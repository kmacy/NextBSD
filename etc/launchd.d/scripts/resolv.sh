#!/bin/sh
#
# Removed dependency from /etc/rc.

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/resolv.pid"
touch $pidfile

# if the info is available via dhcp/kenv
# build the resolv.conf
#
if [ ! -e /etc/resolv.conf -a \
    -n "`/bin/kenv dhcp.domain-name-servers 2> /dev/null`" ]; then
	/bin/cat /dev/null > /etc/resolv.conf

	if [ -n "`/bin/kenv dhcp.domain-name 2> /dev/null`" ]; then
		echo domain `/bin/kenv dhcp.domain-name` > /etc/resolv.conf
	fi

        set -- `/bin/kenv dhcp.domain-name-servers`
        for ns in `IFS=','; echo $*`; do
                echo nameserver $ns >> /etc/resolv.conf;
        done
fi

