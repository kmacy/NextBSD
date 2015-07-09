#!/bin/sh
#
# Removed dependency from /etc/rc.

##############################################################################
# Read and parse Bluetooth device configuration file
##############################################################################

bluetooth_read_conf()
{
	_file=$1
	_namespace=$2
	_err=0

	if [ ! -e $_file ]; then
		return 0
	fi

	if [ ! -f $_file -o ! -r $_file ]; then
		echo "Bluetooth configuration file $_file is not a file or not readable"
		exit 1
	fi

	while read _line
	do
		case "$_line" in
		\#*)
			continue
			;;

		*)
			if [ -z "$_line" ]; then
				continue;
			fi


			if expr "$_line" : "[a-zA-Z0-9_]*=" > /dev/null 2>&1; then
				eval "${_namespace}${_line}"
			else
				echo "Unable to parse line \"$_line\" in $_file"
				_err=1
			fi
			;;
		esac
	done < $_file

	return $_err
}

##############################################################################
# Setup Bluetooth stack. Create and connect nodes
##############################################################################

bluetooth_setup_stack()
{
	dev=$1
	shift
	hook=$1
	shift

	# Setup HCI 
	ngctl mkpeer ${dev}: hci ${hook} drv \
		> /dev/null 2>&1 || return 1

	ngctl name ${dev}:${hook} ${dev}hci \
		> /dev/null 2>&1 || return 1

	ngctl msg ${dev}hci: set_debug ${bluetooth_device_hci_debug_level} \
		> /dev/null 2>&1 || return 1

	# Setup L2CAP
	ngctl mkpeer ${dev}hci: l2cap acl hci \
		> /dev/null 2>&1 || return 1

	ngctl name ${dev}hci:acl ${dev}l2cap \
		> /dev/null 2>&1 || return 1

	ngctl msg ${dev}l2cap: set_debug ${bluetooth_device_l2cap_debug_level} \
		> /dev/null 2>&1 || return 1

	# Connect HCI node to the Bluetooth sockets layer
	ngctl connect ${dev}hci: btsock_hci_raw: raw ${dev}raw \
		> /dev/null 2>&1 || return 1

	# Connect L2CAP node to Bluetooth sockets layer
	ngctl connect ${dev}l2cap: btsock_l2c_raw: ctl ${dev}ctl \
		> /dev/null 2>&1 || return 1

	ngctl connect ${dev}l2cap: btsock_l2c: l2c ${dev}l2c \
		> /dev/null 2>&1 || return 1

	# Initilalize HCI node
	${hccontrol} -n ${dev}hci reset \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci read_bd_addr \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci read_local_supported_features \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci read_buffer_size \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci write_scan_enable 0 \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci write_class_of_device ${bluetooth_device_class} \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci write_authentication_enable 0 \
			> /dev/null 2>&1 || return 1

	case "${bluetooth_device_encryption_mode}" in
	[Nn][Oo][Nn][Ee]|0)
		${hccontrol} -n ${dev}hci write_encryption_mode 0 \
			> /dev/null 2>&1 || return 1
		;;

	[Pp][2][Pp]|1)
		${hccontrol} -n ${dev}hci write_encryption_mode 1 \
			> /dev/null 2>&1 || return 1
		;;

	[Al][Ll][Ll]|2)
		${hccontrol} -n ${dev}hci write_encryption_mode 2 \
			> /dev/null 2>&1 || return 1
		;;

	*)
		echo "Unsupported encryption mode ${bluetooth_device_encryption_mode} for device ${dev}"
		return 1
		;;
	esac

	${hccontrol} -n ${dev}hci write_node_role_switch 0 \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci change_local_name "${bluetooth_device_local_name}" \
		> /dev/null 2>&1 || return 1

	${hccontrol} -n ${dev}hci initialize \
		> /dev/null 2>&1 || return 1

	return 0
}

##############################################################################
# Shutdown Bluetooth stack. Destroy all nodes
##############################################################################

bluetooth_shutdown_stack()
{
	dev=$1

	ngctl shutdown ${dev}hci: > /dev/null 2>&1
	ngctl shutdown ${dev}l2cap: > /dev/null 2>&1

	return 0
}

##############################################################################
# bluetooth_start()
##############################################################################

