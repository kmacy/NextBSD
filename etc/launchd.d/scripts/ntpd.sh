#!/bin/sh
#

launch_xml="/sbin/launch_xml -get pfsense"
conf=/etc/ntpd.conf
ntpd=/usr/local/sbin/ntpd # uses OpenNTPD, not the standard FreeBSD one

cat <<END > $conf
#
# SolidWall OpenNTPD Configuration File
#
END

for server in `$launch_xml.system.timeservers`
do
	echo servers $server >> $conf
done

if [ ! -d /var/empty ]
then
	mkdir -p /var/empty
	chmod ug+rw /var/empty/.
fi

exec $ntpd -d -s -f $conf

