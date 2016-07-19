Maerklin CAN Wireshark/LUA extensions
=====================================

Usage
```
# with data
tshark -Vx -X lua_script:maerklin.lua -r maerklin.pcap
# short
tshark     -X lua_script:maerklin.lua -r maerklin.pcap | grep -i Maerklin
```
