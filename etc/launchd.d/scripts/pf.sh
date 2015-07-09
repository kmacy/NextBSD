#!/bin/sh
#
# Removed dependency from /etc/rc.

pf_prestart()
{
	# load pf kernel module if needed
	if ! kldstat -q -m pf ; then
		if kldload pf ; then
			echo 'pf module loaded.'
		else
			echo 'pf module failed to load.'
			return 1
		fi
	fi
	return 0
}

pf_start()
{
	echo "Enabling pf."
	$pf_program -Fall > /dev/null 2>&1
	$pf_program -f "$pf_rules" $pf_flags
	if ! $pf_program -s info | grep -q "Enabled" ; then
		$pf_program -e
	fi
}

pf_stop()
{
	if $pf_program -s info | grep -q "Enabled" ; then
		echo "Disabling pf."
		$pf_program -d
	fi
}

pf_check()
{
	echo "Checking pf rules."
	$pf_program -n -f "$pf_rules"
}

pf_reload()
{
	echo "Reloading pf rules."
	$pf_program -n -f "$pf_rules" || return 1
	# Flush everything but existing state entries that way when
	# rules are read in, it doesn't break established connections.
	$pf_program -Fnat -Fqueue -Frules -FSources -Finfo -FTables -Fosfp > /dev/null 2>&1
	$pf_program -f "$pf_rules" $pf_flags
}

pf_resync()
{
	$pf_program -f "$pf_rules" $pf_flags
}

pf_status()
{
	$pf_program -s info
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/pf.pid"
touch $pidfile
pf_program="/usr/sbin/dummy_pf"
pf_prestart
pf_start
exit 0
