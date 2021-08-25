#!/bin/sh

parnam=$1

if [ $parnam = "system" ] ; then
	sudo /sbin/reboot -f
elif [ $parnam = "mrsystem" ] ; then
	sudo /etc/init.d/mrsystem restart
elif [ $parnam = "halt" ] ; then
	sudo /sbin/halt -f
fi