bluetooth_start()
{
	dev=$1

	# Automatically load modules
	kldload ng_bluetooth > /dev/null 2>&1
	kldload ng_hci > /dev/null 2>&1
	kldload ng_l2cap > /dev/null 2>&1
	kldload ng_btsocket > /dev/null 2>&1

	# Try to figure out device type by looking at device name
	case "${dev}" in
	# sioX - serial/UART Bluetooth device
	sio*)
		kldload ng_h4 > /dev/null 2>&1

		hook="hook"

		# Obtain unit number from device.
		unit=`expr ${dev} : 'sio\([0-9]\{1,\}\)'`
		if [ -z "${unit}" ]; then
			echo "Unable to get sio unit number: ${dev}"
			exit 1
		fi

		${hcseriald} ${hcseriald_opt} -f /dev/cuad${unit} -n ${dev}
		sleep 1 # wait a little bit

		if [ ! -f "/var/run/hcseriald.${dev}.pid" ]; then
			echo "Unable to start hcseriald on ${dev}"
			exit 1
		fi
		;;

	# 3Com Bluetooth Adapter 3CRWB60-A
	btccc*)
		hook="hook"

		# Obtain unit number from device.
		unit=`expr ${dev} : 'btccc\([0-9]\{1,\}\)'`
		if [ -z "${unit}" ]; then
			echo "Unable to get bt3c unit number: ${dev}"
			exit 1
		fi
		;;

	# USB Bluetooth adapters
	ubt*)
		hook="hook"

		# Obtain unit number from device.
		unit=`expr ${dev} : 'ubt\([0-9]\{1,\}\)'`
		if [ -z "${unit}" ]; then
			echo "Unable to get ubt unit number: ${dev}"
			exit 1
		fi
		;;

	# Unknown
	*)
		echo "Unsupported device: ${dev}"
		exit 1
		;;
	esac

	# Be backward compatible and setup reasonable defaults 
	bluetooth_device_authentication_enable="0"
	bluetooth_device_class="ff:01:0c"
	bluetooth_device_connectable="1"
	bluetooth_device_discoverable="1"
	bluetooth_device_encryption_mode="0"
	bluetooth_device_hci_debug_level="3"
	bluetooth_device_l2cap_debug_level="3"
	bluetooth_device_local_name="`/usr/bin/uname -n` (${dev})"
	bluetooth_device_role_switch="1"

	# Load default device configuration parameters
	_file="/etc/defaults/bluetooth.device.conf"

	if ! bluetooth_read_conf $_file bluetooth_device_ ; then
		echo "Unable to read default Bluetooth configuration from $_file"
		exit 1
	fi

	# Load device specific overrides
	_file="/etc/bluetooth/$dev.conf"

	if ! bluetooth_read_conf $_file bluetooth_device_ ; then
		echo "Unable to read Bluetooth device configuration from $_file"
		exit 1
	fi

	# Setup stack
	if ! bluetooth_setup_stack ${dev} ${hook} ; then
		bluetooth_shutdown_stack $dev
		echo "Unable to setup Bluetooth stack for device ${dev}"
		exit 1
	fi
		
	return 0
}

##############################################################################
# bluetooth_stop()
##############################################################################

bluetooth_stop()
{
	dev=$1

	# Try to figure out device type by looking at device name
	case "${dev}" in
	# sioX - serial/UART Bluetooth device
	sio*)
		if [ -f "/var/run/hcseriald.${dev}.pid" ]; then
			kill `cat /var/run/hcseriald.${dev}.pid`
			sleep 1 # wait a little bit
		fi
		;;

	# 3Com Bluetooth Adapter 3CRWB60-A
	btccc*)
		;;

	# USB Bluetooth adapters
	ubt*)
		;;

	# Unknown
	*)
		echo "Unsupported device: ${dev}"
		exit 1
		;;
	esac

	bluetooth_shutdown_stack ${dev}

	return 0
}

##############################################################################
# Start here
##############################################################################

hccontrol="${bluetooth_hccontrol:-/usr/sbin/hccontrol}"
hcseriald="${bluetooth_hcseriald:-/usr/sbin/hcseriald}"
hcseriald_opt="-d"

# used to emulate "requires/provide" functionality
pidfile="/var/run/bluetooth.pid"
touch $pidfile

bluetooth_start

exit 0
