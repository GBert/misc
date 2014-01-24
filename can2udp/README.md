## M\*rklin CS2 Gateway emulator

This Git (OpenWRT feed) contains files to emulate the M\*rklin CS2 Gateway:

- can2udp only transfers CAN frames to UDP and vice versa
- can2lan does the same as can2udp, but also listen on TCP (for M\*rklinApp)

can2udp is working fine with Rocrail and Gleisbox (60113). The can2lan is in
alpha state.

The other files are for testing or pre-alpha state.

### Using
<pre><code>
Usage: can2udp -l \<port\> -d \<port\> -i \<can interface\>
   Version 0.2
         -l <port>           listening UDP port for the server - default 15731
         -d <port>           destination UDP port for the server - default 15730
         -b <broadcast_addr> broadcast address - default 255.255.255.255
         -i <can int>        can interface - default can0
         -f                  running in foreground
</pre></code>
<pre><code>
Usage: can2lan -u \<udp_port\> -t \<tcp_port\> -d \<udp_dest_port\> -i \<can interface\>
   Version 0.5
         -u <port>           listening UDP port for the server - default 15731
         -t <port>           listening TCP port for the server - default 15731
         -d <port>           destination UDP port for the server - default 15730
         -b <broadcast_addr> broadcast address - default 255.255.255.255
         -i <can int>        can interface - default can0
         -f                  running in foreground
</pre></code>

  

