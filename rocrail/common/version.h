const int revisionnr = 
13560
;
const char* commithash = 
"e18e7865fd98e642ce85268322fb395daf5cf6ab";
const char* origin = "LOCAL"
;
const char* revlog = 
"13560 2018-02-16 17:53:11 +0100 schedulegraph: scale 5 added\n" \
"13559 2018-02-16 15:27:51 +0100 control: set booster uid to 1 in case both uid and address are not set for use in the power manager\n" \
"13558 2018-02-16 14:15:46 +0100 schedulegraph: wip\n" \
"13557 2018-02-16 12:20:12 +0100 schedulegraph: WIP\n" \
"13556 2018-02-15 11:54:18 +0100 translation merge: sl, de, fr, sr_latin, pr_BR, nl\n" \
"13555 2018-02-15 08:22:39 +0100 loc,seltab: update occ.xml with the reserved seltab blockID\n" \
"13554 2018-02-14 17:49:19 +0100 srcp: init S88 bus after connect\n" \
"13553 2018-02-14 15:36:12 +0100 http: check support key only at start\n" \
"13552 2018-02-14 12:16:34 +0100 guiframe: commented out the loco show/hide in the view menu\n" \
"13551 2018-02-14 11:11:26 +0100 guiframe: hide context menu item in the loco grid\n" \
"13550 2018-02-14 08:12:09 +0100 srcp: trace corrections\n" \
"13549 2018-02-13 11:17:35 +0100 p50x: removed the unused ctsretry variable\n" \
"13548 2018-02-13 11:14:12 +0100 digints: ctsretry is in ms which must be divided by 10\n" \
"13547 2018-02-13 11:00:03 +0100 p50: reduce wait for CTS to max 10 x 10ms\n" \
"13546 2018-02-13 09:27:18 +0100 funcmd: standalone option added to disable an extra V command in case of f0\n" \
"13545 2018-02-12 12:22:12 +0100 route: removed the NOTs from the conditions\n" \
"13544 2018-02-12 11:59:01 +0100 basedlg: show bus also in HEX in case the UID is set\n" \
"13543 2018-02-12 11:52:22 +0100 route: fix for condition with type all\n" \
"13542 2018-02-12 11:21:10 +0100 bidib: show uid in hex at command\n" \
"13541 2018-02-12 09:30:52 +0100 sensorevents: show bus as hex in case the value > 16 bit\n" \
"13540 2018-02-12 09:16:08 +0100 bidib: filter MSG_LC_STAT on product ID to report a sensor or accessory\n" \
"13539 2018-02-12 07:58:59 +0100 item: activate the hidden selected loco before setting it in a block\n" \
"13538 2018-02-11 15:19:16 +0100 bidib: ignore error in case of MSG_LC_NA reports port 0xFFFF ???\n" \
"13537 2018-02-11 15:11:26 +0100 bidib: ignore error in case of MSG_LC_NA reports port 0xFFFF ???\n" \
"13536 2018-02-11 14:54:57 +0100 route: trigger signals set\n" \
"13535 2018-02-11 13:40:23 +0100 locseldlg: show all option added\n" \
"13534 2018-02-10 08:27:14 +0100 turntabledialog: label correction for embeded block speed\n" \
"13533 2018-02-10 08:23:41 +0100 tt: optional bridgespeed in case of embeded block\n" \
"13532 2018-02-09 18:20:29 +0100 output: trace level correction\n" \
"13531 2018-02-09 18:17:43 +0100 rocnetnode: revert pi3 hardware option\n" \
"13530 2018-02-09 15:32:32 +0100 rocnetnode: raspi type 3 added for using ttyS0 instead of ttyAMA0, this is only for RFID reader directly connected to the RasPi\n" \
"13529 2018-02-09 12:24:17 +0100 rocview: loco show/hide added in the loco filter view menu\n" \
"13528 2018-02-09 07:53:42 +0100 location: use %callerdesc% for (fake) train ID\n" \
"13527 2018-02-08 08:57:16 +0100 location: fix for updating child nodes, actions\n" \
"13526 2018-02-08 08:00:16 +0100 dsd2010: error/info messages corrected\n" \
"13525 2018-02-07 12:18:29 +0100 signals: correction for checking the wrong route for white signal aspect\n" \
"13524 2018-02-07 12:06:00 +0100 roclcdrv: check at enter if its the last goto block\n" \
"13523 2018-02-07 09:33:50 +0100 tt: revert invert flag for dsd2010\n" \
"13522 2018-02-07 09:21:35 +0100 tt: invert flag support for dsd2010 added\n" \
"13521 2018-02-07 08:53:44 +0100 translation merge: sl\n" \
"13520 2018-02-07 07:55:57 +0100 model: fix for overwriting the schedule init hour with real time\n" \
"13519 2018-02-07 07:51:50 +0100 item: format function menu entries for the tt without prefix zero\n" \
"13518 2018-02-07 07:50:03 +0100 issuedlg: replace invalid chars for the file system with underscores in the subject\n" \
"13517 2018-02-06 17:50:51 +0100 location: replace children on modify\n" \
"13516 2018-02-06 15:33:27 +0100 cbus: trace the unexpected event from cancab v2 because MERG should fix this\n" \
"13515 2018-02-06 14:38:49 +0100 cbus: ignore accessory events from node 0xFFFF CANCAB v2\n" \
"13514 2018-02-06 11:21:50 +0100 dsd2010: fix for reporting the bridge sensor state\n" \
"13513 2018-02-06 09:31:05 +0100 dsd2010: function command support\n" \
"13512 2018-02-06 08:45:50 +0100 dsd2010: trace turn direction\n" \
"13511 2018-02-06 08:12:51 +0100 dsd2010: error/info evaluating extended\n" \

"log end";
