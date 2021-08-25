## CAN Gateway

This Git (OpenWRT feed) contains files to exchange data between CAN and LAN:

- can2udp only transfers CAN frames to UDP and vice versa
- can2lan does the same as can2udp, but also listen on TCP

can2udp is working fine with Rocrail and Gleisbox (60113). The can2lan aims
to emulate the CS2 CAN2LAN protocol more closely.

The programm ist based on:
http://streaming.maerklin.de/public-media/cs2/cs2CAN-Protokoll-2_0.pdf

The other files are for testing or pre-alpha state.

### Using
```
Usage: can2udp -l <port> -d <port> -i <can interface>
   Version 0.92
         -l <port>           listening UDP port for the server - default 15731
         -d <port>           destination UDP port for the server - default 15730
         -b <broadcast_addr> broadcast address - default 255.255.255.255
         -i <can int>        can interface - default can0
         -f                  running in foreground
         -v                  verbose output (in foreground)
```
```
Usage: can2lan -c <config_dir> -u <udp_port> -t <tcp_port> -d <udp_dest_port> -i <can interface>
   Version 1.26
         -c <config_dir>     set the config directory
         -u <port>           listening UDP port for the server - default 15731
         -t <port>           listening TCP port for the server - default 15731
         -s <port>           second listening TCP server port - default 15732
         -d <port>           destination UDP port for the server - default 15730
         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan
         -i <can int>        can interface - default can0
         -k                  use a connected CAN member for config source
         -m                  doing MS1 workaround - default: don't do it
         -f                  running in foreground
         -v                  verbose output (in foreground)
```

### Using under Debian/Ubuntu

To compile can2udp under Debian/Ubuntu you need to install some libs:
```
sudo apt-get install zlib1g-dev libpcap-dev
cd src
make
```


