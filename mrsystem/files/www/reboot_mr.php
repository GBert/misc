<HTML>
<HEAD>
<LINK HREF="mrsystem.css" REL="stylesheet">
<TITLE>Restart MRSystem</TITLE>
</HEAD>
<BODY>
<DIV CLASS="odiv">
<H1>Restart MRSystem</H1>
<?php system("./restart.sh mrsystem"); ?>
<TABLE class="mtab1">
   <colgroup span="2" width="50%"></colgroup>
   <TR>
      <TD>
         <DIV>
            <TABLE class="mtab2">
               <TR>
                  <TD>
                     <FORM NAME='sel' ACTION='index.htm'>
                        <INPUT CLASS='cmenu cback' TYPE='submit' VALUE='zur&uuml;ck zum Hauptmen&uuml;'>
                     </FORM>
                  </TD>
               </TR>
            </TABLE>
         </DIV>
      </TD>
   </TR>
</TABLE>
</DIV>
</BODY>
</HTML>
