#!/bin/sh
#
# Removed dependency from /etc/rc.

isdnd_start()
{
	echo -n 'ISDN subsystem setup:'

	# Check for pcvt driver (VT100/VT220 emulator)
	#
	if [ -x /usr/sbin/ispcvt ]; then
		if /usr/sbin/ispcvt; then
			# No vidcontrol if we are using pcvt
			#
			isdn_screenflags=NO
		fi
	fi

	# Start isdnd
	#
	echo -n ' isdnd'
	case ${isdn_fsdev} in
	[Nn][Oo] | '')
		/usr/sbin/isdnd ${isdn_flags}
		;;
	*)
		# Change vidmode of ${isdn_fsdev}
		#
		case ${isdn_screenflags} in
		[Nn][Oo])
			;;
		*)
			/usr/sbin/vidcontrol < ${isdn_fsdev} > ${isdn_fsdev} 2>&1 ${isdn_screenflags}
			;;
		esac

		/usr/sbin/isdnd ${isdn_flags} -f -r ${isdn_fsdev} -t ${isdn_ttype}
		;;
	esac

	# Start isdntrace
	#
	echo -n ' isdntrace'
	nohup /usr/sbin/isdntrace ${isdn_traceflags} >/dev/null 2>&1 &
	echo '.'
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/isdnd.pid"
touch $pidfile
isdnd_start
exit 0
