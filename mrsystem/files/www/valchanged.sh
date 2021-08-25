#!/bin/bash

cfgnam=$1
tabnam=$2
parnam=$3
parold=$4
parnew=$5
mrsystem=/etc/mrsystem
mrstart=/etc/mrstart
mrlok=/etc/mrlok


chg_int_mrsystem() {
	sudo /bin/sed -i -e "s;^\($1 $2\);$1 $3;" $mrsystem
}

chg_txt_mrsystem() {
	sudo /bin/sed -i -e "s;^\($1 \"$2\"\);$1 \"$3\";" $mrsystem
}

chg_txt_mrstart() {
	sudo /bin/sed -i -e "s;^\($1 $2\);$1 $3;" $mrstart
}

enable_service() {
	chg_txt_mrstart $1 "stop" "start"
}

disable_service() {
	chg_txt_mrstart $1 "start" "stop"
}

chg_ip_mrsystem() {
	oip1=`echo $parold | cut -f 1 -d "."`
	oip2=`echo $parold | cut -f 2 -d "."`
	oip3=`echo $parold | cut -f 3 -d "."`
	oip4=`echo $parold | cut -f 4 -d "."`
	nip1=`echo $parnew | cut -f 1 -d "."`
	nip2=`echo $parnew | cut -f 2 -d "."`
	nip3=`echo $parnew | cut -f 3 -d "."`
	nip4=`echo $parnew | cut -f 4 -d "."`
	sudo sed -i -r -e "s;^$1 \"$oip1\.$oip2\.$oip3\.$oip4\";$1 \"$nip1\.$nip2\.$nip3\.$nip4\";" $mrsystem
}

chg_parm_lokadr() {
	sudo /bin/sed -i -e "s;^\($1,$2\);$3,$2;" $mrlok
}

chg_parm_mrlok() {
	sudo /bin/sed -i -e "s;^\($1,$2\);$1,$3;" $mrlok
}

