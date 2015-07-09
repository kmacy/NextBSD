#!/bin/sh
#
# Removed dependency from /etc/rc.

# Start ATM daemons

atm3_start()
{
	echo -n 'Starting ATM daemons:'

	# Get a list of network interfaces
	atm_nif=`atm sh netif | { read junk ; \
	    while read dev junk ; do
		echo "${dev} "
	    done
	}`

	for net in ${atm_nif} ; do
		eval atmarp_args=\$atm_arpserver_${net}
		eval scsparp_args=\$atm_scsparp_${net}

		case ${scsparp_args} in
		[Yy][Ee][Ss])
			case ${atmarp_args} in
			local)
				;;
			*)
				warn "${net}: local arpserver required for SCSP"
				continue
				;;
			esac

			atm_atmarpd="${atm_atmarpd} ${net}"
			atm_scspd=1
			;;
		esac
	done

	# Start SCSP daemon (if needed)
	case ${atm_scspd} in
	1)
		echo -n ' scspd'
		scspd -d
		;;
	esac

	# Start ATMARP daemon (if needed)
	if [ -n "${atm_atmarpd}" ]; then
		echo -n ' atmarpd'
		atmarpd -d ${atm_atmarpd}
	fi
	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/atm3.pid"
touch $pidfile

atm3_start

exit 0
