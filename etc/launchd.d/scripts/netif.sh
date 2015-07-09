#!/bin/sh
#
launch_xml="/sbin/launch_xml -get pfsense.interfaces"
dhclient=/sbin/dhclient
ifconfig=/sbin/ifconfig
grep=/usr/bin/grep
mpd=/usr/local/sbin/mpd

lanipaddr=`$launch_xml.lan.ipaddr`
lanif=`$launch_xml.lan.if`

pppoe_setup() {
	ondemand=`$launch_xml.pppoe.ondemmand`

	if [ ! -z $ondemmand ]
	then 
		ondemand=enable
		idle=`$launch_xml.pppoe.timeout`
	else
		ondemand=disable
	fi

	conf=/var/etc/mpd.conf
	cat <<END > $conf

pppoe:
	new -i ng0 pppoe pppoe
	set iface route default
	set iface $ondemand on-demand
	set iface up-script /usr/local/sbin/ppp-linkup
END

	if [ ! -z $idle ]
	then
		cat <<END >> $conf
	set iface idle $idle
END
	fi

	if [ -f /usr/local/sbin/ppp-linkdown ]
	then
		cat <<END >> $conf
set iface down-script /usr/local/sbin/ppp-linkdown
END
	fi

	if [ $ondemand = "enable" ]
	then
		localip=`$launch_xml.pppoe.local-ip`
		temp=nok

		if [ ! -z $localip ]
		then
			remoteip=`$launch_xml.pppoe.remote-ip`
			if [ ! -z $remoteip ]
			then
				temp=ok
				cat <<END >> $conf
set iface addr $localip $remoteip
END
			fi
		fi

		if [ $temp = "nok" ]
		then
			cat <<END >> $conf
set iface addr 192.0.2.112 192.0.2.113
END
		fi
	fi	

	username=`$launch_xml.pppoe.username`
	password=`$launch_xml.pppoe.password`

	cat <<END >> $conf
set bundle disable multilink
set bundle authname $username
set bundle password $password
set link keep-alive 10 60
set link max-redial 0
set link no acfcomp protocomp
set link disable pap chap
set link accept chap
set link mtu 1492
set ipcp yes vjcomp
set ipcp ranges 0.0.0.0/0 0.0.0.0/0
END

	dnsallowoverride=`$launch_xml.system.dnsallowoverride`

	if [ ! -z $dnsallowoverride ]
	then
		cat <<END >> $conf
set ipcp enable req-pri-dns
END
	fi

	cat <<END >> $conf
open iface

END

	# Generate mpd.links
	links=/var/etc/mpd.links
	wanif=`$launch_xml.wan.if`
	provider=`$launch_xml.pppoe.provider`

	echo <<END >> $links
set link type pppoe
set pppoe iface $wanif
set pppoe service  "$provider"
set pppoe enable originate
set pppoe disable incoming
END
}

pptp_setup() {
	ondemand=`$launch_xml.pptp.ondemmand`

	if [ ! -z $ondemmand ]
	then 
		ondemand=enable
		idle=`$launch_xml.pptp.timeout`
	else
		ondemand=disable
	fi

	conf=/var/etc/mpd.conf
	cat <<END > $conf

pptp:
	new -i ng0 pptp pptp
	set iface route default
	set iface $ondemand on-demand
	set iface up-script /usr/local/sbin/ppp-linkup
END

	if [ ! -z $idle ]
	then
		cat <<END >> $conf
	set iface idle $idle
END
	fi

	if [ -f /usr/local/sbin/ppp-linkdown ]
	then
		cat <<END >> $conf
set iface down-script /usr/local/sbin/ppp-linkdown
END
	fi

	if [ $ondemand = "enable" ]
	then
		cat <<END >> $conf
set iface addr 10.0.0.1 10.0.0.2
END
	fi	

	username=`$launch_xml.pptp.username`
	password=`$launch_xml.pptp.password`

	cat <<END >> $conf
set bundle disable multilink
set bundle authname $username
set bundle password $password
set link keep-alive 10 60
set link max-redial 0
set link no acfcomp protocomp
set link disable pap chap
set link accept chap
set ipcp no vjcomp
set ipcp ranges 0.0.0.0/0 0.0.0.0/0
END

	dnsallowoverride=`$launch_xml.system.dnsallowoverride`

	if [ ! -z $dnsallowoverride ]
	then
		cat <<END >> $conf
set ipcp enable req-pri-dns
END
	fi

	cat <<END >> $conf
open

END

	# Generate mpd.links
	links=/var/etc/mpd.links
	local=`$launch_xml.pptp.local`
	remote=`$launch_xml.pptp.remote`

	echo <<END >> $links
set link type pptp
set pptp enable originate outcall
set pptp disable windowing
set pptp self $local
set pptp peer $remote
END
}

