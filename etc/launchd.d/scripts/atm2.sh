#!/bin/sh
#
# Removed dependency from /etc/rc.

#
# Additional ATM interface configuration
#

atm2_start()
{
	# Configure network interfaces

	# get a list of physical interfaces
	atm_phy=`atm show stat int | { read junk ; read junk ; \
	    while read dev junk ; do
		case ${dev} in
		en[0-9] | en[0-9][0-9])
			;;
		*)
			echo "${dev} "
			;;
		esac
	done ; }`

	for phy in ${atm_phy}; do
		eval netif_args=\$atm_netif_${phy}
		set -- ${netif_args}
		# skip unused physical interfaces
		if [ $# -lt 2 ] ; then
			continue
		fi

		netname=$1
		netcnt=$2
		netindx=0
		while [ ${netindx} -lt ${netcnt} ]; do
			net="${netname}${netindx}"
			netindx=$((${netindx} + 1))
			echo -n " ${net}"

			# Configure atmarp server
			eval atmarp_args=\$atm_arpserver_${net}
			if [ -n "${atmarp_args}" ]; then
				atm set arpserver ${net} ${atmarp_args} ||
				    continue
			fi
		done
	done
	echo '.'

	# Define any permanent ARP entries.
	if [ -n "${atm_arps}" ]; then
		for i in ${atm_arps}; do
			eval arp_args=\$atm_arp_${i}
			atm add arp ${arp_args}
		done
	fi
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/atm2.pid"
touch $pidfile

atm2_start

exit 0
