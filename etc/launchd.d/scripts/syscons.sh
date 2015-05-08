#!/bin/sh -
#
# Removed dependency from /etc/rc.

# stdin must be redirected because it might be for a serial console
#
kbddev=/dev/ttyv0
viddev=/dev/ttyv0

# helper
syscons_configure_keyboard()
{
	# keymap
	#
	case ${keymap} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' keymap';	kbdcontrol < ${kbddev} -l ${keymap}
		;;
	esac

	# keyrate
	#
	case ${keyrate} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' keyrate';	kbdcontrol < ${kbddev} -r ${keyrate}
		;;
	esac

	# keybell
	#
	case ${keybell} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' keybell';	kbdcontrol < ${kbddev} -b ${keybell}
		;;
	esac

	# change function keys
	#
	case ${keychange} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' keychange'
		set - ${keychange}
		while [ $# -gt 0 ]; do
			kbdcontrol <${kbddev} -f "$1" "$2"
			shift; shift
		done
		;;
	esac

	# set this keyboard mode for all virtual terminals
	#
	if [ -n "${allscreens_kbdflags}" ]; then
		echo -n ' allscreens_kbd'
		for ttyv in /dev/ttyv*; do
			kbdcontrol ${allscreens_kbdflags} < ${ttyv} > ${ttyv} 2>&1
		done
	fi
}

syscons_setkeyboard()
{
	kbd=$1
                
	if [ -z "${kbd}" ]; then
		return 1
	fi

	# Check if the kbdmux(4) is the current active keyboard
	kbdcontrol -i < ${kbddev} | grep kbdmux > /dev/null 2>&1
	if [ $? != 0 ]; then
		kbdcontrol -k ${kbd} < ${kbddev} > /dev/null 2>&1
	fi

	echo -n 'Configuring keyboard:'
	syscons_configure_keyboard
	echo '.'
}

syscons_precmd()
{
	if [ ! -c $kbddev ]
	then
		return 1
	fi
	if [ -x /usr/sbin/ispcvt ] && /usr/sbin/ispcvt
	then
		return 1
	fi
	return 0
}

syscons_start()
{
	echo -n 'Configuring syscons:'

	# keyboard
	#
	if [ -n "${keyboard}" ]; then
		echo -n ' keyboard';	syscons_setkeyboard ${keyboard}
	fi

	syscons_configure_keyboard

	# cursor type
	#
	case ${cursor} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' cursor';	vidcontrol < ${viddev} -c ${cursor}
		;;
	esac

	# screen mapping
	#
	case ${scrnmap} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' scrnmap';	vidcontrol < ${viddev} -l ${scrnmap}
		;;
	esac

	# font 8x16
	#
	case ${font8x16} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' font8x16';	vidcontrol < ${viddev} -f 8x16 ${font8x16}
		;;
	esac

	# font 8x14
	#
	case ${font8x14} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' font8x14';	vidcontrol < ${viddev} -f 8x14 ${font8x14}
		;;
	esac

	# font 8x8
	#
	case ${font8x8} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' font8x8';	vidcontrol < ${viddev} -f 8x8 ${font8x8}
		;;
	esac

	# blank time
	#
	case ${blanktime} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' blanktime';	vidcontrol < ${viddev} -t ${blanktime}
		;;
	esac

	# screen saver
	#
	case ${saver} in
	[Nn][Oo] | '')
		;;
	*)
		echo -n ' screensaver'
		for i in `kldstat | awk '$5 ~ "^splash_.*$" { print $5 }'`; do
			kldunload ${i}
		done
		kldstat -v | grep -q _saver || kldload ${saver}_saver
		;;
	esac

	# set this mode for all virtual screens
	#
	if [ -n "${allscreens_flags}" ]; then
		echo -n ' allscreens'
		for ttyv in /dev/ttyv*; do
			vidcontrol ${allscreens_flags} < ${ttyv} > ${ttyv} 2>&1
		done
	fi

	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/syscons.pid"
touch $pidfile
syscons_precmd
syscons_start
exit 0
