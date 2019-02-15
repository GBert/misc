const int revisionnr = 
15043
;
const char* commithash = 
"84d75e5f05c132822b3e21ed02ad82e8158119e3";
const char* origin = "LOCAL"
;
const char* revlog = 
"15043 2019-02-15 14:20:40 +0100 cbusnodedlg: update for CS3\n" \
"15042 2019-02-15 12:31:34 +0100 planpropsdlg: replaced translation remark with notes\n" \
"15041 2019-02-15 12:15:41 +0100 mbus: tcp fix for read in case of block\n" \
"15040 2019-02-15 08:26:18 +0100 rocview: -auiview command line option added guiframe: save remark on plan save too\n" \
"15039 2019-02-15 07:31:22 +0100 guiframe: save remark before shutdown command\n" \
"15038 2019-02-15 07:18:21 +0100 operatordlg: tanslation for label length added\n" \
"15037 2019-02-14 20:11:30 +0100 guiframe: first signal to take over the remark before save\n" \
"15036 2019-02-14 18:03:30 +0100 mbus: ascii fix read string\n" \
"15035 2019-02-14 15:16:42 +0100 mbus: ascii protocol corrections\n" \
"15034 2019-02-14 13:14:51 +0100 mbusdlg: enable host:port field dependent on selected sub library\n" \
"15033 2019-02-14 09:54:13 +0100 mbusdlg: corrected sublib description\n" \
"15032 2019-02-14 09:08:58 +0100 mbus: Lawicel ASCII format added as sublib\n" \
"15031 2019-02-13 14:59:30 +0100 translation merge: zh_CN, cs, en_GB, fr, de, nl\n" \
"15030 2019-02-13 14:35:04 +0100 radius added to loc,block,car,route,operator\n" \
"15029 2019-02-13 09:37:20 +0100 action: add all time&date vars before calling an xmlscript\n" \
"15028 2019-02-13 08:33:42 +0100 xmlscriptdlg: fix for wx 3.1\n" \
"15027 2019-02-12 18:39:06 +0100 notespanel: maxlength is not compat allowed for multiline text controls\n" \
"15026 2019-02-12 17:16:46 +0100 roclcdr,wait: do not reset block triggers in the wait phase\n" \
"15025 2019-02-12 14:39:53 +0100 notespanel: set max text length to 4000\n" \
"15024 2019-02-12 13:13:48 +0100 notespanel: max text set to length 2000\n" \
"15023 2019-02-12 08:21:23 +0100 planpropsdlg: save size and position guiframe: notes tab option\n" \
"15022 2019-02-11 19:48:33 +0100 bidib: revert MSG_BM_ADDRESS null as none occupied\n" \
"15021 2019-02-11 19:12:17 +0100 notespanel: copy/paste fixes\n" \
"15020 2019-02-11 17:38:17 +0100 item: only activate the accelleration table on focus in edit mode\n" \
"15019 2019-02-11 17:24:30 +0100 item: removed cursor accellerators\n" \
"15018 2019-02-11 16:52:44 +0100 rocview: notes panel added\n" \
"15017 2019-02-11 12:24:18 +0100 guiframe: update plan title on planproperties dialog OK\n" \
"15016 2019-02-11 11:22:23 +0100 block: ignore embedded sensor events in case of crossing\n" \
"15015 2019-02-10 19:34:28 +0100 rocview: fixed deprecateed calls\n" \
"15014 2019-02-10 18:00:55 +0100 rocview makefile: removed deprecated flag -> todo: fix them\n" \
"15013 2019-02-10 17:21:21 +0100 freeonenter: maxlen added\n" \
"15012 2019-02-10 15:07:31 +0100 bidib: regard free in case at MSG_BM_ADDRESS no longer an address is detected\n" \
"15011 2019-02-10 14:12:30 +0100 planpropsdlg: translations\n" \
"15010 2019-02-10 13:46:09 +0100 planpropsdlg: show and edit plan properties like title and remark\n" \
"15009 2019-02-09 16:25:51 +0100 mbus: fix for programming decoder protocol\n" \
"15008 2019-02-09 09:08:30 +0100 keyboarddlg: fix for expand\n" \
"15007 2019-02-09 08:36:55 +0100 keyboarddlg: fix for wx3.2\n" \
"15006 2019-02-09 08:36:05 +0100 model: remove tt,seltab,stage objects also from the block list after a remove\n" \
"15005 2019-02-08 18:13:55 +0100 model: use output list for finding an output by address instead of the not thread save map.next\n" \
"15004 2019-02-08 15:53:45 +0100 save clock freeze/go state\n" \
"15003 2019-02-08 11:47:25 +0100 action: clock go/freeze added\n" \
"15002 2019-02-08 11:02:19 +0100 auipanel: remove panel also from list in case of a remove to avoid crash on invalid pointer\n" \
"15001 2019-02-07 15:33:33 +0100 caption perspective menuitem added\n" \
"15000 2019-02-07 11:26:03 +0100 delete complete level incl. all items without broadcasting dependency modifications\n" \
"14999 2019-02-07 09:24:37 +0100 bidib: evaluate MSG_LC_STAT not as accessory event\n" \
"14998 2019-02-07 09:16:13 +0100 guiframe: not change label/grid back ground in case of invisible locos and cell background color off\n" \
"14997 2019-02-06 16:22:32 +0100 planpaneldlg: scale and rotation added\n" \
"14996 2019-02-06 14:42:39 +0100 auipanel: calculate panel size in case the panels is not yet initialised\n" \
"14995 2019-02-06 12:55:26 +0100 item: fix for rotate offset in AUI view\n" \
"14994 2019-02-06 12:03:19 +0100 guiframe: disable show caption becaus it does not work\n" \
"14993 2019-02-06 11:38:22 +0100 auipanel: show on/off panel caption (seems not to work...)\n" \
"14992 2019-02-05 14:07:35 +0100 loc,in: swap timer\n" \
"14991 2019-02-05 12:16:29 +0100 planpanel: regard item size on scale to fit\n" \
"14990 2019-02-05 11:50:01 +0100 loc,model: swap placing timer only for terminal stations\n" \
"14989 2019-02-05 11:38:22 +0100 loconet: evaluate sound 2 packet\n" \
"14988 2019-02-05 08:23:28 +0100 loconet: ibcom f9-f12\n" \
"14987 2019-02-04 14:14:07 +0100 rocview: option grid cell background color\n" \
"14986 2019-02-04 11:49:14 +0100 fback: broadcast a model modify command in case the sensor is invisible\n" \
"14985 2019-02-04 10:40:33 +0100 guiframe: grid cell background color disabled\n" \
"14984 2019-02-03 19:46:06 +0100 feedbackdialog: corrected threshold range and allow zero\n" \
"14983 2019-02-03 18:25:33 +0100 tt: revert sensors events for register values off\n" \
"14982 2019-02-03 14:23:37 +0100 embedded block: support for D&D sensor bus and address\n" \
"14981 2019-02-03 11:57:30 +0100 mgbox: update for link88 (Bert)\n" \
"14980 2019-02-03 10:33:53 +0100 mbusdlg: tcp port field added\n" \
"14979 2019-02-03 09:27:39 +0100 canservice: tcp binary\n" \
"14978 2019-02-03 08:50:29 +0100 mbus: tcp binary instead of ASCII\n" \
"14977 2019-02-02 13:22:08 +0100 trackpickerdlg: set cell background to white; same as SVG background\n" \
"14976 2019-02-02 12:18:09 +0100 exportutils: cs2 level support\n" \
"14975 2019-02-02 11:36:16 +0100 block: check if the embedded fb is operable in case of a simulate command\n" \
"14974 2019-02-02 07:42:53 +0100 lcdriver: casting corrections and comment correction for generating IN in case of enter\n" \
"14973 2019-02-01 16:34:13 +0100 rocdigs makefile: work around for win32 linker\n" \
"14972 2019-02-01 12:29:08 +0100 exportutils: CS2 layout (WIP)\n" \
"14971 2019-02-01 08:40:25 +0100 lcdriver: generate an IN event for the previous block in case it was missing at enter\n" \
"14970 2019-02-01 08:31:55 +0100 signaldialog: pattern label correction\n" \
"14969 2019-01-31 19:16:57 +0100 tt: process sensor events in case off and regval != -1\n" \
"14968 2019-01-31 17:22:15 +0100 exportutils: more CS2 info functions (WIP)\n" \
"14967 2019-01-31 15:23:42 +0100 mbus: use export utilities\n" \
"14966 2019-01-31 13:40:40 +0100 mbus: dummy: only echo frames with commands < ping\n" \
"14965 2019-01-31 12:14:21 +0100 translation merge: fr, en_GB, sr_latin, de, sl, cs, nl\n" \
"14964 2019-01-31 12:03:06 +0100 block: crash fix for create an fbblock\n" \
"14963 2019-01-31 11:46:42 +0100 mbusdlg: option msapp added\n" \
"14962 2019-01-31 11:33:00 +0100 exportutils: provide always 16 (dummy) function entries\n" \
"14961 2019-01-31 10:00:41 +0100 block: init embeded sensor state at startup\n" \
"14960 2019-01-31 09:02:18 +0100 exportutils added\n" \
"14959 2019-01-31 08:19:15 +0100 cs2utils: impossible dependency\n" \
"14958 2019-01-30 21:52:16 +0100 renderer: regard fill none as transparent (Lothar)\n" \
"14957 2019-01-30 17:13:18 +0100 model: check decoder string for NULL value before adding it\n" \
"14956 2019-01-30 14:04:33 +0100 block: update embedded sensor state at init\n" \
"14955 2019-01-30 12:58:07 +0100 hclient: use utility to serialize loco to CS2 format\n" \
"14954 2019-01-30 11:12:23 +0100 block: convert to embedded only for enter/in events, set sensors to 0,0,0\n" \
"14953 2019-01-30 10:36:33 +0100 item: regard the show flag in case of check for overlapping\n" \
"14952 2019-01-30 09:41:51 +0100 mbus: MSApp support (WIP)\n" \
"14951 2019-01-29 19:17:39 +0100 mbus: MSApp support (WIP)\n" \
"14950 2019-01-29 16:39:28 +0100 block: convert to embedded sensor block\n" \
"14949 2019-01-29 11:49:45 +0100 item: regard the simulate sensor flag in blocks with embeded sensors\n" \
"14948 2019-01-29 09:40:39 +0100 tt: process sensor events in case off and regval != -1\n" \
"14947 2019-01-28 21:18:31 +0100 guiapp,guiframe: fix for update stage block\n" \
"14946 2019-01-28 20:51:27 +0100 bidib: fix for single coil switch command in case of flat model\n" \
"14945 2019-01-28 19:14:39 +0100 bidib: do no longer regard MSG_LC_STAT as sensor report\n" \
"14944 2019-01-28 11:15:50 +0100 meter: fix for default background color\n" \
"14943 2019-01-27 18:10:06 +0100 guiframe: use base colors for cell backgroud\n" \
"14942 2019-01-27 17:26:19 +0100 correction user theme acc43 (Lothar)\n" \
"14941 2019-01-27 17:06:30 +0100 base: darkblue improvement\n" \
"14940 2019-01-27 15:52:28 +0100 rocview wrapper: default no mono space font\n" \
"14939 2019-01-27 15:50:58 +0100 rocguiinidlg: fix for graybackground option\n" \
"14938 2019-01-27 15:34:03 +0100 rocview: support for dark theme\n" \
"14937 2019-01-27 13:33:26 +0100 bidib: trace corrections for outputs\n" \
"14936 2019-01-27 13:32:51 +0100 user theme: new symbols\n" \
"14935 2019-01-27 08:09:10 +0100 ledbutton: pushbutton support\n" \
"14934 2019-01-27 07:36:29 +0100 mbus: msapp support (WIP)\n" \
"14933 2019-01-27 07:35:54 +0100 usertheme SLIM added for crossing and ccrossing\n" \
"14932 2019-01-26 18:11:17 +0100 control: command mapping output support added\n" \
"14931 2019-01-26 17:07:01 +0100 item: show goto TT track menu only if there are tracks defined\n" \
"14930 2019-01-26 08:28:32 +0100 function pushbutton support\n" \
"14929 2019-01-26 08:23:59 +0100 function pushbutton support\n" \
"14928 2019-01-25 19:14:15 +0100 guiframe: allow discor only if the automatic mode is off\n" \
"14927 2019-01-25 17:19:49 +0100 locdialog: code is iconnr\n" \
"14926 2019-01-25 16:24:17 +0100 mbus: fix for reading function CVs\n" \
"14925 2019-01-25 16:08:00 +0100 mbus: get functions from the discoverred loco\n" \
"14924 2019-01-25 11:40:13 +0100 rocview: save and restore throttles WS dependent\n" \
"14923 2019-01-24 17:27:33 +0100 loconet: revert extra bit for IB II and F9-F11\n" \
"14922 2019-01-24 14:21:34 +0100 mbus: reset discovery vars after verify\n" \
"14921 2019-01-24 14:18:54 +0100 renderer: support for ccrossing-t\n" \
"14920 2019-01-24 13:05:12 +0100 save loco throttles in the workspace node\n" \
"14919 2019-01-24 11:52:48 +0100 mbus: discovery (WIP)\n" \
"14918 2019-01-24 09:16:54 +0100 control: semi realtime option added\n" \
"14917 2019-01-24 08:07:00 +0100 mbus: fast clock added\n" \
"14916 2019-01-23 19:25:35 +0100 loc: fix for swap timer\n" \
"14915 2019-01-23 19:09:34 +0100 mbusdlg: readonly option added, layout changes\n" \
"14914 2019-01-23 18:37:21 +0100 lcdriver: fix for ghost in virtual slave blocks\n" \
"14913 2019-01-23 16:48:11 +0100 planpanel: fix for module oversizing\n" \
"14912 2019-01-23 15:00:22 +0100 mbus: discovery (WIP)\n" \
"14911 2019-01-23 13:14:30 +0100 loconet: extra bit for F9-F11 in case of IB II\n" \
"14910 2019-01-22 19:24:22 +0100 mbus: fix for activating the standalone trackbox\n" \
"14909 2019-01-22 18:05:55 +0100 mbus: discovery/bind (WIP)\n" \
"14908 2019-01-22 13:27:00 +0100 function type changed in code\n" \
"14907 2019-01-22 09:19:57 +0100 mbus: set locoinfo vmax according the digital protocol\n" \
"14906 2019-01-22 08:25:06 +0100 mbus: function type added\n" \
"14905 2019-01-22 08:21:16 +0100 mgbox: fixes (Bert)\n" \
"14904 2019-01-21 18:20:36 +0100 mbus: send loco info to the MS\n" \
"14903 2019-01-21 17:01:58 +0100 mbus: provide loco IDs dynamically for the MS\n" \
"14902 2019-01-21 14:11:13 +0100 mbus: fix for interpreting emergancy break\n" \
"14901 2019-01-21 11:25:28 +0100 locodialog: fix for assert on set null color\n" \
"14900 2019-01-21 08:25:17 +0100 z21: send unconditional speed commands\n" \
"14899 2019-01-21 07:55:48 +0100 locoio: allow 16 servos\n" \
"14898 2019-01-20 21:58:41 +0100 serial: init default timeout values\n" \
"14897 2019-01-20 21:49:03 +0100 dsd2010: set serial timeout\n" \
"14896 2019-01-20 17:22:45 +0100 mbus: config data (WIP)\n" \
"14895 2019-01-20 09:08:33 +0100 xpressnet: swoffset option added\n" \
"14894 2019-01-20 08:30:10 +0100 loconetdlg: fix for options panel\n" \
"14893 2019-01-19 12:35:59 +0100 rocview controller dialog fixes for 3.2\n" \
"14892 2019-01-19 11:29:36 +0100 mbus: trace sensor ack\n" \
"14891 2019-01-19 10:34:16 +0100 mutex: save thread ID on lock\n" \
"14890 2019-01-19 08:46:47 +0100 model: finddestination lock timeout added\n" \
"14889 2019-01-18 20:21:30 +0100 rocview dialogs: make fit for wx 3.2\n" \
"14888 2019-01-18 19:26:37 +0100 rocview dialogs: make fit for wx 3.2 (WIP)\n" \
"14887 2019-01-18 17:02:44 +0100 rocprodlg: fix for expand flags\n" \
"14886 2019-01-18 15:50:43 +0100 mbus,gc2a: experimetal sensor ack\n" \
"14885 2019-01-18 15:42:04 +0100 renderer: fixed incompat function call 3.0 and 3.1\n" \
"14884 2019-01-18 13:54:45 +0100 hueconfdlg: deprecated events replaced\n" \
"14883 2019-01-18 13:49:13 +0100 rocnetnodedlg: deprecated events replaced\n" \
"14882 2019-01-18 13:23:31 +0100 blockdialog,hueconfdlg: deprecated set cell alignment replaced\n" \
"14881 2019-01-18 13:11:38 +0100 feedbackdialog: fix for deprecated cell alignment call\n" \
"14880 2019-01-18 12:52:30 +0100 rocrailinidialog: replaced deprecated cell alignment functions\n" \
"14879 2019-01-18 12:17:41 +0100 trackpickerdlg: correction for deprecated grid cell function\n" \
"14878 2019-01-18 12:07:49 +0100 rocview dialogs: removed deprecated wxADJUST_MINSIZE\n" \
"14877 2019-01-18 11:45:25 +0100 weatherdlg: fix for deprecated 3.1\n" \
"14876 2019-01-18 11:39:24 +0100 weatherdlg: fix for 3.1\n" \
"14875 2019-01-18 08:19:00 +0100 loconetdlg: fix for saving cmdstn\n" \
"14874 2019-01-17 12:50:50 +0100 mbus: usb fix for recovery\n" \
"14873 2019-01-17 09:24:56 +0100 renderer: comment corrections\n" \
"14872 2019-01-17 09:23:47 +0100 block: fix for unexpected enter flag in case of embeddedfb\n" \
"14871 2019-01-16 17:30:50 +0100 item: fix for take over the block enterside\n" \
"14870 2019-01-16 11:34:19 +0100 mbus,model: signal response\n" \
"14869 2019-01-16 09:18:18 +0100 mbus: use queue for accessory DCC/MM commands\n" \
"14868 2019-01-16 07:29:51 +0100 renderer: fix for determine the plus/min side of a fbblock\n" \
"14867 2019-01-16 07:10:50 +0100 block: reset embeddedfb flags on inst\n" \
"14866 2019-01-15 18:07:57 +0100 typo fix: embeddedfb\n" \
"14865 2019-01-15 17:37:21 +0100 rnnmqtt: extra parameter added\n" \
"14864 2019-01-15 17:14:15 +0100 typo fix: embeddedfb\n" \
"14863 2019-01-15 14:54:39 +0100 clntcon,mqtt: switch dispatching by topic\n" \
"14862 2019-01-15 13:17:09 +0100 mbus: signal aspect support added incl. blink\n" \
"14861 2019-01-15 12:09:30 +0100 analyse: regard embedded sensors in block at cleanup (Lothar)\n" \
"14860 2019-01-15 10:45:56 +0100 item: fix for D&D and first embedded sensor click\n" \
"14859 2019-01-15 10:45:28 +0100 block: fix for enter flag in case of embedded sensors and auto off\n" \
"14858 2019-01-15 09:19:41 +0100 renderer: fix for vertical embeded sensors (wrong side)\n" \
"14857 2019-01-15 08:23:05 +0100 block: stop loop in case the sensor was found to be deleted\n" \
"14856 2019-01-15 08:15:49 +0100 block: delete generated sensors on remove in case of embedded sensors\n" \
"14855 2019-01-15 07:34:55 +0100 translation merge: nl, fr, de, cs, en_GB, sl, sv\n" \
"14854 2019-01-14 17:24:09 +0100 renderer: embedded sensor corrections\n" \
"14853 2019-01-14 15:33:36 +0100 mbus: fix for evaluating programming response addresses\n" \
"14852 2019-01-14 15:25:59 +0100 mbus: fix for accessory commands\n" \
"14851 2019-01-14 10:14:07 +0100 item,renderer: use only parts of the block for simulating embedded sensors\n" \
"14850 2019-01-13 15:54:31 +0100 renderer: adjust block sensor position in case of theme SLIM\n" \
"14849 2019-01-13 15:35:50 +0100 item: use alt+shift to simulate embeded sensors\n" \
"14848 2019-01-13 14:18:52 +0100 block: embededfb IDs changed to %bkid%+/-\n" \
"14847 2019-01-13 12:25:41 +0100 block,item,planpanel: update item recursive on modify\n" \
"14846 2019-01-12 22:10:28 +0100 trackpicker: block embededfb symbol\n" \
"14845 2019-01-12 18:50:40 +0100 trackepickerdlg: block with embeded sensors added\n" \
"14844 2019-01-12 18:02:48 +0100 block: embededfb (WIP)\n" \
"14843 2019-01-12 11:44:36 +0100 powerctrldlg: fix for evaluating the history value\n" \
"14842 2019-01-12 09:31:46 +0100 bidib: report absent option added\n" \
"14841 2019-01-12 08:29:22 +0100 block: fix for arrival pending in case it was already locked by the same loco\n" \
"14840 2019-01-12 08:16:45 +0100 roads: dirall removed\n" \
"14839 2019-01-12 08:05:26 +0100 dirall removed and replaced with straight\n" \
"14838 2019-01-11 22:04:34 +0100 z21,mbus: voltmin added\n" \
"14837 2019-01-11 21:43:15 +0100 z21,mbus: provide load and temp. max values\n" \
"14836 2019-01-11 19:55:35 +0100 bidib: only set absent flag in case the power is on\n" \
"14835 2019-01-11 14:53:45 +0100 model: fix for function count for guest locos\n" \
"14834 2019-01-11 14:30:12 +0100 wmz21: trace correction\n" \
"14833 2019-01-11 11:07:09 +0100 mbus: set sensor value\n" \
"14832 2019-01-11 08:59:37 +0100 bidibidentdlg: label UniqueID extended\n" \
"14831 2019-01-10 17:02:32 +0100 block: do not reset loco current block in case the block is not its current block\n" \
"14830 2019-01-10 14:17:55 +0100 canservice: tcpip support\n" \
"14829 2019-01-10 14:13:15 +0100 canservice: tcpip support (wip)\n" \
"14828 2019-01-10 12:15:06 +0100 makefile,mkdeb.sh: canservice added in case platform is linux\n" \
"14827 2019-01-10 12:06:13 +0100 canservice: trace level corrections\n" \
"14826 2019-01-10 12:01:53 +0100 canservice: client list\n" \
"14825 2019-01-10 11:43:18 +0100 ucan: trace extended for write failed\n" \
"14824 2019-01-10 11:21:13 +0100 canservice: UDP read trace\n" \
"14823 2019-01-10 11:14:39 +0100 canservice: UDP support\n" \
"14822 2019-01-10 10:38:56 +0100 mbus: sleep fix in usb sublib, discovery console command\n" \
"14821 2019-01-09 20:14:08 +0100 block: revert acceptident\n" \
"14820 2019-01-09 14:21:37 +0100 mbus: udp communication added (not tested)\n" \
"14819 2019-01-09 13:22:34 +0100 mbus utils: trace correction and standard frame support added\n" \
"14818 2019-01-09 13:15:00 +0100 rocnetnode: fix for extended ASCII frame\n" \
"14817 2019-01-09 13:05:45 +0100 rocnetnode: fix for creating an extended ASCII frame\n" \
"14816 2019-01-09 12:21:11 +0100 mbusdlg: fix for evaluating sublib\n" \
"14815 2019-01-09 12:20:36 +0100 mbus,rocnetnode: tcp support\n" \
"14814 2019-01-09 11:54:44 +0100 mbus: tcpip support added\n" \
"14813 2019-01-09 11:36:56 +0100 mbusdlg: layout correction\n" \
"14812 2019-01-09 11:36:24 +0100 mbus,rocnetnode: fix for utils\n" \
"14811 2019-01-09 10:15:48 +0100 mbus: purgetime correction -> seconds instead of minutes\n" \
"14810 2019-01-09 09:50:00 +0100 mbus: purge only in case purgetime > 0\n" \
"14809 2019-01-09 09:48:25 +0100 mbusdlg: purgetime option added\n" \
"14808 2019-01-09 09:17:53 +0100 mbus: purge function added rcan: removed\n" \
"14807 2019-01-08 17:18:26 +0100 mbus: set DCC sub protocol on first access\n" \
"14806 2019-01-08 15:52:11 +0100 block: only generate timed events in case arrival is pending\n" \
"14805 2019-01-08 12:06:21 +0100 z21: new function for getting type and firmware\n" \
"14804 2019-01-08 08:46:03 +0100 mbus: statereport option, usb auto reconnect\n" \
"14803 2019-01-07 20:14:01 +0100 mbus: report A,V,T only in case of a change\n" \
"14802 2019-01-07 19:48:50 +0100 mbus: poll A,V,C\n" \
"14801 2019-01-07 12:09:10 +0100 powerman,z21: define the z21 as booster with addr 21\n" \
"14800 2019-01-07 09:14:25 +0100 switch: fix for unregister sensors\n" \
"14799 2019-01-06 16:01:49 +0100 block: reset accept ident flag on startup\n" \
"14798 2019-01-06 14:53:52 +0100 routedialog: wiring layout fix\n" \
"14797 2019-01-06 13:40:29 +0100 guiframe: crash fix\n" \
"14796 2019-01-06 12:26:53 +0100 wmz21: trace level correction\n" \
"14795 2019-01-06 09:11:14 +0100 loc: fix for matching ident code\n" \
"14794 2019-01-05 16:51:35 +0100 system: trace correction in execRunner\n" \
"14793 2019-01-05 16:22:52 +0100 guiframe: support for stage block type added in case of loco event\n" \
"14792 2019-01-05 14:45:56 +0100 rocnetnode: mbus support WIP\n" \
"14791 2019-01-04 19:34:55 +0100 dccpp: update\n" \
"14790 2019-01-04 09:46:08 +0100 mbus: trace fix\n" \
"14789 2019-01-04 09:19:15 +0100 rocview: renamed MGV in GCA\n" \
"14788 2019-01-03 14:01:40 +0100 canservice: wip\n" \
"14787 2019-01-03 12:21:03 +0100 mbus: cc added for testing\n" \
"14786 2019-01-02 20:26:15 +0100 auipanel: support for zoom2fit in case of 90,270 rotation\n" \
"14785 2019-01-02 13:31:27 +0100 throttledlg: removed stay on top\n" \
"14784 2019-01-02 12:19:59 +0100 mbus: send poll command at SoD to get events from Gleisreporter*\n" \
"14783 2019-01-01 21:56:33 +0100 throttle too narrow...\n" \
"14782 2019-01-01 19:02:15 +0100 guiframe: fix for zoo2fit in case the plan is rotated 90 or 270\n" \
"14781 2019-01-01 17:47:30 +0100 tt: set accessory flag for ttdec switch commands (bidib)\n" \
"14780 2019-01-01 16:45:29 +0100 mbus: trace level corrections\n" \
"14779 2019-01-01 11:40:06 +0100 mbus: PT fix\n" \
"14778 2019-01-01 11:07:30 +0100 mbus: report accessory command response\n" \
"14777 2019-01-01 10:28:27 +0100 mbus: fix for PT\n" \
"14776 2019-01-01 07:56:35 +0100 update 2019\n" \
"14775 2018-12-31 15:51:51 +0100 mbus: server type added\n" \
"14774 2018-12-31 15:35:09 +0100 cbusnodedlg: new types added\n" \
"14773 2018-12-31 15:25:02 +0100 mbus: respond to ping with a Pi01CAN on board\n" \
"14772 2018-12-31 14:58:55 +0100 signal: fix for swapped green/yellow addresses in case of 3 and 4 apect signals with default processing\n" \
"14771 2018-12-31 14:37:49 +0100 tt: add bus parameter for ttdec\n" \
"14770 2018-12-31 11:35:09 +0100 fix for desktop item\n" \
"14769 2018-12-31 09:46:03 +0100 mbus: accessorry config added\n" \
"14768 2018-12-30 18:09:38 +0100 mbusdlg: pt added\n" \
"14767 2018-12-30 17:55:25 +0100 mbus: pt relais support\n" \
"14766 2018-12-30 15:22:24 +0100 rocdigs: generation definitions update for mbus\n" \
"14765 2018-12-30 15:19:52 +0100 wiki plan: fixes\n" \
"14764 2018-12-30 15:06:12 +0100 operatordlg: open throttle with parent the Rocview frame instead of this dialog to avoid a crash after closing it\n" \
"14763 2018-12-30 15:02:33 +0100 basepanel: get rotation\n" \
"14762 2018-12-30 13:39:12 +0100 mbus: ebraek and pom added\n" \
"14761 2018-12-30 13:17:37 +0100 throttledlg: stay on top flag set\n" \
"14760 2018-12-30 09:15:24 +0100 mbusdlg: new dialog added for MBus\n" \
"14759 2018-12-29 19:52:52 +0100 rocview: compiler fixes for Ubuntu 12.04\n" \
"14758 2018-12-29 19:16:06 +0100 mttmfcc: fix for checking none bidi unit\n" \
"14757 2018-12-29 19:07:03 +0100 mbus: optional activate the mm2 mfx and dcc protocol\n" \
"14756 2018-12-29 17:47:24 +0100 mbus: ping added after activating for check for a trackbox\n" \
"14755 2018-12-29 17:31:58 +0100 mbus: activate trackbox\n" \
"14754 2018-12-29 15:49:34 +0100 mbus: set extended frame flag\n" \
"14753 2018-12-29 15:14:43 +0100 mbus: socketcan added\n" \
"14752 2018-12-29 13:23:34 +0100 mttmfcc: read tt confirmation byte\n" \
"14751 2018-12-29 10:58:14 +0100 rocview: AUI perspective support extended\n" \
"14750 2018-12-29 07:25:51 +0100 guiapp: no use the perspective xml file as plan file\n" \
"14749 2018-12-28 12:21:06 +0100 finddest: trace level change from warning to find for no destination found\n" \
"14748 2018-12-28 12:19:51 +0100 nmra-rp922: split up primary and extended addressing\n" \
"14747 2018-12-28 09:21:12 +0100 Rocrail.desktop: remove\n" \
"14746 2018-12-28 09:17:39 +0100 loc,block: set show flag if needed on identified locos\n" \
"14745 2018-12-27 20:23:57 +0100 ecosdlg: (mcs2) fix for preselecting 500000bps\n" \
"14744 2018-12-27 18:05:53 +0100 block: reactivated the exit event\n" \
"14743 2018-12-27 10:40:05 +0100 guiapp: command line option -persp added\n" \
"14742 2018-12-26 14:29:47 +0100 model: fix for corrupt map first/next at block init\n" \
"14741 2018-12-25 12:08:07 +0100 install: no longer provide a server icon\n" \
"14740 2018-12-25 09:04:36 +0100 lenzdlg: correction for double ID\n" \
"14739 2018-12-25 08:11:43 +0100 block: removed ignore reserved events plus/min\n" \
"14738 2018-12-24 15:53:03 +0100 xpressnet: option for roco functions > 12 which are not conform xpressnet\n" \
"14737 2018-12-24 10:23:23 +0100 xpressnet: function traces extended\n" \
"14736 2018-12-23 15:08:47 +0100 rocview: fixes for none utf8 file systems for reading svg and opening workspaces\n" \
"14735 2018-12-23 13:07:04 +0100 cmdln: REVERT convert windows latin command line args to utf8\n" \
"14734 2018-12-23 11:40:04 +0100 cmdln: convert windows latin command line args to utf8\n" \
"14733 2018-12-23 10:29:29 +0100 auipanel,planpanel: resize fix to disable scrollbars\n" \
"14732 2018-12-23 08:36:25 +0100 ecosdlg: moved the baudrate choice up\n" \
"14731 2018-12-22 15:07:03 +0100 wrapper,timedactions: fix for light control\n" \
"14730 2018-12-22 13:48:07 +0100 rocview: suffix tripple dot at menu items save/load perspective\n" \
"14729 2018-12-22 10:29:19 +0100 translation merge: de, en_GB, nl\n" \
"14728 2018-12-22 08:38:26 +0100 rocview: trace corrections\n" \
"14727 2018-12-21 15:00:54 +0100 actionsctrldlg: fix for condition type stage\n" \
"14726 2018-12-21 14:53:24 +0100 auipanel,planpanel: disable scrollbars in case of view mode AUI\n" \
"14725 2018-12-21 07:56:09 +0100 auipanel: modular layout support\n" \
"14724 2018-12-20 17:19:38 +0100 auipanel: trace level fix for panel events\n" \
"14723 2018-12-20 16:44:05 +0100 item: fix for update throttle initiated from block\n" \
"14722 2018-12-20 16:26:25 +0100 guiframe: inform AUI panel of loco events too\n" \
"14721 2018-12-20 12:25:33 +0100 auipanel: use a translated message in case of a zlevel title conflict\n" \
"14720 2018-12-20 12:05:09 +0100 auipanel: handle zlevel title conflicts\n" \
"14719 2018-12-20 10:24:21 +0100 auipanel: rescale panels induvidual, save scale in perspective\n" \
"14718 2018-12-20 08:23:16 +0100 renderer: bigger direction arrow\n" \
"14717 2018-12-20 08:03:03 +0100 cmdrecoder,trace: stay on top flag\n" \
"14716 2018-12-20 07:39:09 +0100 timedactions: revert stay on top flag\n" \
"14715 2018-12-19 14:08:55 +0100 timedactions: stay on top flag set\n" \
"14714 2018-12-19 11:02:57 +0100 rocview: save/load AUI perspective incl. rotation\n" \
"14713 2018-12-19 08:11:09 +0100 auipanel: panel name is needed for restoring the perspective\n" \
"14712 2018-12-18 21:07:09 +0100 auipanel: use a generated name to avoid conflicts\n" \
"14711 2018-12-18 15:58:15 +0100 planpanel: rotate 0,90,180,270\n" \
"14710 2018-12-18 12:24:57 +0100 block: reset closereq flag in case of open state command\n" \
"14709 2018-12-18 10:26:12 +0100 block: reservedevents plus/min option added\n" \
"14708 2018-12-18 08:57:59 +0100 guiframe: crash fix fo deleting and creating modpanel\n" \
"14707 2018-12-18 08:10:11 +0100 guiframe: crash fix fo deleting and creating modpanel\n" \
"14706 2018-12-17 15:34:06 +0100 planpanel: show rotate menuitem as check item so its visible if the panel is already rotated\n" \
"14705 2018-12-17 15:12:31 +0100 auipanel without notebook\n" \
"14704 2018-12-17 13:56:25 +0100 planpanel: popup menu iten flip changed in rotate\n" \
"14703 2018-12-17 12:20:25 +0100 guiframe: disable edit mode in case a panel is flipped\n" \
"14702 2018-12-17 11:38:12 +0100 translation merge: de, nl\n" \
"14701 2018-12-17 11:15:30 +0100 bidib: power flag correction for MSG_BM_CONFIDENCE\n" \
"14700 2018-12-17 11:02:56 +0100 planpanel: flip command added\n" \
"14699 2018-12-16 15:06:38 +0100 planpanel: auipanel support for looking up an ID\n" \
"14698 2018-12-16 14:03:48 +0100 auipanel: model update function added\n" \
"14697 2018-12-16 10:41:44 +0100 cbusnodedlg: fix for getting GC2 port stat, desc corrections for ArduCAN\n" \
"14696 2018-12-16 08:45:13 +0100 loc: check active state\n" \
"14695 2018-12-16 08:34:27 +0100 block: ignore events in case not reserved option added\n" \
"14694 2018-12-15 10:35:51 +0100 cbusnodedlg: fix for set gc2\n" \
"14693 2018-12-15 10:19:04 +0100 item: fix for invalid block text\n" \
"14692 2018-12-14 13:04:58 +0100 item: show number of cars in block symbols\n" \
"14691 2018-12-14 09:11:12 +0100 cbusnodedlg: fix for set gc2\n" \
"14690 2018-12-14 07:36:26 +0100 locdialog: standalone flag on consist tab\n" \
"14689 2018-12-13 15:25:57 +0100 cbusnodedlg: arducangbmc links to arducanir\n" \
"14688 2018-12-13 13:51:11 +0100 translation merge: sr_latin, sl,de, en_GB, nl\n" \
"14687 2018-12-13 11:48:30 +0100 rocnetnode: fix for crash after disconnecting client from CAN service\n" \
"14686 2018-12-13 11:43:00 +0100 added a line for the removal of MDI\n" \
"14685 2018-12-13 10:16:41 +0100 loc,timer: revert reset schedule in timer, reset schedule after arrive in the gotoblock\n" \
"14684 2018-12-13 09:17:48 +0100 timer: reset schedule in case the run flag is off\n" \
"14683 2018-12-13 07:53:48 +0100 throttledlg: open loco dialog on the consist tab\n" \
"14682 2018-12-12 14:46:25 +0100 throttledlg,loc: standalone command added\n" \
"14681 2018-12-12 12:06:56 +0100 throttledlg: show master/slave\n" \
"14680 2018-12-12 09:05:09 +0100 loc: fix for check2in modification\n" \
"14679 2018-12-11 10:00:12 +0100 actionsctrldlg: condition state reserved added for blocks\n" \
"14678 2018-12-11 08:31:27 +0100 guiframe: restore MDI and AUI flags on init plan notebook\n" \
"14677 2018-12-11 08:15:09 +0100 guiframe: switch back to modview in caase of a modular layout and remove the AUI panel if defined\n" \
"14676 2018-12-10 18:35:18 +0100 auipanel: fix for switching workspace\n" \
"14675 2018-12-10 15:44:44 +0100 item: throttle popup menu item added\n" \
"14674 2018-12-10 12:42:06 +0100 rocguiinidlg: layout corrections\n" \
"14673 2018-12-10 11:30:55 +0100 improvement of signal wait in case of lightsignals\n" \
"14672 2018-12-10 10:53:09 +0100 auipanel: set title to Track panels\n" \
"14671 2018-12-10 08:29:40 +0100 auipanel: save and restore perspective in the workspace node\n" \
"14670 2018-12-10 08:11:40 +0100 control: check if the issue directory is absolute before copy traces\n" \
"14669 2018-12-09 16:58:23 +0100 dtc: DinaSys support removed\n" \
"14668 2018-12-09 16:06:30 +0100 rocguiinidlg: removed the reset AUI perspective\n" \
"14667 2018-12-09 14:03:51 +0100 auipanel: reject perspective in case one or more level titles are not included\n" \
"14666 2018-12-09 13:11:53 +0100 sensorevents: fix for qos/stamp model: fix for reporting\n" \
"14665 2018-12-09 11:25:28 +0100 auipanel: added unsupported functions\n" \
"14664 2018-12-09 10:53:02 +0100 info.plist: 4k support\n" \
"14663 2018-12-09 10:22:59 +0100 rocview: reset AUI perspective checkbox\n" \
"14662 2018-12-09 07:52:41 +0100 virtual: trace level correction for release loco command\n" \
"14661 2018-12-08 20:09:04 +0100 auipanel: disable close button\n" \
"14660 2018-12-08 15:07:11 +0100 auipanel: further implementation of common function\n" \
"14659 2018-12-08 14:42:09 +0100 auipanel: save perspective, promote model events to all panels\n" \
"14658 2018-12-08 11:38:08 +0100 rocview make file: aui added\n" \
"14657 2018-12-08 10:58:10 +0100 rocview: optiona AUI view (WIP)\n" \
"14656 2018-12-08 08:11:21 +0100 model: crash fix incase looking up a car with id null\n" \
"14655 2018-12-08 07:46:15 +0100 z21: use always the same bus for loconet detectors and inputs\n" \
"14654 2018-12-07 13:53:25 +0100 locdialog: fix for set serviced\n" \
"14653 2018-12-06 19:53:17 +0100 model: fix for get real block\n" \
"14652 2018-12-06 11:38:10 +0100 model: ignore the loco direction at selecting a destination\n" \
"14651 2018-12-06 10:40:55 +0100 block: ignore event timers in case the loco is already IN\n" \
"14650 2018-12-06 09:06:16 +0100 mttmfcc: slx815 support -> bus value added\n" \
"14649 2018-12-05 14:35:47 +0100 loc: reset route sync on softreset\n" \
"14648 2018-12-05 10:25:28 +0100 themeprops: added missing evaluation of options\n" \
"14647 2018-12-05 07:57:02 +0100 mttmfcc: bus byte added for the tt command (copied from slx)\n" \
"14646 2018-12-04 10:51:48 +0100 block: fix for setting occupancy in case of state closed/open\n" \
"14645 2018-12-04 08:27:56 +0100 item,block: show platform name in blocks in case of free (commented out)\n" \
"14644 2018-12-03 10:15:59 +0100 mttmfcc: support for slx815\n" \
"14643 2018-12-03 08:01:05 +0100 control,switch: regard route switch time in case of save position is set\n" \
"14642 2018-12-02 18:41:39 +0100 bidib: removed the echo commands option\n" \
"14641 2018-12-02 17:23:10 +0100 control: copy the last trace in case the list of trace files is invalid\n" \
"14640 2018-12-02 15:53:51 +0100 control: trace level correction for switch mapping\n" \
"14639 2018-12-02 12:01:52 +0100 z12,loconet: block address + 1 in the new long multi sense\n" \
"14638 2018-12-02 11:08:07 +0100 loconet: multi sense long support\n" \
"14637 2018-12-02 10:51:28 +0100 z21: multi sense long support\n" \
"14636 2018-12-02 08:01:51 +0100 importdlg: use the find selection off the caller dialog\n" \
"14635 2018-12-01 16:02:41 +0100 rocview: keep original ID on import\n" \
"14634 2018-12-01 14:50:27 +0100 issue: keep original trace file timestamp\n" \
"14633 2018-12-01 13:26:36 +0100 importdlg: crash fix incase index is empty\n" \
"14632 2018-12-01 10:10:03 +0100 find added in output and action dialog\n" \
"14631 2018-12-01 08:04:10 +0100 control: trace level fix for switch mapping\n" \
"14630 2018-11-30 16:20:30 +0100 control: fix for double path separator\n" \
"14629 2018-11-30 15:16:24 +0100 control: removed extra prefix for trace file in case of issue listing\n" \
"14628 2018-11-30 14:48:52 +0100 control: trace added on copy issue trace files\n" \
"14627 2018-11-30 14:43:43 +0100 find in signal and schedule dialog\n" \
"14626 2018-11-30 11:58:28 +0100 regard find field content on refresh index\n" \
"14625 2018-11-30 11:47:15 +0100 importdlg: find textfield added\n" \
"14624 2018-11-30 09:27:46 +0100 translation merge: de, nl, en_GB, sl, sr_latin\n" \
"14623 2018-11-30 08:50:15 +0100 control: fix for init actions\n" \
"14622 2018-11-30 08:15:24 +0100 model: if no car is found by ID try it by address\n" \
"14621 2018-11-30 07:58:01 +0100 rocnet: generate decoder ID\n" \
"14620 2018-11-29 15:18:45 +0100 control: include all trace files in the issue\n" \
"14619 2018-11-29 13:27:02 +0100 cardlg: type dependent UIC combo content\n" \
"14618 2018-11-29 12:21:59 +0100 car: evaluate UIC in function isState\n" \
"14617 2018-11-29 12:14:42 +0100 cardlg: UIC combo with category letters\n" \
"14616 2018-11-29 11:54:44 +0100 car: UIC field added\n" \
"14615 2018-11-29 08:48:06 +0100 find in block,switch.feedback dialogs added\n" \
"14614 2018-11-29 08:27:41 +0100 routedialog: find text field added (test)\n" \
"14613 2018-11-28 15:52:07 +0100 z21: signal absent in case of multi sense\n" \
"14612 2018-11-28 10:37:44 +0100 rocview: option tool separators added\n" \
"14611 2018-11-28 10:14:25 +0100 cbus: revert checking delay at this level -> switch object\n" \
"14610 2018-11-28 10:06:58 +0100 switch: regard active delay in case of followup commands\n" \
"14609 2018-11-28 08:12:17 +0100 loconet, z21: type absend -> absent\n" \
"14608 2018-11-28 07:14:35 +0100 cbusnodedlg: ArdCAN GBM M+C added\n" \
"14607 2018-11-27 19:57:42 +0100 adding the missing rocweb-base.js\n" \
"14606 2018-11-27 15:04:05 +0100 cbus: reject switch commands regarding switch time\n" \
"14605 2018-11-27 10:32:37 +0100 loc: remove pending timed events on IN\n" \
"14604 2018-11-27 08:35:08 +0100 port range increased to 65535\n" \
"14603 2018-11-27 08:03:40 +0100 rocdigs: ignore wloc.release command in case it is not supported by the CS\n" \
"14602 2018-11-26 14:00:18 +0100 locdialog: use empty png to reset function icons under GTK\n" \
"14601 2018-11-26 10:13:41 +0100 switch: fix for processing occupancy/ctc events\n" \
"14600 2018-11-26 09:14:12 +0100 locdialog: work around for updating the function bitmaps\n" \
"14599 2018-11-26 08:28:56 +0100 locdialog: init function icon improvement; no effect...\n" \
"14598 2018-11-25 16:13:33 +0100 zimocan: only evaluate events, no acks, in case of port4\n" \
"14597 2018-11-25 15:25:01 +0100 zimocan: fix for evaluating port4 event\n" \
"14596 2018-11-25 15:08:22 +0100 loc: check for goto block in the route\n" \
"14595 2018-11-25 09:07:05 +0100 loc: trace correction in case of check consist\n" \
"14594 2018-11-25 08:52:12 +0100 loc: skip finder in case the goto block has a direct route\n" \
"14593 2018-11-25 08:05:16 +0100 loc: polarisation flag added\n" \
"14592 2018-11-24 11:55:01 +0100 model: traces added on resume all\n" \
"14591 2018-11-23 08:19:21 +0100 zoom to fit: use the selected tab\n" \
"14590 2018-11-23 08:11:44 +0100 zoomdlg: process enter in the combobox\n" \
"14589 2018-11-23 07:19:11 +0100 scalecombo check correction\n" \
"14588 2018-11-22 16:47:57 +0100 zoom range corrections\n" \
"14587 2018-11-22 16:43:40 +0100 zoom range corrections\n" \
"14586 2018-11-22 16:30:10 +0100 zoomdlg: max 400\n" \
"14585 2018-11-22 16:17:13 +0100 zoom max 400\n" \
"14584 2018-11-22 15:04:25 +0100 rocnetnode: sensor report over CAN\n" \
"14583 2018-11-22 12:31:22 +0100 guiframe: increase maxX and maxY in case of fit zoom\n" \
"14582 2018-11-22 11:18:44 +0100 rcan tcp: thread for (re)connect\n" \
"14581 2018-11-22 08:47:57 +0100 rocview: zoom color icons added\n" \
"14580 2018-11-22 08:16:28 +0100 translation merge: de, nl\n" \
"14579 2018-11-22 07:55:18 +0100 rocweb: fix for clicking on the FG text\n" \
"14578 2018-11-21 16:38:11 +0100 rocview: zoom2fit icon added\n" \
"14577 2018-11-21 16:09:56 +0100 rocview: zoomm to fit added\n" \
"14576 2018-11-21 14:23:48 +0100 rocweb: error fix in case of a function definition > 28\n" \
"14575 2018-11-21 10:46:12 +0100 rocnetnode: tcpip CAN connection added\n" \
"14574 2018-11-21 08:24:19 +0100 rcan: utils fix\n" \
"14573 2018-11-20 18:28:51 +0100 guiframe: show server work space in title bar\n" \
"14572 2018-11-15 16:55:58 +0100 block: reset the reserved flag on reset\n" \
"14571 2018-11-14 07:43:13 +0100 locdialog: increased function timers from 1000 to 10000\n" \
"14570 2018-11-13 10:07:11 +0100 translation merge: de, sr_latin, fr, sl, en_UK, nl\n" \
"14569 2018-11-11 08:18:38 +0100 wmz21: fix for reporting a switch event to the wlm\n" \
"14568 2018-11-10 15:27:44 +0100 model: reject objects without ID set\n" \
"14567 2018-11-10 14:08:17 +0100 rcan: wip freeze\n" \
"14566 2018-11-09 16:32:09 +0100 rocrail.xml: fix for feedback array\n" \
"14565 2018-11-09 11:37:06 +0100 rcan: wip\n" \
"14564 2018-11-08 10:44:48 +0100 rcan: wip\n" \
"14563 2018-11-07 16:38:01 +0100 guiframe: loco number added to the grid\n" \
"14562 2018-11-07 08:24:05 +0100 modify: added recursive flag\n" \
"14561 2018-11-07 07:46:46 +0100 timedactions: revert rip xmlscript path\n" \
"14560 2018-11-06 16:17:40 +0100 loc: revert removing all child nodes at modify if no new are available\n" \
"14559 2018-11-06 11:40:05 +0100 ucan: return can file descripter as -1 on error\n" \
"14558 2018-11-06 11:36:25 +0100 ucan: use close directly in the openCAN function\n" \
"14557 2018-11-06 11:34:41 +0100 ucan: check return codes at openCAN\n" \
"14556 2018-11-06 10:10:05 +0100 roclcdr: trace level corrections\n" \
"14555 2018-11-05 09:13:36 +0100 actionsctrldlg: use combobox for loco ID\n" \
"14554 2018-11-04 17:02:57 +0100 renderer: revert change for wx master branch\n" \
"14553 2018-11-04 16:50:43 +0100 rocview:fixes for the wx master branch\n" \
"14552 2018-11-04 15:53:11 +0100 rocnetnodedlg: fix for looking up the channel usage in case of none servo\n" \
"14551 2018-11-04 15:15:04 +0100 xmlscript: fix for condition text not equal\n" \
"14550 2018-11-04 13:08:28 +0100 rocnetnodedlg: fix for initializing usage\n" \
"14549 2018-11-04 10:49:50 +0100 cbus/utils: show junk/trace at reading ASCII Frames\n" \
"14548 2018-11-04 10:48:06 +0100 mbus: try to read the loks from MS2\n" \
"14547 2018-11-04 10:13:53 +0100 tamsmc: fix for inendbyte\n" \
"14546 2018-11-04 10:05:30 +0100 model: take over some extra attributes at generate loco\n" \
"14545 2018-11-04 07:52:48 +0100 SLIM-COLOR: fix for accessory 20 (Peter)\n" \
"14544 2018-11-03 19:04:47 +0100 output: support for command on,off,active added\n" \
"14543 2018-11-03 09:49:18 +0100 signaldialog: added show id option\n" \
"14542 2018-11-02 19:18:07 +0100 blockdialog: evaluate sensors also on apply to take over none selected\n" \
"14541 2018-11-02 15:45:47 +0100 model: moved sleep in timedoff thread to avoid loop with wrong number of sensors\n" \
"14540 2018-11-02 10:54:49 +0100 mbus: evaluate config date... not realy usable\n" \
"14539 2018-11-01 16:58:30 +0100 timedactions: rip path in case of a xml command\n" \
"14538 2018-11-01 15:08:39 +0100 mbus: inform Rocrail of a verified loco\n" \
"14537 2018-11-01 12:16:04 +0100 DB-Signals user theme added (Bernd)\n" \
"14536 2018-11-01 12:11:15 +0100 zimocan: commented out the nocheck udp option\n" \
"14535 2018-11-01 11:03:34 +0100 wsocket: copy fix\n" \
"14534 2018-11-01 10:11:35 +0100 SLIM-COLOR: button-5 complete\n" \
"14533 2018-11-01 09:23:02 +0100 usocket: copy error fixed\n" \
"14532 2018-11-01 09:19:11 +0100 socket function UDPNoCeck added\n" \
"14531 2018-11-01 09:03:52 +0100 SLIM-COLOR: bunnton-5\n" \
"14530 2018-11-01 08:16:42 +0100 dccpp: regard swap gates on report\n" \
"14529 2018-11-01 08:06:17 +0100 SLIM: button 5 occ and route\n" \
"14528 2018-10-31 08:17:05 +0100 SLIM-COLOR: button 5 added for outputs as decoupler\n" \
"14527 2018-10-30 10:52:39 +0100 planpanel: fix for moving an item multiple times\n" \
"14526 2018-10-30 08:16:14 +0100 mbus: report state mcs2: trace fix z21: trace fix\n" \
"14525 2018-10-29 14:58:36 +0100 dccpp: echo only for Arduino pin I/O\n" \
"14524 2018-10-29 14:39:37 +0100 makefile: removes -ansi option\n" \
"14523 2018-10-29 14:31:01 +0100 loc: replace cpp comments with c\n" \
"14522 2018-10-29 10:45:46 +0100 mbus: loco/function support added\n" \
"14521 2018-10-29 10:05:01 +0100 loc: fix for restore gotoblock\n" \
"14520 2018-10-28 18:12:41 +0100 trackdialog: fix for setting blockid and sensorid\n" \
"14519 2018-10-28 15:51:08 +0100 mbus: wip\n" \
"14518 2018-10-28 13:46:19 +0100 loc: save gotoblock on exit and restore on startup\n" \
"14517 2018-10-27 14:02:12 +0200 hue: leave out sat attribute in case of use xy\n" \
"14516 2018-10-27 11:27:41 +0200 zimocan: typo fixes\n" \
"14515 2018-10-26 15:09:47 +0200 cbusnodedlg: fix for testing flat node outputs\n" \
"14514 2018-10-26 09:10:19 +0200 car: mdate field added\n" \
"14513 2018-10-26 08:16:43 +0200 block: save reserved flag at startup\n" \
"14512 2018-10-26 07:46:23 +0200 mbus: write config added\n" \
"14511 2018-10-25 10:53:19 +0200 rocview: option popup for device ping\n" \
"14510 2018-10-24 16:36:40 +0200 mbus: wip\n" \
"14509 2018-10-24 11:03:18 +0200 cbusnodedlg: version corrected\n" \
"14508 2018-10-24 11:02:23 +0200 mbus: wip\n" \
"14507 2018-10-23 15:34:01 +0200 mbus: ping support\n" \
"14506 2018-10-23 11:42:47 +0200 xmlscript: use tokenizer also for replace all vars\n" \
"14505 2018-10-22 15:54:44 +0200 mcs2,mgbox: fix for reading serial ASCII\n" \
"14504 2018-10-22 13:03:33 +0200 var: check by text values for ## to skip\n" \
"14503 2018-10-22 11:05:31 +0200 mbus: accessory event support\n" \
"14502 2018-10-22 10:42:13 +0200 mbus: discovery response\n" \
"14501 2018-10-22 09:33:54 +0200 mbus: wip\n" \
"14500 2018-10-22 07:22:22 +0200 var: fix for negative numbers\n" \
"14499 2018-10-21 19:32:20 +0200 use (s)rand\n" \
"14498 2018-10-21 18:23:44 +0200 locdialog: revert placing checkbox\n" \
"14497 2018-10-21 17:09:49 +0200 mbus: wip\n" \
"14496 2018-10-21 10:52:00 +0200 wrapper: mbus added\n" \
"14495 2018-10-20 16:44:12 +0200 mbus library added for testing\n" \
"14494 2018-10-20 07:32:11 +0200 locdialog: fix for testing the consist locos\n" \
"14493 2018-10-19 14:43:04 +0200 demo images modified by Rainer\n" \
"14492 2018-10-19 11:53:14 +0200 wiki plan update\n" \
"14491 2018-10-19 11:43:43 +0200 check window/dialog position only in case of one monitor\n" \
"14490 2018-10-19 11:00:56 +0200 mdimanager: check if the tab xy position is on the current monitor size\n" \
"14489 2018-10-19 10:35:07 +0200 trainimage: invert invert\n" \
"14488 2018-10-19 09:03:16 +0200 translation merge: de, nl, sl, sr_latin\n" \
"14487 2018-10-18 15:17:58 +0200 xmlscript: light support added\n" \
"14486 2018-10-18 15:03:29 +0200 lightctrl action support added\n" \
"14485 2018-10-18 13:10:31 +0200 umsetzen -> run around\n" \
"14484 2018-10-18 13:08:08 +0200 translation for 'Umsetzen'\n" \
"14483 2018-10-18 10:41:24 +0200 fback: fix for initial inverted state\n" \
"14482 2018-10-18 10:17:35 +0200 loc: activate manual mode also if the loco runs\n" \
"14481 2018-10-18 08:31:31 +0200 loc: fix for BBT key in case of terminal station\n" \
"14480 2018-10-17 14:57:36 +0200 messages: keep 'swap loco image', no better translation\n" \
"14479 2018-10-17 14:56:50 +0200 var: fix for negative values\n" \
"14478 2018-10-17 11:44:15 +0200 car/trainpanel: reset train image on init\n" \
"14477 2018-10-17 10:34:45 +0200 rn-cbus: translate rn to cbus and visa versa (wip)\n" \
"14476 2018-10-17 07:47:33 +0200 check if mdi and dialog position is not negative\n" \
"14475 2018-10-16 10:57:12 +0200 carpanel: regard the car show flag at init\n" \
"14474 2018-10-16 10:48:54 +0200 trainimage: fix for init\n" \
"14473 2018-10-16 10:30:00 +0200 trainimage: option swap loco added\n" \
"14472 2018-10-15 16:17:05 +0200 guiframe: fix for save as\n" \
"14471 2018-10-15 09:58:32 +0200 loc: fix for BBT blockenterside in case of terminal station\n" \
"14470 2018-10-15 08:36:26 +0200 cbusnodedlg: fix for setall index and gc2 sync option\n" \
"14469 2018-10-15 07:39:56 +0200 cbus: simplified reading the dlc of an ASCII frame\n" \
"14468 2018-10-14 15:37:04 +0200 cbus: syncid added\n" \
"14467 2018-10-14 11:45:06 +0200 cbus: determine the dlc by detecting the ';'\n" \
"14466 2018-10-14 08:45:09 +0200 cbus: linefeed option added for the GC2a\n" \
"14465 2018-10-13 11:37:14 +0200 mgbox: crash fix\n" \
"14464 2018-10-13 08:56:13 +0200 popup cbusnodedlg after getting a opc_pnn\n" \
"14463 2018-10-13 07:58:52 +0200 cbus: canid range 1...255\n" \
"14462 2018-10-12 15:39:07 +0200 cbus: module type gc2a added\n" \
"14461 2018-10-12 15:20:35 +0200 cbus: hw version correction\n" \
"14460 2018-10-12 15:12:32 +0200 ucan: trace correction\n" \
"14459 2018-10-12 12:58:56 +0200 xmlscript: string not equal comparator - added\n" \
"14458 2018-10-12 11:18:29 +0200 cbusnodedlg: enable options for flat nodes\n" \
"14457 2018-10-12 11:16:58 +0200 routedialog: keep selected command after up/down\n" \
"14456 2018-10-12 07:55:38 +0200 cbus: removed bootloader support\n" \
"14455 2018-10-11 11:42:44 +0200 cbus tcp: fix for keep alive message\n" \
"14454 2018-10-10 17:20:12 +0200 xmlscript: fix for feedback state\n" \
"14453 2018-10-10 14:33:51 +0200 cbus: use long event OPCs in case the bus is set > 0\n" \
"14452 2018-10-10 10:18:49 +0200 cbusnodedlg: disable evtnr/evtaddr column in case of flat model\n" \
"14451 2018-10-10 08:41:09 +0200 cbus: regard the long OPCs\n" \
"14450 2018-10-10 07:42:25 +0200 cbus: alternative SoD nodenumber 0x8000\n" \
"14449 2018-10-09 13:17:12 +0200 xmlscript: fix for resolving the sensor command variables at start\n" \
"14448 2018-10-09 10:18:18 +0200 xmlscript: add output properties\n" \
"14447 2018-10-07 10:34:43 +0200 stage: ignore sensor events from already occupied sections\n" \
"14446 2018-10-07 09:50:48 +0200 lcdriver: reset pending location in case of loco reset\n" \
"14445 2018-10-06 15:16:37 +0200 cbus: fix for extended ASCII frames\n" \
"14444 2018-10-06 13:15:25 +0200 cbusnodedlg: firmware tab restored\n" \
"14443 2018-10-06 12:31:15 +0200 rascii: reset in buffer before reading\n" \
"14442 2018-10-06 08:22:57 +0200 changed netid from int to string\n" \
"14441 2018-10-05 16:01:15 +0200 translation merge: sr_latin, fr, sl, de, pt_BR nl, en_GB\n" \
"14440 2018-10-05 14:48:52 +0200 cbus: set CBUS protocol on generate\n" \
"14439 2018-10-05 14:46:26 +0200 model: publish found decoders to all clients, and remove generated at shutdown\n" \
"14438 2018-10-05 11:18:08 +0200 cbus: watch automatically the CAN modules\n" \
"14437 2018-10-05 10:33:04 +0200 output: svg type accessory option added\n" \
"14436 2018-10-05 08:49:45 +0200 cbus: invalidate session in case the CS goes offline\n" \
"14435 2018-10-04 13:08:28 +0200 cbus: revert -> CBUS only uses 7bit CAN-ID, 4 high bits for prio\n" \
"14434 2018-10-04 11:33:42 +0200 cbus: fix for CANID -> 8 bit instead of 7\n" \
"14433 2018-10-03 15:56:31 +0200 throttle: fix for macOS Mojave (wrong constructor for graphic context)\n" \
"14432 2018-10-03 14:37:41 +0200 guiframe: popup a confirmation in case of global soft reset\n" \
"14431 2018-10-03 12:22:33 +0200 translation corrections for reset auto mode\n" \
"14430 2018-10-03 10:38:53 +0200 loc: fix for setting loco in block after a reset all\n" \
"14429 2018-10-02 21:13:28 +0200 messages: replaced automat with auto mode\n" \
"14428 2018-10-02 10:42:31 +0200 guiframe,item: use own translation for locomotive (soft) reset\n" \
"14427 2018-10-01 19:26:25 +0200 rocrailinidialog: preselect virtual controller\n" \
"14426 2018-10-01 16:47:23 +0200 accdecdlg: enable programming in case of cbus protocol\n" \
"14425 2018-10-01 13:22:26 +0200 model: broadcast decoder state to the clients\n" \
"14424 2018-10-01 12:39:01 +0200 model: fix for decoder on/offline flag\n" \
"14423 2018-10-01 11:40:51 +0200 cbus: specify an emergency message with missing node numbers\n" \
"14422 2018-10-01 11:26:58 +0200 guiframe: show popup in case of an emergency\n" \
"14421 2018-10-01 11:14:38 +0200 cbus: report emergency state in case not all decoders are online\n" \
"14420 2018-10-01 08:28:35 +0200 model: rip occ.xml path in case its not found\n" \
"14419 2018-10-01 08:08:23 +0200 rocrail app: virtual command line option added\n" \
"14418 2018-09-30 17:21:02 +0200 cbus: use node number for checking the decoders\n" \
"14417 2018-09-30 17:04:48 +0200 cbusdlg: decoder check\n" \
"14416 2018-09-30 16:53:08 +0200 cbus: optional decoder check\n" \
"14415 2018-09-30 15:41:07 +0200 bidib: switch trace fix\n" \
"14414 2018-09-30 10:49:58 +0200 rocrailinidialog: fixed missing IID\n" \
"14413 2018-09-29 19:18:22 +0200 rocrailinidialog: command mapping bus unsigned\n" \
"14412 2018-09-28 14:29:07 +0200 bidib: fix for accessory error flag\n" \
"14411 2018-09-28 11:33:30 +0200 makewin64: use 2 make jobs instead of only one\n" \
"14410 2018-09-28 07:52:00 +0200 xmlscript: resolve all variables in the attributes\n" \
"14409 2018-09-28 07:34:06 +0200 main: shutdown needs a rc\n" \
"14408 2018-09-27 10:18:01 +0200 loc: broadcast loco length and weight too\n" \
"14407 2018-09-27 09:32:00 +0200 app: main returns 70 in case a shutdown command came from a client\n" \
"14406 2018-09-27 08:05:08 +0200 switch: support undef field state event in case of error\n" \
"14405 2018-09-27 07:30:51 +0200 virtual: show command in loco trace\n" \
"14404 2018-09-27 07:29:05 +0200 modbus: trace correction\n" \
"14403 2018-09-27 07:20:33 +0200 mgbox: ms2 enumeratie fix (Bert)\n" \
"14402 2018-09-26 09:20:00 +0200 bidibidentdlg: removed firmware update tab\n" \
"14401 2018-09-26 08:36:59 +0200 model: regard depart side in case of lookup schedule\n" \
"14400 2018-09-26 07:41:08 +0200 guiframe: update loco length and weight also at update loco list\n" \
"14399 2018-09-25 20:14:43 +0200 model: use the signal list for llokup a fitting event instead of the map\n" \
"14398 2018-09-25 08:23:10 +0200 cbus: trace corrections\n" \
"14397 2018-09-25 07:56:21 +0200 cbusdlg: only enable command ack in case of tcp/ip (GC1e)\n" \
"14396 2018-09-25 07:49:46 +0200 cbus: command ack only in case of the GC1e\n" \
"14395 2018-09-24 17:10:09 +0200 cbus,utils: fix for extended ASCII id\n" \
"14394 2018-09-24 17:06:32 +0200 cbus,utils: fix for parsing MERG CAN-ID\n" \
"14393 2018-09-24 12:27:20 +0200 cbusnodedlg: help index fix\n" \
"14392 2018-09-24 12:18:44 +0200 cbus: fix for initial loco command in case of session request\n" \
"14391 2018-09-24 11:45:39 +0200 cbusnodedlg: removed the firmware tab\n" \
"14390 2018-09-24 11:13:24 +0200 cbus: moved the ASCII layer down to the sub library level\n" \
"14389 2018-09-23 15:11:08 +0200 mgbox: serial fix (Bert)\n" \
"14388 2018-09-23 13:59:34 +0200 cbus: qCmd out size increased; too small for a CAN frame\n" \
"14387 2018-09-23 08:49:46 +0200 guiframe: show length,weight in the consist column in case > 0 and no train is assigned\n" \
"14386 2018-09-23 08:09:13 +0200 spl: LED type support\n" \
"14385 2018-09-23 07:37:15 +0200 spl: revert protocol extension\n" \
"14384 2018-09-22 17:02:30 +0200 spl: LED port type support for 32 bits\n" \
"14383 2018-09-22 15:01:16 +0200 cbus: changed internal communication from ASCII to binary\n" \
"14382 2018-09-21 15:05:29 +0200 cbus: can extended ID support\n" \
"14381 2018-09-21 11:51:54 +0200 cbus: can init CAN frame buffer\n" \
"14380 2018-09-21 11:15:23 +0200 cbus,rocs: CAN flags added\n" \
"14379 2018-09-20 10:46:03 +0200 cbus: can fix for available\n" \
"14378 2018-09-20 10:40:34 +0200 cbus: can wip\n" \
"14377 2018-09-20 10:18:10 +0200 cbus: can revert\n" \
"14376 2018-09-20 10:02:48 +0200 cbus: can use binary frame as CANGCA1\n" \
"14375 2018-09-20 09:03:02 +0200 cbus: can trace level correction\n" \
"14374 2018-09-19 13:07:12 +0200 cbus: can wip\n" \
"14373 2018-09-19 12:30:00 +0200 cbus: can wip\n" \
"14372 2018-09-19 12:11:10 +0200 cbus: can wip\n" \
"14371 2018-09-19 12:04:05 +0200 cbus: can wip\n" \
"14370 2018-09-19 11:56:33 +0200 cbus: can wip\n" \
"14369 2018-09-19 11:18:52 +0200 cbus: socketCAN wip\n" \
"14368 2018-09-19 10:40:25 +0200 ucan: WIP\n" \
"14367 2018-09-19 10:12:24 +0200 cbus: socketcan support (WIP)\n" \
"14366 2018-09-19 08:36:05 +0200 ucan: include added\n" \
"14365 2018-09-19 08:33:45 +0200 ucan: includes added\n" \
"14364 2018-09-19 08:29:19 +0200 rocs: can added\n" \
"14363 2018-09-19 07:32:45 +0200 crosscompile text update (copy from wiki)\n" \
"14362 2018-09-18 10:37:44 +0200 location: get loco list from stage blocks at init\n" \
"14361 2018-09-18 08:27:47 +0200 p50: sleep 50ms after CTS is active at protver 2\n" \
"14360 2018-09-18 08:21:05 +0200 mttmfcc: purge all at shutdown\n" \
"14359 2018-09-17 12:12:09 +0200 rocweb: update and base file preparation\n" \
"14358 2018-09-17 08:33:14 +0200 loc: release loco from stage in case of a moveto D&D command\n" \
"14357 2018-09-17 08:09:28 +0200 p50: protver 2 for sending a change direction without pefixing a V=0 command\n" \
"14356 2018-09-16 10:55:31 +0200 stage: set loco current block in case of D&D\n" \
"14355 2018-09-16 09:40:38 +0200 loc: SBT correction in case of a V_hint\n" \
"14354 2018-09-16 08:24:16 +0200 loc: SBT correction in case of a V_hint\n" \
"14353 2018-09-15 12:23:36 +0200 loc: ignore SBT and BAT incase of go manual (half automatic)\n" \
"14352 2018-09-15 08:29:28 +0200 switch: occupancy sensors not exclusive but shared\n" \
"14351 2018-09-14 18:17:31 +0200 routedialog: fix for init crossingblocksignals (Achim)\n" \
"14350 2018-09-14 15:51:15 +0200 renamedlg: clear IIDs from the comboboxes in case of other selection then IID\n" \
"14349 2018-09-14 14:52:46 +0200 massoth: trace loco command in case of getting a slot\n" \
"14348 2018-09-14 12:20:18 +0200 renamedlg: bus field added\n" \
"14347 2018-09-13 13:56:15 +0200 rocweb: bridge sensor/plus symbol corrections\n" \
"14346 2018-09-13 13:28:30 +0200 rocweb: tt sensor correction\n" \
"14345 2018-09-13 11:56:43 +0200 rocweb: SLIM option added for rendering the TT to fit to the SLIM theme\n" \
"14344 2018-09-13 11:05:08 +0200 translation merge: fr\n" \
"14343 2018-09-13 10:13:23 +0200 renderer: function for rotationg only one point for orientation\n" \
"14342 2018-09-13 10:04:25 +0200 rocweb: rotate tt gridcon point to match orientation\n" \
"14341 2018-09-13 08:40:29 +0200 rocweb: render turntable gridcon\n" \
"14340 2018-09-13 08:13:12 +0200 mttmfcc: purge only if all functions are off too\n" \
"14339 2018-09-12 15:32:13 +0200 renderer: simplify rotating\n" \
"14338 2018-09-12 15:10:04 +0200 renderer: simplification of roatating points\n" \
"14337 2018-09-12 09:58:41 +0200 renderer: rotate tt track with a sub function\n" \
"14336 2018-09-12 08:34:10 +0200 renderer: some fine tuning for calculating the tt bridge plus side\n" \
"14335 2018-09-12 08:12:39 +0200 renderer: use cos(-1) as pi\n" \
"14334 2018-09-12 07:53:53 +0200 renderer: fix for drawing the tt bridge plus side in case of graphicalcontext\n" \
"14333 2018-09-11 21:07:07 +0200 virtual: return lenz standard decoder ID for CV7 and 8\n" \
"14332 2018-09-11 15:03:19 +0200 xmlscriptdlg: save anyway option added in case xml is not well formed\n" \
"14331 2018-09-11 12:09:31 +0200 rocweb: show the tt bridge plus side\n" \
"14330 2018-09-11 10:17:09 +0200 mttmfcc: trash reply buffer in case > 256\n" \
"14329 2018-09-11 10:14:56 +0200 mttmfcc: exception in case numer of reply bytes are more then 256\n" \
"14328 2018-09-11 10:03:15 +0200 mttmfcc: function 17-32 implemented for firmware 1.13\n" \
"14327 2018-09-11 08:05:12 +0200 mttmfcc: purge option added\n" \
"14326 2018-09-10 14:46:54 +0200 renderer: show the tt bridge plus side as circle instead of a square\n" \
"14325 2018-09-10 08:21:47 +0200 route: trace level correction for setting crossing block signals\n" \
"14324 2018-09-10 08:02:38 +0200 routedialog: reactiveted the crossingblocksignals option\n" \
"14323 2018-09-09 10:53:01 +0200 rocweb: default tt bridgepos corrected\n" \
"14322 2018-09-09 10:43:59 +0200 rocweb: fix for missing tt symbol size\n" \
"14321 2018-09-09 10:09:26 +0200 mttmfcc: slot purger added\n" \
"14320 2018-09-09 08:59:04 +0200 translation merge: nl, de, sl\n" \
"14319 2018-09-08 15:34:44 +0200 block: inform loco in case of delete\n" \
"14318 2018-09-08 10:36:59 +0200 planpanel: prevent overwriting the add item node with mouse re-enter\n" \
"14317 2018-09-08 08:39:00 +0200 basedlg: check if the dialog is in a visible screen position to restore, if not the x,y will be reset to 10,10\n" \
"14316 2018-09-07 19:30:03 +0200 inno setup: theme paths corrections\n" \
"14315 2018-09-07 16:34:14 +0200 moved SpDrS60-extra to the user themes\n" \
"14314 2018-09-07 16:20:30 +0200 inno setup: typo correction\n" \
"14313 2018-09-07 15:58:57 +0200 inno setup: user themes option added\n" \
"14312 2018-09-07 15:55:56 +0200 userthemes\n" \
"14311 2018-09-07 14:34:03 +0200 themes: removed basic and iltis\n" \
"14310 2018-09-07 14:27:35 +0200 rocguiinidlg: layout correction\n" \
"14309 2018-09-07 10:58:32 +0200 trackpickerdlg: removed roads threeway\n" \
"14308 2018-09-07 10:38:16 +0200 renderer: disable show IDs for road symbols\n" \
"14307 2018-09-07 10:12:59 +0200 roads: added ccrossing and road/track option in the selectiondlg\n" \
"14306 2018-09-07 08:57:45 +0200 roclcdr: fix for reenter in case of half automatic mode\n" \
"14305 2018-09-07 08:11:06 +0200 trackpickerdlg: added road short block\n" \
"14304 2018-09-07 07:58:28 +0200 Roads: fixes for small block symbol and occ color\n" \
"14303 2018-09-07 07:49:56 +0200 rocrailinidialog: moved the 'half auto stop at in' to the loco section\n" \
"14302 2018-09-06 14:19:46 +0200 roads theme: clean up and make compatible with the standard theme\n" \
"14301 2018-09-06 11:47:29 +0200 rocguiinidlg: minimal toolbar icon size 24\n" \
"14300 2018-09-06 10:55:20 +0200 guiframe: toolbar icon size option added\n" \
"14299 2018-09-06 09:51:22 +0200 trackpicker: fix for road threeway and crossing\n" \
"14298 2018-09-06 08:02:27 +0200 renderer: use SLIM grey for the turntable tracks in case SLIM is active\n" \
"14297 2018-09-05 14:38:07 +0200 trainimage: only reinit the complete train image in case somethings has been changed\n" \
"14296 2018-09-05 12:18:57 +0200 cellrenderer: use default bitmap depth\n" \
"14295 2018-09-05 12:12:23 +0200 bidib: use bigger buffer sizes for reading\n" \
"14294 2018-09-05 11:25:00 +0200 block: replaced the stack sensor key[256] with a dynamic string to avoid stack overflow\n" \
"14293 2018-09-05 10:58:36 +0200 loc: take over the manual flag in case of modify\n" \
"14292 2018-09-05 10:34:38 +0200 block: fix for setting loco current block in case of reserved and not occupied\n" \
"14291 2018-09-05 08:51:22 +0200 rocpro: fix for triggering detect\n" \
"14290 2018-09-04 10:12:00 +0200 cellrenderer: use bitmap depth of 8 instead the default screen color depth\n" \
"14289 2018-09-04 08:20:12 +0200 rocprodlg: fix for setting the expected CV\n" \
"14288 2018-09-03 11:55:35 +0200 car: broadcast change of locality ion case of modify and the locality changed\n" \
"14287 2018-09-03 10:37:29 +0200 trackpickerdlg: rescale image cursor in case of Windows\n" \
"14286 2018-09-03 09:50:40 +0200 lcdriver: trace level correction\n" \
"14285 2018-09-03 09:50:07 +0200 trainimage: memory leak fix\n" \
"14284 2018-09-02 14:55:46 +0200 trackpickerdlg: no d&d in case of image cursor\n" \
"14283 2018-09-02 09:37:23 +0200 planpanel: image cursor (wip)\n" \
"14282 2018-09-01 14:49:01 +0200 rocguidialog: added option imagecursor\n" \
"14281 2018-09-01 11:17:15 +0200 imagecursor option (wip)\n" \
"14280 2018-09-01 08:18:20 +0200 translation merge: de, nl, sl, sr_latin\n" \
"14279 2018-09-01 07:35:44 +0200 trackpickerdlg: imagecursor\n" \
"14278 2018-08-31 10:37:19 +0200 in: trace level correction for an unexpected IN\n" \
"14277 2018-08-30 14:58:31 +0200 blockgroup: check the driver of the loco if a block is reserved as destination to prevent a ghost\n" \
"14276 2018-08-30 14:34:47 +0200 planpanel: test flag for drag symbol cursor, default false\n" \
"14275 2018-08-30 14:06:20 +0200 rocview: fix for demo workspace path trackpickerdlg: show selected symbol as cursor (wip)\n" \
"14274 2018-08-30 07:58:06 +0200 rocrailinidialog: moved unlock at ebreak to route\n" \
"14273 2018-08-29 11:42:35 +0200 events/in: check if the automatic is on before tracing an exception for IN\n" \
"14272 2018-08-29 11:36:54 +0200 fback: trace correction for bidi direction\n" \
"14271 2018-08-29 11:28:48 +0200 blockgroup: remove unused reservations\n" \
"14270 2018-08-29 10:14:45 +0200 route: synsgcmd option added\n" \
"14269 2018-08-29 07:50:24 +0200 guiframe: get pwd path for opening initial the workspace dialog\n" \
"14268 2018-08-28 15:01:44 +0200 translation merge: en_GB, sr_latin, sl, nl, de\n" \
"14267 2018-08-28 12:09:39 +0200 roclcdr: fix for schedule wait flag in case of second next block\n" \
"14266 2018-08-28 10:50:06 +0200 base: use no leading zeros in the generated symbol ID\n" \
"14265 2018-08-28 10:40:56 +0200 roclcdr: call loco reset in case of an (ghost) exception reset\n" \
"14264 2018-08-28 09:20:20 +0200 planpanel: symbol prefix added\n" \
"14263 2018-08-28 07:32:34 +0200 guiframe: openworkspace initial with '.'\n" \
"14262 2018-08-27 14:12:15 +0200 fback: removed double check regval on actions\n" \
"14261 2018-08-27 10:34:55 +0200 roclcdr: check if the event match for durrent oder destination block\n" \
"14260 2018-08-27 08:16:19 +0200 fback: write register value in a variable with reg_ prefix\n" \
"14259 2018-08-27 03:32:05 +0200 guiframe: open demo workspave menu item added\n" \
"14258 2018-08-27 03:20:34 +0200 wrapper: default no auto open workspace\n" \
"14257 2018-08-26 18:29:17 +0200 swim to the moon\n" \
"14256 2018-08-26 17:31:47 +0200 rocview: no default workspace\n" \
"14255 2018-08-26 15:09:52 +0200 turntable: show border option added\n" \
"14254 2018-08-26 11:08:39 +0200 item: fix for moving a text object\n" \
"14253 2018-08-26 10:02:55 +0200 roclcdr: evaluation fix for initialize blockgroup in case of second next block\n" \
"14252 2018-08-26 08:22:18 +0200 trainimage: request image from server in case not locally found\n" \
"14251 2018-08-26 07:33:58 +0200 inno setup: themespath corrected to user links\n" \
"14250 2018-08-25 16:41:38 +0200 inno setup: sym link to web\n" \
"14249 2018-08-25 15:34:04 +0200 inno setup: link to svg directory\n" \
"14248 2018-08-25 14:50:23 +0200 rocview: default workspace .\n" \
"14247 2018-08-25 12:21:38 +0200 roclcdr: redesigned blockgroup reservation and release management\n" \
"14246 2018-08-25 08:52:07 +0200 system: trace errno in case child process could not be started\n" \
"14245 2018-08-25 08:35:17 +0200 system: trace error on fail starting a child process guiapp: reduced the connect wait from 300 to 30\n" \
"14244 2018-08-24 14:03:08 +0200 rocnetnode: blank all LEDs with aspect value 0xFFFF\n" \
"14243 2018-08-24 13:52:14 +0200 signal: calculate nr of LEDs for the new extra aspect values a1-a5\n" \
"14242 2018-08-24 12:11:02 +0200 loc: reset gotoblock after arrive\n" \
"14241 2018-08-24 11:58:14 +0200 loc: reset gotoblock at reset and mode stop\n" \
"14240 2018-08-24 11:48:12 +0200 loc: reset gotoblock at stop command\n" \
"14239 2018-08-24 10:58:36 +0200 modbusdlg: translation fix for register range\n" \
"14238 2018-08-24 10:48:19 +0200 signal: data structure for the save state\n" \
"14237 2018-08-24 10:11:59 +0200 guiframe: load loco images alos in case the image is not shown\n" \
"14236 2018-08-24 10:11:11 +0200 signal: save state before blank\n" \
"14235 2018-08-24 07:19:53 +0200 readme: DE fix (RainerK)\n" \
"14234 2018-08-23 14:22:09 +0200 readme.txt: added windows line feeds\n" \
"14233 2018-08-23 14:09:57 +0200 innosetup script fix for double entry\n" \
"14232 2018-08-23 12:21:35 +0200 innosetup: readme added\n" \
"14231 2018-08-23 11:21:58 +0200 signal: filter out the blank command for saving in case the signal is already blank\n" \
"14230 2018-08-23 10:32:22 +0200 signal: fix for evaluating state in case of aspect control\n" \
"14229 2018-08-23 10:08:57 +0200 block: fix for setting reserved flag in case of group lock\n" \
"14228 2018-08-23 08:28:53 +0200 signal: fix for unknown unblank command\n" \
"14227 2018-08-22 17:31:23 +0200 signal: revert copy command before blank\n" \
"14226 2018-08-22 16:56:40 +0200 cbusnodedlg: update arducan servo description\n" \
"14225 2018-08-22 14:40:56 +0200 signal: save always the last command to be available at unblank\n" \
"14224 2018-08-22 14:02:35 +0200 modbusdlg: translation for coil range added\n" \
"14223 2018-08-22 10:55:41 +0200 block: fix for broadcasting reserved in case of block group lock\n" \
"14222 2018-08-22 10:12:14 +0200 block,model: trace corrections\n" \
"14221 2018-08-22 09:52:02 +0200 modbus: write single coil rocrail addr-1\n" \
"14220 2018-08-22 08:31:42 +0200 locdialog: removed placing option\n" \
"14219 2018-08-21 16:06:54 +0200 trace corrections\n" \
"14218 2018-08-21 16:00:45 +0200 rocrail: trace corrections\n" \
"14217 2018-08-21 14:25:49 +0200 modbus: switch support added\n" \
"14216 2018-08-21 11:59:25 +0200 translation merge: de, nl, sl, pt_BR, fr\n" \
"14215 2018-08-21 10:56:11 +0200 modbus: report coils as output events\n" \
"14214 2018-08-21 10:16:22 +0200 rocnet: support 8 leds for aspect signals with 16bit value\n" \
"14213 2018-08-21 10:05:41 +0200 modbus: addressing fix for reading coils and inputs\n" \
"14212 2018-08-21 07:25:37 +0200 cbusnodedlg: AdruCAN I/O and Servo added\n" \
"14211 2018-08-20 15:16:07 +0200 inno setup script: use userpf instead of pf\n" \
"14210 2018-08-20 15:01:27 +0200 rocnetnode: signal aspect value 0xFF is regarded as turn off all LEDs\n" \
"14209 2018-08-20 12:24:27 +0200 renderer: fix for scale the plus side of the bridge\n" \
"14208 2018-08-20 11:25:32 +0200 turntabledialog: show grid connection in the tracks overview\n" \
"14207 2018-08-20 10:53:18 +0200 signal: 5 extra aspect nr.values\n" \
"14206 2018-08-20 09:58:31 +0200 modbus: fix for polling ranges\n" \
"14205 2018-08-20 09:48:48 +0200 renderer: turntable track connection rotation support\n" \
"14204 2018-08-20 08:39:14 +0200 renderer: grin connection option added for backwards compat\n" \
"14203 2018-08-20 07:33:31 +0200 userial: no direct IO for platform __aarch64__\n" \
"14202 2018-08-20 07:28:48 +0200 usystem: disable direct IO for __aarch64__\n" \
"14201 2018-08-20 07:11:45 +0200 usystems: #elif fix\n" \
"14200 2018-08-20 06:41:28 +0200 usystem: fix for none sys/io.h platforms\n" \
"14199 2018-08-20 06:17:40 +0200 renderer: optiona turntable track grid connections\n" \
"14198 2018-08-20 06:01:06 +0200 renderer: revert drawing tracks inline to the border\n" \
"14197 2018-08-19 15:59:26 +0200 inno setup: set priviledge to lowest\n" \
"14196 2018-08-19 10:25:29 +0200 modbus: analog output support added\n" \
"14195 2018-08-19 08:10:39 +0200 cbusnodedlg: arducan calibrate buttons added\n" \
"14194 2018-08-19 07:43:47 +0200 car: imagenr added\n" \
"14193 2018-08-18 10:28:50 +0200 stage: return rest length of the trailing free section on is suited\n" \
"14192 2018-08-18 08:16:02 +0200 loc: skip the go command if the current block is the same as gotoblock\n" \
"14191 2018-08-17 15:32:33 +0200 renderer: draw the tt tracks to the symbol rectangle border\n" \
"14190 2018-08-17 11:16:15 +0200 model: free the goto block destination, if reserved, if its found and cannot be used and/or is reserved\n" \
"14189 2018-08-17 10:15:30 +0200 lcdriver: trace on reset goto block\n" \
"14188 2018-08-17 10:14:32 +0200 lcdriver: release a reserved goto block on reset\n" \
"14187 2018-08-17 10:00:45 +0200 blockgroup: check for blockgroup on reserve for conditions none recursive\n" \
"14186 2018-08-17 08:12:08 +0200 cbusnodedlg: correction for the ArduIR help link\n" \
"14185 2018-08-17 07:59:40 +0200 cbusnodedlg: ArduIR invert checkboxes added\n" \
"14184 2018-08-17 07:32:56 +0200 locowidget: fix for throttle size\n" \
"14183 2018-08-16 14:49:18 +0200 modbusdlg: poll coil flag fix\n" \
"14182 2018-08-16 14:37:50 +0200 REVERT blockgroup: check for blockgroup on reserve for conditions\n" \
"14181 2018-08-16 08:30:32 +0200 modbus: trace correction\n" \
"14180 2018-08-16 08:29:51 +0200 modbus: report coils as output field events\n" \
"14179 2018-08-15 15:30:45 +0200 modbus: coil polling added\n" \
"14178 2018-08-15 14:28:08 +0200 modbus: holding register support\n" \
"14177 2018-08-15 13:24:20 +0200 modbus: regard poll offset\n" \
"14176 2018-08-15 12:16:21 +0200 modbus: device list added for polling inputs, registers and holdings\n" \
"14175 2018-08-15 11:44:25 +0200 model: check at isDepartAllowed also if the loco is in mode auto\n" \
"14174 2018-08-15 07:36:37 +0200 blockgroup: check for blockgroup on reserve for conditions\n" \
"14173 2018-08-14 15:48:34 +0200 update wiki plan\n" \
"14172 2018-08-14 14:02:47 +0200 cbusnodedlg: ArduCAN IR write vars and events (WIP)\n" \
"14171 2018-08-14 11:32:29 +0200 cbusnodedlg: ArduCAN IR support\n" \
"14170 2018-08-14 11:06:54 +0200 cbusnodedlg: ArduCAN IR support (WIP)\n" \
"14169 2018-08-14 07:55:09 +0200 lcdriver: fix for ghost detection in second next block\n" \
"14168 2018-08-14 07:23:49 +0200 cbusnodedlg: manu DIY added\n" \
"14167 2018-08-14 07:23:03 +0200 cbusnodedlg: manu DIY added\n" \
"14166 2018-08-13 11:35:14 +0200 cbusnodedlg: show schumo99 with small caps\n" \
"14165 2018-08-13 11:32:43 +0200 cbusnodedlg: show schumo99 IR module in the index\n" \
"14164 2018-08-13 08:13:52 +0200 blockgroup: fix for reserving and loco via block\n" \
"14163 2018-08-13 07:35:38 +0200 modbusdlg: fix for sleep label\n" \
"14162 2018-08-12 12:48:14 +0200 tt: fix for not showing routes (Lothar)\n" \
"14161 2018-08-11 15:09:30 +0200 readme: update links\n" \
"14160 2018-08-11 15:04:39 +0200 readme: update\n" \
"14159 2018-08-11 11:02:56 +0200 mdimanager: reset position to 10,10 in case the mdi frame is no longer visible\n" \
"14158 2018-08-11 08:23:39 +0200 loclcdr: reset reserved routes and blocks in case of ghost by IN\n" \
"14157 2018-08-11 07:55:22 +0200 car stylesheet: update Andy\n" \
"14156 2018-08-10 16:50:48 +0200 model: fix to avoid double mapping sensors\n" \
"14155 2018-08-10 10:38:10 +0200 translation merge: de, nl, en_GB, pt_BR\n" \
"14154 2018-08-10 09:01:21 +0200 rocview: fix for offline status\n" \
"14153 2018-08-10 08:47:16 +0200 rocview: show edit mode in the status bar\n" \
"14152 2018-08-10 08:10:48 +0200 rocview: show edit mode in the status bar\n" \
"14151 2018-08-09 18:04:52 +0200 max. running option added: zero is default where all may run\n" \
"14150 2018-08-09 13:55:17 +0200 item: disable operating in case of edit mode\n" \
"14149 2018-08-09 11:27:44 +0200 guiframe,mdimanager: trace out display size and MDI position\n" \
"14148 2018-08-09 10:32:55 +0200 modbus: write single coil server support\n" \
"14147 2018-08-08 08:35:16 +0200 zimocan: save port4 state in the nidnode too\n" \
"14146 2018-08-08 08:10:34 +0200 zimocan: use a nidnode to combine port6 and data\n" \
"14145 2018-08-08 07:26:02 +0200 rocguiinidlg: reset MDIFrame position at disable\n" \
"14144 2018-08-07 06:49:14 +0200 model: fix checking if all locos are idle or stop\n" \
"14143 2018-08-07 06:37:57 +0200 model: fix for itterating the feedback map, use list instead\n" \
"14142 2018-08-06 07:11:50 +0200 modbusdlg: help link fix\n" \
"14141 2018-08-05 10:12:49 +0200 modbus: poll fine tuning added\n" \
"14140 2018-08-05 09:53:58 +0200 model: fix for initialising static arrays for updating sensor info\n" \
"14139 2018-08-05 08:38:38 +0200 modbusdlg: IP field added\n" \
"14138 2018-08-05 07:55:24 +0200 innosetup script fix\n" \
"14137 2018-08-04 15:12:51 +0200 modbus: transact async to avoid blocking at connect if the device is not online\n" \
"14136 2018-08-04 14:24:53 +0200 inno-setup: optional desktop icon for Rocview\n" \
"14135 2018-08-04 09:41:46 +0200 modbus setup dlg added\n" \
"14134 2018-08-03 15:45:10 +0200 modbus: input register support added\n" \
"14133 2018-08-03 14:19:22 +0200 translation merge: de, pt_BR, fr, sl, nl\n" \
"14132 2018-08-03 10:51:33 +0200 modbus: try to get the device vendor name at initial connect\n" \
"14131 2018-08-03 08:29:44 +0200 modbus: reconnect fixes\n" \
"14130 2018-08-03 07:51:38 +0200 modbus: keep connections alive\n" \
"14129 2018-08-02 18:18:25 +0200 modbus: check if fb polling is activated, use a different user defined communication port\n" \
"14128 2018-08-02 15:49:58 +0200 modbus: polling digital inputs\n" \
"14127 2018-08-02 15:20:21 +0200 modbus: WIP\n" \
"14126 2018-08-02 12:56:30 +0200 operatordlg: documentation button added\n" \
"14125 2018-08-02 11:58:36 +0200 modbus: request header fix\n" \
"14124 2018-08-02 10:44:01 +0200 modbus: WIP\n" \
"14123 2018-07-31 13:27:44 +0200 loconet: fix calling ibcom pom for reading\n" \
"14122 2018-07-31 12:13:27 +0200 loconet,ibcom: Uhli POM write packet\n" \
"14121 2018-07-30 15:20:27 +0200 ecos: fix for multiple cereating locos in case the ecos does not respond fast enough\n" \
"14120 2018-07-30 10:08:20 +0200 loc: BAT\n" \
"14119 2018-07-29 12:15:35 +0200 removed obsolete svnlog references and wrappers\n" \
"14118 2018-07-29 10:31:57 +0200 item: show bididir in sensor tooltip\n" \
"14117 2018-07-29 10:26:05 +0200 bidi direction: use only if its reported\n" \
"14116 2018-07-29 09:04:29 +0200 bidi direction: use only if its reported\n" \
"14115 2018-07-29 07:54:14 +0200 zimocan: ABA value guesed for port6 events\n" \
"14114 2018-07-28 19:28:42 +0200 zimocan: port6 address offset fix\n" \
"14113 2018-07-28 10:18:49 +0200 translation merge: sl, en_GB, pt_BR, sr_latin, nl, de\n" \
"14112 2018-07-26 08:32:33 +0200 statbar: lamp added for init field\n" \
"14111 2018-07-26 08:08:01 +0200 throttledlg: resize fixes\n" \
"14110 2018-07-25 08:15:15 +0200 guiframe: show init field ready in the statusbar\n" \
"14109 2018-07-25 08:08:25 +0200 guiframe: disable initfieldready popup\n" \
"14108 2018-07-25 07:48:26 +0200 node: safty checks at clone\n" \
"14107 2018-07-24 15:56:47 +0200 initfield popup in case its ready\n" \
"14106 2018-07-24 09:23:59 +0200 throttledlg: button stop and dir swapped\n" \
"14105 2018-07-24 07:37:09 +0200 throttledlg: resize improvements\n" \
"14104 2018-07-23 11:43:13 +0200 lc node memory leak fixed\n" \
"14103 2018-07-22 10:45:19 +0200 block: memory leak fix\n" \
"14102 2018-07-22 08:06:02 +0200 zimocan: evaluate direction bit of accessory data\n" \
"14101 2018-07-22 07:59:52 +0200 zimocan: accessory data only support for type=0x11\n" \
"14100 2018-07-22 07:50:10 +0200 zimocan: support for accessory port6 added\n" \
"14099 2018-07-21 17:25:50 +0200 zimocan: never ending story\n" \
"14098 2018-07-21 14:17:41 +0200 throttledlg: static image border under linux 10 pix :(\n" \
"14097 2018-07-21 10:24:23 +0200 throttledlg: use 4 pixels for border before scaling image\n" \
"14096 2018-07-21 08:10:30 +0200 throttle,guiframe: save and restore default throttle size\n" \
"14095 2018-07-20 08:08:03 +0200 throttledlg: fix for scaling loco image and resizing controls\n" \
"14094 2018-07-19 17:42:23 +0200 lc: fix for new meter parameter\n" \
"14093 2018-07-19 08:18:13 +0200 meter: show optional the bidi speed reported by the decoder\n" \
"14092 2018-07-17 15:19:48 +0200 stage,lcdriver: virtual automatic improved for staging blocks\n" \
"14091 2018-07-16 08:48:03 +0200 zimocan: trace level corrections\n" \
"14090 2018-07-16 07:59:19 +0200 zimocan: support for sensor module range 0xD000-0xDFFF added\n" \
"14089 2018-07-12 09:22:39 +0200 modplan: merge node attributes from the rt.xml\n" \
"14088 2018-07-11 15:48:42 +0200 mkrpm.sh: update\n" \
"14087 2018-07-11 09:05:44 +0200 translation merge: nl, de, pt_BR, en_GB\n" \
"14086 2018-07-11 08:10:25 +0200 model: fix for getSwitchList return type\n" \
"14085 2018-07-10 08:35:05 +0200 switch: only use delay in case of two switch motors\n" \
"14084 2018-07-10 08:32:10 +0200 loc: fix for ignore IN event in case of BBT genrate IN\n" \
"14083 2018-07-10 08:05:43 +0200 blockgroup: not calling goto block in case the reserving loco runs with a schedule\n" \
"14082 2018-07-09 15:09:49 +0200 guiframe, rocrailinidlg: info level moved to user\n" \
"14081 2018-07-09 07:41:55 +0200 guiframe: rocrail trace menu order changed to resemble the dialog\n" \
"14080 2018-07-08 10:58:30 +0200 throttle, ledbutton, slider: check minimal size before paint\n" \
"14079 2018-07-08 10:10:31 +0200 rocview: option for pause at stop rocview: rocrail trace menu\n" \
"14078 2018-07-08 07:44:44 +0200 throttledlg: set absolute min size\n" \
"14077 2018-07-07 15:54:31 +0200 throttledlg: fix for for default dialog size\n" \
"14076 2018-07-07 14:24:03 +0200 throttledlg: no resize in case another loco is selected\n" \
"14075 2018-07-06 10:09:27 +0200 locdialog: SBT export/import added\n" \
"14074 2018-07-06 09:59:22 +0200 guiframe: fix for restoring throttle size and position\n" \
"14073 2018-07-06 08:18:13 +0200 slider: fix for calculating the new value\n" \
"14072 2018-07-04 15:23:12 +0200 throttledlg: no border around the loco image\n" \
"14071 2018-07-04 14:24:51 +0200 throttledlg: fix for restore size and position\n" \
"14070 2018-07-04 13:50:41 +0200 slider: fix for resize and update current thumb position\n" \
"14069 2018-07-04 12:10:25 +0200 slider: fix for resizing and value range\n" \
"14068 2018-07-04 10:46:02 +0200 throttledlg: smaller and better resizing\n" \
"14067 2018-07-03 13:19:48 +0200 switch: traces added for pause and delay timing\n" \
"14066 2018-07-02 13:06:41 +0200 switch: fixed the route pause for dual motor switches\n" \
"14065 2018-07-02 08:00:16 +0200 block: check for state acceptghost\n" \
"14064 2018-07-02 07:55:04 +0200 block: start/stopacceptghost commands/actions added\n" \
"14063 2018-07-01 08:13:30 +0200 loc: skip speed command for consist members in case the loco is paused\n" \
"14062 2018-06-30 10:24:03 +0200 translation merge: en_GB\n" \
"14061 2018-06-30 09:07:13 +0200 actionsctrldlg: stage added to the conditions\n" \
"14060 2018-06-29 07:54:33 +0200 control: use the switch model list for checking the save positions to avoid a crash using the map iteration\n" \
"14059 2018-06-28 08:16:56 +0200 mysensors: removed\n" \
"14058 2018-06-27 11:09:01 +0200 routedlg: reset button added\n" \
"14057 2018-06-27 09:47:37 +0200 guiapp: merge route node also in case it has no commands or other child nodes\n" \
"14056 2018-06-27 07:56:06 +0200 blockgroup: default critical section\n" \
"14055 2018-06-26 17:21:09 +0200 blockgroup: trace improvements\n" \
"14054 2018-06-26 07:38:39 +0200 mgbox: fix voor uitlezen channel config cs2 (Bert)\n" \
"14053 2018-06-24 10:06:07 +0200 blockgroup: broadcast changes to the clients\n" \
"14052 2018-06-24 10:05:25 +0200 model: save schedule ID pointer in case it did not change on modify\n" \
"14051 2018-06-23 16:22:15 +0200 roclcdr: crash fixes\n" \
"14050 2018-06-23 11:36:26 +0200 modplan: save the MVTrack in the route file\n" \
"14049 2018-06-22 16:09:00 +0200 rocview base: better readable color for analyser trace\n" \
"14048 2018-06-22 14:42:10 +0200 user theme crossing update from Duesselklaus\n" \
"14047 2018-06-22 10:19:38 +0200 blockgroupingdlg: set prev_id in case the ID has been modified\n" \
"14046 2018-06-22 10:06:53 +0200 model: regard the stop mode also for checking if all locos are idle\n" \
"14045 2018-06-22 09:51:06 +0200 guiframe: fix for D&D in case the loco mode is stop instead of idle\n" \
"14044 2018-06-21 08:16:18 +0200 block: copy the lock ID string to avoid loosing it with temp. IDs\n" \
"14043 2018-06-19 10:01:33 +0200 rfbooster: ignore upper/lower case for evaluating car events\n" \
"14042 2018-06-18 09:40:43 +0200 ecos: fix for reporting correct PADA switch address\n" \
"14041 2018-06-18 07:40:09 +0200 blockgroupingdlg: translation fix for condition schedule\n" \
"14040 2018-06-17 07:46:38 +0200 ecos: crash fix for tracing the protver as string...\n" \
"14039 2018-06-16 11:41:54 +0200 blockgroupingdlg: show condition column schedule\n" \
"14038 2018-06-16 11:31:50 +0200 blockgroup: condition option schedule added\n" \
"14037 2018-06-13 10:22:25 +0200 lcdriver: set the run flag to true in case of a gogo command\n" \
"14036 2018-06-13 09:25:56 +0200 lcdriver: fix for isRun in case phase is not IDLE\n" \
"14035 2018-06-13 08:03:10 +0200 item,planpanel: check if the window has mouse capture before releasing the mouse\n" \
"14034 2018-06-12 17:14:12 +0200 translation merge: es, sl, sr_latin, de, fr, pt_BR, nl\n" \
"14033 2018-06-12 16:53:06 +0200 loc: take over V_cru in case of modify in auto mode\n" \
"14032 2018-06-12 08:11:33 +0200 fback: broadcast the original properties on modify\n" \
"14031 2018-06-11 19:24:36 +0200 fback: fixes for broadcast event without IID\n" \
"14030 2018-06-11 11:55:17 +0200 p.o. box replaced with real address\n" \
"14029 2018-06-11 07:58:10 +0200 bididir: disable at auto option added\n" \
"14028 2018-06-11 07:31:14 +0200 print: disable DC user scale\n" \
"14027 2018-06-10 13:49:07 +0200 item: regard stop state for state signals\n" \
"14026 2018-06-10 10:31:51 +0200 switchdialog: bus presentation and evaluatioon correction frog\n" \
"14025 2018-06-10 10:13:43 +0200 route-,signal-,switchdialog: fix for CTC bus number evaluation and presentation\n" \
"14024 2018-06-10 07:41:56 +0200 ecos: work around for the ecos detector\n" \
"14023 2018-06-09 07:21:33 +0200 block: regard bidi direction in case the loco runs in halfautomatic mode\n" \
"14022 2018-06-08 13:50:56 +0200 mysensors: wip\n" \
"14021 2018-06-08 13:47:52 +0200 app: trace correction on ini changes\n" \
"14020 2018-06-08 08:27:35 +0200 enter: check schedule entry actions correction in case no dest found and no swap\n" \
"14019 2018-06-08 07:42:06 +0200 wmz21: use number for switch address in cass of A2ID mapping and shortids\n" \
"14018 2018-06-07 11:16:44 +0200 app: take over the modified analyser options on the fly\n" \
"14017 2018-06-07 10:39:42 +0200 rocview: show analyser traces in the server window\n" \
"14016 2018-06-07 09:44:53 +0200 rocrailinidialog: added trace level analyser\n" \
"14015 2018-06-07 08:14:43 +0200 wmz21: filter event on dirfun to avoid taking over default direction\n" \
"14014 2018-06-06 14:43:43 +0200 rocnetnode: adjusted the base address for pi08 to 0x60\n" \
"14013 2018-06-06 07:59:22 +0200 block: use bidi direction in case auto is off or accept ident and the loco ist auto off\n" \
"14012 2018-06-05 12:05:53 +0200 control: filter swdbsize on useshortid\n" \
"14011 2018-06-05 12:01:51 +0200 control: determine the swdbsize for WLM only for the useshortid switches\n" \
"14010 2018-06-05 11:58:52 +0200 switch: useshortid option added\n" \
"14009 2018-06-05 11:31:35 +0200 virtual: filter out the shortid command\n" \
"14008 2018-06-05 10:26:47 +0200 wmz21: send switch IDs to the wlms\n" \
"14007 2018-06-04 08:31:04 +0200 sensorevents: export/import added\n" \
"14006 2018-06-04 07:33:27 +0200 loc: broadcast the complete properties on modify\n" \
"14005 2018-06-03 16:20:55 +0200 loc: if a consist loco is not found at swap, try by train\n" \
"14004 2018-06-02 11:40:29 +0200 renamedlg: find OID in action list\n" \
"14003 2018-06-02 08:19:21 +0200 loc: isstate stop added\n" \
"14002 2018-06-02 08:10:54 +0200 roclcdr: use mode string stop in case of idle and auto off\n" \
"14001 2018-06-01 08:46:10 +0200 wmz21: only send a response on a LAN_X_GET_TURNOUT_INFO if the switch is known in Rocrail\n" \
"14000 2018-05-31 17:10:26 +0200 wmz21: revert switch info fix\n" \
"13999 2018-05-30 12:54:00 +0200 loconet: multi sense address correction\n" \
"13998 2018-05-30 07:29:44 +0200 ecos: pom read/write for accessory support added\n" \
"13997 2018-05-29 17:20:55 +0200 schedulegraphdlg: fix for setting schedule #1\n" \
"13996 2018-05-27 07:46:47 +0200 ecos: removed version 1 option -> report switch events always as switch events\n" \
"13995 2018-05-26 14:51:48 +0200 basedlg: crash fix if no xxlist is available in the plan.xml\n" \
"13994 2018-05-26 13:40:18 +0200 mgbox: only send measurement changes (Bert)\n" \
"13993 2018-05-23 13:59:40 +0200 action, xmlscript: propagate regval\n" \
"13992 2018-05-23 07:49:42 +0200 loc: save placing at linktrain for use at unlinktrain\n" \
"13991 2018-05-23 06:31:50 +0200 loc: unlinktrain take over block enter side from master\n" \
"13990 2018-05-23 06:05:44 +0200 mgbox: retrieving the link device ID (Bert)\n" \
"13989 2018-05-22 07:55:12 +0200 loc: fix for unlinktrain in case enter side is min\n" \
"13988 2018-05-22 07:27:48 +0200 mgbox: check bus length (Bert)\n" \
"13987 2018-05-21 16:52:36 +0200 guiframe: sort by image file name\n" \
"13986 2018-05-21 15:51:57 +0200 rocnet: report rc/rs at state\n" \
"13985 2018-05-21 15:44:19 +0200 wmz21: provide turnout info\n" \
"13984 2018-05-21 14:04:10 +0200 wmz21: LAN_X_TURNOUT_INFO fix\n" \
"13983 2018-05-21 12:22:17 +0200 mgbox: the maerklin cs2+ links88 drama (to be continued by Bert)\n" \
"13982 2018-05-21 12:06:02 +0200 guiframe: system action state error added\n" \
"13981 2018-05-21 08:40:11 +0200 loc: link/unlink train: added missing loco block commands\n" \
"13980 2018-05-21 08:23:44 +0200 planpanel: fix for status bar clipboard text\n" \
"13979 2018-05-21 07:33:39 +0200 wmz21: save lights on function command, send all function states to the CS\n" \
"13978 2018-05-20 08:33:38 +0200 control: crash fix\n" \
"13977 2018-05-19 11:00:44 +0200 control: use %regval% for error code\n" \
"13976 2018-05-18 21:34:16 +0200 mgbox: support message changed\n" \
"13975 2018-05-18 20:44:32 +0200 mgbox: corrections (Bert)\n" \
"13974 2018-05-18 08:11:00 +0200 ecos: POM support added (firmware 4.2.3+ is needed)\n" \
"13973 2018-05-17 11:57:28 +0200 loc: unlink support in a virtual block\n" \
"13972 2018-05-16 14:32:22 +0200 linktrain: virtual block support added (unlinktrain in vb is WIP)\n" \
"13971 2018-05-16 11:43:00 +0200 locdialog: exclude locos and trains from consist selection if they are already in the consist\n" \
"13970 2018-05-16 11:40:58 +0200 blockdialog: select slave blocks by dialog\n" \
"13969 2018-05-15 12:11:36 +0200 lcdriver: activate first schedule of tour if no free destination is available\n" \
"13968 2018-05-14 12:21:28 +0200 block: followup lock ID in case of an unlink train command\n" \
"13967 2018-05-14 08:19:20 +0200 ecos: check is socket is valid before using it\n" \
"13966 2018-05-14 08:07:53 +0200 wmz21: fix for lights in case of none rocrail loco\n" \
"13965 2018-05-13 08:12:38 +0200 locdialog: add train on multiunit tab\n" \
"13964 2018-05-12 08:12:38 +0200 trainimage: show linked trains\n" \
"13963 2018-05-12 07:52:37 +0200 translation merge: de, nl, fr, sl\n" \
"13962 2018-05-11 15:06:05 +0200 loc,guiframe: fixes for (un)link a train\n" \
"13961 2018-05-11 07:48:44 +0200 link/unlink train (wip)\n" \
"13960 2018-05-08 13:53:29 +0200 schedulegraphdlg: default settings to none scaled\n" \
"13959 2018-05-08 12:14:39 +0200 scheduledialog: process enter in the group filter combobox\n" \
"13958 2018-05-08 11:29:08 +0200 locationgraph: show time now\n" \
"13957 2018-05-08 11:17:30 +0200 schedulegraphdlg: label fix from scale to adjust\n" \
"13956 2018-05-08 10:43:39 +0200 scheduledialog: use combobox for groupID and groupFilter\n" \
"13955 2018-05-08 09:17:51 +0200 locationsdlg: fix for selecting schedules\n" \
"13954 2018-05-07 16:05:38 +0200 scheduledialog: translation fix\n" \
"13953 2018-05-07 15:41:54 +0200 railwayline: removed\n" \
"13952 2018-05-07 12:19:00 +0200 schedules: record schedule time for none regular stops too\n" \
"13951 2018-05-07 12:13:09 +0200 scheduledialog: layout correction\n" \
"13950 2018-05-07 11:51:11 +0200 schedule: removed railway line\n" \
"13949 2018-05-07 10:52:19 +0200 translation merge: nl, en_GB, de, fr\n" \
"13948 2018-05-07 09:17:14 +0200 schedulegraph: copy/paste fix\n" \
"13947 2018-05-07 09:14:49 +0200 locationgraph,schedulegraph: use scrollwin* events to refresh the whole canvas under windows and linux\n" \
"13946 2018-05-07 08:24:26 +0200 schedulegraph: mousewheel eventhandler to invalidate the whole canvas\n" \
"13945 2018-05-07 08:17:10 +0200 locationgraph: mousewheel eventhandler to invalidate the whole canvas\n" \
"13944 2018-05-07 07:48:38 +0200 locationgraph: mousewheel eventhandler to invalidate the whole canvas\n" \
"13943 2018-05-06 15:38:32 +0200 messages: adjust added\n" \
"13942 2018-05-06 11:25:46 +0200 locationgraph: set vertical scroll style flag\n" \
"13941 2018-05-06 07:47:06 +0200 scheduledlg: fix for showing depart time for first entry\n" \
"13940 2018-05-05 18:36:47 +0200 scheduledialog: fix for destination grid scroll\n" \
"13939 2018-05-05 14:57:15 +0200 locationsdlg: only show schedules in the list on the graph tab which contains a block of the slected loctaion\n" \
"13938 2018-05-05 12:57:35 +0200 locationgraph: destination text option added\n" \
"13937 2018-05-05 12:18:17 +0200 locationgraph: show destination\n" \
"13936 2018-05-05 10:52:04 +0200 locationsdlg: open schedule on dclick in list\n" \
"13935 2018-05-05 10:33:13 +0200 schedule entry: arrive time added\n" \
"13934 2018-05-05 08:28:06 +0200 locationsdlg: sort schedule color list\n" \
"13933 2018-05-05 08:19:48 +0200 locationsdialog: show schedule colors on the graph tab\n" \
"13932 2018-05-05 07:41:11 +0200 loc: inform slave locos in case of a pause command\n" \
"13931 2018-05-05 06:57:47 +0200 ecos: fix for address offset in case of RailCom\n" \
"13930 2018-05-04 22:16:47 +0200 locationgraph: fix for calculating y position\n" \
"13929 2018-05-04 15:33:44 +0200 scheduledialog: show none regular stop time in the destination table between brackets\n" \
"13928 2018-05-04 14:44:41 +0200 locationgraph: scaling support\n" \
"13927 2018-05-04 13:18:52 +0200 locationgraph: show trainid or scheduleid\n" \
"13926 2018-05-04 12:29:46 +0200 locationgraph: disabled scale height\n" \
"13925 2018-05-04 12:10:02 +0200 locationgraph: use schedule color\n" \
"13924 2018-05-04 12:02:02 +0200 locationgraph: skip none regular stops and a fix for hourly time\n" \
"13923 2018-05-04 11:41:44 +0200 locationgraph: show schedule entries es red line\n" \
"13922 2018-05-04 11:07:18 +0200 locationgraph: WIP\n" \
"13921 2018-05-04 08:20:11 +0200 locationdlg: graph tab added (wip)\n" \
"13920 2018-05-04 07:40:09 +0200 om32: show switch delay in trace\n" \
"13919 2018-05-03 08:31:44 +0200 initialize: fix for checking the auto state in case of checking the block wait flag\n" \
"13918 2018-05-02 18:15:04 +0200 z21: revert loconet detector address change\n" \
"13917 2018-05-02 08:10:08 +0200 z21: Xpressnet GBM option added for activating Loconet Detector evaluation\n" \
"13916 2018-05-01 14:43:20 +0200 loc: check if loco is in auto mode before pause request\n" \
"13915 2018-04-30 19:32:13 +0200 analyse: fix for direction arrows next to blocks (Lothar)\n" \
"13914 2018-04-30 18:08:06 +0200 routedialog: remove autogen flag also removes description (Lothar)\n" \
"13913 2018-04-30 10:24:35 +0200 translation merge: pt_BR, en_GB, sr_latin, nl, de\n" \
"13912 2018-04-29 13:02:30 +0200 rocnetnode: trace level correction\n" \
"13911 2018-04-29 11:51:00 +0200 loc: fix for pause\n" \
"13910 2018-04-29 11:10:12 +0200 powerctrl: history log size in minutes added\n" \
"13909 2018-04-29 08:12:19 +0200 model: disable usebididir after the first auto on command\n" \
"13908 2018-04-29 07:55:46 +0200 rocnetnode: only try to init the LED chips incase one or more has been detected\n" \
"13907 2018-04-29 07:47:31 +0200 rocnetnode: trace fix\n" \
"13906 2018-04-29 07:46:25 +0200 rocnet: do not automatically take over the found MQTT broker\n" \
"13905 2018-04-29 07:33:18 +0200 SLIM-COLOR: raster switches (Peter)\n" \
"13904 2018-04-29 07:26:00 +0200 loc: fix for pause\n" \
"13903 2018-04-28 10:47:06 +0200 loc: reset pause flag on init to signal the clients the correct state at startup\n" \
"13902 2018-04-28 09:51:18 +0200 loc: allow turn of pause in case of no auto mode\n" \
"13901 2018-04-28 08:21:57 +0200 loc: pause flag added\n" \
"13900 2018-04-27 11:19:04 +0200 rocnetnode: crash fix if no rocnetnode.ini is available\n" \
"13899 2018-04-27 07:32:08 +0200 text: new text variable for date and temp. added\n" \
"13898 2018-04-26 15:49:36 +0200 loc: informall flag added\n" \
"13897 2018-04-26 08:14:31 +0200 guiframe: disable edit actions and variables in case of automatic mode\n" \
"13896 2018-04-26 07:52:37 +0200 operator: removed the combination field\n" \
"13895 2018-04-25 12:43:26 +0200 basedlg: regard the active suffix at sorting the show column\n" \
"13894 2018-04-24 07:56:59 +0200 locdialog: show active flag on the index tab\n" \
"13893 2018-04-23 12:02:22 +0200 model: added missing return in case lookup schedule is disabled\n" \
"13892 2018-04-22 11:45:58 +0200 rocnetnode: fix for calculating the on delay\n" \
"13891 2018-04-22 10:26:59 +0200 signal: dim parameter added for dimming LED type signals\n" \
"13890 2018-04-22 07:57:07 +0200 model: common lookupschedule command added\n" \
"13889 2018-04-21 10:46:17 +0200 rocnetnode: auto brightnes fix for keep two blinking LED in sync\n" \
"13888 2018-04-21 08:20:18 +0200 rocnetnode: fix for update LED brightness\n" \
"13887 2018-04-21 08:17:23 +0200 rocnetnode: improved signal processing\n" \
"13886 2018-04-20 15:37:37 +0200 rocnetnode: signal processor thread added to be able to wait between LEDs be turned off and LEDs be turned on.\n" \
"13885 2018-04-20 08:08:21 +0200 scheduledialog: timeprocessing index column added\n" \
"13884 2018-04-20 07:51:17 +0200 model: swprops command added\n" \
"13883 2018-04-20 07:37:29 +0200 timetabledlg: memory leak fix\n" \
"13882 2018-04-19 08:33:22 +0200 bidib: Tams update\n" \
"13881 2018-04-18 16:04:52 +0200 bidib images\n" \
"13880 2018-04-18 15:59:17 +0200 bidib: Tams images\n" \
"13879 2018-04-18 14:51:59 +0200 rocnetnode: scan and init Pi08 boards after read options\n" \
"13878 2018-04-18 10:48:43 +0200 signal: calculate the number of LEDs by evaluating the aspect values (for LED signals and aspect number control only)\n" \
"13877 2018-04-18 10:28:17 +0200 rocnetnode: fix for brightness mode\n" \
"13876 2018-04-18 08:20:26 +0200 rocnetnode: brightness mode for weather, day, night\n" \
"13875 2018-04-18 08:03:19 +0200 timetabledlg: cellspacing option added\n" \
"13874 2018-04-17 14:47:21 +0200 rocnetnode: use weather brightness for signal LEDs\n" \
"13873 2018-04-17 11:14:39 +0200 rocnetnode: signal aspect value support added\n" \
"13872 2018-04-17 08:17:29 +0200 rocnetnode: disable blink on first signal LED\n" \
"13871 2018-04-17 08:01:58 +0200 rocnetnode: simple aspect signals extended\n" \
"13870 2018-04-17 07:47:28 +0200 model: extended the lcprops command to itterate all\n" \
"13869 2018-04-16 11:42:17 +0200 timetabledlg: only show loco ID in case the train ID is not set\n" \
"13868 2018-04-16 09:08:10 +0200 rocnetnode: dimming fix for LEDs\n" \
"13867 2018-04-16 08:54:45 +0200 rocnetnode: simple aspect LED signal support\n" \
"13866 2018-04-16 08:24:54 +0200 rocnetnode: LED output blink support\n" \
"13865 2018-04-15 16:22:46 +0200 rocnetnode: macro extended for LED outputs, signal extended for simple LED aspect signals\n" \
"13864 2018-04-15 14:04:24 +0200 rocnetnode: support for LED with temp. base 0x06\n" \
"13863 2018-04-15 10:28:52 +0200 locationdlg: show line position\n" \
"13862 2018-04-15 10:20:32 +0200 location: railway line position\n" \
"13861 2018-04-15 08:49:05 +0200 analyse: reset/init listeners improved\n" \
"13860 2018-04-14 15:41:57 +0200 locationsdlg: fix for moving down blocks\n" \
"13859 2018-04-14 14:53:48 +0200 location: removed railway/position from properties; should be maintained in the railwayline properties\n" \
"13858 2018-04-13 16:36:04 +0200 analyse: inform blocks of changed properties to re-initialise listeners\n" \
"13857 2018-04-13 16:30:23 +0200 operator: fix for race condition?\n" \
"13856 2018-04-13 15:38:24 +0200 operator: revert redirecting commands to the assigned loco\n" \
"13855 2018-04-13 08:45:33 +0200 seltab: BBT option added\n" \
"13854 2018-04-13 08:08:50 +0200 analyse: set analysed flag on broadcast to signal to replace all child nodes even if there are none in the modified properties\n" \
"13853 2018-04-13 06:36:13 +0200 model: trace corrections after analyse\n" \
"13852 2018-04-12 14:44:28 +0200 rocnet: show in clock sync trace the brightness\n" \
"13851 2018-04-12 10:34:22 +0200 ulni: tracelevel changed to BYTE for undocumented OPCs\n" \
"13850 2018-04-12 10:32:32 +0200 ulni: trace level changed to BYTE for undocumented OPCs\n" \
"13849 2018-04-12 10:28:57 +0200 trackpickerdlg,planpanel: raster switch type support\n" \
"13848 2018-04-12 08:31:17 +0200 SLIM: raster switches\n" \
"13847 2018-04-11 15:46:32 +0200 SLIM: raster switches (WIP)\n" \
"13846 2018-04-11 12:01:46 +0200 rocprodlg: removed decoderdb link\n" \
"13845 2018-04-11 11:56:51 +0200 wrapper: default string section value set to empty instead of zero\n" \
"13844 2018-04-11 11:08:11 +0200 operatordlg: moved the train ID to the index tab\n" \
"13843 2018-04-11 10:57:47 +0200 operatordlg,railwaydlg: abox support added\n" \
"13842 2018-04-11 06:28:26 +0200 analyse: broadcast block properties after removing the generated routes\n" \
"13841 2018-04-10 14:06:51 +0200 SLIM-COLOR: button svg #4 added\n" \
"13840 2018-04-10 13:58:19 +0200 SLIM: button svg #4 added\n" \
"13839 2018-04-10 11:57:42 +0200 operator: redirect unhandled commands to the linked loco\n" \
"13838 2018-04-10 08:30:20 +0200 lc: replaced nested calls to the meter\n" \
"13837 2018-04-10 07:53:32 +0200 analyse: broadcast block changes\n" \
"13836 2018-04-10 07:18:17 +0200 json: use alloc instaed of dup\n" \
"13835 2018-04-09 14:36:04 +0200 locdialog: layout correction\n" \
"13834 2018-04-09 14:35:41 +0200 lc: init kmhMax\n" \
"13833 2018-04-09 14:35:08 +0200 translation merge: en_GB,fr,pt_BR,de\n" \
"13832 2018-04-09 12:00:37 +0200 cellrenderer: check if image is OK\n" \
"13831 2018-04-09 10:11:09 +0200 rocweb: use default zlevel 0 if no z attribute is found\n" \
"13830 2018-04-09 08:30:37 +0200 model: load occ.xml only if exist\n" \
"13829 2018-04-09 08:11:05 +0200 json: memory leak fix\n" \
"13828 2018-04-09 08:09:51 +0200 rocweb: revert test changes\n" \
"13827 2018-04-08 11:02:00 +0200 powermandlg: show the HEX UID as static text\n" \
"13826 2018-04-08 10:39:36 +0200 powermandlg: booster index extended with IID and description\n" \
"13825 2018-04-08 08:32:47 +0200 weather: run weater in case of one or more outputs or with color table for updating temp and bri\n" \
"13824 2018-04-08 08:27:59 +0200 weather: do weather also in case of no lamps for temperature and brightness\n" \
"13823 2018-04-08 08:22:46 +0200 weatherdlg: select outputs by dialog\n" \
"13822 2018-04-08 07:22:26 +0200 powermandlg: show UID as unsigned\n" \
"13821 2018-04-07 16:49:05 +0200 guiapp: merge a route event only deep if it has child nodes to avoid loosing commands\n" \
"13820 2018-04-07 15:26:53 +0200 powermandlg: take over the UID in the HEX field after apply\n" \
"13819 2018-04-07 14:54:58 +0200 powerman: fix for negative booster UID\n" \
"13818 2018-04-07 11:43:45 +0200 rocnetnode: show temp. in trace in case of a clock sync\n" \
"13817 2018-04-07 11:21:05 +0200 clock brightness set by weather\n" \
"13816 2018-04-07 10:52:37 +0200 weatherdlg: label correction\n" \
"13815 2018-04-07 10:47:55 +0200 colorpanel: show temperature color picker added\n" \
"13814 2018-04-07 08:43:25 +0200 weather: temperature line added\n" \
"13813 2018-04-06 15:21:08 +0200 rocnetnode: show temp and date only the half time of the clock\n" \
"13812 2018-04-06 15:14:50 +0200 rocnetnode: scan I2C for Pi08 art startup\n" \
"13811 2018-04-06 11:45:15 +0200 tt: state pending added\n" \
"13810 2018-04-06 10:55:13 +0200 rocnetnode: pi08 (wip)\n" \
"13809 2018-04-06 10:47:21 +0200 analyse,model: trace level corrections\n" \
"13808 2018-04-06 10:33:44 +0200 analyse: broadcast item changes to the clients\n" \
"13807 2018-04-06 10:15:37 +0200 tt,xmlscript: tt state support\n" \
"13806 2018-04-06 08:49:53 +0200 fundialog: list stage blocks too\n" \
"13805 2018-04-06 08:36:51 +0200 hue: tracing correctiongs to avoid overflow\n" \
"13804 2018-04-06 08:01:50 +0200 hue: clean up the xml node after use to avoid memory leak\n" \
"13803 2018-04-06 07:34:44 +0200 wrapper: reset whell counter option default not active\n" \
"13802 2018-04-06 00:47:45 +0200 locseldlg: fix for showing cars without address set\n" \
"13801 2018-04-06 00:29:44 +0200 hue: byte trace length fix\n" \
"13800 2018-04-05 14:19:28 +0200 control: clock sync year+1900 fix\n" \
"13799 2018-04-05 07:17:35 +0200 modplan: fix for merge nodes in lists in case the ID is already in the list\n" \
"13798 2018-04-04 08:42:36 +0200 socket: readln extra buffer len parameter to avoid overflow\n" \
"13797 2018-04-03 11:11:02 +0200 z21: removed trace line for loconet detector support\n" \
"13796 2018-04-03 10:10:47 +0200 action: vcurrentloco action added to restore/reactivate the current speed after for example an ebreak\n" \
"13795 2018-04-03 09:51:54 +0200 z21: disable LAN_LOCONETDETECTOR without any option\n" \
"13794 2018-04-03 08:29:15 +0200 z21: disable LocoNetDetector broadcasting if not wanted\n" \
"13793 2018-04-03 08:22:45 +0200 z21: default no loconet detector evaluation, to activate set protver to 1\n" \
"13792 2018-04-03 08:00:05 +0200 translation merge: pt_BR, sl, nl, de\n" \
"13791 2018-04-02 16:36:40 +0200 z21: adjust loconetdetector reported sensor address\n" \
"13790 2018-04-02 07:55:05 +0200 rocpro: fix for setting long address in the loco properties\n" \
"13789 2018-04-01 16:51:48 +0200 sensorevents: fix for bus field on sorting columns\n" \
"13788 2018-04-01 08:12:37 +0200 SLIM-COLOR: stage.svg in SLIM format\n" \
"13787 2018-04-01 07:36:34 +0200 SLIM-COLOR: stage corrections (Peter)\n" \
"13786 2018-03-31 17:44:36 +0200 in: fix for using the block enterside\n" \
"13785 2018-03-31 10:39:54 +0200 rocnetnode: set clock divider back to 1 in case of a timeout\n" \
"13784 2018-03-31 10:37:34 +0200 rocnetnode: set clock sync timeout mode on startup\n" \
"13783 2018-03-31 10:32:06 +0200 rocnetnode: display timeout watchdog\n" \
"13782 2018-03-31 10:28:28 +0200 rocnetnode: display timeout watchdog, show floating dashes\n" \
"13781 2018-03-31 10:15:54 +0200 rocnetnode: display timeout watchdog\n" \
"13780 2018-03-31 10:10:20 +0200 rocnetnode: display timeout watchdog\n" \
"13779 2018-03-31 10:03:55 +0200 rocnetnode: display timeout watchdog\n" \
"13778 2018-03-31 09:48:08 +0200 block: enterside wait option, default both sides\n" \
"13777 2018-03-31 06:54:53 +0200 rocnetnode: clock options\n" \
"13776 2018-03-31 05:03:58 +0200 guiapp: merge route in case of modify and not visible\n" \
"13775 2018-03-30 15:49:19 +0200 modelutils: rename signal ID dependencies\n" \
"13774 2018-03-30 14:10:45 +0200 rocnetnode: negative temp\n" \
"13773 2018-03-30 14:08:06 +0200 rocnet: fix for negative temp\n" \
"13772 2018-03-30 13:40:29 +0200 rocnetnode: negative temp.\n" \
"13771 2018-03-30 13:27:17 +0200 rocnetnode: show temp\n" \
"13770 2018-03-30 12:06:41 +0200 rocnetnode: show temp\n" \
"13769 2018-03-30 11:56:50 +0200 rocnetnode: show temp\n" \
"13768 2018-03-30 11:31:48 +0200 rocnetnode: auto display scan, show date\n" \
"13767 2018-03-30 10:49:22 +0200 base: genID with 3 digits\n" \
"13766 2018-03-30 10:41:37 +0200 planpanel: use genID also for track type\n" \
"13765 2018-03-30 10:35:33 +0200 rocnetnodedlg: set readonly flag for the I2C scan results\n" \
"13764 2018-03-30 10:34:54 +0200 planpanel: popup ID dialog option added\n" \
"13763 2018-03-30 09:06:40 +0200 blockdialog: allow 5% arrive/depart speed\n" \
"13762 2018-03-29 15:54:35 +0200 rocnetnode: clock dot banging\n" \
"13761 2018-03-29 15:11:04 +0200 rocnetnode: fix for nrclocks\n" \
"13760 2018-03-29 14:45:24 +0200 rocnetnode: clock brightness setup added\n" \
"13759 2018-03-29 13:23:33 +0200 rocnetnode: clock dot step 1 1000ms in case div > 1\n" \
"13758 2018-03-29 12:18:00 +0200 rocnetnode: clock dot step every 5 model seconds\n" \
"13757 2018-03-29 12:09:24 +0200 rocnetnode: clock running dot in case divider > 1\n" \
"13756 2018-03-29 12:06:12 +0200 rocnetnode: clock running dots in case divider > 1\n" \
"13755 2018-03-29 11:46:30 +0200 rocnetnode: flash clock colon only if divider is 1\n" \
"13754 2018-03-29 11:12:25 +0200 rocnetnode: clock support\n" \
"13753 2018-03-29 11:08:19 +0200 rocnetnode: clock support (WIP)\n" \
"13752 2018-03-29 10:47:52 +0200 rocnetnode: clock support (WIP)\n" \
"13751 2018-03-29 08:35:51 +0200 rocnetnode: trace I2C scan\n" \
"13750 2018-03-29 08:08:14 +0200 rocnet: clock config added\n" \
"13749 2018-03-28 11:44:54 +0200 rocnetnode: clock display support (WIP)\n" \
"13748 2018-03-28 10:37:18 +0200 rocprodlg: take over values after a DIP OK/Apply\n" \
"13747 2018-03-28 03:49:11 +0200 location: scheduletable on/off command added\n" \
"13746 2018-03-27 13:40:21 +0200 rocpro: take over value from DIP\n" \
"13745 2018-03-27 11:34:55 +0200 tt,action: fixes for broadcasting new state\n" \
"13744 2018-03-27 10:33:59 +0200 tt,traverser: closed symbols, fix for keep state\n" \
"13743 2018-03-26 08:45:48 +0200 translation merge: sl,pr_BR,sr_latin,de,nl\n" \
"13742 2018-03-26 08:23:05 +0200 action,control: time set trace added\n" \
"13741 2018-03-25 13:58:54 +0200 loc: fix for BBT speed!!!\n" \
"13740 2018-03-25 10:23:08 +0200 timedactions: added open/close as commands to the turntable\n" \
"13739 2018-03-25 09:11:50 +0200 tt: regard close state in case of embedded block\n" \
"13738 2018-03-24 12:18:39 +0100 locdialog: speed++ changed in speed +/-\n" \
"13737 2018-03-24 11:07:14 +0100 block: extra freeonenter option route added\n" \
"13736 2018-03-24 10:35:04 +0100 loc: BBT key speed++ added (block enter side)\n" \
"13735 2018-03-23 11:41:31 +0100 rocpro,bidib: accext support added\n" \
"13734 2018-03-23 08:48:57 +0100 z21: usesamesensorbus option added for all LocoNet type sensors on bus 1\n" \
"13733 2018-03-22 14:13:56 +0100 z21: removed the rbus option\n" \
"13732 2018-03-22 14:02:10 +0100 z21: bus test correction\n" \
"13731 2018-03-22 14:00:39 +0100 z21: trace for type LN detector input\n" \
"13730 2018-03-22 08:17:54 +0100 bidib: POM accessory support added\n" \
"13729 2018-03-21 15:44:03 +0100 block: ignore an absent event in case of running in automatic mode\n" \
"13728 2018-03-21 11:05:38 +0100 renderer: mapping fix for dcrossingright-tr-l-occ\n" \
"13727 2018-03-21 09:14:02 +0100 dcrossing fix (Martin)\n" \
"13726 2018-03-21 09:09:10 +0100 tt: virtual automatic support in case of embeded block\n" \
"13725 2018-03-21 08:25:44 +0100 tttrackdialog: fix for evaluating\n" \
"13724 2018-03-20 10:20:25 +0100 renderer: mapping correction for dcrossing right\n" \
"13723 2018-03-20 08:50:23 +0100 analyse,tt: typo fix\n" \
"13722 2018-03-20 07:53:52 +0100 blockdialog, routedialog: disable adding the same loco into the include and exclude list\n" \
"13721 2018-03-19 14:55:07 +0100 route: support for virtual running over a TT by sending it a position event\n" \
"13720 2018-03-19 11:01:15 +0100 z21: use the fbread attribute to enable or disable evaluating the R-BUS\n" \
"13719 2018-03-18 11:29:56 +0100 model: getRealBlock type correction\n" \
"13718 2018-03-18 10:39:48 +0100 z21: option rbus added to be able to disable it\n" \
"13717 2018-03-18 10:25:52 +0100 z21: protver=1 loconet sensor reporting only\n" \
"13716 2018-03-18 09:56:19 +0100 model: lookup real block in case of a modify\n" \
"13715 2018-03-18 08:05:51 +0100 routedialog: fix for evaluating the wiring output IDs\n" \
"13714 2018-03-18 07:56:37 +0100 loc: fix for setting the block enterside and placing\n" \
"13713 2018-03-17 08:25:43 +0100 loconet: save decoder type for dispatching\n" \
"13712 2018-03-16 13:59:56 +0100 stage: check if the stage is closed in case of reserve as crossing block\n" \
"13711 2018-03-16 11:00:11 +0100 z21: support for LocoNet SV programming for LocoIO/GCA50\n" \
"13710 2018-03-16 10:21:03 +0100 esunavi: reduced speed range to 126\n" \
"13709 2018-03-16 10:17:00 +0100 z21: removed version workaround, LocoIO setup (wip)\n" \
"13708 2018-03-16 10:09:53 +0100 issuedlg: replace gt and lt chars with underscores\n" \
"13707 2018-03-15 07:53:07 +0100 dsd2010: use a turntable event for reporting the position instead of an accessory event\n" \
"13706 2018-03-14 21:35:34 +0100 model: the iid must match in case of a turntable event\n" \
"13705 2018-03-14 21:29:18 +0100 model: ignore zero addr/port events for objects other then turntables\n" \
"13704 2018-03-14 10:47:34 +0100 z21: use protver=1 for loconet transponder address 1 offset (DR5088)\n" \
"13703 2018-03-14 08:52:53 +0100 xmlscript: resolve clock variables\n" \
"13702 2018-03-14 07:33:52 +0100 bidib: POM and PT traces corrected\n" \
"13701 2018-03-13 15:54:00 +0100 seltabdlg,textdialog: close dialog on OK in case nothing is selected or empty\n" \
"13700 2018-03-13 11:30:43 +0100 tt: syntax fixes\n" \
"13699 2018-03-13 11:17:31 +0100 tt: optional skip range to avoid running with the bridge into a gap\n" \
"13698 2018-03-13 09:23:15 +0100 block: use also bidi direction in case of a generated loco\n" \
"13697 2018-03-13 09:21:18 +0100 xmlscript: allow multiple case values separated with pipes\n" \
"13696 2018-03-12 18:14:08 +0100 roco: trace fix for sensors\n" \
"13695 2018-03-12 16:14:19 +0100 routedialog: set command combo with array\n" \
"13694 2018-03-12 16:03:13 +0100 timedactions: init ID combo with array\n" \
"13693 2018-03-12 13:59:02 +0100 roco: group offset fix\n" \
"13692 2018-03-12 13:52:22 +0100 roco: fix for info byte group 1\n" \
"13691 2018-03-12 12:20:45 +0100 tt: dsd2010 next/prev fixes\n" \
"13690 2018-03-12 11:28:53 +0100 bidib: report absent in case a railcom sensor lost a loco\n" \
"13689 2018-03-12 10:55:01 +0100 tt: dsd2010 set rotation direction in case of next or prev track\n" \
"13688 2018-03-12 08:20:32 +0100 blockdialog: init comboboxes\n" \
"13687 2018-03-12 08:01:28 +0100 roco: fix for fbmods > 10\n" \
"13686 2018-03-11 15:27:00 +0100 schedulegraph: skip relative timing schedules\n" \
"13685 2018-03-11 15:09:52 +0100 locodialog, blockdialog: init comboboxes\n" \
"13684 2018-03-11 13:48:51 +0100 blockdialog: init\n" \
"13683 2018-03-11 12:07:38 +0100 item: only use the reported bridge position in case it is set\n" \
"13682 2018-03-11 10:43:32 +0100 dialogs: fill comboboxes with string arrays to speed up init\n" \
"13681 2018-03-10 17:19:31 +0100 dsd2010: trce level correction\n" \
"13680 2018-03-10 14:16:02 +0100 rocguiinidlg: moved the help url to the path tab\n" \
"13679 2018-03-10 14:09:05 +0100 roco: use sensor group byte as bus number\n" \
"13678 2018-03-10 13:35:18 +0100 tttrackdialog: block ID appended with blank\n" \
"13677 2018-03-10 11:19:18 +0100 mvtrack: tracelevel changed for init sensors\n" \
"13676 2018-03-10 08:45:15 +0100 block: do not check in case of lock for crossing if the locking ID is an existing loco\n" \
"13675 2018-03-10 08:26:32 +0100 renderer: use transparent brush for drawing the turntable outer circle\n" \
"13674 2018-03-10 07:57:18 +0100 timetabledlg: added the nowrap td attribute\n" \
"13673 2018-03-09 10:31:48 +0100 seltab: remove system event listener on delete\n" \
"13672 2018-03-09 10:28:37 +0100 tt: report new positions to clients (dsd2010)\n" \
"13671 2018-03-08 15:11:13 +0100 rocrailinidialog: layout correction signal on automatic tab\n" \
"13670 2018-03-08 15:04:30 +0100 rocrailinidlialog: replaced text fields with spinbuttons in case of integer values on the automatic tab\n" \
"13669 2018-03-08 11:55:34 +0100 rocrailinidlg: reorganised auto tab\n" \
"13668 2018-03-08 11:20:34 +0100 dsd2010: swapped the rotation direction\n" \
"13667 2018-03-08 07:20:38 +0100 gotodlg: use title viablocks in case of multisel\n" \
"13666 2018-03-07 19:08:02 +0100 dsd2010: sleep little baby sleep\n" \
"13665 2018-03-07 16:33:01 +0100 guiframe: table menu without sub menus\n" \
"13664 2018-03-07 15:49:30 +0100 guiframe: reorganised table menu\n" \
"13663 2018-03-07 14:54:20 +0100 stage: velocity fix at in\n" \
"13662 2018-03-07 14:18:09 +0100 guiframe: reorganised menu tables\n" \
"13661 2018-03-07 11:24:07 +0100 guiframe: table menu reorganised\n" \
"13660 2018-03-07 11:09:13 +0100 guiframe: table menu reorganised\n" \
"13659 2018-03-07 10:34:00 +0100 seltabdlg: index tab added\n" \
"13658 2018-03-07 08:26:57 +0100 translation merge: de, nl, pt_BR, sr_latin\n" \
"13657 2018-03-06 10:04:16 +0100 app: give the clients some more time to disconnect after a shutdown\n" \
"13656 2018-03-06 08:47:35 +0100 routedialog: use the gotodlg for selecting crossing blocks\n" \
"13655 2018-03-06 07:52:01 +0100 rocnetnodedlg: show I2C group in decimal also\n" \
"13654 2018-03-05 15:53:00 +0100 bidib: removed obsolete iocfg flag\n" \
"13653 2018-03-05 15:33:57 +0100 loc, lindesection: nraxis and axle wait added\n" \
"13652 2018-03-05 13:44:45 +0100 cardlg: nraxis added to the details\n" \
"13651 2018-03-05 12:07:47 +0100 mcs2: reader thread set to high priority in case of serial communication\n" \
"13650 2018-03-05 11:20:38 +0100 railwaylinedlg: layout corrections and translations\n" \
"13649 2018-03-05 10:59:57 +0100 mcs2: cc-schnitte support minor improvement\n" \
"13648 2018-03-05 08:29:11 +0100 rocnetnodedlg: button for linking decoder\n" \
"13647 2018-03-04 11:48:44 +0100 bidibprogdlg: use switch commands in case of porttype servo\n" \
"13646 2018-03-04 11:39:55 +0100 bidib: use switch command for testing a servo\n" \
"13645 2018-03-04 11:22:53 +0100 bidib programming dialog: removed accessory\n" \
"13644 2018-03-04 08:58:56 +0100 translation merge: de, nl, sl, pt_BR\n" \
"13643 2018-03-04 08:31:05 +0100 stage: crossing flag at isFree to beable to run through without length\n" \
"13642 2018-03-03 14:27:20 +0100 locationdlg: use linesections in combo\n" \
"13641 2018-03-03 14:06:15 +0100 railwayline dialog: wip\n" \
"13640 2018-03-03 11:40:28 +0100 stage: check section locoid also if length > 0\n" \
"13639 2018-03-03 11:18:23 +0100 swdlg: set fake id for some command stations like dccpp\n" \
"13638 2018-03-03 10:54:19 +0100 virtual: sod generator active at protver=1\n" \
"13637 2018-03-03 10:49:19 +0100 model,fback: only lookup an offset variable if set to avoid waisting time\n" \
"13636 2018-03-03 08:09:10 +0100 virtual: use a thread for reporting SoD events\n" \
"13635 2018-03-03 07:50:43 +0100 virtual: report 1024 sensor at SoD in case version ist set to 2\n" \
"13634 2018-03-03 07:44:55 +0100 virtual: removed 1024 sensor reports at SoD\n" \
"13633 2018-03-02 14:30:58 +0100 ecos: trace corrections\n" \
"13632 2018-03-02 14:26:02 +0100 ecos: ignore set oid 1000+ reply\n" \
"13631 2018-03-02 13:37:19 +0100 ecos: recovery fix\n" \
"13630 2018-03-02 10:32:19 +0100 stage: allow crossing if empty\n" \
"13629 2018-03-02 08:22:07 +0100 xmlscript: fix for connect OR\n" \
"13628 2018-03-01 15:10:05 +0100 schedulegraph: use section as selection\n" \
"13627 2018-03-01 14:54:52 +0100 wrapper: section attribute added in schedule\n" \
"13626 2018-03-01 14:28:46 +0100 bidibdlg: cleaned up\n" \
"13625 2018-02-28 15:41:41 +0100 locations: use independet position units\n" \
"13624 2018-02-28 15:05:54 +0100 rocnetnode: not removing ttyAMA0 as console: Admin job.\n" \
"13623 2018-02-28 11:53:28 +0100 locationsdlg: changed kmpos label\n" \
"13622 2018-02-28 11:34:36 +0100 location: added railway section and changed kmpos in hmpos\n" \
"13621 2018-02-28 09:24:25 +0100 locationsdlg: added modify button on the index tab\n" \
"13620 2018-02-28 08:46:18 +0100 locationdlg: show kmpos in the position column\n" \
"13619 2018-02-27 21:40:22 +0100 rocview: added the preview flag for GTK filedialog\n" \
"13618 2018-02-27 15:20:46 +0100 schedulegraph: use kmpos in case all locations are set\n" \
"13617 2018-02-27 13:38:32 +0100 xmlscript: optional connect attribute for if statement for connection condition, state and class; default and.\n" \
"13616 2018-02-27 11:50:36 +0100 bidib: update product list\n" \
"13615 2018-02-27 08:04:51 +0100 xmlscript: revert; previous version is OK\n" \
"13614 2018-02-27 07:56:18 +0100 xmlscript: regard the not alltrue flag in case of testing the state argument\n" \
"13613 2018-02-26 21:58:11 +0100 bidibidentdlg: revert disable programming tabs\n" \
"13612 2018-02-26 14:22:01 +0100 location: kmpos option added\n" \
"13611 2018-02-26 10:46:05 +0100 wmz21: switch command broadcast to the apps\n" \
"13610 2018-02-25 19:37:49 +0100 xmlscript. weather command support added\n" \
"13609 2018-02-25 18:11:54 +0100 loc: set flag to signal of function with own address\n" \
"13608 2018-02-25 12:17:15 +0100 xmlscript: replace vars in nodes recursive\n" \
"13607 2018-02-25 11:09:11 +0100 wmz21: broadcast switch/output events to the devices\n" \
"13606 2018-02-25 10:38:03 +0100 wmz21: broadcast loco events to the connected devices\n" \
"13605 2018-02-25 08:49:57 +0100 translation merge: nl, de, pt_BR\n" \
"13604 2018-02-24 14:44:40 +0100 model: stop looking for a fitting schedule entry if the given index is out of range\n" \
"13603 2018-02-23 11:55:11 +0100 stage: reject reserve as crossing block\n" \
"13602 2018-02-23 08:12:18 +0100 action: support for loc state automatic added\n" \
"13601 2018-02-23 08:10:50 +0100 loc,actionctrldlg: state automatic added which ist NOT manual\n" \
"13600 2018-02-22 22:02:45 +0100 jssupport: provide the throttle list for update events\n" \
"13599 2018-02-22 14:37:48 +0100 cbus,loc: send release command to the CS\n" \
"13598 2018-02-21 12:30:52 +0100 schedulegraphdlg: select first group automatically at open\n" \
"13597 2018-02-21 11:09:00 +0100 schedulegraph: try to scroll the timenow line into the Y middle\n" \
"13596 2018-02-21 10:47:04 +0100 schedulegraph: show current train location\n" \
"13595 2018-02-21 10:20:55 +0100 wmz21: fix for screwing up the locoinfo\n" \
"13594 2018-02-21 09:22:12 +0100 wmz21: extra support for Z21 App\n" \
"13593 2018-02-21 08:41:14 +0100 loc: publish scheduleinithour to the clients\n" \
"13592 2018-02-21 07:58:16 +0100 jssupport: open/close throttldlg\n" \
"13591 2018-02-20 17:23:00 +0100 action: check state idle,wait,auto,run,manual\n" \
"13590 2018-02-20 15:57:29 +0100 schedulegraph: show loco in regular stop (wip)\n" \
"13589 2018-02-20 15:12:06 +0100 translation for schedulegraph added (Graphic timetable)\n" \
"13588 2018-02-20 15:07:42 +0100 schedulesgraphdlg: translations and titlebar\n" \
"13587 2018-02-20 14:41:51 +0100 schedulegraphdlg: new, wip\n" \
"13586 2018-02-20 11:21:17 +0100 wrapper: jsmap throttle attribute\n" \
"13585 2018-02-20 10:36:04 +0100 wmz21: Z21 App support\n" \
"13584 2018-02-20 09:44:34 +0100 wmz21: z21 app support\n" \
"13583 2018-02-20 08:48:29 +0100 schedulegraph: skip none regular stops\n" \
"13582 2018-02-20 08:47:51 +0100 jssupport: throttle action added\n" \
"13581 2018-02-19 15:45:51 +0100 schedulegraph: white location label backgound color\n" \
"13580 2018-02-19 15:20:32 +0100 schedulegraph: floating location labels\n" \
"13579 2018-02-19 15:03:22 +0100 schedulegraph: scale factor selection added\n" \
"13578 2018-02-19 14:17:47 +0100 location: skip last schedule entries\n" \
"13577 2018-02-19 14:05:13 +0100 schedulegraph: use graphicalcontext at a later point\n" \
"13576 2018-02-19 12:25:23 +0100 schedulegraph: use graphiccontext for the lines\n" \
"13575 2018-02-19 11:29:21 +0100 loc: action state manual added\n" \
"13574 2018-02-19 10:59:39 +0100 schedulegraph: show trainID\n" \
"13573 2018-02-19 10:46:03 +0100 schedulegraph: fix for time over hour, show trainID\n" \
"13572 2018-02-19 09:04:13 +0100 schedulegtraph: color added, fix for time through the hour\n" \
"13571 2018-02-19 07:55:15 +0100 loc: isState run added\n" \
"13570 2018-02-18 15:36:17 +0100 schedualgraph: line thickness set to 1\n" \
"13569 2018-02-18 15:08:51 +0100 schedulegraph: show group\n" \
"13568 2018-02-18 14:10:09 +0100 virtual: disable fake state information in case booster UID is not set\n" \
"13567 2018-02-18 14:03:42 +0100 guiframe: save show flag\n" \
"13566 2018-02-18 11:59:39 +0100 powerctrldlg: fix for right/left click on grid header\n" \
"13565 2018-02-18 10:02:04 +0100 schedulegraph: time from up to down\n" \
"13564 2018-02-18 09:21:24 +0100 SLIM-COLOR: fix for turnoutleft-t-route\n" \
"13563 2018-02-18 08:35:25 +0100 SLIM: fix for turnout left turn occ\n" \
"13562 2018-02-18 08:02:11 +0100 actionsctrldlg: added state idel,wait,auto to the loco type\n" \
"13561 2018-02-17 10:36:27 +0100 actionctrldlg: changed person state into local\n" \
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
"13510 2018-02-06 07:14:48 +0100 dsd2010: sound bit fix (Sven)\n" \
"13509 2018-02-05 16:06:15 +0100 location: substate for platform actions\n" \
"13508 2018-02-05 14:55:28 +0100 dsd2010: report position + 1\n" \
"13507 2018-02-05 14:45:47 +0100 dsd2010: fix for sending data\n" \
"13506 2018-02-05 14:36:40 +0100 dsd2010: trace level correction for read bytes\n" \
"13505 2018-02-05 14:22:36 +0100 dsd2010: use addr and bus reported by the commands\n" \
"13504 2018-02-05 13:16:26 +0100 dsd2010: pause between send bytes\n" \
"13503 2018-02-05 13:10:53 +0100 dsd2010: only report new positions\n" \
"13502 2018-02-05 13:10:03 +0100 dsd2010: only report new positions\n" \
"13501 2018-02-05 12:55:41 +0100 dsd2010: only report new errors\n" \
"13500 2018-02-05 11:50:10 +0100 dsd2010: goto and direction commands added\n" \
"13499 2018-02-05 11:23:52 +0100 dsd2010: library added for rs232 support\n" \
"13498 2018-02-05 08:46:23 +0100 mcs2,mgbox: use constants for reporting switch state\n" \
"13497 2018-02-05 08:44:53 +0100 mgbox: fix for reporting the switch state\n" \
"13496 2018-02-05 08:40:36 +0100 switch: revert field correction\n" \
"13495 2018-02-05 08:37:14 +0100 switch: fix for take over the field state\n" \
"13494 2018-02-05 08:24:07 +0100 blockgroup: first check all condition blocks if one is already reserved to avoid multiple block reservations\n" \
"13493 2018-02-04 09:13:27 +0100 blockgroup: only lock a reserve block if its not already locked\n" \
"13492 2018-02-04 08:43:31 +0100 block: do not save the from block ID in case its not set (Lock by block group)\n" \
"13491 2018-02-03 10:58:11 +0100 tt: use decoder track mapping for type onestep\n" \
"13490 2018-02-03 10:44:39 +0100 timetables: use initial hour for lookup a schedule entry in case of hourly schedules\n" \
"13489 2018-02-03 08:54:39 +0100 tt: onestep support (wip)\n" \
"13488 2018-02-02 15:47:04 +0100 location: check the reporting loco at update if it has already passed this location\n" \
"13487 2018-02-02 13:08:20 +0100 tt: onestep bus added to the commands\n" \
"13486 2018-02-02 09:30:22 +0100 http: start the 5 demo minutes for rocweb after the first connection\n" \
"13485 2018-02-02 08:51:04 +0100 tt: fix for sound on/off command\n" \
"13484 2018-02-02 08:33:34 +0100 tt: onestep support added (wip)\n" \
"13483 2018-02-01 17:20:38 +0100 planpanel: check the tabindex if its not > nr pages\n" \
"13482 2018-02-01 14:07:58 +0100 location: reset lcid,lcbkid at cloning hourly schedules\n" \
"13481 2018-02-01 13:51:59 +0100 location: reset delay for cloned hourly schedules\n" \
"13480 2018-02-01 11:17:00 +0100 openhab: output brightness support added\n" \
"13479 2018-02-01 08:27:30 +0100 model: write occ.xml only if the occupancy filename is set\n" \
"13478 2018-02-01 08:18:31 +0100 location: fix for overwriting the schedule delay\n" \
"13477 2018-01-31 11:36:43 +0100 openhab: output support on/off\n" \
"13476 2018-01-31 10:55:35 +0100 http: rocweb port check fix\n" \
"13475 2018-01-30 11:55:48 +0100 guiframe: help menu reduced\n" \
"13474 2018-01-30 08:28:47 +0100 bidib: save sensor state also on multi sensor messages\n" \
"13473 2018-01-30 08:18:42 +0100 translation merge: sl, sr_latin, en_GB, de, nl, pt_BR\n" \
"13472 2018-01-29 22:16:10 +0100 bidib: always process sensor reports in case of occupied\n" \
"13471 2018-01-29 14:11:14 +0100 file: skip creating the drive letter directory under windows\n" \
"13470 2018-01-29 09:50:22 +0100 wdir: do not skip directories\n" \
"13469 2018-01-29 08:30:04 +0100 rocweb: fix for XS and modular layouts\n" \
"13468 2018-01-28 15:31:00 +0100 rocs: check for _WIN32 and for _WIN64\n" \
"13467 2018-01-28 14:49:10 +0100 import/export workspace messages\n" \
"13466 2018-01-28 12:10:42 +0100 rocview: import workspace and go\n" \
"13465 2018-01-28 11:37:31 +0100 rocview: export active workspace\n" \
"13464 2018-01-27 07:58:18 +0100 switch: exclude from analyse option added\n" \
"13463 2018-01-27 07:47:36 +0100 bidib: do not report aspect in case > 127\n" \
"13462 2018-01-27 07:30:10 +0100 bidib: only reset manual operated accessory if aspect < 128\n" \
"13461 2018-01-26 11:26:05 +0100 rocrailinidialog: show warning and set the rocweb port to zero in case the http service port conflicts with the rocweb port\n" \
"13460 2018-01-26 09:23:22 +0100 http: do not start the Rocweb service if the port is the same as the HTTP service\n" \
"13459 2018-01-26 08:49:31 +0100 bidib: option to reset last known accessory state in case of a manual operation\n" \
"13458 2018-01-25 16:46:45 +0100 cbus: warning on PADA addressing -> must be FLAT\n" \
"13457 2018-01-25 15:59:26 +0100 location: corrections for timetable formating\n" \
"13456 2018-01-24 11:20:26 +0100 schedule: reamrk field added to the entries\n" \
"13455 2018-01-24 09:27:41 +0100 mqtt, clntcon: check if the socket object is valid before using it\n" \
"13454 2018-01-24 09:02:18 +0100 http: fix for c style comment\n" \
"13453 2018-01-24 08:39:06 +0100 http: fix for draining cpu in case os a rocweb connection\n" \
"13452 2018-01-24 08:17:47 +0100 clntcon: wait for the mqtt socket mutex before closing the socket if broken\n" \
"13451 2018-01-23 14:27:25 +0100 upnpdlg: add hue bridge\n" \
"13450 2018-01-23 10:30:11 +0100 timetabledlg: html extended with a via column\n" \
"13449 2018-01-22 13:31:17 +0100 demoplan: removed ZEP\n" \
"13448 2018-01-22 11:50:38 +0100 upnpdlg: removed unused apply button\n" \
"13447 2018-01-22 11:17:33 +0100 upnp: wip\n" \
"13446 2018-01-22 10:38:15 +0100 location: attribute timeframe added to the schedule table entries\n" \
"13445 2018-01-22 09:29:06 +0100 location: add time frame to the schedule entries\n" \
"13444 2018-01-21 16:09:08 +0100 upnpdlg: clear tree on discover\n" \
"13443 2018-01-21 15:21:12 +0100 upnp: smarthome icon\n" \
"13442 2018-01-21 14:08:14 +0100 upnpdlg: wip\n" \
"13441 2018-01-21 10:50:18 +0100 upnp: discover and dialog (WIP)\n" \
"13440 2018-01-20 18:13:32 +0100 rocweb: update speed always on event\n" \
"13439 2018-01-20 17:57:42 +0100 upnp: more realistic service description\n" \
"13438 2018-01-20 15:50:43 +0100 upnp: WIP\n" \
"13437 2018-01-20 14:16:09 +0100 wrapper: removed unused gotoblockgo commando\n" \
"13436 2018-01-20 13:40:28 +0100 importantchanges: xml parser extended\n" \
"13435 2018-01-20 13:17:01 +0100 rocs: added optional support for using xml element values\n" \
"13434 2018-01-20 08:33:31 +0100 wserial: use getlasterror in case the clearcommerror did not succeed\n" \
"13433 2018-01-19 15:48:56 +0100 upnp: basic description file\n" \
"13432 2018-01-19 15:46:43 +0100 upnp: wip\n" \
"13431 2018-01-19 13:38:58 +0100 blockgroupingdlg: label correction from to first block\n" \
"13430 2018-01-19 10:37:23 +0100 blockgroup: optional enterside for conditions\n" \
"13429 2018-01-19 08:32:44 +0100 blockgroupingdlg: label correction from to first block\n" \
"13428 2018-01-18 12:42:12 +0100 dinamo: fix for packet length in case of a long DCC address\n" \
"13427 2018-01-18 10:27:32 +0100 model: fix for crash in case of a pre-reservation\n" \
"13426 2018-01-18 08:39:49 +0100 rocomp: fix for loco address >= 128\n" \
"13425 2018-01-18 08:19:38 +0100 model: check in case of a schedule if a block in a location is already reserved\n" \
"13424 2018-01-17 14:28:28 +0100 blockgroup: regard loco schedule in case of reserve\n" \
"13423 2018-01-17 13:55:19 +0100 upnp: wip\n" \
"13422 2018-01-17 13:12:00 +0100 upnp: wip\n" \
"13421 2018-01-17 09:05:44 +0100 usocket: mac address OSX\n" \
"13420 2018-01-16 18:06:21 +0100 upnp: show udp sender\n" \
"13419 2018-01-16 16:55:26 +0100 renaming smarthome to upnp\n" \
"13418 2018-01-16 14:55:51 +0100 smarthome: wip\n" \
"13417 2018-01-16 08:15:12 +0100 translation merge: de, nl, sl\n" \
"13416 2018-01-15 16:49:50 +0100 showid: more objects\n" \
"13415 2018-01-15 15:13:46 +0100 wrapper: item showid attribute\n" \
"13414 2018-01-15 14:25:13 +0100 messages: show ID translation added\n" \
"13413 2018-01-15 14:13:38 +0100 output: showid option added\n" \
"13412 2018-01-15 11:10:07 +0100 timetable: no stay on top\n" \
"13411 2018-01-15 09:42:00 +0100 timetabledlg: minimize option added\n" \
"13410 2018-01-15 08:38:16 +0100 timetabledlg: translations added\n" \
"13409 2018-01-15 08:12:25 +0100 timetabledlg: save size and position\n" \
"13408 2018-01-14 16:57:29 +0100 automatically popup timetables for locations\n" \
"13407 2018-01-14 10:37:50 +0100 location: format of timetable adjusted\n" \
"13406 2018-01-14 09:34:58 +0100 textdialog: layout correction\n" \
"13405 2018-01-14 09:02:26 +0100 locationsdlg: layout change for options\n" \
"13404 2018-01-14 08:21:30 +0100 location: schedule table html corrections\n" \
"13403 2018-01-14 07:45:54 +0100 planpanel: added the missing capturelost function\n" \
"13402 2018-01-13 18:00:30 +0100 location: optional html format for actions\n" \
"13401 2018-01-13 15:02:10 +0100 text: html option added\n" \
"13400 2018-01-13 13:19:17 +0100 location: via fix for cloned hourly schedules\n" \
"13399 2018-01-13 12:34:30 +0100 text: monospace option added\n" \
"13398 2018-01-13 11:40:58 +0100 messages: publish added\n" \
"13397 2018-01-13 11:39:08 +0100 location: action option added\n" \
"13396 2018-01-13 07:28:16 +0100 tracedlg: type OLcDrive added\n" \
"13395 2018-01-12 14:13:53 +0100 makefile: install demoplan\n" \
"13394 2018-01-12 14:00:29 +0100 bidib: POM support for M4/MfX added\n" \
"13393 2018-01-12 11:15:27 +0100 bidib: trace correction for output\n" \
"13392 2018-01-12 07:58:05 +0100 actionsctrldlg: loco state home added\n" \
"13391 2018-01-12 07:52:17 +0100 loc: state home added\n" \
"13390 2018-01-11 10:28:33 +0100 schedules: fix for update schedule table in case of max delay\n" \
"13389 2018-01-11 09:15:56 +0100 location: schedule table entry maxdelay added\n" \
"13388 2018-01-11 09:02:44 +0100 bidib: support added for more protocol formats\n" \
"13387 2018-01-10 18:25:24 +0100 z21: 2018 and commented out some dyn tests\n" \
"13386 2018-01-10 18:21:55 +0100 2018: as every year\n" \
"13385 2018-01-10 17:51:19 +0100 translation merge: sr_latin, pt_BR, sl, de, nl\n" \
"13384 2018-01-10 14:01:48 +0100 route: optional yellow in case of white aspect\n" \
"13383 2018-01-10 11:24:08 +0100 route: added missing flag for cargo condition\n" \
"13382 2018-01-10 10:32:35 +0100 bidib: extra trace line on location report\n" \
"13381 2018-01-09 15:35:26 +0100 loc: update schedule tables in case a schedule is activated\n" \
"13380 2018-01-09 14:09:58 +0100 location: not lookup a loco for a schedule at init\n" \
"13379 2018-01-09 14:00:51 +0100 location: not evaluate\n" \
"13378 2018-01-09 13:50:39 +0100 route: include/exclude conditions option added\n" \
"13377 2018-01-09 13:37:44 +0100 control: list up the build origin in the issue.txt\n" \
"13376 2018-01-09 11:20:09 +0100 makewin64-full.sh: fix for commandline parameter\n" \
"13375 2018-01-09 10:51:03 +0100 location: fix for compare schedule entry time with model time\n" \
"13374 2018-01-09 07:30:45 +0100 dccpp: do not initialise DTR on connect to avoid a reset\n" \
"13373 2018-01-08 17:48:01 +0100 makefile: version fix for native win\n" \
"13372 2018-01-08 16:09:59 +0100 location: regard hours < model time to skip\n" \
"13371 2018-01-08 15:38:24 +0100 loc: set light attribute incase an event ist dispatched to the operator\n" \
"13370 2018-01-08 10:49:12 +0100 location: keep valid schedule entries in the list to save additional information\n" \
"13369 2018-01-08 10:46:41 +0100 routedialog: layout correction\n" \
"13368 2018-01-08 10:34:05 +0100 routedialog: layout and label corrections\n" \
"13367 2018-01-08 08:30:02 +0100 makefile: do not overwrite the version.h with the build of rocnetnode\n" \
"13366 2018-01-08 08:01:34 +0100 version.h: dummy\n" \
"13365 2018-01-07 17:51:02 +0100 a tip from the build\n" \
"13364 2018-01-07 17:49:52 +0100 a tip from the build\n" \
"13363 2018-01-07 17:41:58 +0100 a tip from the build\n" \
"13362 2018-01-07 14:27:11 +0100 loc: extra trace in case of direct swap placing\n" \
"13361 2018-01-07 13:36:46 +0100 block: inform fifo of a red signal event\n" \
"13360 2018-01-07 08:19:09 +0100 operatordlg: sorting columns added\n" \
"13359 2018-01-06 16:22:16 +0100 loc: isHome function added\n" \
"13358 2018-01-06 12:30:18 +0100 in: ignore wait in case next destination is reserved and the engine type is automobile\n" \
"13357 2018-01-06 10:50:17 +0100 location: fix for updating the wrong hourly schedule\n" \
"13356 2018-01-06 10:42:44 +0100 location: start time added in the schedule table entries\n" \
"13355 2018-01-06 09:04:37 +0100 route: only set the route after lock just one time\n" \
"13354 2018-01-06 08:35:03 +0100 blockgroup: fix for overwriting the groupfree flag with conditions if its not free\n" \
"13353 2018-01-06 08:14:00 +0100 route: fix for not type condition\n" \
"13352 2018-01-06 08:05:12 +0100 location: show current loco, block and delay in the dump\n" \
"13351 2018-01-05 14:51:48 +0100 location: no time update on loco event\n" \
"13350 2018-01-05 11:29:24 +0100 location: schedule table skip old schedule times respecting delay\n" \
"13349 2018-01-05 11:21:30 +0100 http: sleep 10ms if no post was available for rocweb\n" \
"13348 2018-01-05 11:04:02 +0100 http: fix for rocweb support\n" \
"13347 2018-01-05 10:49:48 +0100 route: conditions extended with not type and class\n" \
"13346 2018-01-04 15:30:12 +0100 location: check for future hourly schedules if the exceed hour 23 and subtract 24\n" \
"13345 2018-01-04 14:19:45 +0100 location: regard schedule delay in future test\n" \
"13344 2018-01-04 14:06:33 +0100 block: fifo speed corrections\n" \
"13343 2018-01-04 13:20:32 +0100 guiapp: splashscreen without border\n" \
"13342 2018-01-04 11:21:33 +0100 location: skip schedule entries which are in the past\n" \
"13341 2018-01-04 10:36:21 +0100 control: hour and minute attribute added in the clock sync event\n" \
"13340 2018-01-04 10:21:48 +0100 route: check first if the route is reserved before closed\n" \
"13339 2018-01-04 08:56:22 +0100 block, fback: take over new qos if its not equals with the las reported\n" \
"13338 2018-01-04 08:49:55 +0100 block: traceline for QoS corrected\n" \
"13337 2018-01-04 08:44:39 +0100 block: statistics added for QoS\n" \
"13336 2018-01-03 16:42:40 +0100 location, loc: lcbkid added es current or entering block\n" \
"13335 2018-01-03 16:23:38 +0100 loconet, z21: fix for reporting QoS to an uknown sensor address\n" \
"13334 2018-01-03 14:56:42 +0100 location: sort schedule table by depart time\n" \
"13333 2018-01-03 11:32:04 +0100 update schedule tables in case of a delay\n" \
"13332 2018-01-03 08:47:41 +0100 operator: train combination added\n" \
"13331 2018-01-03 08:46:58 +0100 sensorevents: QoS column added\n" \
"13330 2018-01-02 16:54:34 +0100 location: copy/paste fix check for free block (Achim)\n" \
"13329 2018-01-02 16:34:59 +0100 carpanel, trainpanel: no init at create\n" \
"13328 2018-01-02 11:18:57 +0100 carpanel, trainpanel: fix for column selection\n" \
"13327 2018-01-02 10:48:39 +0100 location: add one hour ahead for hourly schedules\n" \
"13326 2018-01-02 10:26:49 +0100 location: set id in the schedulettable node\n" \
"13325 2018-01-02 10:18:07 +0100 location: publich schedule table\n" \
"13324 2018-01-02 09:39:45 +0100 location: update schedulle table every model minute\n" \
"13323 2018-01-02 07:29:32 +0100 wiki demo plan correction\n" \
"13322 2018-01-01 15:30:57 +0100 schedule: trainnr attribute added\n" \
"13321 2018-01-01 12:05:59 +0100 location: init schedule table (wip)\n" \
"13320 2017-12-31 15:39:25 +0100 location: init schedule table (wip)\n" \
"13319 2017-12-31 15:11:48 +0100 NodeOp.base.toString(scte)\n" \
"13318 2017-12-31 15:02:29 +0100 location: init schedule table (wip)\n" \
"13317 2017-12-31 13:17:43 +0100 route: AND condition option block: platform field added location: schedule table (wip)\n" \
"13316 2017-12-31 08:33:47 +0100 columndlg: check for which tab the config is for the right labels\n" \
"13315 2017-12-30 14:41:54 +0100 block: check last fifo physical IN guiframe: auto reset all also available in none auto mode\n" \
"13314 2017-12-30 09:27:35 +0100 lc, meter: fix for maxkmh in speedometer\n" \
"13313 2017-12-30 08:20:08 +0100 stagedlg: fix for evaluating IDs on the general tab\n" \
"13312 2017-12-30 08:02:00 +0100 model: crash fix for init field\n" \
"13311 2017-12-29 16:56:31 +0100 replaced internal cargo type person with local\n" \
"13310 2017-12-29 15:22:18 +0100 z21, loconet: fix for kmh/qos values > 127\n" \
"13309 2017-12-29 13:54:31 +0100 translation merge: sl, pt_BT, de, sr_latin, nl\n" \
"13308 2017-12-29 13:42:32 +0100 messages: removed unused 'person' translation\n" \
"13307 2017-12-29 12:16:58 +0100 block: added missing post mutex in case virtual block could not unlock slave block (Achim)\n" \
"13306 2017-12-29 11:39:57 +0100 z21: removed test code for qos\n" \
"13305 2017-12-29 11:38:59 +0100 z21, locnet: crash fix for evaluating multi sense QosQ\n" \
"13304 2017-12-29 11:04:29 +0100 textdialog: fix for updating color buttons\n" \
"13303 2017-12-29 08:15:37 +0100 renamed loconet railcom specs file\n" \
"13302 2017-12-28 19:03:07 +0100 actionsctrldlg: revert replace setstringselection\n" \
"13301 2017-12-28 15:42:34 +0100 loconet: dyn correction\n" \
"13300 2017-12-28 15:40:25 +0100 z21: correction for qos\n" \
"13299 2017-12-28 13:59:50 +0100 block, signal: trace level corrections\n" \
"13298 2017-12-28 11:32:57 +0100 block: remove fifo flags from reset trigs\n" \
"13297 2017-12-28 11:14:20 +0100 block: fifoDepart function added to be able to stop the follower at IN\n" \
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
"13246 2017-12-19 15:41:03 +0100 mqtt: trace level corrections\n" \
"13245 2017-12-19 15:31:57 +0100 rocnetnode: support for mqtt (WIP)\n" \
"13244 2017-12-19 15:19:08 +0100 blockgroup: removed the pre-reserve option\n" \
"13243 2017-12-19 14:48:37 +0100 groupingdialog: use blockdialog for selecting free blocks\n" \
"13242 2017-12-19 11:26:25 +0100 blockgroup: reserve option extended\n" \
"13241 2017-12-19 09:48:33 +0100 blockgroup: set loco via block in case of prereserve\n" \
"13240 2017-12-19 08:57:02 +0100 output: param off added for light support\n" \
"13239 2017-12-18 16:13:07 +0100 bidib: position address correction (Achim)\n" \
"13238 2017-12-18 12:25:00 +0100 swdlg: allign corrections to save space\n" \
"13237 2017-12-18 11:54:55 +0100 blockgroup, block: remove prereservations on lock\n" \
"13236 2017-12-18 11:37:08 +0100 rocnet: use the original ini to add rocnetnodes\n" \
"13235 2017-12-18 11:31:10 +0100 roclcdr: speed corrections\n" \
"13234 2017-12-18 08:51:12 +0100 swdlg: protocol and port type added\n" \
"13233 2017-12-17 15:22:11 +0100 mqtt, rocnet: auto discovery\n" \
"13232 2017-12-17 13:20:13 +0100 guiapp: removed global pending exit flag\n" \
"13231 2017-12-17 11:46:38 +0100 swdlg: fix for bitmapbutton in header\n" \
"13230 2017-12-17 11:18:31 +0100 clntcon: extra trace incase broker info is send back\n" \
"13229 2017-12-17 11:03:56 +0100 clntcon: check BROKER-GET not with equals but start with\n" \
"13228 2017-12-17 08:39:07 +0100 dccpp: check for protocol NMRA-DCC\n" \
"13227 2017-12-17 08:16:20 +0100 connectiondialog: always set code values to overwrite the ABC label\n" \
"13226 2017-12-16 16:30:54 +0100 tt: set tt id for multiport type and dccpp\n" \
"13225 2017-12-16 14:52:15 +0100 clntcon: use only one socket for the broker discovery service\n" \
"13224 2017-12-16 14:41:29 +0100 clntcon: broker discovery use host and port at reply\n" \
"13223 2017-12-16 08:21:52 +0100 rocweb: support for road theme sensors\n" \
"13222 2017-12-15 17:14:20 +0100 clntcon: mqtt broker discovery service added\n" \
"13221 2017-12-15 09:28:04 +0100 rocview shutdown fix for deadlock\n" \
"13220 2017-12-15 08:14:57 +0100 rocview shutdown fix for deadlock\n" \
"13219 2017-12-15 08:05:37 +0100 thempropsdlg: title translation\n" \
"13218 2017-12-14 17:29:45 +0100 wrapper: default mqtt broker port number\n" \
"13217 2017-12-14 15:13:48 +0100 bidib: revert update messages because unknown types are used\n" \
"13216 2017-12-14 13:41:45 +0100 bidib: messages update\n" \
"13215 2017-12-14 12:09:07 +0100 tt: set id in switch commands for dccpp support\n" \
"13214 2017-12-14 11:19:41 +0100 rocview shutdown fix for deadlock\n" \
"13213 2017-12-14 09:25:06 +0100 guiframe: reset the option reconnect to server at open workspace\n" \
"13212 2017-12-14 09:05:34 +0100 guiapp: removed not needed semaphore at sending server commands\n" \
"13211 2017-12-13 15:33:22 +0100 extra traces on gui exit\n" \
"13210 2017-12-13 14:48:39 +0100 rcon: trace corrections\n" \
"13209 2017-12-13 14:18:27 +0100 rocnetnode: reset pi01 LEDs before shutdown/exit main\n" \
"13208 2017-12-13 13:50:10 +0100 guiframe: removed asking shutdown all rocnet node because this is already handled in the rocnet setup\n" \
"13207 2017-12-13 13:35:20 +0100 themepropsdlg: close handler added\n" \
"13206 2017-12-13 11:56:47 +0100 rocweb: logo without shade\n" \
"13205 2017-12-13 10:11:24 +0100 themepropsdlg: translations added\n" \
"13204 2017-12-13 09:16:25 +0100 rocweb: show clock on URL parameter z=1000\n" \
"13203 2017-12-13 08:35:07 +0100 clntcon, guiapp: switching workspace fix\n" \
"13202 2017-12-12 11:09:29 +0100 rocview: fix for changing workspace\n" \
"13201 2017-12-11 12:10:17 +0100 themepropsdlg: show color\n" \
"13200 2017-12-11 11:41:12 +0100 themepropsdlg added\n" \
"13199 2017-12-11 09:41:46 +0100 model: check actions on stop all locos\n" \
"13198 2017-12-10 16:10:27 +0100 swdlg: gray icons\n" \
"13197 2017-12-10 15:29:59 +0100 swdlg: layout corrections\n" \
"13196 2017-12-10 13:51:24 +0100 swdlg: alt icons\n" \
"13195 2017-12-10 13:13:08 +0100 swctrldlg: icon\n" \
"13194 2017-12-10 13:01:10 +0100 keyboarddlg: removed unused control key\n" \
"13193 2017-12-10 11:37:05 +0100 swdlg: restore position\n" \
"13192 2017-12-10 10:47:00 +0100 swdlg: combobox for IIDs\n" \
"13191 2017-12-10 10:27:26 +0100 swdlg: use bitmaps on buttons\n" \
"13190 2017-12-09 18:06:17 +0100 stage: stop all locos reverted\n" \
"13189 2017-12-09 11:17:34 +0100 rocctrl: ask before shutdown and EoD\n" \
"13188 2017-12-09 10:51:21 +0100 dialogs: keyboard support sensor, signal, text, output, action\n" \
"13187 2017-12-09 09:40:55 +0100 switchdialog: keyboard support\n" \
"13186 2017-12-09 08:54:03 +0100 routedialog: keyboard support\n" \
"13185 2017-12-09 08:27:08 +0100 finder: unsupported random option removed\n" \
"13184 2017-12-08 17:07:56 +0100 keyboarddlg: keep layout (toggle)button text\n" \
"13183 2017-12-08 16:25:32 +0100 keyboarddlg: revert set focus to the text field\n" \
"13182 2017-12-08 16:18:32 +0100 keyboarddlg: return focus to the text field after button click\n" \
"13181 2017-12-08 15:28:33 +0100 keyboard: DE layout support\n" \
"13180 2017-12-08 14:56:29 +0100 keyboard: alt, ctrl and layout added\n" \
"13179 2017-12-08 11:55:09 +0100 cardlg: keyboard support added\n" \
"13178 2017-12-08 11:29:24 +0100 keyboarddlg: put focus back after ok/cancel\n" \
"13177 2017-12-08 09:57:09 +0100 toolbardlg: fix for print option\n" \
"13176 2017-12-08 09:17:27 +0100 planpanel: use soft keyboard for asking item ID in case context on dclick is active\n" \
"13175 2017-12-08 08:40:46 +0100 rocrailinidialog: keyboard support (wip)\n" \
"13174 2017-12-07 21:15:16 +0100 connectiondialog: keyboard support added\n" \
"13173 2017-12-07 19:41:50 +0100 guiapp: fix for crash setting statis text out of event context\n" \
"13172 2017-12-07 19:09:32 +0100 blockdialog, locdialog: reset last focus pointer on page change\n" \
"13171 2017-12-07 17:09:40 +0100 locdialog: keyboard support (wip)\n" \
"13170 2017-12-07 14:20:46 +0100 item: always take over the loco blockenterside on update (TEST)\n" \
"13169 2017-12-07 13:51:37 +0100 stage: check manual signal if it should be reset\n" \
"13168 2017-12-07 13:39:36 +0100 toolbardlg: forum item added\n" \
"13167 2017-12-07 13:11:20 +0100 blockdialog: keyboard support\n" \
"13166 2017-12-07 12:33:31 +0100 keyboarddlg: layout correction -> US\n" \
"13165 2017-12-07 11:55:17 +0100 keyboarddlg: wip\n" \
"13164 2017-12-07 09:58:58 +0100 keyboard for touch: wip\n" \
"13163 2017-12-06 18:07:18 +0100 loconet lbtcp: recovery fix\n" \
"13162 2017-12-06 17:38:50 +0100 rocview: context menu on dclick option added for touch\n" \
"13161 2017-12-06 13:46:22 +0100 item: open/close stage exit on dclick\n" \
"13160 2017-12-06 08:45:14 +0100 stage: halt flag added for stop/start all locos\n" \
"13159 2017-12-05 19:26:30 +0100 blockdialog: allow main signals used as distant signals\n" \
"13158 2017-12-05 19:17:53 +0100 operatordlg: button added to open the props of the loco which the train is assigned to\n" \
"13157 2017-12-05 17:53:11 +0100 guiframe: close throttles on changing workspace\n" \
"13156 2017-12-05 16:39:43 +0100 trackdialog: remove index tab in case it should not be populated\n" \
"13155 2017-12-05 15:44:04 +0100 route: skip manual signal commands\n" \
"13154 2017-12-05 14:26:20 +0100 bidib: check run flag before attempting to (re)connect\n" \
"13153 2017-12-05 13:43:30 +0100 rocctrl: quit on shutdown event\n" \
"13152 2017-12-05 08:56:24 +0100 control: set var lcid on event timeout\n" \
"13151 2017-12-05 08:32:00 +0100 translation merge: de, sl, sr_latin, fr, en_GB, nl\n" \
"13150 2017-12-04 17:36:15 +0100 trace: level char for analyser\n" \
"13149 2017-12-04 16:41:03 +0100 analyse: added extra trace level and corrected trace level to avoid a flood of messages to the clients\n" \
"13148 2017-12-04 14:37:25 +0100 makefile: copy rocctrl icon\n" \
"13147 2017-12-04 14:32:46 +0100 rocctrl: own icon\n" \
"13146 2017-12-04 14:14:08 +0100 rocctrl: informall flag for system commands added\n" \
"13145 2017-12-04 13:21:00 +0100 rocctrl: menu and frame buttons equal text\n" \
"13144 2017-12-04 13:07:20 +0100 bigger font for buttons\n" \
"13143 2017-12-04 10:22:56 +0100 rocctrl: resume and virtual button added\n" \
"13142 2017-12-04 10:18:07 +0100 rocctrl: automate frame added\n" \
"13141 2017-12-03 22:01:14 +0100 rocctrl: quit on eod\n" \
"13140 2017-12-03 17:21:51 +0100 routedialog: multi delete\n" \
"13139 2017-12-03 17:00:28 +0100 rocctrl: clean up\n" \
"13138 2017-12-03 15:15:09 +0100 rocctrl: messagebox for python 2 and 3\n" \
"13137 2017-12-03 14:33:37 +0100 rocctrl: python 3...\n" \
"13136 2017-12-03 14:31:41 +0100 rocctrl: python 2 and 3 support (Liviu)\n" \
"13135 2017-12-03 12:46:58 +0100 rocrail makefile: added rocctrl\n" \
"13134 2017-12-03 12:41:01 +0100 RocControl desktop shortcut added\n" \
"13133 2017-12-03 12:28:58 +0100 rocctrl: automenu extended\n" \
"13132 2017-12-03 11:18:18 +0100 rocctrl: wip\n" \
"13131 2017-12-03 09:00:57 +0100 rocctrl: WIP\n" \
"13130 2017-12-02 22:37:29 +0100 rocctrl: WIP\n" \
"13129 2017-12-02 18:42:06 +0100 rocctrl: create read handler first after connect\n" \
"13128 2017-12-02 18:31:21 +0100 rocctrl: (WIP) a small Python utility for a small display\n" \
"13127 2017-12-01 16:16:14 +0100 rocview: extra status info at startup\n" \
"13126 2017-12-01 11:30:42 +0100 loc: fix for function timers > 27\n" \
"13125 2017-11-30 22:09:48 +0100 app: -pwr command line option added\n" \
"13124 2017-11-30 22:07:35 +0100 app: -pwr command line option added\n" \
"13123 2017-11-30 14:20:35 +0100 fback: not delete command node in case of redirection to the controller\n" \
"13122 2017-11-30 08:02:19 +0100 mvtrack: reset by xmlscript\n" \
"13121 2017-11-29 22:06:19 +0100 mvtrack: toString implemented\n" \
"13120 2017-11-29 18:55:46 +0100 disable properties in auto mode for stage too\n" \
"13119 2017-11-29 16:13:33 +0100 InnoSetup: do not popup if the destination directory already exist\n" \
"13118 2017-11-29 14:32:45 +0100 InnoSetup: do not popup if the destination directory already exist\n" \
"13117 2017-11-29 14:22:46 +0100 innosetup: do not use the previous install directory automatically\n" \
"13116 2017-11-29 12:52:27 +0100 item: fix for invalid locoID pointer\n" \
"13115 2017-11-29 11:51:24 +0100 mem: isValid function added\n" \
"13114 2017-11-28 17:23:59 +0100 item: try fix locoid mystery...\n" \
"13113 2017-11-28 17:22:35 +0100 locdialog: calibrate labels changed\n" \
"13112 2017-11-28 16:01:25 +0100 innosetup: skip ask for install directory if already installed\n" \
"13111 2017-11-28 11:50:49 +0100 wiki demo plan added\n" \
"13110 2017-11-28 11:34:08 +0100 car: use identifier instead of ident to be conform other objects\n" \
"13109 2017-11-28 09:37:50 +0100 demo/wiki plan options\n" \
"13108 2017-11-28 08:10:34 +0100 rpic: save and restore field bytes\n" \
"13107 2017-11-27 19:55:08 +0100 innosetup: demo or wiki plan choice\n" \
"13106 2017-11-27 13:58:30 +0100 block: trace correction for none matching code\n" \
"13105 2017-11-27 13:36:27 +0100 wait4event: release blockgroup on enter too (test)\n" \
"13104 2017-11-27 11:54:51 +0100 routedialog: popup for removing all autogen with questionmark\n" \
"13103 2017-11-27 11:17:42 +0100 routedialog: remove also tt generated flags\n" \
"13102 2017-11-27 09:57:35 +0100 locdialog: prev function group from 1 to 8\n" \
"13101 2017-11-27 08:12:53 +0100 loc: fix for function > 28 timers\n" \
"13100 2017-11-26 14:38:57 +0100 mvtrack: reset command support added\n" \
"13099 2017-11-26 13:52:02 +0100 fback: do not increase wheelcounter at command\n" \
"13098 2017-11-26 08:48:13 +0100 rocnetnode: init outputs at startup\n" \
"13097 2017-11-26 08:34:05 +0100 rocnetnode: check the new ini from SD before rename it to rocnetnode.ini\n" \
"13096 2017-11-25 19:00:46 +0100 makewin64-full.sh: only one task on the raspi\n" \
"13095 2017-11-25 18:13:47 +0100 roclcdr: free on enter/wheel count fixes\n" \
"13094 2017-11-25 17:05:21 +0100 roclcdr: free on enter/wheel count fixes\n" \
"13093 2017-11-25 15:09:49 +0100 rocnetnode: start of day fix\n" \
"13092 2017-11-25 14:06:33 +0100 rocnetnode: SoD traces added\n" \
"13091 2017-11-25 12:14:39 +0100 rocnetnode: trace errno text\n" \
"13090 2017-11-25 12:04:01 +0100 switch: automode option added for the controller\n" \
"13089 2017-11-25 11:27:08 +0100 switch: control option invert added\n" \
"13088 2017-11-25 10:01:14 +0100 rocnetnode: own default trace file name\n" \
"13087 2017-11-25 09:59:28 +0100 rpic: faster refresh rate\n" \
"13086 2017-11-25 09:43:52 +0100 rocnetnode: fix for unloading and removing the DCC generator\n" \
"13085 2017-11-25 08:32:41 +0100 rocnetnode: fix for unloading and removing the DCC generator\n" \
"13084 2017-11-24 20:14:09 +0100 rocnetnodedlg: prevent programming left channel position greater then the right poisition\n" \
"13083 2017-11-24 16:38:01 +0100 channeltunedlg: prevent setting left(off) position higher then the right(on) position\n" \
"13082 2017-11-24 15:15:54 +0100 switch control: wait until its in position before frre up the routes\n" \
"13081 2017-11-23 17:30:21 +0100 rocnetnode: comment in ansi style...\n" \
"13080 2017-11-23 17:28:23 +0100 rocnetnode: get hostaddress by socket\n" \
"13079 2017-11-23 17:09:46 +0100 rocnetnodedlg: fix for getting update info\n" \
"13078 2017-11-23 08:00:37 +0100 locoio: fix for resizing and save size&position\n" \
"13077 2017-11-22 13:28:52 +0100 makefile: Rocweb desktop added to target install\n" \
"13076 2017-11-22 11:34:48 +0100 rocnet: removed extra byte at switch commands\n" \
"13075 2017-11-22 11:27:56 +0100 guiapp: fix for show routes\n" \
"13074 2017-11-22 08:42:15 +0100 Rocweb desktop file added\n" \
"13073 2017-11-21 18:15:31 +0100 use wheelcount for free up previous block\n" \
"13072 2017-11-21 17:25:23 +0100 use wheelcount for free up previous block\n" \
"13071 2017-11-21 17:15:20 +0100 rpic: checksum byte offset correction\n" \
"13070 2017-11-21 15:09:45 +0100 cbus, loconet: dialog scrolling added\n" \
"13069 2017-11-21 13:32:34 +0100 innosetup: do not use the previous install directory automatically\n" \
"13068 2017-11-21 08:51:23 +0100 makewin64-full.sh: innosetup file variable\n" \
"13067 2017-11-21 08:39:47 +0100 rpic: fix for header and input checksum\n" \
"13066 2017-11-20 21:08:37 +0100 rpic: 100ms refresh rate\n" \
"13065 2017-11-20 20:54:01 +0100 rpic: wip\n" \
"13064 2017-11-20 20:05:24 +0100 rpic: wip\n" \
"13063 2017-11-20 19:21:20 +0100 rpic: wip\n" \
"13062 2017-11-20 18:15:59 +0100 rpic: wip\n" \
"13061 2017-11-20 17:04:46 +0100 guiapp: update route in case not visable\n" \
"13060 2017-11-20 16:34:23 +0100 innosetup: set support for 64bit only in case of 64bit build\n" \
"13059 2017-11-20 16:16:06 +0100 locdialog: use time_t instead of long\n" \
"13058 2017-11-20 15:55:30 +0100 basedlg: use time_t instead of long\n" \
"13057 2017-11-20 14:05:08 +0100 makefiles: native WIN64 support\n" \
"13056 2017-11-20 13:26:23 +0100 makefiles: native WIN64 support\n" \
"13055 2017-11-20 13:21:48 +0100 makefiles: native WIN64 support\n" \
"13054 2017-11-20 12:56:03 +0100 makefiles: native WIN64 support\n" \
"13053 2017-11-20 12:53:20 +0100 makefiles: native WIN64 support\n" \
"13052 2017-11-19 21:53:22 +0100 demo plan: fix for sensor events in block cb2 and sb2\n" \
"13051 2017-11-19 13:32:50 +0100 x64 innosetup support added\n" \
"13050 2017-11-19 13:30:27 +0100 x64 innosetup support added\n" \
"13049 2017-11-19 12:57:42 +0100 added architecture parameter to the inno setup template\n" \
"13048 2017-11-19 12:24:05 +0100 makewin64 update for raspi\n" \
"13047 2017-11-19 11:56:09 +0100 makewin64-full.sh script added\n" \
"13046 2017-11-19 11:19:39 +0100 rocs: isoDate and isoTime use time_t\n" \
"13045 2017-11-19 11:07:52 +0100 time_ use time_t\n" \
"13044 2017-11-19 10:51:12 +0100 time: use time_t instead of long\n" \

"log end";
