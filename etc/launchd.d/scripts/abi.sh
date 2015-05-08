#!/bin/sh
# abi wrapper for use with launchd

sysv_start()
{
        echo -n ' sysvipc'
        kldload sysvmsg >/dev/null 2>&1
        kldload sysvsem >/dev/null 2>&1
        kldload sysvshm >/dev/null 2>&1
}

linux_start()
{
        echo -n ' linux'
        if ! kldstat -v | grep -E 'linux(aout|elf)' > /dev/null; then
                kldload linux > /dev/null 2>&1
        fi
        if [ -x /compat/linux/sbin/ldconfig ]; then
                /compat/linux/sbin/ldconfig
        fi
}

svr4_start()
{
        echo -n ' svr4'
        kldload svr4 > /dev/null 2>&1
}

abi_start()
{
    echo -n 'Additional ABI support:'
    echo "count: " $count

    if [ ${count} != 1 ]
	then exit 1
    fi

    case "$param" in
	sysv*)
	    sysv_start
	    ;;
	linux*)
	    linux_start
	    ;;
	svr4*)
	    svr4_start
	    ;;
	*)
	    echo "invalid param."
	    ;;
    esac
}

# used to emulate "requires/provide" functionality
pidfile="/var/run/abi.pid"
touch $pidfile

param="$1"
count="$#"
abi_start

exit 0

