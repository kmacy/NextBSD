#!/bin/sh

get_timezone_xml()
{
	timezone=`/sbin/launch_xml -get pfsense.system.timezone`
}

get_timezone_xml

cp /usr/share/zoneinfo/$timezone /etc/localtime

exit 0

