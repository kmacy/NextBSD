#!/bin/sh
#
# Removed dependency from /etc/rc.

convert_host_conf()
{
    host_conf=$1; shift;
    nsswitch_conf=$1; shift;

    while read line; do
	line=${line##[ 	]}
	case $line in
        hosts|local|file)
		_nsswitch="${_nsswitch}${_nsswitch+ }files"
		;;
	dns|bind)
		_nsswitch="${_nsswitch}${_nsswitch+ }dns"
		;;
	nis)
		_nsswitch="${_nsswitch}${_nsswitch+ }nis"
		;;
	'#'*)
		;;
	*)
        	printf "Warning: unrecognized line [%s]", $line > "/dev/stderr"
		;;
		
	esac
    done < $host_conf

    echo "hosts: $_nsswitch" > $nsswitch_conf
}

generate_nsswitch_conf()
{
    nsswitch_conf=$1; shift;

    cat >$nsswitch_conf <<EOF
group: compat
group_compat: nis
hosts: files dns
networks: files
passwd: compat
passwd_compat: nis
shells: files
EOF
}

generate_host_conf()
{
    nsswitch_conf=$1; shift;
    host_conf=$1; shift;

    _cont=0
    _sources=""
    while read line; do
	line=${line##[ 	]}
	case $line in
	hosts:*)
		;;
	*)
		if [ $_cont -ne 1 ]; then
			continue
		fi
		;;
	esac
	if [ "${line%\\}" = "${line}\\" ]; then
		_cont=1
	fi
	line=${line#hosts:}
	line=${line%\\}
	line=${line%%#*}
	_sources="${_sources}${_sources:+ }$line"
    done < $nsswitch_conf

    echo "# Auto-generated from nsswitch.conf, do not edit" > $host_conf
    for _s in ${_sources}; do
	case $_s in
	files)
		echo "hosts" >> $host_conf
		;;
	dns)
		echo "dns" >> $host_conf
		;;
	nis)
		echo "nis" >> $host_conf
		;;
	*=*)
		;;
	*)
		printf "Warning: unrecognized source [%s]", $_s > "/dev/stderr"
		;;
	esac
    done
}

nsswitch_start()
{
	# Convert host.conf to nsswitch.conf if necessary
	#
	if [ -f "/etc/host.conf" -a ! -f "/etc/nsswitch.conf" ]; then
		echo ''
		echo 'Warning: /etc/host.conf is no longer used'
		echo '  /etc/nsswitch.conf will be created for you'
		convert_host_conf /etc/host.conf /etc/nsswitch.conf
	fi

	# Generate default nsswitch.conf if none exists
	#
	if [ ! -f "/etc/nsswitch.conf" ]; then
		echo 'Generating nsswitch.conf.'
		generate_nsswitch_conf /etc/nsswitch.conf
	fi

	# Generate host.conf for compatibility
	#
	if [ ! -f "/etc/host.conf" ]; then
		echo 'Generating host.conf.'
		generate_host_conf /etc/nsswitch.conf /etc/host.conf
	fi

}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/nsswitch.pid"
touch $pidfile
nsswitch_start
exit 0
