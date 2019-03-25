#!/usr/bin/perl
# $Id: slotreader.pl,v 1.3 2002/06/28 10:41:49 glenn Exp $
#
# slotreader.pl - UDP multicast slotreader demo for Digitrax Loconet
# author: Glenn G. Butcher
# date: 6/9/2002
#
# ChangeLog:
#
#	0.2: Initial release
#       0.3: Nothing
#       0.3.1: Nothing

use Socket;
use Sys::Hostname;
use POSIX qw(setsid);
use IO::Socket::Multicast;
use Fcntl;

use constant MCAST_GROUP  => '225.0.0.2';
use constant MCAST_PORT   => '4501';
use constant SERVER_ADDR  => 'localhost';  #point this to your loconet server
use constant SERVER_PORT   => '4500';

$timeout = 4;  #seconds;

sub senddata {
  my ($val, $msg, $iaddr, $paddr);
  $msg = shift;
  $iaddr = inet_aton(SERVER_ADDR);
  $paddr = sockaddr_in(SERVER_PORT, $iaddr);
  send(STAT, $msg, 0, $paddr);
}

sub checksum {
  my ($byte, $chcksum);
  my @bytes = split //, shift;
  $chksum = shift @bytes;
  foreach $byte (@bytes) {
    $chksum = $chksum ^ $byte;  #XOR
  }
  return ~$chksum;   # ~ gives 1's complement
}

sub printmsg {
  my $msg = shift;
  my @chars = unpack "C".length($msg), $msg;
  foreach $char (@chars) {
    printf ("%02x ",$char);
  }
}

sub getslot {
  my $slot = shift;
  my $data, @chars, $starttime, $nowtime;
  @chars = ();
  $i = 0;
  my $sock = IO::Socket::Multicast->new(LocalPort=>MCAST_PORT,ReuseAddr=>1);
  $sock->mcast_add(MCAST_GROUP) || die "Couldn't set group: $!\n";
  fcntl $sock, &F_SETFL, O_NONBLOCK;
  my $starttime=time;
  while ($chars[0] ne "\xe7" & $slot ne $chars[2]) {
    $sock->recv($data,1024);
    @chars = unpack "C".length($data), $data;
    $nowtime =time;
    if ($nowtime - $starttime > $timeout) {
      print "$timeout second response timeout...\n"; 
      $sock->close;
      return 0;
    }
  }
  $sock->close;
  return $data;
}

sub loco {
  my $hibyte = shift;
  my $lobyte = shift;
  $hibyte = $hibyte >> 1;
}

$proto = getprotobyname('udp');
socket(STAT, PF_INET, SOCK_DGRAM, $proto) or return "socket failed";


print "\nslot> ";
while (<>) {
  $slot=$_;
  chomp $slot;
  $msg_slot = pack "S", $slot;
  substr($msg_slot, -length($msg_slot)+1) = "";
  $msg = "\xbb$msg_slot\x00";
  $msg = $msg.checksum($msg);
  senddata($msg);
  $data=getslot($slot);
  if ($data ne "0") {  
    @chars = unpack "C".length($data), $data;
    printf "\n  slot:   %02xh\n", $chars[2];
    printf "  status: %02xh\n", $chars[3];
    printf "  loco:   %u\n",   $chars[4]+($chars[9]*128);
    printf "  speed:  %02xh\n",   $chars[5];
    printf "  dirf:   %02xh\n", $chars[6];
  }
  print "\nslot> ";
}