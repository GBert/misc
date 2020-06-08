const int revisionnr = 
780
;
const char* commithash = 
"49d4df26e5a949fa4a2492d0e8428a831ae75c8f";
const char* origin = "LOCAL"
;
const char* revlog = 
"780 2020-06-08 10:35:42 +0200 time unit labels (WIP)\n" \
"779 2020-06-08 08:42:32 +0200 rocnetnode: support for arm64\n" \
"778 2020-06-07 13:23:13 +0200 easciiutils: added stdint header\n" \
"777 2020-06-07 10:18:55 +0200 installwx: set version 3\n" \
"776 2020-06-07 08:02:45 +0200 block: reject virtual slave block in case its defined as slave of itself\n" \
"775 2020-06-07 07:50:20 +0200 mbus: trace level change for verify\n" \
"774 2020-06-04 15:21:06 +0200 guiframe: fix for looking up the loco block; skip reserved blocks\n" \
"773 2020-06-04 10:08:57 +0200 rasciiutils: WIP\n" \
"772 2020-06-03 15:30:18 +0200 guiframe: fix for mapping loco to block in case its also in a virtual block\n" \
"771 2020-06-03 10:27:28 +0200 block: free action on reset block reservation\n" \
"770 2020-06-03 10:12:23 +0200 renderer: fix for new SVG objects in case size > 32\n" \
"769 2020-06-03 09:03:21 +0200 model: restore reservation after init blocks\n" \
"768 2020-06-02 10:16:39 +0200 guiframe: fix for crash on right click train image\n" \
"767 2020-06-01 11:58:20 +0200 cardlg: fx range extended to 32\n" \
"766 2020-05-31 08:32:01 +0200 dialogs: time unit labels (wip)\n" \
"765 2020-05-31 08:03:32 +0200 trackpicker: revert\n" \
"764 2020-05-30 18:10:48 +0200 trackpickerdlg: block correction\n" \
"763 2020-05-30 18:02:21 +0200 trackpickerdlg: block fix\n" \
"762 2020-05-30 10:39:18 +0200 locdialog: time unit lables added\n" \
"761 2020-05-30 10:08:17 +0200 car: 32 functions support\n" \
"760 2020-05-30 08:07:35 +0200 model,loc: restore reservation after processing the occupancy file\n" \
"759 2020-05-28 15:46:07 +0200 rocdigs make file: p50 added\n" \
"758 2020-05-28 14:20:31 +0200 cbus: wio support (wip)\n" \
"757 2020-05-27 15:49:08 +0200 cbus: WIO support\n" \
"756 2020-05-25 08:52:18 +0200 text: fix for mvtrainlen variable\n" \
"755 2020-05-24 08:19:43 +0200 model: generate unregistered sensor option added\n" \
"754 2020-05-23 12:21:54 +0200 cv: use default spinctrl size\n" \
"753 2020-05-23 08:51:45 +0200 rocrailinidialog: service tab time units added\n" \
"752 2020-05-23 08:45:45 +0200 rocrailinidialog: automatic tab time units added\n" \
"751 2020-05-22 15:05:12 +0200 rocs userial: regard ENOTTY also as device error\n" \
"750 2020-05-22 11:30:36 +0200 loc: isState extended with schedule ID\n" \
"749 2020-05-22 09:39:16 +0200 textdialog: max slider range minimal value reduced\n" \
"748 2020-05-22 08:10:32 +0200 wiodlg: railcom cutout option\n" \
"747 2020-05-21 07:40:51 +0200 model: revert clean up prev_* attributes\n" \
"746 2020-05-20 08:14:54 +0200 lieth: trace corrections\n" \
"745 2020-05-20 08:06:06 +0200 lieth: trace fix at disconnect\n" \
"744 2020-05-19 14:57:43 +0200 car: adjust function group in case of mapping\n" \
"743 2020-05-19 08:21:49 +0200 model: remove prev_* attributes on initial load\n" \
"742 2020-05-18 15:03:23 +0200 rocview dialogs: spinctrl size set to default; correct size should be calculated by wx\n" \
"741 2020-05-18 09:53:05 +0200 lieth: recovery improved\n" \
"740 2020-05-18 08:07:04 +0200 clntcon: crash fix on cleanup client connection\n" \
"739 2020-05-17 10:12:53 +0200 rocview: makefile fix for wx libraries in case of static\n" \
"738 2020-05-17 07:43:24 +0200 clntcon: check if the client mutex is initilized\n" \
"737 2020-05-16 14:49:56 +0200 block/switchdialog: layout corrections\n" \
"736 2020-05-16 13:55:53 +0200 srcpcon: Fix: report correct rocrail version to srcp clients (Lothar)\n" \
"735 2020-05-16 13:12:48 +0200 clntcon: protect the client socket on delete by mutex\n" \
"734 2020-05-16 13:00:17 +0200 WIOdlg: small improvements\n" \
"733 2020-05-15 15:27:56 +0200 rocview dialogs: spinctrl default width\n" \
"732 2020-05-15 11:56:32 +0200 bidibidentdlg: added missing stdint header\n" \
"731 2020-05-15 10:00:24 +0200 rocguiinidlg: svg ID text size range increased\n" \
"730 2020-05-14 14:54:11 +0200 memory statistics work around for multiple free...\n" \
"729 2020-05-14 08:21:40 +0200 clntcon: extra socket broken check added\n" \
"728 2020-05-13 11:28:02 +0200 clntcon: crash fix (1) for timing between writer and reader\n" \
"727 2020-05-13 11:07:01 +0200 itemutils: fix for calculate symbol size for none rect crossing\n" \
"726 2020-05-13 10:19:54 +0200 important changes update\n" \
"725 2020-05-11 13:25:27 +0200 allcall address option removed\n" \
"724 2020-05-09 14:53:36 +0200 routectrldlg: resizing fix\n" \
"723 2020-05-09 14:31:45 +0200 routedialog: fix for modify sensor entry\n" \
"722 2020-05-08 13:11:35 +0200 route: check for sensor, fbevent, type\n" \
"721 2020-05-08 10:37:51 +0200 wio: take over only brightness if temp. reported is 110\n" \
"720 2020-05-08 10:23:24 +0200 wio: alive brightness support added\n" \
"719 2020-05-07 10:50:04 +0200 powerctrldlg: layout fixes\n" \
"718 2020-05-07 10:39:46 +0200 routedialog: layout fixes for gtk3\n" \
"717 2020-05-07 10:21:52 +0200 canservice: stdint include for bidib messages header\n" \
"716 2020-05-07 10:19:07 +0200 bidib: stdint includes\n" \
"715 2020-05-07 10:08:48 +0200 routedialog: use default spinctrl width for gtk3\n" \
"714 2020-05-07 09:59:48 +0200 bidib: update messages header\n" \
"713 2020-05-07 08:45:29 +0200 routedialog: show sensor type in the list\n" \
"712 2020-05-06 12:07:56 +0200 bidib: add stop/go at booster on/off commands in case class is dcc_main\n" \
"711 2020-05-06 08:34:41 +0200 control.template: removed dependency libusb\n" \
"710 2020-05-06 07:55:10 +0200 rocomp: removed\n" \
"709 2020-05-04 22:20:12 +0200 ddx: exit\n" \
"708 2020-05-04 18:21:10 +0200 cleanup\n" \
"707 2020-05-04 15:26:15 +0200 installwx.sh: check if apt or zypper\n" \
"706 2020-05-03 15:09:26 +0200 makewin scripts: only use 2 make jobs to avoid a g++ kill signal\n" \
"705 2020-05-03 14:44:27 +0200 inno update\n" \
"704 2020-05-03 10:02:21 +0200 clock: member variable normalized\n" \
"703 2020-05-03 08:07:56 +0200 meter: smaller font size for Windows\n" \
"702 2020-05-02 14:58:52 +0200 z21: trace unhandled packets with trace level BYTE\n" \
"701 2020-05-02 10:04:01 +0200 trace: fix for async writer using 100% CPU in case no file is specified\n" \
"700 2020-05-01 07:47:53 +0200 wrapper: default boosterevent attribute false in case of state events\n" \
"699 2020-04-30 18:08:50 +0200 check if an item is selected\n" \
"698 2020-04-30 15:18:05 +0200 model: do not inform digints in case a generated sensor has been added\n" \
"697 2020-04-30 12:31:53 +0200 wcan: fix for return type\n" \
"696 2020-04-29 15:06:18 +0200 bidib: watchdog fix in case of no guest connection\n" \
"695 2020-04-29 14:09:06 +0200 blockdialog: fix for signal property buttons\n" \
"694 2020-04-29 10:59:31 +0200 bidib: correction for evaluating CS state\n" \
"693 2020-04-29 09:51:21 +0200 trace: use a thread to write\n" \
"692 2020-04-29 07:53:41 +0200 guiframe: filter out booster events for updating the power icon\n" \
"691 2020-04-28 14:41:47 +0200 wiodlg: fast PT option instead RC cutout\n" \
"690 2020-04-28 11:01:45 +0200 lodi: traces\n" \
"689 2020-04-28 10:58:40 +0200 lodi: programming\n" \
"688 2020-04-28 10:44:49 +0200 lodi: programming support added\n" \
"687 2020-04-27 15:30:48 +0200 rocrailinidialog: lodi added\n" \
"686 2020-04-27 15:24:43 +0200 lodi: basic implementation\n" \
"685 2020-04-27 08:53:02 +0200 bidib: trace node sequence on seqAck\n" \
"684 2020-04-26 15:04:00 +0200 lodi: sensor processing (wip)\n" \
"683 2020-04-26 14:38:28 +0200 lodi: sensor evaluating (WIP)\n" \
"682 2020-04-26 13:38:59 +0200 installwx.sh: added extra lines\n" \
"681 2020-04-26 13:13:14 +0200 linux dependency update\n" \
"680 2020-04-26 11:05:16 +0200 rocview: use default spinbutton size for GTK3\n" \
"679 2020-04-26 10:26:39 +0200 bidib: allow only a tcp guest connection in case the communication is disabled\n" \
"678 2020-04-26 07:18:02 +0200 bidib: disable message processing in case a guest is connected\n" \
"677 2020-04-25 17:24:12 +0200 loc: fix for mapped function\n" \
"676 2020-04-25 16:18:32 +0200 rocview: mirror loco image in block option added\n" \
"675 2020-04-25 14:25:35 +0200 lodi: loco speed command\n" \
"674 2020-04-25 10:41:32 +0200 canservice: fix for draining CPU on 5550 service; CBUS\n" \
"673 2020-04-25 10:05:36 +0200 cbus: tracelevel correction for clock commands\n" \
"672 2020-04-25 08:55:13 +0200 bidib: stop using the message sequence after the wizard did connect\n" \
"671 2020-04-25 08:07:11 +0200 router: fix for raster switch (Lothar)\n" \
"670 2020-04-24 15:11:26 +0200 lodi: wip\n" \
"669 2020-04-24 15:00:56 +0200 lodi: WIP\n" \
"668 2020-04-24 08:40:33 +0200 translation merge: en_GB, fr, sl, de, nl\n" \
"667 2020-04-24 08:29:10 +0200 loc: only map function if its not the same in case the mapped decoder address is zero\n" \
"666 2020-04-24 08:08:01 +0200 bidib: rewrap Wizard messages to fit the sequence as needed\n" \
"665 2020-04-23 15:08:16 +0200 bidib: tcp service added for Wizard\n" \
"664 2020-04-23 08:05:29 +0200 bidibdlg: removed the UDP type\n" \
"663 2020-04-23 07:59:21 +0200 bidib: udp not using sequence number\n" \
"662 2020-04-23 07:47:50 +0200 bidib: tcp not using sequence\n" \
"661 2020-04-22 17:41:05 +0200 canservice: server socket recovery\n" \
"660 2020-04-22 17:30:46 +0200 ASCII logo\n" \
"659 2020-04-22 16:58:50 +0200 canservice: socket nodelay\n" \
"658 2020-04-22 11:12:27 +0200 model: generate unknown sensor objects first after the model has been initialized\n" \
"657 2020-04-22 11:11:33 +0200 wio: report global RailCom reader with address 101\n" \
"656 2020-04-22 10:15:09 +0200 canservice: extra command line options for tracing and usbdev\n" \
"655 2020-04-22 08:27:24 +0200 canservice: usb connection recovery added\n" \
"654 2020-04-22 08:19:23 +0200 canservice: usb service added\n" \
"653 2020-04-22 08:16:44 +0200 bidib: tcp/ip support\n" \
"652 2020-04-21 21:56:05 +0200 model: revert generating sensors in case unknown\n" \
"651 2020-04-21 07:57:52 +0200 stagedlg: show cleaning stamp\n" \
"650 2020-04-20 16:07:17 +0200 rocdigs: remove ucon88\n" \
"649 2020-04-20 11:53:43 +0200 feedbackdialog: generated flag added\n" \
"648 2020-04-20 11:14:44 +0200 model: generate a temp. sensor node in case an event is unknown\n" \
"647 2020-04-20 08:17:58 +0200 wio: dual coil switch type support\n" \
"646 2020-04-19 14:41:11 +0200 loc: allow zero function mapping address\n" \
"645 2020-04-19 12:06:12 +0200 loc: allow zero address for function mapping\n" \
"644 2020-04-19 10:57:02 +0200 hue: tracelevel correction for not supported sensor status\n" \
"643 2020-04-19 10:31:54 +0200 hue: fix for free memory type\n" \
"642 2020-04-19 07:43:10 +0200 SpDrS60: alt blinkin dir container\n" \
"641 2020-04-18 18:20:22 +0200 wio: sent also date and temp. on clock commands\n" \
"640 2020-04-18 18:19:48 +0200 rocpro: config button added\n" \
"639 2020-04-17 12:14:23 +0200 xmlscript: fix for - comparator\n" \
"638 2020-04-17 10:43:17 +0200 bidib: auto sysenable and SoD\n" \
"637 2020-04-16 19:18:28 +0200 xmlscript: regard @ prefix for variable in condition\n" \
"636 2020-04-16 15:50:47 +0200 action: allow all block commands\n" \
"635 2020-04-16 15:09:15 +0200 router,location: fix for forcing first free subblock\n" \
"634 2020-04-16 13:43:00 +0200 blockdialog: actions waiton/waitoff added\n" \
"633 2020-04-16 10:34:42 +0200 action: support added for nonecommuter\n" \
"632 2020-04-15 16:06:23 +0200 loc: swap function rename\n" \
"631 2020-04-15 10:19:51 +0200 finddest: regard swap timer\n" \
"630 2020-04-14 15:04:43 +0200 loc: set recorded schedule as generated\n" \
"629 2020-04-14 14:53:19 +0200 loc: record schedule\n" \
"628 2020-04-13 17:25:52 +0200 WIO: PT support\n" \
"627 2020-04-12 14:54:46 +0200 loc: fix for V0 on swap mutex timeout (deadlock)\n" \
"626 2020-04-12 11:11:03 +0200 cleanstamp in all block types\n" \
"625 2020-04-12 11:08:38 +0200 wio: trace RailCom type name\n" \
"624 2020-04-11 18:03:56 +0200 guiframe: fix auto resize columns\n" \
"623 2020-04-10 13:06:50 +0200 readme.txt extended with command to install wx under Archlinux\n" \
"622 2020-04-10 09:17:19 +0200 wio: added missing rascii parameter in case of type servo\n" \
"621 2020-04-09 22:11:15 +0200 bidib: use output value for on in case of accessory\n" \
"620 2020-04-09 15:59:32 +0200 rocview.cmd: quot parameters\n" \
"619 2020-04-09 10:57:18 +0200 block: fix for wait check in case of type shunting\n" \
"618 2020-04-09 10:13:02 +0200 planpanel: check for overlapping on selection copy/move\n" \
"617 2020-04-08 08:19:16 +0200 item: add blank context menu for signals with 5 or more aspects\n" \
"616 2020-04-07 16:39:14 +0200 signaldialog: label corrections for pattern yellow address\n" \
"615 2020-04-07 08:49:04 +0200 loc: stop automode in case of gohome command and is already at home\n" \
"614 2020-04-07 07:48:59 +0200 stage: manual compress will overrules one time the FiFo in locations\n" \
"613 2020-04-06 16:34:05 +0200 rascii: extended the accessory programming commands\n" \
"612 2020-04-06 14:22:47 +0200 app: version info corrected in the trace header\n" \
"611 2020-04-06 10:26:32 +0200 rascii: fix for function command and FX state\n" \
"610 2020-04-06 10:11:04 +0200 rascii: pton/ptoff, accessory cvget/cvset and function state added\n" \
"609 2020-04-06 08:01:10 +0200 route: fix for checking signal with blank command\n" \
"608 2020-04-05 15:36:57 +0200 blockdialog: show the last cleaning-stamp on the statistics tab\n" \
"607 2020-04-05 15:14:54 +0200 translation merge: pt_BR, sl, sr_latin, fr, nl, de\n" \
"606 2020-04-05 14:59:47 +0200 guiframe: disable go home all in case automatic is off\n" \
"605 2020-04-05 10:18:18 +0200 block: use time for last cleaning\n" \
"604 2020-04-05 09:57:52 +0200 go home all by filter\n" \
"603 2020-04-04 15:43:32 +0200 model: select the block with the lowest cleaning count in case of loco type cleaning\n" \
"602 2020-04-04 10:08:18 +0200 columndlg: save/restore size and position\n" \
"601 2020-04-04 08:15:30 +0200 signal: max 5 aspect support for pattern control\n" \
"600 2020-04-04 07:49:55 +0200 item: disable alternative SVG context menu for tt, seltab, tx\n" \
"599 2020-04-03 10:41:58 +0200 bidib: regard gate value if addressing is MADA\n" \
"598 2020-04-03 10:10:19 +0200 signaldialog: layout corrections\n" \
"597 2020-04-02 18:27:06 +0200 wrapper: altsvg default false\n" \
"596 2020-04-02 13:35:45 +0200 signal: support for 3 address pattern\n" \
"595 2020-04-02 11:18:53 +0200 item: altsvg option added\n" \
"594 2020-04-02 10:37:31 +0200 planpanel: check for alternative SVG path for all symbols types\n" \
"593 2020-04-02 07:59:07 +0200 bidib: MSG_BM_CONFIDENCE only as trace\n" \
"592 2020-04-01 14:56:54 +0200 wio: show alive timeout value in trace\n" \
"591 2020-04-01 10:47:20 +0200 wiodlg: changed type throttle into control\n" \
"590 2020-04-01 10:33:59 +0200 wiodlg: type text extended with DCC booster\n" \
"589 2020-04-01 08:28:10 +0200 bidib: only SoD on feature size in case master was enabled\n" \
"588 2020-03-31 10:20:53 +0200 rasciiutils added to use the same base for WIO and RASCII (wip)\n" \
"587 2020-03-31 10:16:32 +0200 variabledlg: description column added\n" \
"586 2020-03-31 08:47:03 +0200 bidib: sys_enable first after power on or sod\n" \
"585 2020-03-30 14:27:36 +0200 locdialog: SBT/BAT delete all added\n" \
"584 2020-03-30 13:54:12 +0200 item: regard operatable for pushbutton down\n" \
"583 2020-03-30 10:19:31 +0200 rascii: fix for calculating speed for the DIRV command\n" \
"582 2020-03-29 13:03:43 +0200 revert rascii utils\n" \
"581 2020-03-29 12:23:58 +0200 signal: optional parameter added\n" \
"580 2020-03-28 14:28:57 +0100 renderer: shunt support for dcrossing left/right sl/sr\n" \
"579 2020-03-28 14:22:07 +0100 output: tristate flip trace added\n" \
"578 2020-03-27 08:59:07 +0100 renderer: shunt support for crossing*0m\n" \
"577 2020-03-27 08:32:08 +0100 renderer: support for accessory shunting\n" \
"576 2020-03-27 08:10:02 +0100 rascii: fix for draining CPU in case of a write with no connection\n" \
"575 2020-03-27 08:03:09 +0100 rascii: fix for function F0\n" \
"574 2020-03-26 17:08:06 +0100 block/routedialog: reject adding empty ID in the include/exclude list\n" \
"573 2020-03-26 13:14:46 +0100 renderer: fix for semphoremain shunt red\n" \
"572 2020-03-25 07:43:20 +0100 renderer: support for shunt and prefix signal symbols\n" \
"571 2020-03-24 08:21:20 +0100 renderer: shunt color mapping fix\n" \
"570 2020-03-23 11:21:55 +0100 rocview: auto size column option added\n" \
"569 2020-03-23 07:48:21 +0100 guiframe: fix for autosize loco grid columns in automode\n" \
"568 2020-03-21 17:27:06 +0100 fix for autogen a guest loco\n" \
"567 2020-03-21 14:14:40 +0100 zimocan: reject group count > 64\n" \
"566 2020-03-21 08:17:34 +0100 control: fix for initial SVG paths\n" \
"565 2020-03-20 13:18:59 +0100 trackpickerdlg: use standard basedlg position function\n" \
"564 2020-03-20 08:01:35 +0100 seltab: fix for deleting an empty seltab\n" \
"563 2020-03-18 15:23:29 +0100 stage: Vmin to first section option added\n" \
"562 2020-03-18 12:36:10 +0100 channeltunedlg: auto ticks added\n" \
"561 2020-03-18 12:29:11 +0100 channeltunedlg: extra buttons for plus/min/test\n" \
"560 2020-03-18 10:34:16 +0100 switchdialog: fix for channeltune\n" \
"559 2020-03-18 07:46:17 +0100 locdialog: added translation for swaponmanualdir\n" \
"558 2020-03-18 07:37:37 +0100 mbus: regard actdelay switch flag to make it possible to use zero\n" \
"557 2020-03-18 07:30:23 +0100 stagedlg: fix for selecting wait type\n" \
"556 2020-03-17 10:54:55 +0100 trackpickerdlg: removed accessory-40\n" \
"555 2020-03-17 10:50:48 +0100 trackpickerdlg: removed unsupported track-3\n" \
"554 2020-03-17 08:01:58 +0100 fix for SVG track-3 accessory\n" \
"553 2020-03-16 11:31:51 +0100 operatordlg: fix for no image icon in case of gray icons\n" \
"552 2020-03-16 08:52:52 +0100 loc: release posible other loco from train before assign\n" \
"551 2020-03-15 14:40:37 +0100 remove accessory 1\n" \
"550 2020-03-14 11:16:42 +0100 stage: Vmin in case target section is zero and arrive speed is in percent\n" \
"549 2020-03-14 10:33:53 +0100 new icon\n" \
"548 2020-03-13 14:21:53 +0100 ecos: support for 'new' system events\n" \
"547 2020-03-13 12:11:30 +0100 SLIM-COLOR: blockstate fill color fix\n" \
"546 2020-03-12 12:16:58 +0100 renderer: seltab in case of SLIM with small rounded corners\n" \
"545 2020-03-12 08:54:54 +0100 SLIM-COLOR: fix for closed small block\n" \
"544 2020-03-12 08:50:14 +0100 SLIM-COLOR: route symbols added\n" \
"543 2020-03-10 13:38:52 +0100 rocnetnode: softinit flag added to avoid reinitializing the whole I2C\n" \
"542 2020-03-10 12:23:34 +0100 WIO: support for pulselength in query response\n" \
"541 2020-03-10 09:30:47 +0100 planpaneldlg: replace option for prefix\n" \
"540 2020-03-09 21:43:23 +0100 route: regard recursive in modify\n" \
"539 2020-03-09 12:20:26 +0100 new default r2rnet multicast address and port\n" \
"538 2020-03-09 07:43:08 +0100 text: removed variable lcdestblock, was the same as lcdest\n" \
"537 2020-03-08 14:28:26 +0100 startrocrail script added into the installer\n" \
"536 2020-03-08 14:16:57 +0100 start script added for RPi\n" \
"535 2020-03-08 09:05:16 +0100 SLIM-COLOR: added missing block ident symbol\n" \
"534 2020-03-07 09:57:06 +0100 SpDrS60: blockstate occ and route added\n" \
"533 2020-03-07 08:58:15 +0100 cbus: extra dispatch traces added\n" \
"532 2020-03-05 15:56:46 +0100 locationsdlg: fix for help links\n" \
"531 2020-03-04 13:31:39 +0100 text: variable lcdestblock added\n" \
"530 2020-03-04 13:11:28 +0100 got: only sleep in the read loop in case nothing has been read\n" \
"529 2020-03-04 12:18:16 +0100 router: include stdint added\n" \
"528 2020-03-04 07:31:46 +0100 router: fix for function check on 32bit systems (Lothar)\n" \
"527 2020-03-03 07:49:41 +0100 locationsdlg: help tab sensitive\n" \
"526 2020-03-02 15:49:54 +0100 wrapper: removed ori attribute from datareq\n" \
"525 2020-03-02 15:46:59 +0100 router: fix check loco function 32 (Lothar)\n" \
"524 2020-03-02 14:58:07 +0100 outputdialog: fix for swapping parameter on/off with channeltune dialog\n" \
"523 2020-03-02 10:19:46 +0100 text: check if gotoblock is not null before using it as variable value\n" \
"522 2020-03-02 10:17:07 +0100 text: lcgotoblock variable added\n" \
"521 2020-02-29 16:14:19 +0100 control: support for datareq type svg\n" \
"520 2020-02-29 10:31:46 +0100 weather: trace level corrections\n" \
"519 2020-02-29 08:43:51 +0100 translation merge: nl, fr, sl, sr_latin, da, de\n" \
"518 2020-02-28 14:20:29 +0100 mbusdlg: show tooltip for TX/RX port\n" \
"517 2020-02-27 17:04:18 +0100 app: trace line added in case of force EoD\n" \
"516 2020-02-27 16:51:34 +0100 locodialog: pwm range extended\n" \
"515 2020-02-27 16:50:28 +0100 mbus: udp ports selectable\n" \
"514 2020-02-27 16:00:36 +0100 mbus udp: use generic reader thread name\n" \
"513 2020-02-27 15:13:34 +0100 dccpp: generate ID incase of switch and output objects without any\n" \
"512 2020-02-27 13:50:01 +0100 model: replace all blanks with underscores in case of adding a variable\n" \
"511 2020-02-27 13:35:13 +0100 app: fix for double calling EoD\n" \
"510 2020-02-27 10:51:03 +0100 rocrail icon in png format\n" \
"509 2020-02-26 21:47:37 +0100 start EoD in case of shutdown and automatic on\n" \
"508 2020-02-26 19:19:00 +0100 app: stop auto mode before save the model\n" \
"507 2020-02-26 13:59:21 +0100 clntcon: crash fix for using invalid socket handle\n" \
"506 2020-02-26 13:58:34 +0100 WIO: send pwm as extra parameter with VDIR commands\n" \
"505 2020-02-25 21:50:13 +0100 routedialog: label correction for condition type\n" \
"504 2020-02-25 11:11:40 +0100 cbus: support for binary tcp\n" \
"503 2020-02-24 19:23:47 +0100 guiframe: fix for re-restore control dialogs\n" \
"502 2020-02-24 17:02:58 +0100 powerctrldlg: catch label right click to prevent dispatching it to the grid which is under the dialog\n" \
"501 2020-02-24 16:47:32 +0100 powerctrldlg: sort colums with left click instead of right click\n" \
"500 2020-02-24 15:22:24 +0100 app: disable blocking shutdown in case of console mode mbus,tcp: connect retry sleep increased to 5 seconds\n" \
"499 2020-02-24 14:54:19 +0100 mbus,tcp: protect disconnect with a mutex\n" \
"498 2020-02-23 09:04:48 +0100 mbus: fix for discovering the loco name in case the packets are not sequential\n" \
"497 2020-02-22 14:04:12 +0100 mbus,tcp: crash fix on invalid socket\n" \
"496 2020-02-22 09:11:58 +0100 mbus: added device type WIO-CAN\n" \
"495 2020-02-21 17:17:56 +0100 mbus: update shifts trackbox from devtype 0x10 to 0x11\n" \
"494 2020-02-21 15:03:16 +0100 cbusnodedlg: canservice description\n" \
"493 2020-02-21 14:20:06 +0100 clntcon: ack alive events from CAN type too\n" \
"492 2020-02-21 12:10:42 +0100 wiodlg: CAN type added\n" \
"491 2020-02-20 14:41:04 +0100 loc: revert save block enter side on soft reset\n" \
"490 2020-02-20 14:20:12 +0100 mbus,usb: tracelevel correction\n" \
"489 2020-02-20 12:11:44 +0100 mbus usb: avoid read timeouts\n" \
"488 2020-02-19 11:17:52 +0100 loc,locdialog: allow bbt delay only in case of fixed\n" \
"487 2020-02-18 10:33:12 +0100 revert sensor accessory type\n" \
"486 2020-02-18 08:51:08 +0100 schedules: trace level correction for index\n" \
"485 2020-02-18 08:45:53 +0100 bidib: set fbtype to accessory in case of a port event\n" \
"484 2020-02-17 08:11:05 +0100 item: show loco ID in the popup title too\n" \
"483 2020-02-16 10:16:57 +0100 xnet: check for version 3.8+ in case of request POM read data from CS\n" \
"482 2020-02-16 09:18:00 +0100 xnet: lzv200 POM read\n" \
"481 2020-02-15 11:17:32 +0100 rascii: update\n" \
"480 2020-02-15 10:41:00 +0100 wio,clntcon: push mode events in case of throttle\n" \
"479 2020-02-14 11:30:52 +0100 check for manual red signal before the swap\n" \
"478 2020-02-13 17:49:20 +0100 loc: check is a consist is not itself to avoid an endless loop\n" \
"477 2020-02-13 10:58:38 +0100 locdialog: crash fix on export in case the file could not be opened\n" \
"476 2020-02-12 10:06:54 +0100 wio: provide loco mode in case of events\n" \
"475 2020-02-11 14:34:29 +0100 loc: save current block enterside for use on soft reset\n" \
"474 2020-02-11 11:26:17 +0100 car: filter direction/lights changes\n" \
"473 2020-02-11 10:43:10 +0100 wio: support for railcom dyn\n" \
"472 2020-02-10 13:29:49 +0100 routedlg: revert layout corrections\n" \
"471 2020-02-10 08:43:36 +0100 cardlg: stay on top flag added in the message dialog on delete\n" \
"470 2020-02-09 13:53:22 +0100 cardlg: fix for not popup message?\n" \
"469 2020-02-09 08:22:09 +0100 cardlg: sorting fix\n" \
"468 2020-02-08 10:22:49 +0100 wio: try find the loco by short ID\n" \
"467 2020-02-07 08:39:06 +0100 wio: memory leak fixes\n" \
"466 2020-02-05 15:11:59 +0100 router: accept IID WIO as valid\n" \
"465 2020-02-04 13:15:54 +0100 wiodlg: railcom option\n" \
"464 2020-02-04 13:15:29 +0100 translation merge: de,nl,fr\n" \
"463 2020-02-04 11:06:28 +0100 stage: trace level for enter sensor occupied moved to level automatic\n" \
"462 2020-02-03 15:08:15 +0100 wiodlg: railcom cutout option added\n" \
"461 2020-02-03 12:03:59 +0100 wio: work around for old compilers\n" \
"460 2020-02-03 11:23:05 +0100 mkdeb: removed rocraild\n" \
"459 2020-02-03 10:37:41 +0100 model: generate loco on discover/sync\n" \
"458 2020-02-03 08:48:53 +0100 guiframe: disable popupmenu items in case of none active loco\n" \
"457 2020-02-02 16:59:41 +0100 rocrail deamon support ended\n" \
"456 2020-02-02 14:16:12 +0100 wio: allow 50 steps for mobile\n" \
"455 2020-02-02 10:22:31 +0100 wio: ebreak command added\n" \
"454 2020-02-01 16:03:31 +0100 item: regard rotate parameter on change in case of an mjpg stream\n" \
"453 2020-02-01 14:25:41 +0100 wio: process RFID error events\n" \
"452 2020-02-01 12:56:56 +0100 mbus: trace level change for 2.0 no-channels\n" \
"451 2020-02-01 10:31:12 +0100 loc: in case of active BAT overwrite the V in foreign commands\n" \
"450 2020-02-01 08:09:37 +0100 z21: send multisense long QoS to the loco instead of the reporting sensor\n" \
"449 2020-01-31 11:05:47 +0100 routedlg: size hints to fit labels\n" \
"448 2020-01-31 08:14:26 +0100 rocrailinidialog: router label correction\n" \
"447 2020-01-30 17:09:50 +0100 translation added for expired key\n" \
"446 2020-01-30 13:55:38 +0100 translation merge: de\n" \
"445 2020-01-30 12:05:06 +0100 translation merge: it, sl, fr, sr_latin, cs, nl, de\n" \
"444 2020-01-30 09:36:57 +0100 xmlscript: switch unlock support added\n" \
"443 2020-01-30 08:31:47 +0100 doc: check max remark length to avoid a crash\n" \
"442 2020-01-29 13:21:50 +0100 trackpickerdlg: fix for single slip dcrossing...\n" \
"441 2020-01-29 10:57:37 +0100 trackpicker: single slip added\n" \
"440 2020-01-29 08:20:01 +0100 wiodlg: CAM option\n" \
"439 2020-01-28 09:51:00 +0100 wio: reboot command added wio: cam framesize option\n" \
"438 2020-01-27 15:10:44 +0100 xmlscript: block command unlock added\n" \
"437 2020-01-27 13:50:06 +0100 wio: ignore wiocam for alive\n" \
"436 2020-01-27 11:37:01 +0100 mjpg: rotate 90 left/right option added\n" \
"435 2020-01-27 10:54:48 +0100 wiodlg: fix for setting sync option\n" \
"434 2020-01-27 09:34:44 +0100 switch: fix for state pointer used for setting the frog\n" \
"433 2020-01-26 12:06:38 +0100 model: only reset active locos\n" \
"432 2020-01-26 09:16:45 +0100 wio: fix for servo already in position\n" \
"431 2020-01-26 08:05:27 +0100 routedialog: takeover command ID in case of modify\n" \
"430 2020-01-24 15:06:40 +0100 wiodlg: layout corrections\n" \
"429 2020-01-24 11:54:24 +0100 wiodlg: resorted query index\n" \
"428 2020-01-24 11:23:33 +0100 button svg corrections\n" \
"427 2020-01-24 10:41:28 +0100 wiodlg: enable buttons first after selection\n" \
"426 2020-01-23 15:04:45 +0100 moved cam buttons to the accessory theme\n" \
"425 2020-01-23 09:26:13 +0100 new symbols for CAM control\n" \
"424 2020-01-22 17:05:36 +0100 trackpickerdlg: register sensor added\n" \
"423 2020-01-22 16:20:11 +0100 ecos: only request function descriptions in case the sync option ist set\n" \
"422 2020-01-22 11:00:57 +0100 wio: trace added for output servo type\n" \
"421 2020-01-21 14:32:00 +0100 renderer: fix for register sensor in case of occ/route\n" \
"420 2020-01-21 10:23:59 +0100 action: fix for go home\n" \
"419 2020-01-20 14:52:11 +0100 lodi: framework added (wip)\n" \
"418 2020-01-20 14:50:40 +0100 route: fix for tracelevel\n" \
"417 2020-01-20 14:36:15 +0100 route: not wait for pending route in case not locked\n" \
"416 2020-01-20 11:52:59 +0100 wiodlg: open browser for stream in case of CAM type\n" \
"415 2020-01-20 07:16:01 +0100 renderer: fix for register sensor color in case of none SLIM\n" \
"414 2020-01-19 17:29:12 +0100 route: revert change for forcing routes\n" \
"413 2020-01-19 16:19:23 +0100 wio: fix for sound action\n" \
"412 2020-01-18 21:56:00 +0100 routedialog: command buttons moved\n" \
"411 2020-01-18 18:51:04 +0100 wio: CAM type added\n" \
"410 2020-01-18 10:18:11 +0100 SLIM: accessory 6 for register sensor\n" \
"409 2020-01-18 07:46:13 +0100 SpDrS60: accessory 6 added\n" \
"408 2020-01-17 15:56:42 +0100 renderer: regard SLIM grey in case of filling the register sensor\n" \
"407 2020-01-17 15:49:19 +0100 accessory 6 for register sensor\n" \
"406 2020-01-17 15:27:52 +0100 renderer: register sensor symbol support (WIP)\n" \
"405 2020-01-17 15:27:03 +0100 register sensor symbol\n" \
"404 2020-01-17 12:16:49 +0100 route: fix for resetting pending route\n" \
"403 2020-01-17 11:57:06 +0100 wiodlg: OTA button added\n" \
"402 2020-01-16 15:02:40 +0100 SpDrS60: small stage symbols\n" \
"401 2020-01-16 14:01:31 +0100 SLIM-COLOR: small stage symbol\n" \
"400 2020-01-16 12:16:45 +0100 SLIM: small stage symbols (Claus)\n" \
"399 2020-01-16 10:57:46 +0100 stage-s svg added\n" \
"398 2020-01-16 10:55:50 +0100 smallsymbol option for stage\n" \
"397 2020-01-16 08:27:47 +0100 SLIM-COLOR: RFID sensor symbol accessory 5\n" \
"396 2020-01-16 08:20:46 +0100 SLIM-COLOR: RFID sensor symbol accessory 5\n" \
"395 2020-01-15 17:19:49 +0100 mjpg: no default streaming file name\n" \
"394 2020-01-15 15:42:30 +0100 wiodlg: booster option\n" \
"393 2020-01-15 15:22:04 +0100 wiodlg: changed CS to power monitor\n" \
"392 2020-01-15 14:50:33 +0100 svg: set attributes with base class\n" \
"391 2020-01-15 14:46:33 +0100 svg,renderer: ellipse added\n" \
"390 2020-01-15 13:07:58 +0100 svg,renderer: polyline added\n" \
"389 2020-01-15 11:57:35 +0100 rascii,powerman: extra trace for power event\n" \
"388 2020-01-15 11:30:25 +0100 svg,renderer: line added, fix for rect rotation\n" \
"387 2020-01-15 09:30:13 +0100 renderer: draw arc from path using DC in case of GC: GC does not offer this function\n" \
"386 2020-01-15 09:12:29 +0100 svg,renderer: rounded rectangle added\n" \
"385 2020-01-15 08:21:11 +0100 item: check is bg color has been changed on model event\n" \
"384 2020-01-14 20:06:06 +0100 svg,renderer: regard the SVG source sequence\n" \
"383 2020-01-14 15:52:51 +0100 svg: polygon support added\n" \
"382 2020-01-14 14:31:01 +0100 RASCII: report state on event\n" \
"381 2020-01-14 11:48:52 +0100 svg: support for viewBox\n" \
"380 2020-01-13 14:43:42 +0100 massoth: use maxbufsize for dumping read bytes\n" \
"379 2020-01-13 13:08:15 +0100 wio: length correction on SETWIO\n" \
"378 2020-01-13 10:43:20 +0100 rascii: fix for cvget response\n" \
"377 2020-01-12 11:29:47 +0100 wiodlg: show id and type after selection in the titlebar\n" \
"376 2020-01-12 10:55:25 +0100 remove e4s.c\n" \
"375 2020-01-12 10:45:58 +0100 e4s: remove from makefile\n" \
"374 2020-01-11 22:06:37 +0100 wiodlg: board option added\n" \
"373 2020-01-11 22:05:34 +0100 e4s removed\n" \
"372 2020-01-11 12:03:39 +0100 wio: fix for sending shortid wiodlg: size fix\n" \
"371 2020-01-11 11:34:11 +0100 loc,wio: send dirf also in case of transfering shortids\n" \
"370 2020-01-10 15:44:05 +0100 wio: fix for query\n" \
"369 2020-01-10 15:28:55 +0100 WIO: reorganise WIP\n" \
"368 2020-01-09 18:14:49 +0100 action,switch: allow unloack with *\n" \
"367 2020-01-09 15:18:32 +0100 wio: crash fix at using wio list without mutex\n" \
"366 2020-01-08 15:22:32 +0100 routedialog: fix for prev/next if no route is selected\n" \
"365 2020-01-08 15:06:13 +0100 action: dispatch boostertemp\n" \
"364 2020-01-08 09:16:57 +0100 wio: allow zero length sound file name to stop the current play\n" \
"363 2020-01-08 08:28:28 +0100 switch: allow switch commands from locoID * in case of locked\n" \
"362 2020-01-07 16:41:47 +0100 removed the no longer used restricted loconet flags\n" \
"361 2020-01-07 14:25:06 +0100 powerman: use boostertemp\n" \
"360 2020-01-07 12:21:12 +0100 guiframe: loco owner column added\n" \
"359 2020-01-07 11:19:55 +0100 wio: repeat sound parameter\n" \
"358 2020-01-06 15:07:37 +0100 wio: support port type sound\n" \
"357 2020-01-06 10:37:19 +0100 wio: sound action added\n" \
"356 2020-01-05 08:17:06 +0100 loc: sent shunting commands to consist members too\n" \
"355 2020-01-04 18:06:02 +0100 controler: variable typo fix\n" \
"354 2020-01-04 17:23:46 +0100 switch: allow * as loco ID to unlock\n" \
"353 2020-01-04 16:01:05 +0100 clntcon: crash fix for cleaning up socket on client disconnect\n" \
"352 2020-01-04 13:23:47 +0100 mbus: revert master option\n" \
"351 2020-01-04 12:28:23 +0100 wio: save WIO hostname for use in the dialog\n" \
"350 2020-01-04 09:36:06 +0100 planpanel: added save as dialog after selecting action AMP\n" \
"349 2020-01-03 16:38:53 +0100 wiodlg: added html button to open browser\n" \
"348 2020-01-03 15:05:11 +0100 translation merge: sl, fr, sr_latin, cs, sv, de, nl\n" \
"347 2020-01-03 14:01:44 +0100 selectdialog,planpanel: use the prefix field as AMP comment\n" \
"346 2020-01-03 13:23:54 +0100 planpanel: add comment to the generated AMP\n" \
"345 2020-01-03 13:21:35 +0100 selectdialog, planpanel: create an AMP from selection\n" \
"344 2020-01-01 18:27:27 +0100 added missing translation\n" \
"343 2020-01-01 18:26:20 +0100 cv: update grid on set\n" \
"342 2020-01-01 18:08:05 +0100 normalize version\n" \
"341 2020-01-01 10:53:18 +0100 revert version.h to the original\n" \
"340 2019-12-31 22:47:57 +0100 copyright update\n" \
"339 2019-12-31 16:39:49 +0100 mbus: unique threadname for the tcp reader\n" \
"338 2019-12-30 10:37:29 +0100 rascii: fix for evaluating rfid\n" \
"337 2019-12-29 18:17:33 +0100 loc: take over discoverred function text in case not set in an existing fundef child node\n" \
"336 2019-12-29 17:03:22 +0100 clntcon: check socket\n" \
"335 2019-12-29 16:18:07 +0100 item: dispatch mouse motion to planpanel in case of edit modplan\n" \
"334 2019-12-29 15:27:07 +0100 zimocan: use target as nid in case of group 0x01 (accessory)\n" \
"333 2019-12-29 14:59:02 +0100 block: only take over wheel count in case of arrival pending\n" \
"332 2019-12-29 11:51:08 +0100 clntcon: check if wio socket is still valid befor using it\n" \
"331 2019-12-28 15:41:19 +0100 clntcon: fix for state broadcast\n" \
"330 2019-12-28 13:28:44 +0100 clntcon: fix for WIO RASCII read size\n" \
"329 2019-12-28 11:09:11 +0100 wiodlg: label correction\n" \
"328 2019-12-28 09:08:34 +0100 wiodlg: option display rotate added\n" \
"327 2019-12-27 18:13:10 +0100 ecos function description\n" \
"326 2019-12-27 16:46:01 +0100 ecos: only synchronise function descriptions in case the sync option is set\n" \
"325 2019-12-26 21:08:08 +0100 ecos: support for funcicon\n" \
"324 2019-12-26 15:04:17 +0100 guiframe: avoid saving empty perspective id\n" \
"323 2019-12-26 14:42:13 +0100 guiframe: check for a valid perspective id before adding it to the submenu\n" \
"322 2019-12-26 12:02:48 +0100 ecos,loc: discover function descriptions\n" \
"321 2019-12-25 09:23:47 +0100 selectdialog: prefix option added\n" \
"320 2019-12-24 08:35:57 +0100 clntcon: publish state events to WIO clients\n" \
"319 2019-12-23 19:20:43 +0100 wio: shutdown event added\n" \
"318 2019-12-23 18:06:36 +0100 wio: state event for throttle statusbar\n" \
"317 2019-12-23 11:51:11 +0100 wiodlg: sync option added\n" \
"316 2019-12-23 11:40:44 +0100 wio: fix for NULL throttleid\n" \
"315 2019-12-23 10:40:27 +0100 wio: add throttle id\n" \
"314 2019-12-22 18:33:17 +0100 wio: power commands support\n" \
"313 2019-12-22 12:27:53 +0100 wiodlg: deep sleep option added\n" \
"312 2019-12-21 11:49:10 +0100 clock: fontsize correction\n" \
"311 2019-12-18 14:09:05 +0100 desktoplink.cmd: added extra quots around the path parameters to allow blanks\n" \
"310 2019-12-18 13:18:39 +0100 locdialog: crash fix for init functions in case of no loco selected\n" \
"309 2019-12-18 13:07:18 +0100 wio: send short IDs\n" \
"308 2019-12-18 08:01:36 +0100 accgroupdlg: fix for deleting accessories from the list\n" \
"307 2019-12-17 07:49:59 +0100 rocprodlg: fix for expand/colapse tree in case of empty\n" \
"306 2019-12-16 19:43:15 +0100 lightctrldlg: reset button added\n" \
"305 2019-12-16 14:34:03 +0100 wiodlg: extended RIC layout option added\n" \
"304 2019-12-16 12:03:23 +0100 opendcc: progress popup removed\n" \
"303 2019-12-16 08:27:01 +0100 lightctrl: revert turning off lights: only turnoff on exact match to make it possible to define more entries for the same output\n" \
"302 2019-12-16 08:13:12 +0100 blockdialog: increased depart delay range\n" \
"301 2019-12-15 22:05:05 +0100 rocrailinidialog: fill IID comboboxes\n" \
"300 2019-12-15 17:18:21 +0100 var: clone node on publish to provide all child nodes\n" \
"299 2019-12-14 17:48:55 +0100 loc: regard the invert flag in case of field events\n" \
"298 2019-12-14 16:53:55 +0100 model: reject removal of important objects in case they are reserved\n" \
"297 2019-12-14 15:52:54 +0100 wiodlg: option for extern DCC\n" \
"296 2019-12-14 15:12:04 +0100 model: removed softreset before delete\n" \
"295 2019-12-14 10:41:37 +0100 wiodlg: option corrections\n" \
"294 2019-12-14 08:31:16 +0100 windows setup: removed service\n" \
"293 2019-12-14 08:19:45 +0100 node: removed unused function for setting 'replacechilds'\n" \
"292 2019-12-13 19:30:05 +0100 node: replacechild directly in the merge function as parameter instead of temp. attribute\n" \
"291 2019-12-13 19:00:45 +0100 node: use underscored attribute names for control\n" \
"290 2019-12-13 18:44:25 +0100 fix for zombie node control atttributes like 'replacechilds'\n" \
"289 2019-12-13 08:04:44 +0100 loc: useschedule action added\n" \
"288 2019-12-13 07:54:55 +0100 guiapp: fixes for merge node recursive and replace childs\n" \
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
