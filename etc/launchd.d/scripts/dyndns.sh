#!/bin/sh
#
launch_xml="/sbin/launch_xml -get pfsense"

enable=`$launch_xml.dyndns.dyndns.enable`

if [ -z $enable ]
then
	# Dyndns not enabled
	exit 0
fi

type=`$launch_xml.dyndns.type`
host=`$launch_xml.dyndns.host`
username=`$launch_xml.dyndns.username`
password=`$launch_xml.dyndns.password`
wildcard=`$launch_xml.dyndns.wildcard`
mx=`$launch_xml.dyndns.mx`
service=`$launch_xml.dyndns.service`

# Insert here the dyndns client process! 
#exec dyndnsclient args

exit 0
