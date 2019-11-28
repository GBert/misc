#!/bin/sh

set -x
rescue=RPi-MCP2515-rescue

find ./ -type f -exec perl -pi -e "s/$rescue:C /Device:C /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:R /Device:R /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:PWR_FLAG /power:PWR_FLAG /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:\+3V3 /power:+3V3 /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:\+5V /power:+5V /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:\+5VA /power:+5VA /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:VCOM /power:VCOM /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:GNDA /power:GNDA /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:GND /power:GND /g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:CONN_01X/Connector_Generic:Conn_01x/g" {} \;
find ./ -type f -exec perl -pi -e "s/$rescue:CONN_02X(\d+)/Connector_Generic:Conn_02x\$1_Odd_Even/g" {} \;
