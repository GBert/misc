#!/bin/bash
#------------------------------------------------------------------------------
#
# Can-Can Bash Script
#
# Copyright (c) 2014 Darron M Broad
#
#------------------------------------------------------------------------------
#
# This file is part of the Can-Can CAN bus interface project.
#
# Can-Can is licensed under the CC BY-NC-SA 4.0.
#
# See file /LICENSE for details.
# 
#------------------------------------------------------------------------------

ID=$(id -u)

function notroot {
	if test $ID -ne "0"; then
		echo "error: not root"
 	       	exit 1
	fi
}

TRY="try: slcan.sh stop or can0|can1 start|stop|status"

if test $# = 1; then
	if test ! "$1" = "stop"; then
		echo $TRY
	        exit 1
	fi
	notroot
	killall slcand 2>/dev/null
	if test $? -eq 0; then
		echo "stopped"
	else
		echo "error: not started"
	fi
	exit 0
fi 

if test $# != 2; then
	echo $TRY
        exit 1
fi 

CAN=$1

if test "$CAN" = "can0"; then
	kctrl restore
	gpio_setfunc 30 ALT3
	CMD="slcand -t hw -S 1000000 /dev/ttyAMA0 can0"
elif test "$CAN" = "can1"; then
	CMD="slcand -S 460800 /dev/ttyPL2303 can1"
else
	echo $TRY
        exit 1
fi

PID=$(ps auxw | grep "$CMD" | grep -v grep | awk '{print $2}')

OPT=$2

case "$OPT" in
start) 
	notroot
	if test ! -z "$PID"; then
		kill $PID
		echo "stopped"
		sleep 1
	fi
	$CMD
	sleep 1
	ip link set $CAN up type can
	ip link set $CAN txqueuelen 1024
	echo "started"
	;;
stop)
	if test ! -z "$PID"; then
		notroot
		kill $PID
		echo "stopped"
		sleep 1
	else
		echo "error: not started"
	fi
	;;
status)
	if test ! -z "$PID"; then
		ip -s -d link show $CAN
	else
		echo "error: not started"
	fi
	;;
*)
	echo $TRY
	exit 1
	;;
esac
exit 0
