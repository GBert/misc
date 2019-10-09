const int revisionnr = 
82
;
const char* commithash = 
"4ccba80b6f12d0995ad10a541b6c1d331a9ac3e7";
const char* origin = "LOCAL"
;
const char* revlog = 
"82 2019-10-09 10:49:02 +0200 mvtrack: optional, experimetal train length calculation\n" \
"81 2019-10-09 08:53:31 +0200 32 pix icon added\n" \
"80 2019-10-08 10:37:49 +0200 loc: fix for taking over the CS IID at field events\n" \
"79 2019-10-08 09:05:33 +0200 mbus: discontinue master support\n" \
"78 2019-10-08 08:58:34 +0200 loc: crash fix at properties -> removed save FX to avoid data corruption\n" \
"77 2019-10-07 11:51:49 +0200 wiodlg: help link corrected\n" \
"76 2019-10-07 11:34:08 +0200 wio: mobile function support\n" \
"75 2019-10-07 07:46:07 +0200 naming corrections\n" \
"74 2019-10-06 14:24:17 +0200 wio: link sec loco addr with wio rfid events\n" \
"73 2019-10-06 09:28:09 +0200 mbus: check if the configdataName is valid before opening a file\n" \
"72 2019-10-06 09:15:50 +0200 rocweb: the css of jquery mobile must be the last one\n" \
"71 2019-10-06 08:09:33 +0200 wio: ebreak correction\n" \
"70 2019-10-05 11:25:01 +0200 wiodlg: support for subtype i2crfid\n" \
"69 2019-10-05 10:42:19 +0200 wio: acknowledge RFID events\n" \
"68 2019-10-04 15:41:32 +0200 wio: set IID to WIO on events\n" \
"67 2019-10-04 14:11:57 +0200 wio: rfid added\n" \
"66 2019-10-04 14:11:34 +0200 seltab: offpos option added\n" \
"65 2019-10-04 10:02:53 +0200 wiodlg: servo type added\n" \
"64 2019-10-03 11:47:08 +0200 fine tune dialog for servo outputs and switches\n" \
"63 2019-10-03 08:56:06 +0200 wio: servo support\n" \
"62 2019-10-02 16:03:24 +0200 wio: single gate servo switch support\n" \
"61 2019-10-02 14:08:26 +0200 wio: basic servo support\n" \
"60 2019-10-02 10:32:08 +0200 lcdriver: fix for events from managed block\n" \
"59 2019-10-02 09:56:34 +0200 wio: diff in watchdog between WIO and RICi\n" \
"58 2019-10-02 08:05:38 +0200 mbus: use dynamic thread names to allow multiple instances\n" \
"57 2019-10-02 07:56:11 +0200 powerman: event data corruption protection added\n" \
"56 2019-10-01 14:07:30 +0200 issuedlg: fix for endless loop under Linux GTK\n" \
"55 2019-10-01 12:09:05 +0200 clntcon: trace level correction\n" \
"54 2019-10-01 08:50:37 +0200 schedules: fix for over writing the localtime structure\n" \
"53 2019-10-01 07:35:07 +0200 loc: ignore swap on direction change in case of slave loco\n" \
"52 2019-09-30 16:01:49 +0200 xnet: trace corrections\n" \
"51 2019-09-30 15:36:01 +0200 wio: fix for conf in query\n" \
"50 2019-09-30 15:00:07 +0200 locdialog: preselect port number on CAM type select\n" \
"49 2019-09-30 11:38:05 +0200 wio: support for battery and rssi added\n" \
"48 2019-09-30 10:37:58 +0200 wio: trace level corrections\n" \
"47 2019-09-30 10:31:29 +0200 loc: save fx on get properties\n" \
"46 2019-09-30 08:40:22 +0200 mbusdlg: fix for setting host in case of socketcan\n" \
"45 2019-09-29 10:57:49 +0200 accdecdlg: wio type added\n" \
"44 2019-09-29 10:41:43 +0200 loc: ignore swap on direction change if the loco is a slave\n" \
"43 2019-09-29 08:41:44 +0200 icon update with 48px\n" \
"42 2019-09-29 08:37:29 +0200 48px icon (Herman)\n" \
"41 2019-09-28 17:48:52 +0200 wio: goodbye event support\n" \
"40 2019-09-28 17:48:18 +0200 wio: goodbye event support\n" \
"39 2019-09-28 16:32:47 +0200 add the missing package folder\n" \
"38 2019-09-28 15:13:21 +0200 48px icon\n" \
"37 2019-09-28 15:09:16 +0200 96px icon\n" \
"36 2019-09-28 10:25:33 +0200 wio: watchdog\n" \
"35 2019-09-28 10:16:26 +0200 routedialog: update index after lock and reset\n" \
"34 2019-09-27 14:38:04 +0200 wio: show accu level on alive event\n" \
"33 2019-09-27 11:28:13 +0200 wio: check alive\n" \
"32 2019-09-27 08:00:00 +0200 renderer: draw output as color in case colortype is set independent from output type\n" \
"31 2019-09-26 15:29:47 +0200 rocweb icon\n" \
"30 2019-09-26 11:54:29 +0200 wiodlg: native type added\n" \
"29 2019-09-26 11:07:17 +0200 wio: alive support (wip)\n" \
"28 2019-09-25 17:35:44 +0200 translation merge: cs, nl, sl, pt_BR sr_latin, fr, de, zh_CN\n" \
"27 2019-09-25 14:45:31 +0200 clntcon: remove demo timer for MQTT clients\n" \
"26 2019-09-25 14:18:25 +0200 wiodlg: enable/disable WIO/RICi panel on select\n" \
"25 2019-09-25 14:03:56 +0200 wiodlg: type selection\n" \
"24 2019-09-25 11:17:30 +0200 loc: invdir option added for more confusion 8)\n" \
"23 2019-09-25 10:21:22 +0200 wio: set i2c added\n" \
"22 2019-09-24 17:26:42 +0200 xnet\n" \
"21 2019-09-24 16:20:01 +0200 rpm spec correction for icon\n" \
"20 2019-09-24 13:18:50 +0200 locdialog: placing flag removed from interface tab\n" \
"19 2019-09-24 10:58:17 +0200 wio: rici conf added\n" \
"18 2019-09-24 08:35:33 +0200 rnet and xnet added\n" \
"17 2019-09-23 15:40:00 +0200 wiodlg: WIP\n" \
"16 2019-09-23 14:41:39 +0200 mbus ascii: sleep 10ms in case of nothing to read\n" \
"15 2019-09-23 14:37:49 +0200 mbus ascii: fix for reading\n" \
"14 2019-09-23 14:24:06 +0200 wiodlg: WIP\n" \
"13 2019-09-22 15:03:49 +0200 wiodlg: WIP\n" \
"12 2019-09-22 11:37:52 +0200 old animated gif\n" \
"11 2019-09-22 10:00:23 +0200 removed router check at startup\n" \
"10 2019-09-22 09:07:53 +0200 obsolete icon deleted\n" \
"9 2019-09-22 08:55:52 +0200 wiodlg: WIP\n" \
"8 2019-09-22 08:43:23 +0200 clntcon: removed function to lookup duplets\n" \
"7 2019-09-22 08:19:42 +0200 clntcon: disable removal of client duplets\n" \
"6 2019-09-22 08:19:01 +0200 router: trace corrections\n" \
"5 2019-09-21 15:01:35 +0200 wiodlg: WIP\n" \
"4 2019-09-21 11:32:32 +0200 WIO dialog added: WIP\n" \
"3 2019-09-21 08:35:10 +0200 translation cleanup\n" \
"2 2019-09-20 15:21:09 +0200 router: WIP\n" \
"1 2019-09-20 14:03:13 +0200 Initial commit\n" \

"log end";
