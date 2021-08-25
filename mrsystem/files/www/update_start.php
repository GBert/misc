<HTML>
<HEAD>
<LINK HREF="mrsystem.css" REL="stylesheet">
<TITLE>Update</TITLE>
<META http-equiv="refresh" content="1; URL=update.php">
</HEAD>
<BODY>
<DIV CLASS="odiv">
<H1>Update</H1>
</DIV>
<P>
<TABLE BORDER="1" WIDTH="100%">
<TR>
<TD CLASS='nameven'>
<PRE>
<?php
$logfile="/tmp/update.log";
$pkg=$_FILES["package"]["name"];
$full=$_FILES["package"]["tmp_name"];
system("mv $full /tmp/$pkg");
system("./mrupdate /tmp/$pkg > $logfile  2>&1 &");
?>
</PRE>
</TD>
</TR>
</TABLE>
<P>
<TABLE class="mtab1">
<colgroup span="2" width="50%"></colgroup>
<TR>
<TD>
<FORM NAME="sel" ACTION="index.htm">
<INPUT CLASS="cmenu cback" TYPE="submit" VALUE="zur&uuml;ck zum Hauptmen&uuml;">
</FORM>
</TD>
</TR>
</TABLE>
</BODY>
</HTML>
