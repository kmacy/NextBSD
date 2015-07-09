#!/bin/sh
#
# Removed dependency from /etc/rc.

PERMS="644"

motd_start()
{
	#	Update kernel info in /etc/motd
	#	Must be done *before* interactive logins are possible
	#	to prevent possible race conditions.
	#
	echo -n 'Updating motd'
	if [ ! -f /etc/motd ]; then
		install -c -o root -g wheel -m ${PERMS} /dev/null /etc/motd
	fi

	if [ ! -w /etc/motd ]; then
		echo ' ... /etc/motd is not writable, update failed.'
		return
	fi

	T=`mktemp -t motd`
	uname -v | sed -e 's,^\([^#]*\) #\(.* [1-2][0-9][0-9][0-9]\).*/\([^\]*\) $,\1 (\3) #\2,' > ${T}
	awk '{if (NR == 1) {if ($1 == "SolidBSD") {next} else {print "\n"$0}} else {print}}' < /etc/motd >> ${T}

	cmp -s $T /etc/motd || {
		cp $T /etc/motd
		chmod ${PERMS} /etc/motd
	}
	rm -f $T

	echo .
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/motd.pid"
touch $pidfile
motd_start
exit 0
