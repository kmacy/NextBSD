#!/bin/sh
#
# Removed dependency from /etc/rc.

# Load nfs modules if they were not compiled into the kernel
nfsserver_start()
{
	if ! sysctl vfs.nfsrv >/dev/null 2>&1; then
		if ! kldload nfsserver; then
			echo 'Could not load NFS server module'
			return 1
		fi
	fi
	return 0
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/nfsserver.pid"
touch $pidfile
nfsserver_start
exit 0
