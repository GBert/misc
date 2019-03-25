#!/usr/bin/perl
#
# $Id: lococlient.pl,v 1.4 2002/06/28 10:41:49 glenn Exp $
#
# lococlient.pl - UDP multicast demo client in Perl for Digitrax Loconet
# author: Glenn G. Butcher
# date: 6/9/2002
#
#ChangeLog:
#
#	0.1: Initial release.
#	0.2: Changed port numbers.
#       0.3: Nothing
#       0.3.1: Nothing

use strict;
use IO::Socket::Multicast;

use constant GROUP => '225.0.0.2';
use constant PORT  => '4501';

my $sock = IO::Socket::Multicast->new(LocalPort=>PORT,ReuseAddr=>1);
$sock->mcast_add(GROUP) || die "Couldn't set group: $!\n";

my ($data,$char,@chars);
$|=1;

print "Listening to ".GROUP.":".PORT."...\n";
while (1) {

  next unless $sock->recv($data,1024);
  @chars = unpack "C".length($data), $data;
  foreach $char (@chars) {
    printf ("%02x ",$char);
  }
  print "\n";
}
