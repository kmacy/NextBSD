#!/bin/sh
#
# Removed dependency from /etc/rc.

ipfs_prestart()
{
	# Do not continue if either ipnat or ipfilter is not enabled or
	# if the ipfilter module is not loaded.
	#
	if ! sysctl net.inet.ipf.fr_pass >/dev/null 2>&1; then
		echo "ipfilter module is not loaded"
	fi
	return 0
}

ipfs_start()
{
	if [ -r /var/db/ipf/ipstate.ipf -a -r /var/db/ipf/ipnat.ipf ]; then
		${ipfs_program} -R ${rc_flags}
		rm -f /var/db/ipf/ipstate.ipf /var/db/ipf/ipnat.ipf
	fi
}

ipfs_stop()
{
	if [ ! -d /var/db/ipf ]; then
		mkdir /var/db/ipf
		chmod 700 /var/db/ipf
		chown root:wheel /var/db/ipf
	fi
	${ipfs_program} -W ${rc_flags}
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ipfs.pid"
touch $pidfile
ipfs_prestart
ipfs_start
exit 0
