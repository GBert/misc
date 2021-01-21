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
#	0.60	30.12.2020	Anpassung Smartphone
#	1.00	10.01.2021	Fehler beim oeffnen der Ausgabedatei abgefangen
#						Trigger "cansend can0 180002F0#" wird bei neuer Datei gesendet
#	1.01	16.01.2021	neu ms1relay Restart-Button
#	1.02	16.01.2021	Vorbelegung aus /www/config/lokomotive.ms1
#	1.03	16.01.2021	ms1relay Restart-Button deaktiviert

use strict;
$|=1;            # Flush immediately.

my($tool, %Parameter, $xx, $version, $lokDat, $ms1Dat, $rtr, $trigger, @ms1Loks);

$0 =~ s!.*/!!;
$tool = $0;
$tool =~ s/\.pl//;
$version = "1.03";

$lokDat = "/www/config/lokomotive.cs2";
$ms1Dat = "/www/config/lokomotive.ms1";
$trigger = "cansend can0 180002F0#";

%Parameter = readParameter();

header($tool);

if (!(defined($Parameter{action}))) {
	@ms1Loks = readLokNames($ms1Dat);
	showLoks($lokDat, \@ms1Loks);
}
elsif ($Parameter{action} eq "rueckfrage" ) {
	rueckfrage(\%Parameter);
}
elsif ($Parameter{action} eq "Dateischreiben" ) {
	$rtr = lokliste4ms1($lokDat, $ms1Dat, \%Parameter, $trigger);
	if ( $rtr ne "OK") {
		print "$rtr";
		homeButton();
	}
}
elsif ($Parameter{action} eq "restart" ) {
	restart_ms1relay();
}
else {
	@ms1Loks = readLokNames($ms1Dat);
	showLoks($lokDat, \@ms1Loks);
}

footer($tool, $version);

sub restart_ms1relay{
	my($OLDpid, $NEWpid, $rtr);

	print"<div class=\"container\">";
	$OLDpid = `ps | grep " ms1relay " | grep -v grep | cut -d " " -f 1-2`;
	`/etc/init.d/ms1relay stop`;
	`sleep 5`;
	$rtr = `/etc/init.d/ms1relay start`;
	print "<br>$rtr<br>";
	$NEWpid = `ps | grep " ms1relay " | grep -v grep | cut -d " " -f 1-2`;

	print "<h1></h1>";
	print"<div>$rtr</div>";

	homeButton();
	print "</div></div>";
}

sub homeButton{

	print "<form method=\"post\">";
	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"\">Home</button>";
	print "</form>";
}
sub lokliste4ms1{
	my($lokDat, $ms1Dat, $refParameter, $trigger) = @_;

	my(%Parameter, $xx, $LokName, $lokOK, $zeile, $zeile2, $lokwerte, $snr, %SymboTausch);

	%Parameter = %{$refParameter};

	print"<div class=\"container\">";
	print "<h1></h1>";

	$SymboTausch{7} = 1;
	$SymboTausch{11} = 1;

	open (IN, $lokDat);
	open (OUT, ">$ms1Dat") or return("Error: open $ms1Dat: $!");

	print "<form method=\"post\">";

	$lokOK = "true";
	$LokName = "";
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
#	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"restart\">Restart ms1relay</button>";
	print "</form>";
	print"</div>";
	close IN;
	close OUT;
	system($trigger);
	return("OK");
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

sub readLokNames{
	my($lokDat) = @_;

	my($zeile, $LokName, @LokListe);

	open (IN, $lokDat);
	while ($zeile = <IN>) {
		if ($zeile =~ /\.name=(.*)/i ){
			$LokName = $1;
			push @LokListe, $LokName;
		}
	}
	close IN;

	return(@LokListe);
}

sub showLoks{
	my ($lokDat, $refLokListe) = @_;

	my($zeile, $LokName, $lokNameNorm, $lokZaehler, $xx, @LokListe, $Lok, $select);

	@LokListe = @{$refLokListe};

	$lokZaehler = 0;
	open (IN, $lokDat);

	$select = "";
	print"<div class=\"container\">";
	print "<h1>lokomotive.cs2</h1>";
	print "<form method=\"post\">";
	while ($zeile = <IN>) {
		if ($zeile =~ /\.name=(.*)/i ){
			$LokName = $1;
			$lokNameNorm = $LokName;
			$lokNameNorm =~ s/ /%20/g;
			$lokZaehler++;
			$xx = sprintf("LNr_%04d", $lokZaehler);
			# in aktueller ms1-Liste vorhanden
			foreach $Lok (@LokListe) {
				if ($Lok eq $LokName) {
					$select = "checked";
				}
			}
			print "<div>";
			print "<input type=\"checkbox\" name=\"$xx\" value=\"$lokNameNorm\" $select>";
			print "$LokName";
			print"</div>\n";
			#print "<br>";
			$select = "";
		}
	}
	print "<button type=\"submit\" class=\"btn action\" name=\"action\" value=\"rueckfrage\">Datei schreiben</button>";
	print "</form>";
	print"</div>";
	close IN;
}

