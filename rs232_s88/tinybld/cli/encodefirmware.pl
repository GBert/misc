#!/usr/bin/perl -w
#
# Turn a firmware .hex file into a header file
#
# This allows the firmware to be embedded into the bootloader executable
#
# Usage:
#    ./encodefirmware.pl something.hex > firmware-data.h
#
# Copyright (c) 2009, Joseph Heenan                                              
# All rights reserved.                                                           
#                                                                                
# Redistribution and use in source and binary forms, with or without             
# modification, are permitted provided that the following conditions are met:    
#     * Redistributions of source code must retain the above copyright           
#       notice, this list of conditions and the following disclaimer.            
#     * Redistributions in binary form must reproduce the above copyright        
#       notice, this list of conditions and the following disclaimer in the      
#       documentation and/or other materials provided with the distribution.     
#     * Neither the name of Joseph Heenan nor the names of any other             
#       contributors may be used to endorse or promote products derived          
#       from this software without specific prior written permission.            
#                                                                                
# THIS SOFTWARE IS PROVIDED BY Joseph Heenan ''AS IS'' AND ANY                   
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED      
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE         
# DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY             
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES     
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;   
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND    
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS  
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                   

use strict;

my $filename = $ARGV[0];
if (!defined($filename))
{
    print "Syntax: $0 <.hex file>\n";
    exit 1;
}

$filename =~ s/\.hex$//;

print <<"EOD";
static const char version[] = "$filename";
static const char *firmware[] =
{
EOD

while (<>)
{
    s/[\r\n]//g;
    # obfuscate the data - add 15 to the values of the data characters
    # (which are :, 0-9, A-F(
    $_ =~ y/0-:A-F/?-IP-U/;
    print "  \"".$_."\\n\",\n";
}
print "  \"\"\n};\n";
