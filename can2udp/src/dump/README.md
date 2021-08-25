Maerklin CAN Wireshark/LUA extensions
=====================================

Usage
```
# with data
tshark -Vx -X lua_script:maerklin.lua -r maerklin.pcap
# short
tshark     -X lua_script:maerklin.lua -r maerklin.pcap | grep -i Maerklin

####
# WireShark with Maerklin Data only
# 
wireshark -i eno1 -f "port 15730 or port 15731 or port 15732 or port 80" -X lua_script:maerklin.lua
```
