#!/bin/sh
#
# Removed dependency from /etc/rc.

# Change some defaults for serial devices.
# Standard defaults are:
#	dtrwait 300 drainwait `sysctl -n kern.drainwait`
#	initial cflag from <sys/ttydefaults.h> = cread cs8 hupcl
#	initial iflag, lflag and oflag all 0
#	speed 9600
#	special chars from <sys/ttydefaults.h>
#	nothing locked
# except for serial consoles the initial iflag, lflag and oflag are from
# <sys/ttydefaults.h> and clocal is locked on.

default() {
	# Reset everything changed by the other functions to initial defaults.

	dc=$1; shift	# device name character
	drainwait=`sysctl -n kern.drainwait`

	for i in $*
	do
		comcontrol /dev/tty${dc}${i} dtrwait 300 drainwait $drainwait
		stty < /dev/tty${dc}${i}.init -clocal crtscts hupcl 9600 reprint ^R
		stty < /dev/tty${dc}${i}.lock -clocal -crtscts -hupcl 0
		stty < /dev/cua${dc}${i}.init -clocal crtscts hupcl 9600 reprint ^R
		stty < /dev/cua${dc}${i}.lock -clocal -crtscts -hupcl 0
	done
}

maybe() {
	# Special settings.

	dc=$1; shift

	for i in $*
	do
		# Don't use ^R; it breaks bash's ^R when typed ahead.
		stty < /dev/tty${dc}${i}.init reprint undef
		stty < /dev/cua${dc}${i}.init reprint undef
		# Lock clocal off on dialin device for security.
		stty < /dev/tty${dc}${i}.lock clocal
		# Lock the speeds to use old binaries that don't support them.
		# Any legal speed works to lock the initial speed.
		stty < /dev/tty${dc}${i}.lock 300
		stty < /dev/cua${dc}${i}.lock 300
	done
}

modem() {
	# Modem that supports CTS and perhaps RTS handshaking.

	dc=$1; shift

	for i in $*
	do
		# may depend on modem
		comcontrol /dev/tty${dc}${i} dtrwait 100 drainwait 180
		# Lock crtscts on.
		# Speed reasonable for V42bis.
		stty < /dev/tty${dc}${i}.init crtscts 115200
		stty < /dev/tty${dc}${i}.lock crtscts
		stty < /dev/cua${dc}${i}.init crtscts 115200
		stty < /dev/cua${dc}${i}.lock crtscts
	done
}

mouse() {
	# Mouse on either callin or callout port.

	dc=$1; shift

	for i in $*
	do
		# Lock clocal on, hupcl off.
		# Standard speed for Microsoft mouse.
		stty < /dev/tty${dc}${i}.init clocal -hupcl 1200
		stty < /dev/tty${dc}${i}.lock clocal  hupcl
		stty < /dev/cua${dc}${i}.init clocal -hupcl 1200
		stty < /dev/cua${dc}${i}.lock clocal  hupcl
	done
}

terminal() {
	# Terminal that supports CTS and perhaps RTS handshaking
	# with the cable or terminal arranged so that DCD is on
	# at least while the terminal is on.
	# Also works for bidirectional communications to another pc
	# provided at most one side runs getty.
	# Same as modem() except we want a faster speed and no dtrwait.

	dc=$1; shift

	modem ${dc} $*
	for i in $*
	do
		comcontrol /dev/tty${dc}${i} dtrwait 0
		stty < /dev/tty${dc}${i}.init 115200
		stty < /dev/cua${dc}${i}.init 115200
	done
}

# Don't use anything from this file unless you have some buggy programs
# that require it.

# Edit the functions and the examples to suit your system.
# $1 is the call in device identifier, $2 is the call out device identifier
# and the remainder of the line lists the device numbers.

# Initialize assorted 8250-16550 (sio) ports.
# maybe    d  0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v
# mouse    d      2
# modem    d    1
# terminal d  0

# Initialize all ports on a Cyclades-8yo.
# modem    c  00 01 02 03 04 05 06 07

# Initialize all ports on a Cyclades-16ye.
# modem    c  00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f

# Initialize all ports on a Digiboard 8.
# modem    D  00 01 02 03 04 05 06 07
