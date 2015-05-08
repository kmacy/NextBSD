#!/bin/sh
#
chrootpath=/var/dhcpd
launch_xml=/sbin/launch_xml
netcalc=/bin/netcalc

get="$launch_xml -get"


domain=`$get pfsense.system.domain`

if [ -d $chrootpath ]
then
	rm -Rf $chrootpath
fi

for int in `$get pfsense.dhcpd`
do
	enable=`$get pfsense.dhcpd.$int.enable`
	if [ ! -z $enable ]
	then
		gateway=`$get pfsense.dhcpd.$int.gateway`		
		netmask=`$get pfsense.dhcpd.$int.netmask`		
		rangefrom=`$get pfsense.dhcpd.$int.range.from`		
		rangeto=`$get pfsense.dhcpd.$int.range.to`		

		ipaddr=`$get pfsense.interfaces.$int.ipaddr`

		netmaskfull=`$netcalc -fullmask $netmask`
		network=`$netcalc -network $ipaddr $netmask`

		if [ -z $gateway ]
		then
			gateway=$ipaddr
		fi

		if [ ! -d $chrootpath ]
		then
			for i in ./dev ./etc ./usr/local/sbin ./var/db ./usr \
				./lib ./run
			do
				mkdir -p $chrootpath/$i
			done

			chown -R dhcpd:_dhcp $chrootpath/*
			cp /lib/libc.so* $chrootpath/lib
			cp /usr/local/sbin/dhcpd $chrootpath/usr/local/sbin
			chmod a+rx $chrootpath/usr/local/sbin/dhcpd
		fi

		if [ ! -f $chrootpath/etc/dhcpd.conf ]
		then
			cat <<END > $chrootpath/etc/dhcpd.conf
option domain-name "$domain";
default-lease-time 7200;
max-lease-time 86400;
authoritative;
log-facility local7;
ddns-update-style none;
one-lease-per-client true;
deny duplicates;
END
		fi

		cat <<END >> $chrootpath/etc/dhcpd.conf
subnet $network netmask $netmaskfull {
	pool {
		range $rangefrom $rangeto;
	}
	options routers $gateway;
	options domain-name-servers $gateway;
}
END

	fi
done	

# DHCPD not enabled
if [ -z $enable ]
then
	exit 0
fi

exec /usr/local/sbin/dhcpd -f -user -dhcpd -group _dhcp -chroot $chrootpath \
	-cf $chrootpath/etc/dhcpd.conf

