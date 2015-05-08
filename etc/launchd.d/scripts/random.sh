#!/bin/sh
#
# Removed dependency from /etc/rc.

feed_dev_random()
{
	if [ -f "${1}" -a -r "${1}" -a -s "${1}" ]; then
		cat "${1}" | dd of=/dev/random bs=8k 2>/dev/null
	fi
}

random_start()
{
	# Reseed /dev/random with previously stored entropy.
	case ${entropy_dir} in
	[Nn][Oo])
		;;
	*)
		entropy_dir=${entropy_dir:-/var/db/entropy}
		if [ -d "${entropy_dir}" ]; then
			if [ -w /dev/random ]; then
				for seedfile in ${entropy_dir}/*; do
					feed_dev_random "${seedfile}"
				done
			fi
		fi
		;;
	esac

	case ${entropy_file} in
	[Nn][Oo] | '')
		;;
	*)
		if [ -w /dev/random ]; then
			feed_dev_random "${entropy_file}"
		fi
		;;
	esac
}

random_stop()
{
	# Write some entropy so when the machine reboots /dev/random
	# can be reseeded
	#
	case ${entropy_file} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n 'Writing entropy file:'
		rm -f ${entropy_file}
		oumask=`umask`
		umask 077
		if touch ${entropy_file}; then
			entropy_file_confirmed="${entropy_file}"
		else
			# Try this as a reasonable alternative for read-only
			# roots, diskless workstations, etc.
			rm -f /var/db/entropy-file
			if touch /var/db/entropy-file; then
				entropy_file_confirmed=/var/db/entropy-file
			fi
		fi
		case ${entropy_file_confirmed} in
		'')
			echo 'entropy file write failed.'
			;;
		*)
			dd if=/dev/random of=${entropy_file_confirmed} \
			   bs=4096 count=1 2> /dev/null
			echo '.'
			;;
		esac
		umask ${oumask}
		;;
	esac
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/random.pid"
touch $pidfile
random_start
exit 0
