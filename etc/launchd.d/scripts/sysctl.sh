#!/bin/sh
#
# Removed dependency from /etc/rc.

sysctl_start()
{
	#
	# Read in /etc/sysctl.conf and set things accordingly
	#
	if [ -f /etc/sysctl.conf ]; then
		while read var comments
		do
			case ${var} in
			\#*|'')
				;;
			*)
				mib=${var%=*}
				val=${var#*=}

				if current_value=`${SYSCTL} -n ${mib} 2>/dev/null`; then
					case ${current_value} in
					${val})
						;;
					*)
						sysctl ${var}
						;;
					esac
				elif [ "$1" = "last" ]; then
					echo "sysctl ${mib} does not exist."
				fi
				;;
			esac
		done < /etc/sysctl.conf
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/sysctl.pid"
touch $pidfile
sysctl_start
exit 0
