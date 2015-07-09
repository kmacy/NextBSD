#!/bin/sh
#
# Removed dependency from /etc/rc.

purgedir()
{
	local dir file

	if [ $# -eq 0 ]; then
		purgedir .
	else
		for dir
		do
		(
			cd "$dir" && for file in .* *
			do
				# Skip over logging sockets
				[ -S "$file" -a "$file" = "log" ] && continue
				[ -S "$file" -a "$file" = "logpriv" ] && continue
				# Skip over solidbase.pid
				# launchd takes care of cleanup
				[ "$file" = "solidbase.pid" ] && continue

				[ ."$file" = .. -o ."$file" = ... ] && continue
				if [ -d "$file" -a ! -L "$file" ]
				then
					purgedir "$file"
				else
					rm -f -- "$file"
				fi
			done
		)
		done
	fi
}

cleanvar_prestart()
{
	# These files must be removed only the first time this script is run
	# on boot.
	#
	rm -f /var/run/clean_var /var/spool/lock/clean_var
}

cleanvar_start () 
{
	if [ -d /var/run -a ! -f /var/run/clean_var ]; then
		purgedir /var/run
		# And an initial utmp file
		(cd /var/run && cp /dev/null utmp && chmod 644 utmp;)
		>/var/run/clean_var
	fi
	if [ -d /var/spool/lock -a ! -f /var/spool/lock/clean_var ]; then
		purgedir /var/spool/lock
		>/var/spool/lock/clean_var
	fi
	rm -rf /var/spool/uucp/.Temp/*
}

# start here
# used to emulate "requires/provide" functionality
pidfile="/var/run/cleanvar.pid"
touch $pidfile

cleanvar_prestart
cleanvar_start
exit 0