if [ $lanipaddr = "dhcp" ]
then
	$dhclient $lanif

elif [ $lanipaddr != "none" ]
then
	lanmeadia=`$launch_xml.lan.meadia`
	lanmeadiaopt=`$launch_xml.lan.meadiaopt`
	lanmtu=`$launch_xml.lan.mtu`
	lansubnet=`$launch_xml.lan.subnet`

	if [ ! -z $lanmedia ]; then lanmedia=" media $lanmedia"; fi
	if [ ! -z $lanmediaopt ]; then lanmediaopt=" mediaopt $lanmediaopt"; fi
	if [ ! -z $lanmtu ]; then lanmtu=" mtu $lanmtu"; fi

	$ifconfig $lanif inet $lanipaddr/$lansubnet$lanmedia$lanmediaopt$lanmtu	

	if [ ! -z $lanbridge ]
	then
		# Bridgeif contains the bridge interface name (e.g. bridge0)	
		bridgeif=`$ifconfig bridge create`	
	fi
fi

wanipaddr=`$launch_xml.wan.ipaddr`
wanif=`$launch_xml.wan.if`

if [ $wanipaddr = "dhcp" ]
then
	/sbin/dhclient $wanif
fi

if [ $wanipaddr = "pppoe" ]
then
	pppoe_setup
	exec $mpd -d /var -d /var/etc/ -p $pidfile pppoe

elif [ $wanipaddr = "pptp" ]
then
	pptp_setup
	local=`$launch_xml.pptp.local`
	subnet=`$launch_xml.pptp.subnet`
	$ifconfig $wanif $local/$subnet
	exec $mpd -d /var -d /var/etc/ -p $pidfile pptp

elif [ $wanipaddr != "none" ]
then
	wanmeadia=`$launch_xml.wan.meadia`
	wanmeadiaopt=`$launch_xml.wan.meadiaopt`
	wanmtu=`$launch_xml.wan.mtu`

	if [ ! -z $wanmedia ]; then wanmedia=" media $wanmedia"; fi
	if [ ! -z $wanmediaopt ]; then wanmediaopt=" mediaopt $wanmediaopt"; fi
	if [ ! -z $wanmtu ]; then wanmtu=" mtu $wanmtu"; fi

	$ifconfig $wanif inet $wanipaddr/$wansubnet$wanmedia$wanmediaopt$wanmtu	
fi

# OLD VERSION (SQL):

# Removed dependency from /etc/rc.

#SQLITE_CMD="/sbin/bsdsqlite3"
#CONFIGDB="/usr/share/solidbase/data/configdb"
#CONFIGTBL="host_conf"
#IFCFG_CMD="/sbin/ifconfig"

# start here
# used to emulate "requires/provide" functionality
#echo "Configuring device: $2"
#key="ifconfig_"$2

#value=`${SQLITE_CMD} ${CONFIGDB} 'select value from '${CONFIGTBL}' where key="'${key}'"'`

#if [ $1=="start" ]; then
#    ${IFCFG_CMD} $2 up
#    ${IFCFG_CMD} $2 $value
#elif [ $1=="stop" ]; then
#    ${IFCFG_CMD} $2 down
#fi
