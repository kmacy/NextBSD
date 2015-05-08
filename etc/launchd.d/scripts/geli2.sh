#!/bin/sh
#
# Removed dependency from /etc/rc

geli2_start()
{
	devices=`geli_make_list`

	for provider in ${devices}; do
		provider_=`ltr ${provider} '/' '_'`
		geli detach -l ${provider}
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/geli2.pid"
touch $pidfile
geli2_start
exit 0
