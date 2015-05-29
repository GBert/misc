#!/bin/sh
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

TRY="try: can.sh canX start|stop|status"

if test $# != 2; then
        echo $TRY
        exit 1
fi

ID=$(id -u)

function notroot {
        if test $ID -ne "0"; then
                echo "error: not root"
                exit 1
        fi
}

CAN=$1

OPT=$2

case "$OPT" in
start) 
	notroot
	ip link set $CAN down
	ip link set $CAN up type can bitrate 250000
	ip link set $CAN txqueuelen 1024
        ;;
stop)
	notroot
	ip link set $CAN down
        ;;
status)
        ip -s -d link show $CAN
        ;;
*)
        echo $TRY
        exit 1
        ;;
esac
exit 0
