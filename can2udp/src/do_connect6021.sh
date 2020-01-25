#!/bin/sh

cansend can0 00360300#
sleep 1
cansend can0 00360300#36303CB8E4
cansend can0 00360300#36303CB811
sleep 1
# cansend can0 003A0300#36303CB800
# sleep 1
cansend can0 00440300#36303CB80000
cansend can0 00440300#36303CB8FFFF
sleep 1
cansend can0 00000300#0000000001
