#!/bin/sh
#
# Removed dependency from /etc/rc.

_sockfile="/var/run/syslogd.sockets"
evalargs="rc_flags=\"\`set_socketlist\` \$rc_flags\""
altlog_proglist="named"

syslogd_precmd()
{
	#	Transitional symlink for old binaries
	#
	if [ ! -L /dev/log ]; then
		ln -sf /var/run/log /dev/log
	fi
	rm -f /var/run/log

	#	Create default list of syslog sockets to watch
	#
	( umask 022 ; > $_sockfile )

	#	If running named(8) or ntpd(8) chrooted, added appropriate
	#	syslog socket to list of sockets to watch.
	#
	for _l in $altlog_proglist; do
		eval _ldir=\$${_l}_chrootdir
		echo "${_ldir}/var/run/log" >> $_sockfile
	done

	#	If other sockets have been provided, change run_rc_command()'s
	#	internal copy of $syslogd_flags to force use of specific
	#	syslogd sockets.
	#
	if [ -s $_sockfile ]; then
		echo "/var/run/log" >> $_sockfile
		eval $evalargs
	fi

	return 0
}

set_socketlist()
{
	_socketargs=
	for _s in `cat $_sockfile | tr '\n' ' '` ; do
		_socketargs="-l $_s $_socketargs"
	done
	echo $_socketargs
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/syslogd.pid"
touch $pidfile
syslogd_precmd
exec /sbin/nodaemon /usr/sbin/syslogd /var/run/syslog.pid
