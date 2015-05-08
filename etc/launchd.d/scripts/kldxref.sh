#!/bin/sh
#
# Removed dependency from /etc/rc.

kldxref_start () {
	if [ -n "$kldxref_module_path" ]; then
		MODULE_PATHS="$kldxref_module_path"
	else
		MODULE_PATHS=`sysctl -n kern.module_path`
	fi
	IFS=';'
	for MODULE_DIR in $MODULE_PATHS; do
		echo "Building $MODULE_DIR/linker.hints"
		kldxref "$MODULE_DIR"
	done
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/kldxref.pid"
touch $pidfile
kldxref_start
exit 0
