const int revisionnr = 
13296
;
const char* commithash = 
"876b56f1b2f1554c79d7e5ec71cca2f91eae418e";
const char* revlog = 
"13296 2017-12-28 08:43:26 +0100 http: wait for post instead of poll for it to save CPU resources\n" \
"13295 2017-12-27 20:07:00 +0100 initialize: trace level correction\n" \
"13294 2017-12-27 19:19:51 +0100 routedialog: crash fix\n" \
"13293 2017-12-27 16:58:50 +0100 roclcdr: get state name function added for tracing\n" \
"13292 2017-12-27 11:47:16 +0100 rocview makefile: -Wno-deprecated-declarations\n" \
"13291 2017-12-27 10:37:35 +0100 loconet: railcom dyn messages support added\n" \
"13290 2017-12-27 10:22:36 +0100 z21: railcom dyn messages support added\n" \
"13289 2017-12-27 09:41:27 +0100 http: prevent deadlock in the rocwweb broadcast service\n" \
"13288 2017-12-27 09:18:17 +0100 rocview: replaced setstringselection with setvalue in comboboxes to be case sensitive (wx work-around)\n" \
"13287 2017-12-26 22:03:56 +0100 blockdialog: wx work-around for setting signal IDs\n" \
"13286 2017-12-26 16:02:42 +0100 block: correction of the depart fifo flag\n" \
"13285 2017-12-26 10:56:33 +0100 moved node-red to its own repository\n" \
"13284 2017-12-26 09:45:32 +0100 blockgroup: check the conditions too on isFree at destination selection\n" \
"13283 2017-12-26 08:16:27 +0100 switch, signal: sod flag added\n" \
"13282 2017-12-25 12:10:05 +0100 node-red: update\n" \
"13281 2017-12-25 08:37:47 +0100 node-red: rcp send exceptions also to the info output\n" \
"13280 2017-12-24 17:21:32 +0100 stage: fix for evaluating the stopspeedtolastsection option\n" \
"13279 2017-12-24 16:53:43 +0100 node-red: fix for getting the node reference\n" \
"13278 2017-12-24 15:44:39 +0100 mqtt: fix for stack overflow\n" \
"13277 2017-12-24 15:09:50 +0100 rnmqtt: check if the keep alive timer > 0\n" \
"13276 2017-12-24 14:46:30 +0100 node-red: rcp node with 2 outputs for exceptions and info\n" \
"13275 2017-12-24 14:27:07 +0100 node-red: evaluate rcp node\n" \
"13274 2017-12-24 14:07:31 +0100 node-red: command node (wip)\n" \
"13273 2017-12-23 16:24:21 +0100 loc: trace level correction for checking function events\n" \
"13272 2017-12-23 15:47:59 +0100 actionsctrldlg: minimal index list height\n" \
"13271 2017-12-23 15:37:41 +0100 keyboarddlg: capture enter from text field to exit dialog with ok\n" \
"13270 2017-12-23 14:05:10 +0100 location: check first if a block in the location is already reserved before looking up the first free block available\n" \
"13269 2017-12-23 11:57:18 +0100 action: clone the action control node before starting the timed therad\n" \
"13268 2017-12-23 11:36:27 +0100 action: check for global shutdown and or quit flag\n" \
"13267 2017-12-23 10:07:59 +0100 rocview: app exit minor change to prevent using trace and connection after disconnect\n" \
"13266 2017-12-22 12:33:04 +0100 block: fifoin for first car too\n" \
"13265 2017-12-22 11:17:41 +0100 route, signal: fix for atfree commands in manual mode\n" \
"13264 2017-12-22 09:04:47 +0100 block: fifoin action state added\n" \
"13263 2017-12-22 08:45:03 +0100 block, text, action: eventlcid variable support\n" \
"13262 2017-12-22 08:44:02 +0100 actionsctrldlg: crash fix at add\n" \
"13261 2017-12-21 20:05:48 +0100 blockgroupingdlg: translation correction\n" \
"13260 2017-12-21 14:59:03 +0100 action, block: eventlcid variable added\n" \
"13259 2017-12-21 14:12:54 +0100 roclcdr: replaced long with time_t to prevent win64 crash\n" \
"13258 2017-12-20 17:42:13 +0100 bidib: typo\n" \
"13257 2017-12-20 17:17:29 +0100 bidib: revert messages update ...\n" \
"13256 2017-12-20 16:53:17 +0100 bidib: messages update (introduced c99 check)\n" \
"13255 2017-12-20 11:21:09 +0100 rocnetnode: revision correction\n" \
"13254 2017-12-20 10:41:45 +0100 rocnetnode: mqtt support (wip)\n" \
"13253 2017-12-20 10:05:38 +0100 rocnetnode: mqtt support (wip)\n" \
"13252 2017-12-20 09:20:47 +0100 rocnetnode: mqtt support (wip)\n" \
"13251 2017-12-20 09:18:24 +0100 rocnet: sublibrary evaluation fix\n" \
"13250 2017-12-19 18:03:47 +0100 mqtt: subscribe as soon as the broker connection is OK\n" \
"13249 2017-12-19 17:27:09 +0100 rocnetnode: initialise the mqtt mutex\n" \
"13248 2017-12-19 16:32:27 +0100 rocnet: cleanup discovery thread\n" \
"13247 2017-12-19 16:15:57 +0100 rocnet: use the mqtt library return code to prevent draining cpu\n" \

"log end";
