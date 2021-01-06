#!/usr/bin/perl -w
#
##################################
#
# ms1Display.pl
#
##################################
#
# Tool zur Auswahl welche Lokomotiven zur MS1 uebertragen werden sollen
#

#	0.50	27.12.2020	Start
#	0.60	30.12.2020

use strict;
$|=1;            # Flush immediately.

my($tool, %Parameter, $xx, $version, $lokDat, $ms1Dat);

$0 =~ s!.*/!!;
$tool = $0;
$tool =~ s/\.pl//;
$version = "0.60";

$lokDat = "/www/config/lokomotive.cs2";
$ms1Dat = "/www/config/lokomotive.ms1";

%Parameter = readParameter();

header($tool);

if ($Parameter{action} eq "rueckfrage" ) {
	rueckfrage(\%Parameter);
}
elsif ($Parameter{action} eq "Dateischreiben" ) {
	lokliste4ms1($lokDat, $ms1Dat, \%Parameter);
}
else {
	showLoks($lokDat);
}

footer($tool, $version);


sub lokliste4ms1{
	my($lokDat, $ms1Dat, $refParameter) = @_;
	
	my(%Parameter, $xx, $LokName, $lokOK, $zeile, $zeile2, $lokwerte, $snr, %SymboTausch);
	
	%Parameter = %{$refParameter};
	
	print"<div class=\"container\">";
	print "<h1></h1>";
	
	$SymboTausch{7} = 1;
	$SymboTausch{11} = 1;
	
	open (IN, $lokDat);
	open (OUT, ">$ms1Dat") or print "ERROR";
	
	print "<form method=\"post\">";
	
	$lokOK = "true";
	# neue Lokliste schreiben
	while ($zeile = <IN>) {
		$zeile2 = $zeile;
 		chomp($zeile2);
		chomp($zeile2);
		if ($zeile2 eq "lokomotive") {
			$lokOK = "false";
			
			# Soll die Lok uebernommen werden?
			foreach $xx (sort keys %Parameter) {
				$LokName =~ s/ /%20/g;
				if ( $Parameter{$xx} eq $LokName) {
					$LokName =~ s/%20/ /g;
					print"$LokName wurde uebernommen<br>";
					print OUT $lokwerte;
				}
			}
			$lokwerte = "";
		}
		if ($zeile2 =~ /.name=(.*)/ ) {
			$LokName = $1;
		}
		if ($zeile2 =~ /.symbol=(\d*)/ ) {
			$snr = $1;
			if (defined($SymboTausch{$snr})) {
				$zeile =~ s/$snr/$SymboTausch{$snr}/;
			}
		}
		$lokwerte .= $zeile;
		print OUT "$zeile" if ($lokOK eq "true");
	}
	
	# Soll die letzte Lok auch uebernommen werden?
	foreach $xx (sort keys %Parameter) {
		$LokName =~ s/ /%20/g;
		if ( $Parameter{$xx} eq $LokName) {
			$LokName =~ s/%20/ /g;
			print"$LokName wurde uebernommen<br>";
			print OUT $lokwerte;
		}
	}
	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"\">Home</button>";
	print "</form>";
	print"</div>";		
	close IN;
	close OUT;
}

sub rueckfrage{
	my($refParameter) = @_;
	
	my(%Parameter, $xx, $LokName);
	
	%Parameter = %{$refParameter};
	
	print"<div class=\"container\">";
	print "<h1></h1>";
	print"<div>Folgende Lokomotiven werden f&uuml;r die MS1 &uuml;ber nommen:</div>";
	print "<h1></h1>";
	print "<form method=\"post\">";
	foreach $xx (sort keys %Parameter) {
		if ($xx =~ /^LNr_/) {
			$LokName = $Parameter{$xx};
			$LokName =~ s/%20/ /g;
			print"<div class=\"center\">$LokName</div>\n";
			print"<input type=\"hidden\" name=\"$xx\" value=\"$Parameter{$xx}\">";
		}
		
	}
	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"Dateischreiben\">OK</button>";
	print "<button type=\"submit\" class=\"btn abbruch\" name=\"action\" value=\"\">Abbruch</button>";
	print "</form>";
	print"</div>";
}

sub readParameter{
	my($form, $pair, $key, $value, %inputs );
	if ( $ENV{'REQUEST_METHOD'} eq "GET" &&
	      $ENV{'QUERY_STRING'} ne '') {
	     $form = $ENV{'QUERY_STRING'};
	     }
	elsif ( $ENV{'REQUEST_METHOD'} eq "POST" ) {
	     read(STDIN,$form, $ENV{'CONTENT_LENGTH'});
	}
	else {
		return;
	}
	
	#
	# Now the variable $form has your input data.
	# Create your associative array.
	#
	foreach $pair (split('&', $form)) {
		if ($pair =~ /(.*)=(.*)/) {  # found key=value;
			($key, $value) = ($1,$2);     # get key, value.
			$value =~ s/\+/ /g;  # substitute spaces for + signs.
			$value =~ s/%(..)/pack('c',hex($1))/eg;
			$inputs{$key} = $value;   # Create Associative Array.
		}
	}
	return(%inputs);
}

sub header {
	my($tool) = @_;
	
	print "Content-type: text/html\n\n";
	print qq| 
	<!Doctype html>
	<html>
	    <head>
	        <title>$tool</title>
	        <meta charset="utf-8" />
	        <link rel="stylesheet" type="text/css" href="/ms1Display.css">
	        <meta apple-mobile-web-app-capable="yes">
	        <meta name="viewport" content="width=device-width, initial-scale=1.0">
	    </head>
	    
	    <body>
	    	<header id="header">
	    	<div class="ueb">$tool</div>
	    	</header>
	|;
}

sub footer {
	my($tool, $version) = @_;
	
	print qq| 
	<footer id="footer">
		<div class="footer-text">
			$tool $version
		</div>
	</footer>
	</body>
	</html>
	|;
}   
sub showLoks{
	my ($lokDat) = @_;
	
	my($zeile, $lokName, $lokNameNorm, $lokZaehler, $xx);
	
	$lokZaehler = 0;
	open (IN, $lokDat);
	
	print"<div class=\"container\">";
	print "<h1>lokomotive.cs2</h1>";
	print "<form method=\"post\">";
	while ($zeile = <IN>) {
		if ($zeile =~ /\.name=(.*)/i ){
			$lokName = $1;
			$lokNameNorm = $lokName;
			$lokNameNorm =~ s/ /%20/g;
			$lokZaehler++;
			$xx = sprintf("LNr_%04d", $lokZaehler);
			print "<div>";
			print "<input type=\"checkbox\" name=\"$xx\" value=\"$lokNameNorm\">";
			print "$lokName";
			print"</div>\n";
			#print "<br>";
		}
	}
	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"rueckfrage\">Datei schreiben</button>";
	print "</form>";
	print"</div>";
	close IN;
}  

