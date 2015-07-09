#!/bin/sh
#
# Removed dependency from /etc/rc.

ipsec_prestart()
{
	if [ ! -f "$ipsec_file" ]; then
		echo "$ipsec_file not readable; ipsec start aborted."
			#
			# If booting directly to multiuser, send SIGTERM to
			# the parent (/etc/rc) to abort the boot
			#
		if [ "$autoboot" = yes ]; then
			echo "ERROR: ABORTING BOOT (sending SIGTERM to parent)!"
			kill -TERM $$
			exit 1
		fi
		return 1
	fi
	return 0
}

ipsec_start()
{
	echo "Installing ipsec manual keys/policies."
	${ipsec_program} -f $ipsec_file
}

ipsec_stop()
{
	echo "Clearing ipsec manual keys/policies."

	# still not 100% sure if we would like to do this.
	# it is very questionable to do this during shutdown session, since
	# it can hang any of remaining IPv4/v6 session.
	#
	${ipsec_program} -F
	${ipsec_program} -FP
}

ipsec_reload()
{
	echo "Reloading ipsec manual keys/policies."
	${ipsec_program} -F
	${ipsec_program} -FP
	${ipsec_program} -f "$ipsec_file"
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/ipsec.pid"
touch $pidfile
ipsec_program="/sbin/setkey"
ipsec_file="/etc/ipsecfile"

ipsec_prestart
ipsec_start
exit 0
