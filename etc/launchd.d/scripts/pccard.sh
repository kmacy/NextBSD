#!/bin/sh
#
# Removed dependency from /etc/rc.

pccard_start()
{
	if [ ! -c /dev/card0 ]; then
		exit 0
	fi
	echo -n 'Setup PC-CARD:'
	case ${pccard_mem} in
	[Dd][Ee][Ff][Aa][Uu][Ll][Tt])
		pccardc pccardmem 0xd0000 1>/dev/null && echo -n ' memory'
		;;
	*)
		pccardc pccardmem ${pccard_mem} 1>/dev/null && echo -n ' memory'
		;;
	esac

	if [ -n "${pccard_beep}" ]; then
		pccardc beep ${pccard_beep} && echo -n ' beep'
	fi

	if [ -n "${pccard_conf}" ]; then
		pccardd_flags="${pccardd_flags} -f ${pccard_conf}"
	fi

	pccardd ${pccardd_flags} && echo -n ' pccardd'
	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/pccard.pid"
touch $pidfile
pccard_start
exit 0
