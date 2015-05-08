#!/bin/sh
#
# Removed dependency from /etc/rc.
# XXX: should require `mail'!

virecover_start()
{
	[ -d /var/tmp/vi.recover ] || return
	find /var/tmp/vi.recover ! -type f -a ! -type d -delete
	vibackup=`echo /var/tmp/vi.recover/vi.*`
	if [ "${vibackup}" != '/var/tmp/vi.recover/vi.*' ]; then
		echo -n 'Recovering vi editor sessions:'
		for i in /var/tmp/vi.recover/vi.*; do
			# Only test files that are readable.
			if [ ! -r "${i}" ]; then
				continue
			fi

			# Unmodified nvi editor backup files either have the
			# execute bit set or are zero length.  Delete them.
			if [ -x "${i}" -o ! -s "${i}" ]; then
				rm -f "${i}"
			fi
		done

		# It is possible to get incomplete recovery files, if the editor
		# crashes at the right time.
		virecovery=`echo /var/tmp/vi.recover/recover.*`
		if [ "${virecovery}" != "/var/tmp/vi.recover/recover.*" ]; then
			for i in /var/tmp/vi.recover/recover.*; do
				# Only test files that are readable.
				if [ ! -r "${i}" ]; then
					continue
				fi

				# Delete any recovery files that are zero length,
				# corrupted, or that have no corresponding backup file.
				# Else send mail to the user.
				recfile=`awk '/^X-vi-recover-path:/{print $2}' < "${i}"`
				if [ -n "${recfile}" -a -s "${recfile}" ]; then
					sendmail -t < "${i}"
				else
					rm -f "${i}"
				fi
			done
		fi
		echo '.'
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/virecover.pid"
touch $pidfile
virecover_start
exit 0
