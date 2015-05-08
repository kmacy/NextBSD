#!/bin/sh
#
# Removed dependency from /etc/rc.

echo ACCOUNTING.SH

accounting_command="/usr/sbin/accton"
accounting_file="/var/account/acct"

accounting_start()
{
	_dir=`dirname "$accounting_file"`
	if [ ! -d `dirname "$_dir"` ]; then
		if ! mkdir -p "$_dir"; then
			return 1
		fi
	fi
	if [ ! -e "$accounting_file" ]; then
		touch "$accounting_file"
	fi

	if [ ! -f ${accounting_file} ]; then
		( umask 022 ; > ${accounting_file} )
	fi
	${accounting_command} ${accounting_file}
}

# start here
pidfile="/var/run/accounting.pid"
touch $pidfile

accounting_start
exit 0
