<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- Universal Config Interface -->
<!-- Author: RN | last change: 16.09.2018  -->
<!-- designed for MB'S digital model railroad system  -->
<html>
	<head>
		<link href="./uci.css" rel="stylesheet">
		<title>UCI</title>
	</head>
	<body>

		<?php

			function unquote($strval) { return str_replace("\"","",$strval); } // remove quotes from string if exist
			function quote($strval) { return ("\"".$strval."\""); } // add quotes to string


			// ================================================================================================
			// function get_splitted_string($sts, $qflag)
			//
			//   purpose: generates array of strings from single string by splitting it at space chars
			//            with respect to quoted parts. If quotes are found they remain in returned array.
			//   params : $sts = string to separate, may contain spaces and/or quotes
			//            $qflag = flag to decide if quoted parts of $sts are also quoted in result (if 1 they are)
			//   returns: array with splitted elements, may be empty if $sts has been an empty string
			// ================================================================================================
			function get_splitted_string($sts, $qflag)
			{
				$result = array();
				$sts = trim($sts);
				if(strlen($sts)==0) return $result;
				$parts = explode("\"", $sts);
				$pcnt = count($parts);
				if($pcnt>0) // if par1 contains any quotes
				{
					if(($pcnt&1)>0) // odd number of parts = valid -> alternating split
					{
						$toggle = ($sts[0]=='"')?1:0;//detect start condition
						for($pidx=$toggle; $pidx<$pcnt; $pidx++)//skip 0 if start with quoted
						{
							if($toggle==1)//quoted part
							{
								array_push($result, ($qflag)?quote($parts[$pidx]):$parts[$pidx]);//add quoted part to result
							}
							else//unquoted part
							{
								if(strlen(trim($parts[$pidx]))>0)//if unquoted part contains something
								{
									$result = array_merge($result, explode(" ", trim($parts[$pidx])));//add unquoted part(s) to result
								}//if
							}
							$toggle = 1 - $toggle;
						}//for $pidx
					}
					else // even number of parts = invalid -> normal split
					{
						$result = explode(" ", $sts);
					}//else
				}
				else // no quote -> normal split
				{
					$result = explode(" ", $sts);
				}//else
				return $result;
			}//get_splitted_string


			// ================================================================================================
			// function getparlinearr($parname, $chklinearr, $qflag)
			//
			//   purpose: examines lines of files for information about selected parameter:
			//            if line begins with parname its content is splitted to an array of strings and returned
			//   params : $parname = name of parameter to get information for
			//            $chklinearr = array of lines to check for selected parameter name
			//            $qflag = flag to decide if quoted parts of $sts are also quoted in result (if 1 they are)
			//
			//   returns: array of strings found, first entry contains requested parameter name
			// ================================================================================================
			function getparlinearr($parname, $chklinearr, $qflag)
			{
				foreach($chklinearr as $chkline)
				{
					$chkline = trim($chkline); // allow indentations
					if((strlen($chkline) > 2) && (substr($chkline,0,2) != "//"))
					{
						$parts = get_splitted_string($chkline, $qflag);

//if("ex_qstr"==$parname){
//foreach($parts as $showme) echo $showme,"<br />";
//}
						if(trim($parts[0] == $parname)) return $parts;
					}//if
				}//foreach
				$parts[] = "";//define empty result
				return $parts;
			}//getparlinearr



			// ================================================================================================
			// function getpartype($parname)
			//
			//   purpose: get defied type of parameter for selected name
			//   params : $parname = name of parameter to get information for
			//   returns: type of parameter, one of {'str', 'ip', 'int', 'cb', 'sel', 'rb', 'bits'}
			// ================================================================================================
			function getpartype($parname)
			{
				global $defarr;
				$partype = "str";//default
				$parlinearr = getparlinearr($parname, $defarr, 1);
				if(count($parlinearr)>1) $partype = $parlinearr[1];
				return $partype;
			}//getpartype


			// ================================================================================================
			// function getparvals($parname)
			//
			//   purpose: used for bits, radiobuttons and selectboxes,
			//            get array of predefined values from file.def
			//   params : $parname = name of parameter to get information for
			//   returns: array of predefined values
			// ================================================================================================
			function getparvals($parname)
			{
				global $defarr;
				$parlinearr = getparlinearr($parname, $defarr, 0);
				//if(count($parlinearr)>2) $parlinearr = array_slice($parlinearr, 2); // extract list of values
				if(count($parlinearr)>2) $parlinearr = explode(" ",unquote($parlinearr[2])); // extract list of values
				return $parlinearr;
			}//getparvals


			// ================================================================================================
			// function getparhelp($parname)
			//
			//   purpose: generates html attibute 'title' for displayed config name containing description
			//   params : $parname = name of parameter to get information for
			//   returns: html attibute formatted as "title='Info about parameter read from file.def'"
			// ================================================================================================
			function getparhelp($parname)
			{
				global $defarr;
				$parhelp = "";//default
				$parlinearr = getparlinearr($parname, $defarr, 0);
				//echo($parname.":".(string)count($parlinearr).", ");
				//if(count($parlinearr)>1) $parhelp = "title='".unquote(implode(" ", array_slice($parlinearr, 1))."'");
				if(count($parlinearr)>3) $parhelp = "title='".unquote($parlinearr[3])."'";
				return $parhelp;
			}//getparhelp


			// ================================================================================================
			// function getparrange($parname, $present)
			//
			//   purpose: generates html attibute 'title' for displayed config value containing 
			//            restrictions information and present value
			//   params : $parname = name of parameter to get information for
			//   returns: html attibute formatted as "title='restrictions, (present value is XY)"
			// ================================================================================================
			function getparrange($parname, $present)
			{
				global $defarr;
				$rangeinfo="";//preset
				$presinfo = "(present value is ".(string)$present.")";
				$parlinearr = getparlinearr($parname, $defarr, 0);
				if(count($parlinearr)>1)
				{
					switch($parlinearr[1])
					{
						case "int":
							$rangeinfo = (string)$parlinearr[2]."..".(string)$parlinearr[3]."  ";
							break;
						case "str":
							$rangeinfo = (string)$parlinearr[2]."..".(string)$parlinearr[3]." chars  ";
							break;
						case "ip":
							$rangeinfo = "d.d.d.d  ";
							break;
						case "txt":
							$rangeinfo = (string)$parlinearr[2]."..".(string)$parlinearr[3]." chars  ";
							break;
					}//switch
				}//if
				return "title='$rangeinfo$presinfo'";
			}//getparrange


			// ================================================================================================
			// function validatepar($pnam)
			//
			//   purpose: checks if selected parameter matches to range restictions in config definitions file
			//   params : $pnam = name of parameter to validate
			//   returns: adapted value to set (resolved bits, selected value)
			//            (indirect: set global $validated to 0 if not set, 1 if validated or 2 if not validated
			// ================================================================================================
			function validatepar($pnam)
			{
				global $validated;//will change during validation
				global $partype;//RO
				if($partype != "bits")
				{
					if(isset($_POST[$pnam]))
					{
						$rawval = $_POST[$pnam];
					}else{
						return ""; //abort if not set, $validated remains 0 (unset)
					}//else
				}//if not bits

				if(($partype != "cb") && ($partype != "ip")) $pvala = getparvals($pnam);//get array of predefined values

				switch($partype){
					case "str": //string => "text"
						$rawval = unquote($rawval);//force defined syntax to examine
						//$setvalue = quote($rawval);//force defined syntax to store
						$setvalue = $rawval;//quotes amy be added on store if needed
						$validated = ((strlen($rawval) >= $pvala[0]) && (strlen($rawval) <= $pvala[1]))?1:2;
						break;
					case "int": //integer => text
						$setvalue = $rawval;
						$intval = (integer)$setvalue;
						$validated = (((string)$intval==$rawval) && ($intval >= $pvala[0]) && ($intval <= $pvala[1]))?1:2;
						break;
					case "ip": //ip => "text"
						$rawval = unquote($rawval);//force defined syntax to examine
						//$setvalue = quote($rawval);//force defined syntax to store
						$setvalue = $rawval;//quotes amy be added on store if needed
						$validated=(ereg("^((25[0-5]|2[0-4][0-9]|1?[0-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1?[0-9]?[0-9])$", $rawval, $array))?1:2;
						break;
					case "cb": //checkbox
						$setvalue = 0;
						if($rawval == "on") $setvalue = 1;
						$validated = 1;//no real validation required
						break;
					case "sel": //select
						$setvalue = $rawval;
						$validated = 1;//no real validation required
						break;
					case "rb": //radio
						$setvalue = $rawval;
						$validated = 1;//no real validation required
						break;
					case "bits": //bits
						$setvalue = 0;
						for($j=0; $j<count($pvala); $j++)
						{
							if(isset($_POST[$pnam."_$j"]))
							{
								$rawval = $_POST[$pnam."_$j"];
								if($rawval == "on") $setvalue += (1<<$j);
							}//if
						}//for
						$validated = 1;//no real validation required
						break;
					case "txt": //string => "text"
						$rawval = unquote($rawval);//force defined syntax to examine
						//$setvalue = quote($rawval);//force defined syntax to store
						$setvalue = $rawval;//quotes amy be added on store if needed
						$validated = ((strlen($rawval) >= $pvala[0]) && (strlen($rawval) <= $pvala[1]))?1:2;
						break;
					default: //undefined=> text
						if(isset($_POST[$parname]))
						{
							$setvalue = $_POST[$parname];
						}//if
						break;
				}//switch
				return $setvalue;
			}//validatepar


			//MAIN Part, always done ##############################################################

			//get cfg-array from content of *.uci
			$uci = (isset($_POST["uci"]))?$_POST["uci"]:""; //get *.uci if posted
			$back = (isset($_POST["back"]))?$_POST["back"]:""; //get link for back-button if posted
			$seltype = "but";//default
			$cfgname = "undefined";
			if($uci != "")
			{
				$ucilinearr = file($uci); // get tab config file content to array of lines

				foreach($ucilinearr as $uciline) //treat config file
				{
					$uciline = trim($uciline); // allow indentations
					if(substr($uciline,0,7) == "cfgname")
					{
						$parts = explode("=", $uciline);
						$cfgname = unquote(trim($parts[1]));
					}
					else if(substr($uciline,0,7) == "seltype")
					{
						$parts = explode("=", $uciline);
						$selval = unquote(trim($parts[1]));
						if(($selval == "sel") || ($selval == "tab")) $seltype = $selval; // else remain "but"
					}
					else if((strlen($uciline) > 2) && (substr($uciline,0,2) != "//"))
					{
						$parts = get_splitted_string($uciline, 0);
						$cfgs[] = array("tabtxt"=>$parts[0], "filcfg"=>$parts[1], "fildef"=>$parts[2]);
					}//if
				}//foreach
			}
			else // if no tabfile defined:
			{
				$cfgs[] = array("tabtxt"=>"error", "filcfg"=>"", "fildef"=>"");
			}//else

			$icfg = (isset($_POST["icfg"]))?$_POST["icfg"]:0; //get index of selected config
			$cmd = (isset($_POST["cmd"]))?$_POST["cmd"]:"get"; //get command
			$tabdesc = $cfgs[$icfg]["tabtxt"]; // get tab description

			$cfglinearr = file($cfgs[$icfg]["filcfg"]); // get config
			$defarr = file($cfgs[$icfg]["fildef"]); // get defines and help for selected config
			$changed = 0;
			foreach($cfglinearr as $cfgline) //treat config file
			{
				$cfgline = trim($cfgline); // allow indentations
				if((strlen($cfgline) > 2) && (substr($cfgline,0,2) != "//"))//allow emply lines and comments
				{
					$parts = get_splitted_string($cfgline, 1);
					$parname = trim($parts[0]);
					$parvalue = unquote(trim($parts[1])); //old param value
					$setvalue = $parvalue;//preset

					$partype0 = getpartype($parname);
					$partype = unquote($partype0);
					$parquote = ($partype == $partype0)?0:1;//detect and memorise if we need quotes in stored config file
					
//echo "parquote=".$parquote."<br>";

					$parhelp = getparhelp($parname);
					$validated = 0;
					if($cmd == "set")
					{
						$setvalue = validatepar($parname);//may change content of $validated
						if("$parvalue" != "$setvalue") // if changed
						{
							if($validated != 2) // if valid
							{
								$storedval=(1==$parquote)?quote($parvalue):$parvalue;
								$valtostore=(1==$parquote)?quote($setvalue):$setvalue;
								if(DIRECTORY_SEPARATOR == '/')
								{
									system("./valchanged.sh $cfgname $tabdesc $parname $storedval $valtostore"); // CONFIG CHANGE EVENT
								}
								else
								{
									system("valchanged.bat $cfgname $tabdesc $parname $storedval $valtostore"); // CONFIG CHANGE EVENT
								}
								$parvalue = $setvalue;
								if($validated == 0) $validated = 1;//unvalidated change -> validated
								$changed = 1;//any change
							}
						}
						else // if unchanged
						{
							if($validated == 1) $validated = 0;// if set but unchanged
						}//else
					}//if set
					$parrange = getparrange($parname, $parvalue);//." nix"; //(present value is '".(string)$parvalue."')";
					$params[] =	array("pnam"=>$parname, "pval"=>$parvalue, "pset"=>$setvalue, "ptyp"=>$partype, "pquote"=>$parquote, "phlp"=>$parhelp, "prng"=>$parrange, "v"=>$validated);
				}//if
			}//foreach

			// write changed values (if any and if validated) to selected config file
			if(($cmd == 'set') && ($changed == 1)){
				if($fp = fopen($cfgs[$icfg]["filcfg"], 'w'))
				{
					for($i=0; $i<count($params); $i++)
					{
						$valtostore=(1==$params[$i]["pquote"])?quote($params[$i]["pval"]):$params[$i]["pval"];
						//fwrite($fp, $params[$i]["pnam"]." ".$params[$i]["pval"]."\n");
						fwrite($fp, $params[$i]["pnam"]." ".$valtostore."\n");
					}//for
					fclose($fp);
				}//if
			}//if



			// ================================================================================================
			// function do_uci()
			//
			//   purpose: generates form containing a row of buttons triggering a post request to select config file
			//   params : -
			//   returns: -
			// ================================================================================================
			function do_uci()
			{
				global $seltype;
				global $icfg;
				global $cfgs;
				global $uci;
				global $back;

				switch($seltype)
				{
					case "sel": //=>selectbox + button
						echo("<form name='sel' action='".$_SERVER['PHP_SELF']."' method='post'>");
						echo("<input type='hidden' name='uci' value='$uci'>");
						echo("<input type='hidden' name='back' value='$back'>");
						echo("<table class='ctable1'><tr><td width='100%' class='tdsel'>");
						echo("<select name='icfg' class='csel' onchange='this.form.submit()' >");
						for($i=0; $i<count($cfgs); $i++)
						{
							echo("<option value='".$i."'".(($icfg == $i)?"selected":"").">".$cfgs[$i]["tabtxt"]."</option>");
						}//for
						echo("</select></td></tr></table></form>");					
						// echo("<h1>ausgew&auml;hlt: ".$cfgs[$icfg]["tabtxt"]."</h1>");					
						break;

					case "tab": //=>tabs
						for($i=0; $i<count($cfgs); $i++)
						{
							echo("<form name='sel' action='".$_SERVER['PHP_SELF']."' method='post'>");
							echo("<input type='hidden' name='uci' value='$uci'>");
							echo("<input type='hidden' name='icfg' value='$i'>");
							echo("<input type='hidden' name='back' value='$back'>");

							echo("<input class='cseltab2 ");
							if($icfg == $i) echo("cactive2");
							echo("' type='submit' value='".$cfgs[$i]["tabtxt"]."'></form>");
						}//for
							break;

					default: //=>buttons
						for($i=0; $i<count($cfgs); $i++)
						{
							echo("<form name='sel' action='".$_SERVER['PHP_SELF']."' method='post'>");
							echo("<input type='hidden' name='uci' value='$uci'>");
							echo("<input type='hidden' name='icfg' value='$i'>");
							echo("<input type='hidden' name='back' value='$back'>");

							echo("<input class='cseltab1 ");
							if($icfg == $i) echo("cactive1");
							echo("' type='submit' value='".$cfgs[$i]["tabtxt"]."'></form>");
						}//for
						break;
				}//switch
			}//do_uci


			// ================================================================================================
			// function dotable()
			//
			//   purpose: generates form containing adaptive table for all config values in selected file
			//            submit button for form is placed below table triggering a post request
			//            reload button (without submission) is placed below submit button
			//   params : -
			//   returns: -
			// ================================================================================================
			function do_table()
			{
				global $params;
				global $icfg;
				global $back;
				global $uci;
				global $seltype;
				global $numactive;
				echo("<form name='cfg' action='".$_SERVER['PHP_SELF']."' method='post'>");
				echo("<div class='tdiv".(($seltype=="tab")?"2":"1")."'><table class='ctable1'><tr><th>Parameter</th><th>Wert</th></tr>");
				$numactive = 0;
				for($i=0; $i<count($params); $i++)
				{
					$neo = ($i&1)?"nameven":"namodd";
					$veo = ($i&1)?"valeven":"valodd";
					$vst = "v".(string)$params[$i]["v"];//text style with respect to validation

					echo("<tr><td class='cpnam ".$neo." ".$vst."n' ".$params[$i]["phlp"].">".$params[$i]["pnam"]."</td>");
					echo("<td class='cpval ".$veo." ".$vst."v' ".$params[$i]["prng"].">");
					switch($params[$i]["ptyp"])
					{
						case "cb": //=>checkbox
							echo("<input name='".$params[$i]["pnam"]."' type='checkbox' ".(($params[$i]["pset"]=="1")?"checked":"")." >");
							$numactive = $numactive + 1;
							break;
						case "sel": //=>select
							echo("<select name='".$params[$i]["pnam"]."' class='ced ".$vst."v $veo' >");
							$pvala = getparvals($params[$i]["pnam"]);//get array of predefined values
							for($j=0; $j<count($pvala); $j++)
							{
								echo("<option ".(($params[$i]["pset"]==$pvala[$j])?"selected":"").">".$pvala[$j]."</option>");
							}//for
							echo("</select>");
							$numactive = $numactive + 1;
							break;
						case "rb": //=>radio
							$pvala = getparvals($params[$i]["pnam"]);//get array of predefined vaues
							for($j=0; $j<count($pvala); $j++)
							{
								//echo("$j=".$pvala[$j]."  ");
								echo("<input type='radio' name='".$params[$i]["pnam"]."' value='".$pvala[$j]."' ".(($params[$i]["pset"]==$pvala[$j])?"checked":"")."/>".$pvala[$j]);
							}//for
							$numactive = $numactive + 1;
							break;
						case "bits": //=>cbs
							$pvala = getparvals($params[$i]["pnam"]);//get array of predefined vaues
							for($j=0; $j<count($pvala); $j++)
							{
								$chk = (((integer)$params[$i]["pset"] & (1<<$j)) > 0)?"checked":"";
								echo("<input type='checkbox' name='".$params[$i]["pnam"]."_$j'".$chk."/>".$pvala[$j]);
							}//for
							$numactive = $numactive + 1;
							break;
						case "txt": //str, ip, int => text
							echo("<input class='ced ".$vst."v $veo' name='".$params[$i]["pnam"]."' type='text' value='".$params[$i]["pset"]."' disabled >");
							break;
						default: //str, ip, int => text
							echo("<input class='ced ".$vst."v $veo' name='".$params[$i]["pnam"]."' type='text' value='".$params[$i]["pset"]."' >");
							$numactive = $numactive + 1;
							break;
					}//switch
					echo("</td></tr>");
				}//for
				echo("</table></div><br/>");
				//apply button:
				echo("<input type='hidden' name='cmd' value='set'>");
				echo("<input type='hidden' name='icfg' value='".$icfg."'>");
				echo("<input type='hidden' name='uci' value='".$uci."'>");
				echo("<input type='hidden' name='back' value='".$back."'>");
				if ($numactive > 0)
				{
					echo("<input class='cbutton1' type='submit' value='&uuml;bernehmen'></form>");
				}//if

				//reload-button:
				echo("<form name='rel' action='".$_SERVER['PHP_SELF']."' method='post'>");
				echo("<input type='hidden' name='cmd' value='get'>");
				echo("<input type='hidden' name='icfg' value='".$icfg."'>");
				echo("<input type='hidden' name='uci' value='".$uci."'>");
				echo("<input type='hidden' name='back' value='".$back."'>");
				if ($numactive > 0)
				{
					echo("<input class='cbutton2' type='submit' value='neu laden'></form>");
				}//if

				//back to main menu ?:
				if(strlen($back) > 2)
				{
					echo("<form name='back' action='".$back."' >");
					echo("<input class='cbutton2' type='submit' value='zur&uuml;ck zum Hauptmen&uuml;'></form>");
				}//if
			}//do_table

		?>

		<div class="odiv">
			<div class="cdiv">
				<?php do_uci();?>
			</div>
			<br/>
			<div class="cdiv">
				<?php do_table();?>
			</div>
		</div>
	</body>
</html>
