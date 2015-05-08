#!/bin/sh

# used to emulate "requires/provide" functionality
pidfile="/var/run/chgname.pid"
touch $pidfile

for i in `ls *.plist`
do
	var=`echo $i | sed -e 's/\.plist$//'`
	cat $i | sed -e "s/abi/\\$var/" > $i
done