if [ $cfgnam = "sysconfig" ] ; then
	if [ $parnam = "zentrale" ] ; then
		chg_int_mrsystem zentrale $parold $parnew
	elif [ $parnam = "broadcast" ] ; then
		if [ $parold = "off" ] ; then
			if [ $parnew = "off" ] ; then
				chg_int_mrsystem broadcast 0 0
			elif [ $parnew = "on" ] ; then
				chg_int_mrsystem broadcast 0 1
			fi
		elif [ $parold = "on" ] ; then
			if [ $parnew = "off" ] ; then
				chg_int_mrsystem broadcast 1 0
			elif [ $parnew = "on" ] ; then
				chg_int_mrsystem broadcast 1 1
			fi
		fi
	elif [ $parnam = "udpbc" ] ; then
		chg_ip_mrsystem udpbc $parold $parnew
	elif [ $parnam = "address" ] ; then
		chg_ip_mrsystem address $parold $parnew
	elif [ $parnam = "port" ] ; then
		chg_int_mrsystem port $parold $parnew
	elif [ $parnam = "interface" ] ; then
		chg_txt_mrsystem interface $parold $parnew
	elif [ $parnam = "can_interface" ] ; then
		chg_txt_mrsystem can_interface $parold $parnew
	elif [ $parnam = "cs2_path" ] ; then
		chg_txt_mrsystem cs2_path $parold $parnew
	elif [ $parnam = "protokoll" ] ; then
		chg_int_mrsystem protokoll $parold $parnew
	elif [ $parnam = "gleissignal" ] ; then
		chg_txt_mrsystem gleissignal $parold $parnew
	elif [ $parnam = "sync" ] ; then
		chg_int_mrsystem sync $parold $parnew
	elif [ $parnam = "conn_tcp" ] ; then
		chg_int_mrsystem conn_tcp $parold $parnew
	elif [ $parnam = "wakeup_s88" ] ; then
		chg_int_mrsystem wakeup_s88 $parold $parnew
	elif [ $parnam = "gpio_s88" ] ; then
		chg_txt_mrsystem gpio_s88 "$parold" "$parnew"
	elif [ $parnam = "hide_ms2" ] ; then
		chg_int_mrsystem hide_ms2 "$parold" "$parnew"
	elif [ $parnam = "emu_host_com" ] ; then
		chg_int_mrsystem emu_host_com "$parold" "$parnew"
	elif [ $parnam = "serial_line" ] ; then
		chg_txt_mrsystem serial_line "$parold" "$parnew"
	elif [ $parnam = "num_lokfkt" ] ; then
		chg_int_mrsystem num_lokfkt "$parold" "$parnew"
		CONFIGPATH=`fgrep cs2_path /etc/mrsystem | cut -f 2 -d '"'`
		rm -f $CONFIGPATH/lokomotive.cs2
	elif [ $parnam = "write_web" ] ; then
		if [ $parold = "off" ] ; then
			if [ $parnew = "off" ] ; then
				chg_int_mrsystem write_web 0 0
			elif [ $parnew = "on" ] ; then
				chg_int_mrsystem write_web 0 1
			fi
		elif [ $parold = "on" ] ; then
			if [ $parnew = "off" ] ; then
				chg_int_mrsystem write_web 1 0
			elif [ $parnew = "on" ] ; then
				chg_int_mrsystem write_web 1 1
			fi
		fi
	elif [ $parnam = "update_file" ] ; then
		full_name="/home/developer/updates/"$parnew
		sudo /usr/local/bin/mrupdate $full_name
	elif [ $parnam = "client_cc" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrcc"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrcc"
		fi
	elif [ $parnam = "client_cs2eth" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrcs2eth"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrcs2eth"
		fi
	elif [ $parnam = "client_cs2sl" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrcs2sl"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrcs2sl"
		fi
	elif [ $parnam = "client_ms1" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrms1"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrms1"
		fi
	elif [ $parnam = "client_ms2" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrms2"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrms2"
		fi
	elif [ $parnam = "client_ms24l" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrms24l"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrms24l"
		fi
	elif [ $parnam = "client_slcan" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrslcan"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrslcan"
		fi
	elif [ $parnam = "client_zentrale" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrzentrale"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrzentrale"
		fi
	elif [ $parnam = "client_gpios88" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "startmrgpios88"
		elif [ $parnew = "disable" ] ; then
			disable_service "startmrgpios88"
		fi
	elif [ $parnam = "client_cceth" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrcceth"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrcceth"
		fi
	elif [ $parnam = "client_srcp" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrsrcp"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrsrcp"
		fi
	elif [ $parnam = "client_tty" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrtty"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrtty"
		fi
	elif [ $parnam = "client_lok" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "mrlok"
		elif [ $parnew = "disable" ] ; then
			disable_service "mrlok"
		fi
	elif [ $parnam = "drehscheibe" ] ; then
		if [ $parnew = "enable" ] ; then
			enable_service "drehscheibe"
		elif [ $parnew = "disable" ] ; then
			disable_service "drehscheibe"
		fi
	elif [ $parnam = "special_lok_adr" ] ; then
		chg_parm_lokadr special_lok_name "$parold" "$parnew"
	elif [ $parnam = "special_lok_name" ] ; then
		chg_parm_mrlok special_lok_name "$parold" "$parnew"
	elif [ $parnam = "f01" ] ; then
		chg_parm_mrlok f01 "$parold" "$parnew"
	elif [ $parnam = "f02" ] ; then
		chg_parm_mrlok f02 "$parold" "$parnew"
	elif [ $parnam = "f03" ] ; then
		chg_parm_mrlok f03 "$parold" "$parnew"
	elif [ $parnam = "f04" ] ; then
		chg_parm_mrlok f04 "$parold" "$parnew"
	elif [ $parnam = "f05" ] ; then
		chg_parm_mrlok f05 "$parold" "$parnew"
	elif [ $parnam = "f06" ] ; then
		chg_parm_mrlok f06 "$parold" "$parnew"
	elif [ $parnam = "f07" ] ; then
		chg_parm_mrlok f07 "$parold" "$parnew"
	elif [ $parnam = "f08" ] ; then
		chg_parm_mrlok f08 "$parold" "$parnew"
	elif [ $parnam = "f09" ] ; then
		chg_parm_mrlok f09 "$parold" "$parnew"
	elif [ $parnam = "f10" ] ; then
		chg_parm_mrlok f10 "$parold" "$parnew"
	elif [ $parnam = "f11" ] ; then
		chg_parm_mrlok f11 "$parold" "$parnew"
	elif [ $parnam = "f12" ] ; then
		chg_parm_mrlok f12 "$parold" "$parnew"
	elif [ $parnam = "f13" ] ; then
		chg_parm_mrlok f13 "$parold" "$parnew"
	elif [ $parnam = "f14" ] ; then
		chg_parm_mrlok f14 "$parold" "$parnew"
	elif [ $parnam = "f15" ] ; then
		chg_parm_mrlok f15 "$parold" "$parnew"
	elif [ $parnam = "f16" ] ; then
		chg_parm_mrlok f16 "$parold" "$parnew"
	fi
elif [ $cfgnam = "konfig" ] ; then
	CONFIGPATH=`fgrep cs2_path /etc/mrsystem | cut -f 2 -d '"'`
	GERAET_VRS=$CONFIGPATH/geraet.vrs
	Uid=`fgrep guiuid $GERAET_VRS | cut -f 2 -d =`
	Response=0
	Command=0
	Prio=0
	DLC=8
	hextab=`printf "%X" $tabnam | cut -f 2 -d "x"`
	b1=`echo "$hextab" | cut -b 1-2`
	byte1=`printf "%d" 0x$b1`
	b2=`echo "$hextab" | cut -b 3-4`
	byte2=`printf "%d" 0x$b2`
	b3=`echo "$hextab" | cut -b 5-6`
	byte3=`printf "%d" 0x$b3`
	b4=`echo "$hextab" | cut -b 7-8`
	byte4=`printf "%d" 0x$b4`
	byte5=11
	byte6=`echo $parnam | cut -f 2 -d "/" | cut -f 1 -d "_"`
	hexparnew=`printf "%X" $parnew | cut -f 2 -d "x"`
	b7=`echo "$parnew" | cut -b 1-2`
	byte7=`printf "%d" 0x$b7`
	b8=`echo "$parnew" | cut -b 3-4`
	byte8=`printf "%d" 0x$b8`
	/usr/local/bin/mrinject $Uid $Response $Command $Prio $DLC $byte1 $byte2 $byte3 $byte4 $byte5 $byte6 $byte7 $byte8
fi
