#!/bin/sh
#
# Removed dependency from /etc/rc.

ugidfw_load()
{
	if [ -r "${bsdextended_script}" ]; then
		. "${bsdextended_script}"
	fi
}

ugidfw_precmd()
{
	if ! sysctl security.mac.bsdextended
          then kldload mac_bsdextended
	    if [ "$?" -ne "0" ]
	      then echo Unable to load the mac_bsdextended module.
	      return 1
	else
	  return 0
	  fi
	fi
	return 0
}

ugidfw_start()
{
	[ -z "${bsdextended_script}" ] && bsdextended_script=/etc/rc.bsdextended

	if [ -r "${bsdextended_script}" ]; then
		ugidfw_load
		echo "MAC bsdextended rules loaded."
	fi
}

ugidfw_stop()
{
	# Disable the policy
	#
	kldunload mac_bsdextended
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ugidfw.pid"
touch $pidfile
ugidfw_start
exit 0
