#!/bin/sh
#
# Removed dependency from /etc/rc.

feed_dev_random()
{
	if [ -f "${1}" -a -r "${1}" -a -s "${1}" ]; then
		cat "${1}" | dd of=/dev/random bs=8k 2>/dev/null
	fi
}

initrandom_start()
{
	soft_random_generator=`sysctl kern.random 2>/dev/null`

	echo -n 'Entropy harvesting:'

	if [ \! -z "${soft_random_generator}" ] ; then
		if [ -w /dev/random ]; then
			${SYSCTL_W} kern.random.sys.harvest.interrupt=1 >/dev/null
			echo -n ' interrupts'
		fi
	    
		if [ -w /dev/random ]; then
			${SYSCTL_W} kern.random.sys.harvest.ethernet=1 >/dev/null
			echo -n ' ethernet'
		fi

		if [ -w /dev/random ]; then
			${SYSCTL_W} kern.random.sys.harvest.point_to_point=1 >/dev/null
			echo -n ' point_to_point'
		fi

		# XXX temporary until we can improve the entropy
		# harvesting rate.
		# Entropy below is not great, but better than nothing.
		# This unblocks the generator at startup
		( ps -fauxww; sysctl -a; date; df -ib; dmesg; ps -fauxww; ) \
		    | dd of=/dev/random bs=8k 2>/dev/null
		cat /bin/ls | dd of=/dev/random bs=8k 2>/dev/null

		# First pass at reseeding /dev/random.
		#
		case ${entropy_file} in
		[Nn][Oo] | '')
			;;
		*)
			if [ -w /dev/random ]; then
				feed_dev_random "${entropy_file}"
			fi
			;;
		esac

		echo -n ' kickstart'
	fi

	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/initrandom.pid"
touch $pidfile
initrandom_start
exit 0
