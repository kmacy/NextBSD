#!/bin/sh

launch_xml_cmd="/sbin/launch_xml -get"
launch_xml="$launch_xml_cmd pfsense.snmpd"
conf=/usr/local/etc/snmpd.conf
bsnmpd=/usr/sbin/bsnmpd
pidfile=/var/run/snmpd.pid

enable=`$launch_xml.enable`

if [ ! -z $enable ]
then
	exit 0
fi

syslocation=`$launch_xml.syslocation`
syscontact=`$launch_xml.syscontact`
rocommunity=`$launch_xml.rocommunity`
pollport=`$launch_xml.pollport`

cat <<END > $conf
location := "$syslocation"
contact := "$syscontact"
read := "$rocommunity"
END

trapenable=`$launch_xml.trapenable`

if [ ! -z $trapenable ]
then
	trapserver=`$launch_xml.trapserver`
	trapserverport=`$launch_xml.trapserverport`
	trapstring=`$launch_xml.trapstring`

	cat <<END >> $conf
traphost := "$trapserver"
trapport := "$trapserverport""
trap := "$trapstring"
END

fi

cat <<END >> $conf
system := 1 # pfSense
%snmpd
begemotSnmpdDebugDumpPdus	= 2
begemotSnmpdDebugSyslogPri = 7
begemotSnmpdCommunityString.0.1 = \$(read)
END

if [ ! -z $trapenable ]
then
	cat <<END >> $conf
begemotTrapSinkStatus.[\$(traphost)].\$(trapport) = 4
begemotTrapSinkVersion.[\$(traphost)].\$(trapport) = 2
begemotTrapSinkComm.[\$(traphost)].\$(trapport) = \$(trap)
END
fi

cat <<END >> $conf
begemotSnmpdCommunityDisable = 1
END

bindlan=`$launch_xml.bindlan`

if [ ! -z $bindlan ]
then
	bindtoip=`$launch_xml_cmd system.interfaces.lan.ipaddr`
else
	bindtoip="0.0.0.0"
fi

if [ -z $pollport ]
then
	pollport=161
fi	
	
cat <<END >> $conf
begemotSnmpdPortStatus.$bindtoip.$pollport = 1
begemotSnmpdLocalPortStatus."/var/run/snmpd.sock" = 1
begemotSnmpdLocalPortType."/var/run/snmpd.sock" = 4

sysContact = \$(contact)
sysLocation = \$(location)
sysObjectId = 1.3.6.1.4.1.12325.1.1.2.1.\$(system)

snmpEnableAuthenTraps = 2
END

mibii=`$launch_xml.modules.mibii`

if [ ! -z $mibii ]
then
	cat <<END >> $conf
begemotSnmpdModulePath."mibII" = "/usr/lib/snmp_mibII.so"
END
fi	

netgraph=`$launch_xml.modules.netgraph`

if [ ! -z $netgraph ]
then
	cat <<END >> $conf
begemotSnmpdModulePath."netgraph" = "/usr/lib/snmp_netgraph.so"
%netgraph
begemotNgControlNodeName = "snmpd"
END
fi	

pf=`$launch_xml.modules.pf`

if [ ! -z $pf ]
then
	cat <<END >> $conf
begemotSnmpdModulePath."pf" = "/usr/lib/snmp_pf.so"
END
fi	

cat <<END >> $conf
# Config must end with blank line

END

exec $bsnmpd -d -c $conf -p $pidfile
