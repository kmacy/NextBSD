#!/bin/sh -
#
# Removed dependency from /etc/rc.

#
# List current mixer devices to stdout.
#
list_mixers()
{
	( cd /dev ; ls mixer* 2>/dev/null )
}

#
# Save state of an individual mixer specified as $1
#
mixer_save()
{
	local dev

	dev="/dev/${1}"
	if [ -r ${dev} ]; then
		/usr/sbin/mixer -f ${dev} -s > /var/db/${1}-state 2>/dev/null
	fi
}

#
# Restore the state of an individual mixer specified as $1
#
mixer_restore()
{
	local file dev

	dev="/dev/${1}"
	file="/var/db/${1}-state"
	if [ -r ${dev} -a -r ${file} ]; then
		/usr/sbin/mixer -f ${dev} `cat ${file}` > /dev/null
	fi
}

#
# Restore state of all mixers
#
mixer_start()
{
	local mixer

	for mixer in `list_mixers`; do
		mixer_restore ${mixer}
	done
}

#
# Save the state of all mixers
#
mixer_stop()
{
	local mixer

	for mixer in `list_mixers`; do
		mixer_save ${mixer}
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/mixer.pid"
touch $pidfile
mixer_start
exit 0
