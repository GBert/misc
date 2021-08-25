#!/bin/sh

Port=17

echo "$Port" > /sys/class/gpio/export
echo "out" >/sys/class/gpio/gpio${Port}/direction
echo "0" >/sys/class/gpio/gpio${Port}/value

# export Port=17
# echo "1" >/sys/class/gpio/gpio${Port}/value
