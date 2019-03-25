#!/usr/bin/perl
#
# $Id: bcastclient.pl,v 1.2 2002/06/28 10:41:49 glenn Exp $
#
# bcastclient.pl - UDP broadcast demo client in Perl for Digitrax Loconet
# author: Glenn G. Butcher
# date: 6/9/2002
#
#ChangeLog:
#
#	0.1: Initial release.
#	0.2: Changed port numbers.
#       0.3: Nothing
#       0.3.1 Nothing

use strict;
use IO::Socket::INET;

use constant BCASTADDRESS => '127.0.0.1';
use constant PORT  => '4501';

my $sock = IO::Socket::INET->new(Proto=>'udp',LocalPort=>PORT);

my ($data,$char,@chars);
$|=1;

print "Listening to ".BCASTADDRESS.":".PORT."...\n";
while (1) {

  next unless $sock->recv($data,1024);
  @chars = unpack "C".length($data), $data;
  foreach $char (@chars) {
    printf ("%02x ",$char);
  }
  print "\n";
}
