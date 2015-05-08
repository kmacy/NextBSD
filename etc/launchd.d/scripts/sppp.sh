#!/bin/sh
#
# Removed dependency from /etc/rc.

sppp_start()
{
	# Special options for sppp(4) interfaces go here.  These need
	# to go _before_ the general ifconfig since in the case
	# of hardwired (no link1 flag) but required authentication, you
	# cannot pass auth parameters down to the already running interface.
	#
	for ifn in ${sppp_interfaces}; do
		eval spppcontrol_args=\$spppconfig_${ifn}
		if [ -n "${spppcontrol_args}" ]; then
			# The auth secrets might contain spaces; in order
			# to retain the quotation, we need to eval them
			# here.
			eval spppcontrol ${ifn} ${spppcontrol_args}
		fi
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/sppp.pid"
touch $pidfile
sppp_start
exit 0
