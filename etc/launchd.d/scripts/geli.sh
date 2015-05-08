#!/bin/sh
#
# Removed dependency from /etc/rc.

geli_start()
{
	devices=`geli_make_list`

	# If there are no devices return before loading geom_eli.ko.
	if [ -z "${devices}" ]; then
		return
	fi

	kldstat -q -m g_eli || geli load || err 1 'geom_eli module failed to load.'

	if [ -z "${geli_tries}" ]; then
		if [ -n "${geli_attach_attempts}" ]; then
			# Compatibility with rc.d/gbde.
			geli_tries=${geli_attach_attempts}
		else
			geli_tries=`${SYSCTL_N} kern.geom.eli.tries`
		fi
	fi

	for provider in ${devices}; do
		provider_=`ltr ${provider} '/' '_'`

		eval "flags=\${geli_${provider_}_flags}"
		if [ -z "${flags}" ]; then
			flags=${geli_default_flags}
		fi
		if [ -e "/dev/${provider}" -a ! -e "/dev/${provider}.eli" ]; then
			echo "Configuring Disk Encryption for ${provider}."
			count=1
			while [ ${count} -le ${geli_tries} ]; do
				geli attach ${flags} ${provider}
				if [ -e "/dev/${provider}.eli" ]; then
					break
				fi
				echo "Attach failed; attempt ${count} of ${geli_tries}."
				count=$((count+1))
			done
		fi
	done
}

geli_stop()
{
	devices=`geli_make_list`

	for provider in ${devices}; do
		if [ -e "/dev/${provider}.eli" ]; then
			umount "/dev/${provider}.eli" 2>/dev/null
			geli detach "${provider}"
		fi
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/geli.pid"
touch $pidfile
geli_start
exit 0
