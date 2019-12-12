const int revisionnr = 
287
;
const char* commithash = 
"76f915e1083053598b291cb81fa623130b356559";
const char* origin = "LOCAL"
;
const char* revlog = 
"287 2019-12-12 16:34:48 +0100 wiodlg: set display geometry\n" \
"286 2019-12-12 10:34:10 +0100 wiodlg: layout improvements\n" \
"285 2019-12-11 17:50:41 +0100 wrapper: set r2rnet to false\n" \
"284 2019-12-11 17:44:11 +0100 cv: use combobox for IID, and fill it\n" \
"283 2019-12-11 08:51:06 +0100 guiapp: disable asserts\n" \
"282 2019-12-10 17:25:56 +0100 idle: set schedule time at next schedule in a tour\n" \
"281 2019-12-10 17:07:00 +0100 model: softreset before processing a remove command\n" \
"280 2019-12-10 08:17:02 +0100 wiodlg: max nodename length 8\n" \
"279 2019-12-10 08:09:41 +0100 wio: add string terminator in case of nodename\n" \
"278 2019-12-09 14:03:39 +0100 model: reject remove objects in auto mode\n" \
"277 2019-12-09 13:31:04 +0100 node: fix for evaluating the replaceChilds flag...\n" \
"276 2019-12-09 12:02:47 +0100 wio: mobile rfid event with nodename, mapped to code (identifier)\n" \
"275 2019-12-09 11:25:26 +0100 wio: get/set nodename\n" \
"274 2019-12-08 11:45:59 +0100 block: broadcast embedded sensor events in case of crossing block\n" \
"273 2019-12-07 18:20:54 +0100 wiodlg: booster 1A option added\n" \
"272 2019-12-07 14:06:13 +0100 lightctrldlg: translation fix\n" \
"271 2019-12-07 13:47:46 +0100 stage: re-initialize section length and train gap after a modify\n" \
"270 2019-12-07 12:18:17 +0100 wiodlg: removed mobile option mode1\n" \
"269 2019-12-07 11:41:28 +0100 wiodlg: option mobile mode 1 added\n" \
"268 2019-12-07 09:04:17 +0100 wiodlg: labels for serial extended with bps\n" \
"267 2019-12-07 08:33:24 +0100 wio: reorg options\n" \
"266 2019-12-06 19:41:01 +0100 wio: options reorg WIP\n" \
"265 2019-12-06 14:45:42 +0100 wio: step range added to simulate cv2/cv5\n" \
"264 2019-12-06 11:16:55 +0100 wio: mobile sensorid reporting added\n" \
"263 2019-12-06 07:59:37 +0100 translation merge: de, nl, fr, sl\n" \
"262 2019-12-05 17:49:29 +0100 usocket: tracelevel correction system: check key remaining days\n" \
"261 2019-12-05 17:30:41 +0100 lightctrldlg: use until for the to column header (alttxt is to )\n" \
"260 2019-12-05 09:39:19 +0100 clntcon: set wiodcc flag also for wiomobile to ack alive events\n" \
"259 2019-12-05 07:43:54 +0100 lightctrl: trace correction\n" \
"258 2019-12-04 18:49:46 +0100 xmlscript: trace correction\n" \
"257 2019-12-04 17:12:59 +0100 lightctrl: fix for turning lights off\n" \
"256 2019-12-03 22:40:51 +0100 block: fix for none recursive modify\n" \
"255 2019-12-03 08:35:24 +0100 loc,stage: regard stop mode as none automatic mode\n" \
"254 2019-12-02 22:58:54 +0100 modplan: fix for setting the z on addModule to prevent double levels\n" \
"253 2019-12-02 19:02:08 +0100 xmlscriptdlg: crash fix in case no lclist is available\n" \
"252 2019-12-02 15:36:44 +0100 wio: steerwheel angle added to the dirv command\n" \
"251 2019-12-02 15:04:15 +0100 wio: allow 250 speed steps\n" \
"250 2019-12-01 15:23:39 +0100 block,loc: rear protection corrections\n" \
"249 2019-12-01 13:13:10 +0100 wio: loco dirv mass parameter added for mobile\n" \
"248 2019-12-01 10:38:23 +0100 block: check also for V=0 in case the enter trigger was resetted in case of rear protection\n" \
"247 2019-12-01 09:01:23 +0100 modplan: fix for saving modules in case of deletions\n" \
"246 2019-11-30 11:37:33 +0100 wiodlg: mobile type added\n" \
"245 2019-11-30 08:49:05 +0100 wrapper: removed unused module attribute\n" \
"244 2019-11-29 12:20:54 +0100 node: use defined calls for merge attributes\n" \
"243 2019-11-29 10:03:26 +0100 translation merge: de, nl, sl\n" \
"242 2019-11-29 09:31:21 +0100 node: renamed analysed attribute to routed for merging node recursive\n" \
"241 2019-11-29 08:02:47 +0100 rocweb: replace ? in file names with zero\n" \
"240 2019-11-28 14:47:56 +0100 rocnetnode: set channels flags to make sure the servo goes in sleep mode after changing settings.\n" \
"239 2019-11-28 11:54:11 +0100 wio: filter out the 100 degree value\n" \
"238 2019-11-28 11:32:27 +0100 enter: check in route for a swap to force wait\n" \
"237 2019-11-28 10:50:11 +0100 wio: layout corrections for the environment columns\n" \
"236 2019-11-28 08:52:14 +0100 route: automat option NO added\n" \
"235 2019-11-28 08:26:01 +0100 route: fix for check the condition class\n" \
"234 2019-11-28 07:45:53 +0100 http: not overwrite the rocweb image path\n" \
"233 2019-11-27 19:09:30 +0100 wiodlg: battery column without percent sign\n" \
"232 2019-11-27 18:15:03 +0100 wio: query with environment\n" \
"231 2019-11-27 15:59:41 +0100 finder: new option added to skip manual routes\n" \
"230 2019-11-27 13:24:33 +0100 guiframe: fix for assert loco grid select row if none exist\n" \
"229 2019-11-27 11:42:25 +0100 guiframe: fix for assert loco grid select row -1\n" \
"228 2019-11-27 11:28:25 +0100 guiframe: assert fix for select loco grid row -1\n" \
"227 2019-11-27 11:03:28 +0100 wiodlg: added include for standard integers\n" \
"226 2019-11-27 10:02:16 +0100 rocrailinidialog: show/edit rocweb image path\n" \
"225 2019-11-27 09:06:57 +0100 rocweb: html file type support added\n" \
"224 2019-11-26 15:10:03 +0100 wio: add humidity to the alive event\n" \
"223 2019-11-26 11:32:19 +0100 rocview: no questions in case the server sended a shutdown\n" \
"222 2019-11-26 11:06:14 +0100 guiframe: crash fix for scale to fit in case no plan is loaded\n" \
"221 2019-11-25 10:44:22 +0100 wio: sensor pressure added\n" \
"220 2019-11-24 10:58:33 +0100 wio: temp100 support added\n" \
"219 2019-11-24 10:35:09 +0100 wio: use temperature from alive events\n" \
"218 2019-11-23 17:18:10 +0100 accdecdlg: layout fix\n" \
"217 2019-11-23 17:07:27 +0100 debian control update\n" \
"216 2019-11-23 17:00:13 +0100 info.plist: version update\n" \
"215 2019-11-23 10:22:00 +0100 wio: fix for bidi address event\n" \
"214 2019-11-22 12:27:52 +0100 wiodlg: resize all columns\n" \
"213 2019-11-22 11:11:52 +0100 wiodlg: show RailCom in the index\n" \
"212 2019-11-22 10:38:57 +0100 wio: set RailCom reading by dialog\n" \
"211 2019-11-20 15:33:31 +0100 wio: bidi address report added\n" \
"210 2019-11-19 09:12:08 +0100 block: reset signals on exit option added\n" \
"209 2019-11-19 08:52:18 +0100 blockdialog: reset signals on exit\n" \
"208 2019-11-19 08:00:57 +0100 roclcdr: fix for checkinf shortin and inatpre2in\n" \
"207 2019-11-18 15:51:22 +0100 last time removal of dtc\n" \
"206 2019-11-18 14:46:51 +0100 z21: not trace unknown packets\n" \
"205 2019-11-16 14:54:57 +0100 loc: use modified direction on consist dispatch\n" \
"204 2019-11-16 08:39:09 +0100 wio: fix for old compiler\n" \
"203 2019-11-16 08:28:52 +0100 blockdialog: remove scale\n" \
"202 2019-11-16 08:25:45 +0100 block: use plan scale for mvtrack\n" \
"201 2019-11-15 08:04:56 +0100 renderer: blockstate for road svg\n" \
"200 2019-11-14 18:11:50 +0100 roads theme: state signals (Wolfgang)\n" \
"199 2019-11-14 14:33:40 +0100 wio: bidi event added\n" \
"198 2019-11-14 08:01:29 +0100 switch: set delay on blink CTC LED\n" \
"197 2019-11-13 15:18:36 +0100 switch: ctc WIO blink LED support in case of pending\n" \
"196 2019-11-13 14:46:16 +0100 wio: output blink support added\n" \
"195 2019-11-13 11:16:25 +0100 rocrailinidialog: router corrections\n" \
"194 2019-11-13 11:14:11 +0100 rocrailinidialog: corrections\n" \
"193 2019-11-13 11:10:16 +0100 rocrailinidialog: corrections\n" \
"192 2019-11-13 09:07:22 +0100 rocrailinidialog: router source renaming\n" \
"191 2019-11-13 09:03:50 +0100 locationdialog: layout corrections\n" \
"190 2019-11-13 08:58:29 +0100 routedialog: trace corrections\n" \
"189 2019-11-13 08:40:48 +0100 router: renaming source code\n" \
"188 2019-11-13 07:45:15 +0100 model: router trace corrections\n" \
"187 2019-11-12 17:52:36 +0100 finder: crash fix if no route is defined\n" \
"186 2019-11-12 11:54:14 +0100 locationdialog: layout corrections\n" \
"185 2019-11-12 11:22:18 +0100 check at enter if the block is a TT type and force wait\n" \
"184 2019-11-12 10:45:58 +0100 routedialog: fix for selecting lock type in case of tt and seltab\n" \
"183 2019-11-12 08:32:46 +0100 wio: send a decoder event in case of online/offline\n" \
"182 2019-11-11 15:07:40 +0100 wiodlg: show RSSI units in the header\n" \
"181 2019-11-11 13:08:16 +0100 wio: patch stdint.h for type int16_t (Lothar)\n" \
"180 2019-11-11 10:14:18 +0100 wio: convert rssi into int16_t\n" \
"179 2019-11-10 11:10:02 +0100 wio: 16bit cv numbers\n" \
"178 2019-11-09 08:16:53 +0100 locationsdlg: partner select button action added\n" \
"177 2019-11-08 15:51:07 +0100 wio: alive timeout at monitor level\n" \
"176 2019-11-08 14:52:52 +0100 partner added to locations\n" \
"175 2019-11-08 14:37:02 +0100 location: partner flow management\n" \
"174 2019-11-08 14:14:38 +0100 partner added to locations\n" \
"173 2019-11-08 14:12:06 +0100 rocrailinidlg: WIO options added\n" \
"172 2019-11-07 08:21:36 +0100 rocrailinidialog: wio options\n" \
"171 2019-11-07 07:40:03 +0100 rocrailinidlg: WIO options added\n" \
"170 2019-11-07 07:34:35 +0100 wio: watchdog timeout option\n" \
"169 2019-11-06 12:10:54 +0100 wio: show add to list trace at monitor level\n" \
"168 2019-11-06 11:48:58 +0100 makewin.sh: use bash instead of sh\n" \
"167 2019-11-06 10:50:28 +0100 wiodlg: layout correction\n" \
"166 2019-11-06 10:01:43 +0100 makewin.sh: added the zipper command\n" \
"165 2019-11-05 15:54:47 +0100 makewin.sh: simplified windows make script\n" \
"164 2019-11-05 15:35:15 +0100 cleanup\n" \
"163 2019-11-05 13:56:38 +0100 guiframe: report remark in case generic signal addresses\n" \
"162 2019-11-05 08:11:39 +0100 guiframe: report location split up in separate columns\n" \
"161 2019-11-04 11:28:23 +0100 version correction for rocs and lcdriver\n" \
"160 2019-11-04 10:56:36 +0100 report: show gate, description and decoder\n" \
"159 2019-11-04 10:22:35 +0100 signal: attribute rename addr -> addr1\n" \
"158 2019-11-04 07:55:02 +0100 wiodlg: fix for showing type LED on the index tab\n" \
"157 2019-11-03 11:13:39 +0100 guiframe: exclude the car list in case of accessory reporting\n" \
"156 2019-11-03 10:54:04 +0100 rocview: report action added\n" \
"155 2019-11-02 15:14:02 +0100 wio: trace correction for alive timeout\n" \
"154 2019-11-02 15:09:04 +0100 wio: POM cv number correction\n" \
"153 2019-11-02 13:41:59 +0100 trackdialog: show selected track on the plan panel\n" \
"152 2019-11-02 12:32:58 +0100 wio: binstate added\n" \
"151 2019-11-02 12:11:15 +0100 roads theme: fix for the twoway switch (Wolfgang)\n" \
"150 2019-11-02 11:19:55 +0100 wio: trace correction\n" \
"149 2019-11-02 10:12:03 +0100 loc: protect loco properties on field events\n" \
"148 2019-11-02 08:06:19 +0100 roads theme: more symbols (Wolfgang)\n" \
"147 2019-11-01 10:26:42 +0100 made road theme compatible with other themes\n" \
"146 2019-11-01 08:05:55 +0100 powerman: check if it is a booster event before generating a booster object\n" \
"145 2019-10-31 08:51:18 +0100 rocrailinidialog: generate booster option added\n" \
"144 2019-10-31 08:32:07 +0100 Generate booster option in dialog\n" \
"143 2019-10-31 08:25:28 +0100 powerman: option for generating new boosters\n" \
"142 2019-10-30 15:05:22 +0100 signal: fix for calculating the number of LEDs\n" \
"141 2019-10-30 15:00:10 +0100 rascii-const update\n" \
"140 2019-10-30 10:39:46 +0100 wio: fix for DCC accessory command\n" \
"139 2019-10-30 10:16:01 +0100 wio: dcc accessory command added\n" \
"138 2019-10-30 08:22:48 +0100 gotodlg: start loco after a double click\n" \
"137 2019-10-30 07:45:30 +0100 modplan: save and restore scale and metrics\n" \
"136 2019-10-29 13:29:07 +0100 rocdigs: version set to 2.1\n" \
"135 2019-10-29 09:59:46 +0100 version update from 2.0 to 2.1\n" \
"134 2019-10-29 08:36:10 +0100 wio: signal support\n" \
"133 2019-10-29 07:44:26 +0100 renderer: occupied state as highest prio\n" \
"132 2019-10-28 10:25:06 +0100 wio: signal support\n" \
"131 2019-10-27 15:28:26 +0100 translation merge: sr_latin, fr, de, nl\n" \
"130 2019-10-27 15:13:20 +0100 rocrailinidlg: removed maxcon\n" \
"129 2019-10-27 15:07:13 +0100 rocrailinidlg: removed maxcon\n" \
"128 2019-10-27 15:04:54 +0100 rocrailinidialog: removed unused maxcon option\n" \
"127 2019-10-27 11:31:59 +0100 wio: ack the alive event in case of WIO type DCC\n" \
"126 2019-10-26 21:24:31 +0200 clntcon: wio correction\n" \
"125 2019-10-25 15:09:09 +0200 wio: fix for processing loco commands\n" \
"124 2019-10-25 06:22:12 +0200 wiodlg: renamed RICi in RIC\n" \
"123 2019-10-24 10:49:02 +0200 mttmfcc: removed user suggestion\n" \
"122 2019-10-24 08:27:14 +0200 mvtrack: fix for grid size\n" \
"121 2019-10-24 08:20:42 +0200 mvtrackdlg: layout\n" \
"120 2019-10-23 15:59:51 +0200 mttmfcc: read CV correction\n" \
"119 2019-10-23 15:26:26 +0200 wio: fix for LED signal commands\n" \
"118 2019-10-23 08:24:38 +0200 wio: short circuit recevery\n" \
"117 2019-10-22 15:30:49 +0200 wio: short circuit event added\n" \
"116 2019-10-22 10:09:03 +0200 wio: signal LED type support added bus:addr aspect\n" \
"115 2019-10-22 09:01:51 +0200 wio: remove ebreak from 127 speed steps\n" \
"114 2019-10-22 08:48:02 +0200 wio: fix for calculating decoder speed\n" \
"113 2019-10-22 08:25:50 +0200 wio: POM\n" \
"112 2019-10-21 15:45:42 +0200 wio: POM added\n" \
"111 2019-10-21 13:07:38 +0200 loc: swapondirchange reverted to revision 53\n" \
"110 2019-10-21 10:52:43 +0200 gotodlg: support for double click\n" \
"109 2019-10-21 10:29:01 +0200 wio: send speed steps\n" \
"108 2019-10-21 09:50:26 +0200 mvtrack: mvtrainlen var added, regard metrics for trainlen\n" \
"107 2019-10-20 15:12:45 +0200 switchdialog: translation typo fix\n" \
"106 2019-10-20 14:04:22 +0200 loc: swapondir at commands\n" \
"105 2019-10-20 11:18:22 +0200 wio: bus added to dcc loco commands\n" \
"104 2019-10-20 10:27:51 +0200 wio: mobile function support\n" \
"103 2019-10-20 09:58:34 +0200 switch/outputdlg: fix for channeltune dialog values\n" \
"102 2019-10-20 08:25:20 +0200 wiodlg: fix for I/O next/prev\n" \
"101 2019-10-20 07:40:42 +0200 mttmfcc: support for new bidi sensor module\n" \
"100 2019-10-18 15:58:47 +0200 loc: allow swap on dir change, but no support for multi head\n" \
"99 2019-10-18 13:32:27 +0200 wio: trace info at monitor level\n" \
"98 2019-10-17 13:08:52 +0200 loc: swap on manual dir changes option and support removed\n" \
"97 2019-10-17 11:21:41 +0200 renderer: ignore white space chars < blank\n" \
"96 2019-10-17 11:04:35 +0200 renderer: show static clock in RocDisplay format\n" \
"95 2019-10-17 10:33:14 +0200 renderer: RocDisplay format support added\n" \
"94 2019-10-17 08:11:16 +0200 wio: reject text > 250\n" \
"93 2019-10-16 14:08:49 +0200 loc: use slave direction on consist events\n" \
"92 2019-10-15 08:56:01 +0200 text: convert display text from utf8 into iso latin\n" \
"91 2019-10-14 15:05:11 +0200 mttmfcc: revert support for new sensor module\n" \
"90 2019-10-13 15:27:44 +0200 wio: send brighness with clock event\n" \
"89 2019-10-13 14:00:15 +0200 loc: fix for checking consist member\n" \
"88 2019-10-13 13:45:31 +0200 mvtrack: fix for save the calctrainlen option\n" \
"87 2019-10-13 12:53:45 +0200 wio: clock sync added\n" \
"86 2019-10-13 11:19:54 +0200 wio: text message type added\n" \
"85 2019-10-13 07:52:17 +0200 loc: fix for field direction\n" \
"84 2019-10-12 12:21:34 +0200 wio: text support\n" \
"83 2019-10-11 08:00:37 +0200 revert 32px icon\n" \
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
