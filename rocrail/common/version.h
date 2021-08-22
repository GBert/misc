const int revisionnr = 
1723
;
const char* commithash = 
"85ead0b70911810b84582915e483a0e3c52ec1f8";
const char* origin = "LOCAL"
;
const char* revlog = 
"1723 2021-08-22 08:09:33 +0200 modbus: extended link/unlink traces\n" \
"1722 2021-08-21 11:46:24 +0200 base: gray correction\n" \
"1721 2021-08-21 11:09:41 +0200 guiframe: loco grid default text color in case of dark background\n" \
"1720 2021-08-21 08:46:38 +0200 ledbutton: check gray background option\n" \
"1719 2021-08-21 08:21:52 +0200 rocview: gray background more dark\n" \
"1718 2021-08-20 16:10:09 +0200 model: loop over all outputs on event to find same addresses\n" \
"1717 2021-08-18 18:36:11 +0200 mbus,virtual: set index in response in case of write\n" \
"1716 2021-08-18 18:10:52 +0200 mfx decspecs from Stephan\n" \
"1715 2021-08-18 08:02:24 +0200 translation merge: de, fr, nl, sl\n" \
"1714 2021-08-18 07:28:08 +0200 print: pre select landscape\n" \
"1713 2021-08-17 18:54:20 +0200 mfx3: update (Stephan)\n" \
"1712 2021-08-17 08:31:20 +0200 locdialog: sort CV table regarding index\n" \
"1711 2021-08-16 22:07:56 +0200 decspec: mfx by Stephan\n" \
"1710 2021-08-16 18:14:33 +0200 rocpro: set CV description on save\n" \
"1709 2021-08-16 16:29:15 +0200 car: fx attribute added for sync clients\n" \
"1708 2021-08-15 11:08:53 +0200 modbus: ulink\n" \
"1707 2021-08-15 08:10:46 +0200 rocpro: readall with index fixes\n" \
"1706 2021-08-14 17:38:25 +0200 modbus: block link corrections\n" \
"1705 2021-08-14 17:05:30 +0200 modbus: moved register change trace level from MONITOR to BYTE\n" \
"1704 2021-08-14 07:27:30 +0200 modbus: link/unlink analog block support added\n" \
"1703 2021-08-13 16:55:04 +0200 rocpro: support for cv list with index -> cv:idx,cv:idx\n" \
"1702 2021-08-13 14:59:50 +0200 rocdigs.xml\n" \
"1701 2021-08-13 12:04:49 +0200 guiframe: update car fx in case no throttle is active\n" \
"1700 2021-08-13 08:19:32 +0200 z21: new booster event/command added\n" \
"1699 2021-08-11 07:56:58 +0200 loc,control: read/write CV record with nr:index key\n" \
"1698 2021-08-10 16:30:47 +0200 operatur: function processing -> all,shift,fdesc\n" \
"1697 2021-08-10 10:16:35 +0200 lcdriver: check for red automatic signal in case of secondnext\n" \
"1696 2021-08-09 11:40:22 +0200 block: revert\n" \
"1695 2021-08-09 10:55:51 +0200 block: only allow BBT in case of wait\n" \
"1694 2021-08-09 10:40:17 +0200 wrapper: block bbt default off\n" \
"1693 2021-08-09 08:18:21 +0200 queue: commented out the printf trace\n" \
"1692 2021-08-09 07:40:21 +0200 operator: try to use the function description for cars\n" \
"1691 2021-08-09 07:22:28 +0200 throttledlg: set label too in case of icon\n" \
"1690 2021-08-08 08:48:51 +0200 guiframe: adjust monitor fontsize added\n" \
"1689 2021-08-07 07:53:56 +0200 cbusnodedlg: fix for setting GC6 SoD address\n" \
"1688 2021-08-07 07:47:53 +0200 wait4event: fix for restoring speed after checking all route commands\n" \
"1687 2021-08-06 08:58:51 +0200 modbus: report coils as inputs option added\n" \
"1686 2021-08-05 09:02:33 +0200 loc: calculate the length in the runner thread to make sure all objects are initialised\n" \
"1685 2021-08-04 15:07:31 +0200 code\n" \
"1684 2021-08-04 07:54:47 +0200 cbusnodedlg: type fix\n" \
"1683 2021-08-03 21:55:32 +0200 modbus: analog loco direction coil address correction\n" \
"1682 2021-08-03 08:32:38 +0200 lc,throttledlg: provide label in the function command\n" \
"1681 2021-08-03 08:08:07 +0200 throttledlg,loc: longclick action support\n" \
"1680 2021-08-03 07:34:01 +0200 car: isState extended with function check\n" \
"1679 2021-08-02 10:32:20 +0200 action,loc: check fndesc at slave locos\n" \
"1678 2021-08-02 08:13:54 +0200 rcan,cbus: SoD sleep option added\n" \
"1677 2021-08-02 07:48:11 +0200 rcan,cbus: SoD range added\n" \
"1676 2021-08-01 08:15:50 +0200 throttledlg,loc: longclick support added\n" \
"1675 2021-07-31 10:37:49 +0200 rocdigs: added missing __properties implementation\n" \
"1674 2021-07-31 10:20:45 +0200 mbus: get properties implemented to get the controller flags\n" \
"1673 2021-07-31 08:14:18 +0200 loconetdlg: label command station added\n" \
"1672 2021-07-30 08:28:04 +0200 loconetdlg,wrapper: removed unused dr5000 CS option\n" \
"1671 2021-07-29 14:50:56 +0200 car: group calculation fix for timed off functions\n" \
"1670 2021-07-29 11:09:20 +0200 wio: support for EVT_STATE added\n" \
"1669 2021-07-29 08:53:56 +0200 routedialog: modify speedconditions only on modify button\n" \
"1668 2021-07-29 08:29:42 +0200 rcan,cbus: SoD only for one address\n" \
"1667 2021-07-29 07:58:27 +0200 rcan: tcp initOK initializing added\n" \
"1666 2021-07-29 07:30:47 +0200 cbus: sleep between the two sensor SoD commands\n" \
"1665 2021-07-29 07:26:43 +0200 rcan,cbus: sleep between SoD sensors and CS\n" \
"1664 2021-07-28 10:15:20 +0200 car: timedoff function group offset correction\n" \
"1663 2021-07-28 08:58:53 +0200 lc,throttledlg: set also V_hint incase of speed buttons\n" \
"1662 2021-07-28 08:20:17 +0200 cbus: fix for long/short events at startup\n" \
"1661 2021-07-27 18:03:17 +0200 car: set group for timed functions\n" \
"1660 2021-07-27 10:54:51 +0200 item: brightness/alpha in colorpicker for output macOS only\n" \
"1659 2021-07-27 07:40:08 +0200 routedialog: change label group to class\n" \
"1658 2021-07-26 17:17:02 +0200 modbus: analog direction\n" \
"1657 2021-07-26 14:43:48 +0200 modbus: add nodeID*256 to the reported coils and registers\n" \
"1656 2021-07-24 14:03:21 +0200 wrapper: car dir attribute\n" \
"1655 2021-07-24 09:23:18 +0200 car: save direction for function commands\n" \
"1654 2021-07-23 09:08:35 +0200 modbus: direction coil analog loco command\n" \
"1653 2021-07-21 08:17:12 +0200 in: do not free route in case wheelcount does not match at in\n" \
"1652 2021-07-20 16:55:25 +0200 modbus: analog loco register range set to max 1000 fpr 100% velocity\n" \
"1651 2021-07-18 08:55:49 +0200 guiframe: show master loco in consist column\n" \
"1650 2021-07-17 10:27:50 +0200 editspro: fix for saving last s88 state\n" \
"1649 2021-07-13 12:33:40 +0200 modbus: trace correction\n" \
"1648 2021-07-13 11:01:33 +0200 block: check on isfree if pending arrival is the same locoID as reserved\n" \
"1647 2021-07-12 22:17:10 +0200 editspro: address mapping corrections\n" \
"1646 2021-07-12 10:02:39 +0200 modbus: analog loco support\n" \
"1645 2021-07-11 13:51:59 +0200 modbusdlg: title correction\n" \
"1644 2021-07-11 13:48:10 +0200 modbus: unitid added for polling\n" \
"1643 2021-07-11 07:40:34 +0200 modbus: set socket receive timeout at poll sleep\n" \
"1642 2021-07-11 07:35:46 +0200 modbus: stop polling in case the queue size > 1000\n" \
"1641 2021-07-08 11:24:09 +0200 modbus: use port as unitID\n" \
"1640 2021-07-08 10:06:28 +0200 block: fix for reset fifo pending arrival\n" \
"1639 2021-07-07 07:50:58 +0200 block: reset fix for fifo\n" \
"1638 2021-07-06 07:52:19 +0200 xmlscript: resolve car function command\n" \
"1637 2021-07-05 07:38:44 +0200 xmlscript: lightctrl support added\n" \
"1636 2021-07-04 08:22:58 +0200 xmlscript: call car function by description with resolved parameter\n" \
"1635 2021-07-03 11:27:06 +0200 carpanel: not reset train image on sort\n" \
"1634 2021-07-03 08:18:33 +0200 car: fix for saving V for none speed commands\n" \
"1633 2021-07-02 10:22:40 +0200 operatordlg: translation correction\n" \
"1632 2021-07-02 10:22:05 +0200 translation merge: nl\n" \
"1631 2021-07-02 07:58:27 +0200 translation merge: nl, de, fr, sr_lation, sl\n" \
"1630 2021-07-02 07:47:41 +0200 operator: center option locomotive added\n" \
"1629 2021-07-01 09:02:15 +0200 output,xnet: set fromoutput flag to regard the actdelay in the xnet library\n" \
"1628 2021-06-30 08:07:01 +0200 model: option locationprio added\n" \
"1627 2021-06-28 07:55:38 +0200 wio: inform car in case of RFID event\n" \
"1626 2021-06-28 07:54:52 +0200 location: check for reserved block which are not occupied\n" \
"1625 2021-06-27 10:49:14 +0200 lcdriver: trigger event timeout only once between events\n" \
"1624 2021-06-25 16:47:11 +0200 wio,loc: provide led brightness also incase of servo\n" \
"1623 2021-06-25 15:11:30 +0200 wiopi: use function brightness for servo speed\n" \
"1622 2021-06-24 17:21:24 +0200 blacklist\n" \
"1621 2021-06-20 07:57:22 +0200 loconet: trace level correction\n" \
"1620 2021-06-19 10:07:59 +0200 wiopi: dual motor support\n" \
"1619 2021-06-18 08:53:59 +0200 rcan,extcan: fix for programming\n" \
"1618 2021-06-18 08:27:49 +0200 rcan,extcan: fix cv index\n" \
"1617 2021-06-18 08:22:24 +0200 rcan,extcan: fix for PT index\n" \
"1616 2021-06-16 08:28:37 +0200 switch: check for inv attribute in case of tune range command\n" \
"1615 2021-06-12 10:55:23 +0200 switch: servo range command added for andRoc\n" \
"1614 2021-06-11 09:18:15 +0200 train D&D fixes\n" \
"1613 2021-06-10 09:19:28 +0200 block: fix for reset fifo\n" \
"1612 2021-06-10 07:52:06 +0200 wiopi: use ip of r2rnet response if host is not set\n" \
"1611 2021-06-08 07:19:55 +0200 guiframe: evaluate only attribute light if its provided in the state event\n" \
"1610 2021-06-07 17:40:51 +0200 wrapper: light default on\n" \
"1609 2021-06-07 08:37:54 +0200 xmlscript: trace flag added\n" \
"1608 2021-06-07 07:32:24 +0200 block: shunting locos should not wait\n" \
"1607 2021-06-03 07:57:23 +0200 location: divide by zero fix\n" \
"1606 2021-06-02 10:42:14 +0200 location: trace correction for loco arrivetime\n" \
"1605 2021-06-01 11:19:49 +0200 location: use last arrive time for init location fifo\n" \
"1604 2021-05-31 09:10:54 +0200 location: init random option\n" \
"1603 2021-05-30 15:36:32 +0200 loc: train state added for XMLScript\n" \
"1602 2021-05-30 10:42:54 +0200 turntabledialog: increased multiport addressing to 16bit\n" \
"1601 2021-05-30 10:33:36 +0200 operator: option freeblockonenter added which will overwrite the loco option\n" \
"1600 2021-05-29 10:01:38 +0200 actionctrldlg: condition train added\n" \
"1599 2021-05-29 10:00:00 +0200 action: condition (!)train added\n" \
"1598 2021-05-29 07:56:14 +0200 location: train only option added\n" \
"1597 2021-05-24 11:36:33 +0200 app: disable find level at startup\n" \
"1596 2021-05-24 10:48:38 +0200 trace: do not dispatch router tracing\n" \
"1595 2021-05-24 08:43:27 +0200 app: disable routing trace level at startup\n" \
"1594 2021-05-23 15:51:04 +0200 svg track 2 and 3 moved to user\n" \
"1593 2021-05-21 14:44:03 +0200 router: removed deprecated symbol support\n" \
"1592 2021-05-21 08:20:10 +0200 trackpicker: removed tracknr 2\n" \
"1591 2021-05-20 07:33:35 +0200 bidibdlg: removed discover\n" \
"1590 2021-05-19 08:14:21 +0200 wiopi: softpwm option on commandline temp.\n" \
"1589 2021-05-19 07:57:17 +0200 wiopi: softpwm option added to enable sound under super user\n" \
"1588 2021-05-17 08:03:54 +0200 action: fix for every and clock turns to zero\n" \
"1587 2021-05-17 07:22:55 +0200 output: fix for value and delayed off\n" \
"1586 2021-05-16 17:11:57 +0200 bidib: discover disabled\n" \
"1585 2021-05-16 11:44:07 +0200 WIO: fix for scanning r2rnet hostname\n" \
"1584 2021-05-16 09:09:18 +0200 lightctrl: fix for new random\n" \
"1583 2021-05-16 09:04:18 +0200 wiopi: io fix for port > 16\n" \
"1582 2021-05-16 08:56:14 +0200 wiopi: io fix for port > 16\n" \
"1581 2021-05-15 16:17:56 +0200 action: every fix revert\n" \
"1580 2021-05-15 08:17:48 +0200 lightctrl: two extra parameter for random range\n" \
"1579 2021-05-14 15:11:10 +0200 output: crash fix\n" \
"1578 2021-05-14 07:51:48 +0200 action: every/timed fixed\n" \
"1577 2021-05-11 22:08:38 +0200 z21: MADA support for switch commands\n" \
"1576 2021-05-11 08:25:24 +0200 location: check state !free too\n" \
"1575 2021-05-11 08:15:56 +0200 action: location state condition added\n" \
"1574 2021-05-11 08:00:05 +0200 location: extended is state free by checking for a free block\n" \
"1573 2021-05-10 17:15:43 +0200 tt: accdec fix:: set IID also for the direction command\n" \
"1572 2021-05-10 16:17:04 +0200 xmlscript,location: state query added\n" \
"1571 2021-05-10 08:25:47 +0200 messages: type fix\n" \
"1570 2021-05-09 10:34:13 +0200 tt: accdec default switch activation delay\n" \
"1569 2021-05-09 10:24:08 +0200 switch: trace crash fix\n" \
"1568 2021-05-09 08:12:04 +0200 wiopi: option BMS added\n" \
"1567 2021-05-08 10:04:10 +0200 tt: deactivate active delay in case of accdec\n" \
"1566 2021-05-07 15:52:48 +0200 item: add color and value on pushbutton output type\n" \
"1565 2021-05-05 15:31:58 +0200 seltab: disable actdelay\n" \
"1564 2021-05-05 09:05:06 +0200 trainimage: D&D init fix\n" \
"1563 2021-05-04 10:50:49 +0200 signal: regard linear signal type on testing state\n" \
"1562 2021-05-03 08:29:19 +0200 trainpanel: use freeze/autoresizecolumns/thaw on updates\n" \
"1561 2021-05-03 07:47:56 +0200 output: fix for delayed off\n" \
"1560 2021-05-02 15:53:31 +0200 locationsdlg: fix on selecting last block in list\n" \
"1559 2021-04-28 07:49:37 +0200 route: shunting permission added\n" \
"1558 2021-04-27 07:58:07 +0200 carpanel: sorting fix for location column\n" \
"1557 2021-04-23 12:03:29 +0200 stage: set cleanstamp on enter\n" \
"1556 2021-04-22 21:51:44 +0200 mbus: revert power by product\n" \
"1555 2021-04-22 15:17:08 +0200 mbus: global power filter correction\n" \
"1554 2021-04-21 10:25:37 +0200 stage: set cleanstamp before broadcast to clients\n" \
"1553 2021-04-21 07:48:19 +0200 mbus: set power events product related\n" \
"1552 2021-04-20 08:06:57 +0200 block: broadcast cleaning stamp on IN\n" \
"1551 2021-04-18 11:50:54 +0200 action: skip action in case system is not ready\n" \
"1550 2021-04-18 11:44:16 +0200 xmlscript: only start script in case the system is ready\n" \
"1549 2021-04-18 11:34:00 +0200 control: check valid instance on getState\n" \
"1548 2021-04-17 10:33:22 +0200 gcode: use bps from ini\n" \
"1547 2021-04-15 14:39:06 +0200 user svg theme: button 27 and 28 by Wolfgang (hobbyman)\n" \
"1546 2021-04-15 07:48:50 +0200 netbidib: added NMRA ID to the UID\n" \
"1545 2021-04-15 07:29:11 +0200 netbidib: Rocrail pairing UID = IANA-PEN << 16 + IANA-Port\n" \
"1544 2021-04-14 16:59:51 +0200 bidibdlg: layout fixes and sublib enable correction\n" \
"1543 2021-04-14 14:52:35 +0200 bidibdlg: discover option added (for future use)\n" \
"1542 2021-04-14 14:30:28 +0200 bidib: removed bus reset because the mc2 disconnect on this message\n" \
"1541 2021-04-14 14:10:31 +0200 netbidib: WIP\n" \
"1540 2021-04-14 11:36:52 +0200 netbidib: WIP\n" \
"1539 2021-04-14 08:02:36 +0200 translation for wiring color outputs\n" \
"1538 2021-04-13 19:38:03 +0200 netbidib: WIP\n" \
"1537 2021-04-12 16:33:18 +0200 item: added route context commands lock and test\n" \
"1536 2021-04-11 07:56:10 +0200 lightctrl: save stop/go state\n" \
"1535 2021-04-10 16:33:52 +0200 guiframe: set lightcontrol toolbar icon at state event\n" \
"1534 2021-04-10 11:33:33 +0200 cbus: POM write by opc 0xc1\n" \
"1533 2021-04-06 10:49:48 +0200 trainpanel,trainimage: reset fix\n" \
"1532 2021-04-06 09:23:23 +0200 ttrainpanel,item: update grid fixes\n" \
"1531 2021-04-06 08:14:43 +0200 trainpanel,item: set location by D&D\n" \
"1530 2021-04-05 12:15:14 +0200 model: check for a car by address if no loco was found\n" \
"1529 2021-04-04 16:48:12 +0200 item: fix for use aspectnumbers (Lothar)\n" \
"1528 2021-04-04 13:20:21 +0200 netbidib: wip\n" \
"1527 2021-04-03 12:25:14 +0200 rocweb: removed supportkey dependencies\n" \
"1526 2021-04-02 18:03:30 +0200 bidib: netbidib (wip)\n" \
"1525 2021-04-01 17:25:42 +0200 gendlg: removes obsolete adjust minsize attributes\n" \
"1524 2021-04-01 16:10:41 +0200 p50x: tcp sublib added for Tams MC2\n" \
"1523 2021-03-30 22:17:33 +0200 makefile: install target fix for rocrail.sh and rocview.sh\n" \
"1522 2021-03-30 09:19:02 +0200 makefile target install without sudo in $Home/Rocrail\n" \
"1521 2021-03-29 22:57:19 +0200 removed makefile install targets\n" \
"1520 2021-03-29 19:26:02 +0200 hsi88dlg: removed obsolete adjust minsize attributes\n" \
"1519 2021-03-29 16:52:01 +0200 hsi88dlg: type option removed\n" \
"1518 2021-03-29 09:32:22 +0200 cbusnodedlg: arducan description\n" \
"1517 2021-03-28 13:25:18 +0200 cbusnodedlg: new ArduCAN modules added\n" \
"1516 2021-03-28 11:24:30 +0200 loc,locdialog: added LED parameters to the functions\n" \
"1515 2021-03-26 14:41:48 +0100 wiopi: LED and script commands are default for mobile type with offset zero\n" \
"1514 2021-03-25 17:38:14 +0100 wiopi: moved SCAD files to repository Rocrail3D\n" \
"1513 2021-03-25 09:08:55 +0100 wiopi: battery board\n" \
"1512 2021-03-24 17:24:54 +0100 wiopi: ID12LA case\n" \
"1511 2021-03-24 08:21:49 +0100 cardlg: on/off pos function attributes added\n" \
"1510 2021-03-23 14:19:24 +0100 wiopi: throttle case update\n" \
"1509 2021-03-23 11:06:11 +0100 bidib: process lights on loco drive command too\n" \
"1508 2021-03-22 19:24:17 +0100 wiopi: Throttle case update\n" \
"1507 2021-03-22 14:17:11 +0100 wiopi: throttle case bottom\n" \
"1506 2021-03-22 13:43:39 +0100 wiopi: throttle case\n" \
"1505 2021-03-22 10:54:18 +0100 wiopi: scad throttle top update\n" \
"1504 2021-03-22 08:10:07 +0100 wiopi: throttle case (WIP)\n" \
"1503 2021-03-21 22:26:20 +0100 wiopi: CAD update\n" \
"1502 2021-03-21 16:56:34 +0100 dinamodlg: removed adjust minsize\n" \
"1501 2021-03-21 15:25:48 +0100 dinamo: hfi option added\n" \
"1500 2021-03-21 13:57:57 +0100 switch: correction for CTC poit LED\n" \
"1499 2021-03-21 12:17:59 +0100 wio: fix for simulating sensor state in WIOlib\n" \
"1498 2021-03-21 09:06:24 +0100 wiopi,hardware: update\n" \
"1497 2021-03-21 09:05:55 +0100 cardlg: LED lables\n" \
"1496 2021-03-21 09:05:18 +0100 wiopi: mobile car option\n" \
"1495 2021-03-21 07:25:37 +0100 switch: CTC color\n" \
"1494 2021-03-20 16:52:19 +0100 wiopi,hardware: measurment correction\n" \
"1493 2021-03-20 12:03:39 +0100 wiopi,hardware: removed prototype WIOpi-01\n" \
"1492 2021-03-20 12:00:23 +0100 wiopi,hardware: WIOpi-01 mounting board\n" \
"1491 2021-03-19 17:10:41 +0100 wiopi,mobile: not start the PWM thread in case of car option set\n" \
"1490 2021-03-19 10:43:48 +0100 switch: CTC color\n" \
"1489 2021-03-19 08:53:20 +0100 translation merge: de, fr, nl, sv\n" \
"1488 2021-03-18 17:08:06 +0100 wiopi: mobikle moderate servo speed for couplers\n" \
"1487 2021-03-18 15:45:46 +0100 route: CTC color\n" \
"1486 2021-03-18 15:07:58 +0100 block: CTC color on free\n" \
"1485 2021-03-18 14:38:27 +0100 block: CTC color\n" \
"1484 2021-03-18 13:34:49 +0100 switch,signal: color CTC\n" \
"1483 2021-03-17 12:20:29 +0100 model: fix memory leak on save with external loco file\n" \
"1482 2021-03-17 10:59:28 +0100 text: regard move command and not replace child nodes\n" \
"1481 2021-03-16 15:36:31 +0100 wiopi: pixel LEDScript addressing fix\n" \
"1480 2021-03-16 14:55:48 +0100 wiopi: pixel LEDScript off command correction\n" \
"1479 2021-03-16 13:58:40 +0100 block: CTC LEDs added\n" \
"1478 2021-03-16 10:29:08 +0100 wiopi: removed s88 type and implementation\n" \
"1477 2021-03-16 09:25:22 +0100 planpanel: disable multi touch on edit mode\n" \
"1476 2021-03-16 07:33:42 +0100 wiopi: fix for pixel LEDScript\n" \
"1475 2021-03-16 07:22:22 +0100 stage: fix for reset\n" \
"1474 2021-03-15 14:52:40 +0100 xmlscriptdlg: weather added\n" \
"1473 2021-03-15 14:48:33 +0100 xmlscriptdlg: fix for 'case switch'\n" \
"1472 2021-03-15 11:54:38 +0100 services: allow to reuse tcp ports\n" \
"1471 2021-03-15 09:12:43 +0100 signal,switch: use weather brightness for CTC LEDs\n" \
"1470 2021-03-15 07:55:41 +0100 switch: CTC color option added\n" \
"1469 2021-03-14 22:37:03 +0100 pixel: brightness increas and decrease added\n" \
"1468 2021-03-14 22:28:32 +0100 car: action correction for function substate\n" \
"1467 2021-03-14 17:54:24 +0100 signal: color support CTC LEDs\n" \
"1466 2021-03-14 10:52:54 +0100 wiopi: 2s/3s corrections\n" \
"1465 2021-03-14 10:10:36 +0100 wiopi: 2s/3s correction\n" \
"1464 2021-03-14 08:50:39 +0100 wiopi: pixel prevoff LEDScript option added\n" \
"1463 2021-03-14 07:29:18 +0100 wiopi: pixel auto increase added\n" \
"1462 2021-03-13 17:46:53 +0100 wiopi: 2s values added\n" \
"1461 2021-03-13 17:13:36 +0100 wiopi: batterytype 4s/3s added\n" \
"1460 2021-03-13 08:45:18 +0100 wiopi: send goodbye on shutdown\n" \
"1459 2021-03-13 08:23:07 +0100 planpanel: trace level for gestures changed to develop\n" \
"1458 2021-03-13 08:01:07 +0100 wiopi: check the pending RFID acknowledge\n" \
"1457 2021-03-12 18:29:48 +0100 wiopi: ebreak and stop command mobile to halt\n" \
"1456 2021-03-12 15:14:33 +0100 wiopi: reset read RFID after ack\n" \
"1455 2021-03-12 10:58:06 +0100 wiopi: fix for lights2 and function 1/2\n" \
"1454 2021-03-12 07:22:53 +0100 wiopi: pixel random C added\n" \
"1453 2021-03-11 16:59:09 +0100 wiopi: battery limit corrections for LiFePO4\n" \
"1452 2021-03-11 11:10:09 +0100 xmlscriptdlg: fix for statement and command switch\n" \
"1451 2021-03-11 11:04:09 +0100 xmlscriptdlg: added model and trace\n" \
"1450 2021-03-11 09:48:48 +0100 planpanel: gesture support added for pan, zoom and longclick\n" \
"1449 2021-03-09 08:20:08 +0100 wio: provide nr of steps on shortid\n" \
"1448 2021-03-08 13:32:45 +0100 text: add lchomedesc\n" \
"1447 2021-03-07 07:35:56 +0100 switch: lookup action by description by ID if description did not match\n" \
"1446 2021-03-05 11:08:30 +0100 switch: action by description support added\n" \
"1445 2021-03-04 17:48:54 +0100 loc: action correction for function substate\n" \
"1444 2021-03-03 08:02:25 +0100 default info level trace deactivated\n" \
"1443 2021-03-03 07:51:04 +0100 z21dlg: loconet detectors option added\n" \
"1442 2021-03-02 15:38:23 +0100 r2rnet default address set to 224.0.1.20\n" \
"1441 2021-03-01 17:27:32 +0100 guiframe: disconnect lightcontrol toolbar icon from state events\n" \
"1440 2021-03-01 15:03:42 +0100 reserve: fix for reset follow up routes\n" \
"1439 2021-03-01 10:52:11 +0100 loc function: on/off pos parameter added\n" \
"1438 2021-03-01 08:10:56 +0100 wiodlg: hide the options tab\n" \
"1437 2021-02-27 16:19:57 +0100 wio: options set/get added (wip)\n" \
"1436 2021-02-27 14:28:20 +0100 modplan: work around for linking a module define mvtrack\n" \
"1435 2021-02-26 11:23:46 +0100 wiopi: cv show mobile functions\n" \
"1434 2021-02-26 11:10:58 +0100 wiopi: mobile f1...f8 added\n" \
"1433 2021-02-26 10:44:30 +0100 wiopi: mobile function support\n" \
"1432 2021-02-26 08:25:29 +0100 rascii: disable clock broadcast in case protver=1\n" \
"1431 2021-02-25 12:28:22 +0100 wiopi: hardware update\n" \
"1430 2021-02-24 15:02:40 +0100 wiopi: set serial device to ttyAMA0 in case of ZeroW (if not set)\n" \
"1429 2021-02-24 15:00:09 +0100 wio: support for option byte 4\n" \
"1428 2021-02-23 10:54:40 +0100 wio: options4 added\n" \
"1427 2021-02-22 15:55:14 +0100 Rocrail Logo in STL format: Thanks to Dagnall\n" \
"1426 2021-02-22 15:22:08 +0100 wiopi: provide environment on query\n" \
"1425 2021-02-22 14:58:43 +0100 wiopi: use also sudo in the reboot an update scripts\n" \
"1424 2021-02-22 14:30:48 +0100 wiopi: hardware update\n" \
"1423 2021-02-22 12:20:59 +0100 wiopi: hardware update\n" \
"1422 2021-02-22 08:32:57 +0100 z21: keepm record of booster loadmax and voltmin\n" \
"1421 2021-02-21 13:30:10 +0100 guiframe: regard selected booster UID also in the status bulbs\n" \
"1420 2021-02-20 15:16:35 +0100 powerctrldlg: fix for selecting booster by uid and subuid\n" \
"1419 2021-02-20 11:46:52 +0100 wiopi: piprefix option added for scanning LAN connection\n" \
"1418 2021-02-20 08:22:53 +0100 wiopi: crash fix on shutdown\n" \
"1417 2021-02-20 08:12:31 +0100 wiopi: scan for a Rocrail server in case a LAN connection is valid\n" \
"1416 2021-02-19 15:51:04 +0100 wiopi: extra pins for H-Bridge-I\n" \
"1415 2021-02-19 14:07:31 +0100 powermandlg: fix for subuid\n" \
"1414 2021-02-19 10:57:07 +0100 wiopi: options and modifications for the WIOpi-01 board\n" \
"1413 2021-02-18 14:56:51 +0100 wiopi: sudo in startwiopi script\n" \
"1412 2021-02-18 08:11:49 +0100 booster: subuid added for multiple port boosters\n" \
"1411 2021-02-17 22:07:49 +0100 wiopi-01 schedule update\n" \
"1410 2021-02-17 15:46:40 +0100 z21: booster support\n" \
"1409 2021-02-17 08:44:07 +0100 accgroup: call outputs with the provided value(birghtness)\n" \
"1408 2021-02-17 08:16:23 +0100 wrapper: boosterevents flag\n" \
"1407 2021-02-17 08:14:43 +0100 z21: flag added for booster events -> not supported because of missing documentation\n" \
"1406 2021-02-16 22:03:49 +0100 loc: crash fix for removing queued commands\n" \
"1405 2021-02-16 10:53:51 +0100 guiframe: start workspace with command line option -virtual in case 'issue' is found in the path\n" \
"1404 2021-02-16 10:27:11 +0100 bidib: MAXDYN define for 64 dynstate values\n" \
"1403 2021-02-16 08:55:50 +0100 bidib: support for dynstate 0...127\n" \
"1402 2021-02-15 12:58:06 +0100 signal: fix for CTC and reset ID\n" \
"1401 2021-02-15 11:26:39 +0100 r2rnet: completely removed RASCII\n" \
"1400 2021-02-15 11:25:35 +0100 r2rnet: removed RASCII loop\n" \
"1399 2021-02-15 11:23:06 +0100 r2rnet: RASCII fix\n" \
"1398 2021-02-15 10:34:57 +0100 seltab: use saved position at startup\n" \
"1397 2021-02-15 09:06:25 +0100 output: use own color if the command does not contain a color node\n" \
"1396 2021-02-14 18:12:31 +0100 guiframe: removed double WIO from programming menu\n" \
"1395 2021-02-14 16:14:37 +0100 wiodlg: show warning in case of deepsleep/shutdown all\n" \
"1394 2021-02-14 15:03:33 +0100 wiopi,wiodlg: shutdown all option added\n" \
"1393 2021-02-14 12:31:46 +0100 block: regard timers on shortin events\n" \
"1392 2021-02-14 10:54:39 +0100 toolbardlg: wio icon added\n" \
"1391 2021-02-14 10:23:36 +0100 guiframe: wio icon added\n" \
"1390 2021-02-12 15:04:19 +0100 gcode: file upload support\n" \
"1389 2021-02-12 14:32:14 +0100 gcode: support added\n" \
"1388 2021-02-12 09:06:34 +0100 loc: remove pending events on (soft) reset\n" \
"1387 2021-02-12 08:04:02 +0100 seltab: crash fix in case a managed block does not exist\n" \
"1386 2021-02-12 07:45:34 +0100 block: show loco image option\n" \
"1385 2021-02-11 11:17:32 +0100 scheduledialog: crash fixes\n" \
"1384 2021-02-11 10:26:39 +0100 var,xmlscript: crash fixes\n" \
"1383 2021-02-10 14:14:24 +0100 xmlscript: binstate support\n" \
"1382 2021-02-10 12:26:36 +0100 auipanel: crash fix\n" \
"1381 2021-02-10 11:03:18 +0100 wiodlg: fix for start WEB Browser for type WIOpi\n" \
"1380 2021-02-09 15:07:45 +0100 wiopi: sysupdate script version from Lothar\n" \
"1379 2021-02-09 10:50:40 +0100 wiopi: update symbol for statusbar\n" \
"1378 2021-02-09 09:12:02 +0100 wiopi: signal sysupdate by battery 200\n" \
"1377 2021-02-08 16:17:24 +0100 wiopi: sysupdate\n" \
"1376 2021-02-08 15:00:45 +0100 wiopi: sysupdate command added\n" \
"1375 2021-02-08 10:43:18 +0100 signal: blank distant signal support for aspect type\n" \
"1374 2021-02-07 16:49:32 +0100 cardlg: description column added\n" \
"1373 2021-02-07 13:44:44 +0100 wiopi: fix for get/set first 32 IOs\n" \
"1372 2021-02-07 12:39:52 +0100 wiopi: fix for r2rnet socket recovering at reboot\n" \
"1371 2021-02-06 16:30:01 +0100 block: check for nested virtual blocks\n" \
"1370 2021-02-05 16:31:27 +0100 guiframe: fix for LightControl in Table menu\n" \
"1369 2021-02-05 15:27:21 +0100 wiopi: EoD option added\n" \
"1368 2021-02-05 14:04:03 +0100 loc: provide light state in case of shunting command\n" \
"1367 2021-02-05 11:17:49 +0100 lightctrl: go/stop command added\n" \
"1366 2021-02-05 09:04:25 +0100 svgutils: fix for rotating south\n" \
"1365 2021-02-05 08:06:56 +0100 r2rnet IP address correction: 224.0.0.1:8051\n" \
"1364 2021-02-04 10:48:25 +0100 r2rnet: netroutes polling option added\n" \
"1363 2021-02-04 10:18:39 +0100 wiopi: added const for user IO\n" \
"1362 2021-02-03 22:05:43 +0100 wrapper: r2rnet default active\n" \
"1361 2021-02-03 13:43:07 +0100 lightctrl: deactivate light if its in no record on\n" \
"1360 2021-02-03 09:54:19 +0100 wiopi: user I/O\n" \
"1359 2021-02-02 18:02:09 +0100 rocweb: help link corrected\n" \
"1358 2021-02-02 12:09:12 +0100 wiopi,r2rnet: reject messages from self\n" \
"1357 2021-02-02 10:40:29 +0100 base: timing fix for scan Servers on R2RNet\n" \
"1356 2021-02-02 10:18:25 +0100 wiopi: r2rnet try IP if hostname does not connect\n" \
"1355 2021-02-02 08:07:13 +0100 finder: location for final destination option added\n" \
"1354 2021-02-02 07:35:44 +0100 srcp: simulate sensor support added\n" \
"1353 2021-02-01 15:23:47 +0100 r2rnet: crash fix in case the client connection is not yet initialized\n" \
"1352 2021-01-31 14:33:39 +0100 wiopi: fix for r2rnet timeout\n" \
"1351 2021-01-31 12:10:23 +0100 tt: support the invert direction for type WIO\n" \
"1350 2021-01-31 09:17:19 +0100 wiopi,model: tracking improved\n" \
"1349 2021-01-31 09:16:51 +0100 route: skip finder option added\n" \
"1348 2021-01-30 11:01:40 +0100 renderer: fix for output shunting\n" \
"1347 2021-01-30 10:44:36 +0100 wio: inform loco and train of a tracking event\n" \
"1346 2021-01-29 13:05:36 +0100 model: lookup block/location by RFID\n" \
"1345 2021-01-28 14:46:13 +0100 wiopi: r2rnet wrapper option default true\n" \
"1344 2021-01-28 10:10:07 +0100 wiopi: r2rnet fix for request\n" \
"1343 2021-01-28 09:38:22 +0100 wiopi: r2rnet thread added for server scan\n" \
"1342 2021-01-27 22:06:24 +0100 wiopi: commented out the auto server find (wip)\n" \
"1341 2021-01-27 15:44:45 +0100 wiopi: auto detect server\n" \
"1340 2021-01-27 11:08:37 +0100 control: crash fix if no default digint is available\n" \
"1339 2021-01-27 10:24:38 +0100 wiopi: mobile stepper motor support\n" \
"1338 2021-01-26 13:54:21 +0100 wiodlg: fix for update message in case of wiopi\n" \
"1337 2021-01-26 11:36:09 +0100 wiopi: BLDC mobile type support added (wip)\n" \
"1336 2021-01-25 15:39:10 +0100 wiopi: do not report servo position in case of ESC or wheel\n" \
"1335 2021-01-25 12:15:06 +0100 wiopi: bridge sensor inputs added for type stepper\n" \
"1334 2021-01-25 11:21:56 +0100 modbus: clock support added\n" \
"1333 2021-01-25 08:29:51 +0100 feedbackdialog: open usage object on dclick\n" \
"1332 2021-01-24 16:42:59 +0100 feedbackdialog: usage added for tt bridge sensors\n" \
"1331 2021-01-24 14:31:50 +0100 feedbackdialog: sblist added for checking usage\n" \
"1330 2021-01-24 12:13:09 +0100 feedbackdialog: show usage of tt, seltab, sg\n" \
"1329 2021-01-24 11:25:23 +0100 wiopi: normalized init/type\n" \
"1328 2021-01-24 10:52:53 +0100 cv: fix for cv0 trace\n" \
"1327 2021-01-23 16:33:38 +0100 wiopi: update.sh remove existing gz before download\n" \
"1326 2021-01-23 15:01:00 +0100 mbus: disable reading config on discovery in case Rocrail is not master\n" \
"1325 2021-01-21 15:29:08 +0100 wiopi: allow up to 16 char node name\n" \
"1324 2021-01-21 12:18:57 +0100 wiopi: support for mosfet, esc and hbridge mobile\n" \
"1323 2021-01-20 08:53:40 +0100 wiopi: stop ESC on shutdown\n" \
"1322 2021-01-20 08:13:47 +0100 wait4event: ignore Check2In in case of last schedule entry\n" \
"1321 2021-01-20 08:02:09 +0100 REVERT loc: disable check2in in case of schedule/tour\n" \
"1320 2021-01-19 10:27:57 +0100 rocrail: releasename set to 'Iron'\n" \
"1319 2021-01-19 09:05:14 +0100 tt: f6915 broadcast new position after position is confirmed\n" \
"1318 2021-01-19 08:26:34 +0100 wiopi: added Pi01 to user pindefs\n" \
"1317 2021-01-19 08:09:30 +0100 wiopi: user pindefs for stepper and mobile\n" \
"1316 2021-01-18 10:50:18 +0100 tt: f6915 check for position sensor\n" \
"1315 2021-01-18 08:53:59 +0100 loc: disable check2in in case of schedule/tour\n" \
"1314 2021-01-17 13:46:00 +0100 wiopi,id12: fix for RFID event type\n" \
"1313 2021-01-17 09:57:59 +0100 wiopi: use pin 12 for mobile PWM\n" \
"1312 2021-01-17 08:48:08 +0100 mbusdlg: fix for portnumber field sizes\n" \
"1311 2021-01-17 08:31:13 +0100 block: close action fix\n" \
"1310 2021-01-16 15:26:05 +0100 wiopi: mobile ESC option (WIP)\n" \
"1309 2021-01-14 19:28:08 +0100 drcmd removed\n" \
"1308 2021-01-14 16:22:33 +0100 wiodlg: label changes on select of wiopi\n" \
"1307 2021-01-14 13:23:46 +0100 block: fix for min/max random wait\n" \
"1306 2021-01-13 13:38:27 +0100 wiopi: fix for reading and ignoring XML\n" \
"1305 2021-01-13 08:51:46 +0100 rocweb: use global image path if not found in the rocweb context\n" \
"1304 2021-01-12 11:50:33 +0100 wiopi: hardware update\n" \
"1303 2021-01-12 10:12:46 +0100 wiopi: headless files\n" \
"1302 2021-01-11 15:22:31 +0100 wiopi: hardware update and native IO support\n" \
"1301 2021-01-10 07:55:38 +0100 wiopi: PCB\n" \
"1300 2021-01-09 11:41:55 +0100 mttmfcc: analog output support added\n" \
"1299 2021-01-08 15:32:46 +0100 planpanel: prevent crash on switchen from AUI to Tab\n" \
"1298 2021-01-08 15:10:13 +0100 wiopi,servo: support for hardware PWM in case of user 0\n" \
"1297 2021-01-08 13:08:31 +0100 wiopi: hardware PWM\n" \
"1296 2021-01-08 08:26:09 +0100 black list\n" \
"1295 2021-01-08 08:23:45 +0100 system: check email\n" \
"1294 2021-01-07 15:01:42 +0100 auipanel: workaround for panel refresh\n" \
"1293 2021-01-07 12:05:28 +0100 item: fix for isMDI -> isAUI\n" \
"1292 2021-01-07 11:49:44 +0100 planpanel: changed MDI into AUI flag\n" \
"1291 2021-01-07 09:18:38 +0100 wiopi: max internal servos 4\n" \
"1290 2021-01-06 14:10:59 +0100 wiopi: write ini file on shutdown\n" \
"1289 2021-01-06 12:06:44 +0100 wiopi,mobile: pulse time for direction relay 250ms\n" \
"1288 2021-01-06 09:08:29 +0100 wiopi,mobile: changed motor control for relay and FET\n" \
"1287 2021-01-05 18:02:05 +0100 loc: revert using V_Rmax/V_Rmin from revision 2.1.930/931\n" \
"1286 2021-01-05 12:19:40 +0100 translation merge: da, cs, fr, de, nl, sl\n" \
"1285 2021-01-05 11:13:10 +0100 wiopi, manager: use request/release I2C access\n" \
"1284 2021-01-05 09:43:03 +0100 output: set actdelay to false in case the asswitch option is set\n" \
"1283 2021-01-05 08:40:11 +0100 wiopi: manager request 3 bytes on poll\n" \
"1282 2021-01-04 16:00:44 +0100 wiopi: display fast 7seg clock added\n" \
"1281 2021-01-04 14:37:49 +0100 wio: use port as I2C slave id\n" \
"1280 2021-01-03 19:06:59 +0100 blockdialog: event sorting optimized\n" \
"1279 2021-01-03 13:51:36 +0100 wiopi: use the ADS1115 for watching the LiPo\n" \
"1278 2021-01-03 10:04:53 +0100 wiopi: support ADS1115\n" \
"1277 2021-01-02 16:11:49 +0100 thread: added usleep\n" \
"1276 2021-01-02 15:37:16 +0100 wiopi: check if a thread is needed, and use softservo\n" \
"1275 2021-01-02 09:42:01 +0100 wiopi: removed the i2c object\n" \
"1274 2021-01-02 07:33:21 +0100 userial: check if wanted bps is > 0\n" \
"1273 2021-01-01 22:10:04 +0100 block: only activate rear protection in case V=0\n" \
"1272 2021-01-01 16:09:19 +0100 wiopi: removed IO library from makefile\n" \
"1271 2021-01-01 16:07:42 +0100 wiopi: hardware I/O\n" \
"1270 2021-01-01 11:08:45 +0100 wiopi: replaced I2C and SPI with own code\n" \
"1269 2021-01-01 08:45:10 +0100 copyright update\n" \
"1268 2020-12-31 15:36:25 +0100 wiopi: use own hardware interface instead of 3part library (WIP)\n" \
"1267 2020-12-31 13:39:16 +0100 wiopi: added scroll text, pin mapping for wPi and PiGPIO\n" \
"1266 2020-12-31 08:32:38 +0100 wiopi: report low battery\n" \
"1265 2020-12-30 16:35:38 +0100 wiopi: LiPo measurement added\n" \
"1264 2020-12-30 10:54:25 +0100 wiopi,display: graphic instructions added\n" \
"1263 2020-12-30 10:25:40 +0100 posseldlg: 24bit position field\n" \
"1262 2020-12-29 22:23:51 +0100 wiopi, pixel: wait until SPI device is ready\n" \
"1261 2020-12-29 15:03:40 +0100 wiopi: update by tar.gz format\n" \
"1260 2020-12-29 14:08:27 +0100 rocview: removed client size adjust option\n" \
"1259 2020-12-29 13:52:42 +0100 guiapp: restore clientsize after initializing frame\n" \
"1258 2020-12-29 13:49:18 +0100 wiopi/wio: cam start/stop\n" \
"1257 2020-12-29 12:09:01 +0100 wiopi: stopcam.sh added\n" \
"1256 2020-12-29 10:48:12 +0100 wiopi: get battery from ADC\n" \
"1255 2020-12-29 07:34:06 +0100 textdialog: translation for active text added\n" \
"1254 2020-12-28 21:37:39 +0100 wio: added missing zero termination on CMD_TEXT\n" \
"1253 2020-12-28 17:17:07 +0100 wio: use fb register type for AIN events\n" \
"1252 2020-12-28 16:03:53 +0100 wio: analog sensor type added\n" \
"1251 2020-12-28 15:05:19 +0100 wiopi: adc added\n" \
"1250 2020-12-28 10:53:24 +0100 stepper: accel/decel improvements\n" \
"1249 2020-12-28 08:06:11 +0100 wiopi: improved socket broken detection\n" \
"1248 2020-12-27 17:32:04 +0100 svgutils: rotation center correction\n" \
"1247 2020-12-27 14:35:18 +0100 wiopi: fix for I2C read and IO restore\n" \
"1246 2020-12-27 13:55:18 +0100 rocguiinidlg: check wx version >= 3.1.5 for geting the display scaling factor\n" \
"1245 2020-12-27 12:24:52 +0100 rocguidialog: listup all monitors\n" \
"1244 2020-12-27 12:17:17 +0100 rocguidialog: list up all connected monitors\n" \
"1243 2020-12-27 11:40:55 +0100 rocguidialog: show scale factor\n" \
"1242 2020-12-27 10:46:18 +0100 rocview: client size adjust for macOS\n" \
"1241 2020-12-27 09:12:14 +0100 textdialog: fix for color labels\n" \
"1240 2020-12-26 14:28:30 +0100 wiopi: pointer fix\n" \
"1239 2020-12-26 09:16:07 +0100 wiopi: communication reorg\n" \
"1238 2020-12-25 21:47:30 +0100 trace: type fix\n" \
"1237 2020-12-25 18:08:29 +0100 textdialog: color labels added\n" \
"1236 2020-12-25 14:12:17 +0100 wiopi: LEDScript support\n" \
"1235 2020-12-25 11:57:32 +0100 wiopi: pinout documentation in the console\n" \
"1234 2020-12-25 09:25:38 +0100 wiopi: global pinout\n" \
"1233 2020-12-24 22:20:45 +0100 renderer: rotation fix for line (Lothar)\n" \
"1232 2020-12-24 13:01:52 +0100 wiopi: moved all hardware interfacing in one object\n" \
"1231 2020-12-23 20:15:07 +0100 z21: removed double, uncontrolled, gethostbyaddr\n" \
"1230 2020-12-23 17:18:03 +0100 str: fix for linux bug\n" \
"1229 2020-12-23 16:35:50 +0100 wiopi: crash fixes\n" \
"1228 2020-12-23 11:15:59 +0100 wiopi:pixel (wip)\n" \
"1227 2020-12-23 10:46:42 +0100 z21: revert removing xnetgbm option\n" \
"1226 2020-12-22 19:15:57 +0100 z21: removed xnetgbm option\n" \
"1225 2020-12-22 17:56:43 +0100 z21: set unconditional LocoNet detectors broadcast flag\n" \
"1224 2020-12-22 12:14:44 +0100 wiopi: Throttle (wip)\n" \
"1223 2020-12-22 11:13:41 +0100 bidib: allow port numbers up to 255\n" \
"1222 2020-12-21 09:37:18 +0100 wio: use new TT attribute stepdelay\n" \
"1221 2020-12-20 15:11:31 +0100 loc: workaround for linux semaphor lock on assigntrain\n" \
"1220 2020-12-20 14:05:41 +0100 xmlscript: resolve text format attribute value\n" \
"1219 2020-12-20 10:02:43 +0100 wiopi: support for two on board servos (sudo)\n" \
"1218 2020-12-20 09:14:52 +0100 wiopi: S88 added\n" \
"1217 2020-12-19 12:11:49 +0100 wiopi: pixel addressing\n" \
"1216 2020-12-19 09:20:03 +0100 wiopi: pixel fix, mobile range implemented\n" \
"1215 2020-12-18 21:36:42 +0100 wiopi: Pixel  (wip)\n" \
"1214 2020-12-18 15:36:37 +0100 wiopi: alive response timeout to stop mobile running\n" \
"1213 2020-12-18 13:28:47 +0100 dccpp: check for space <Q1...> or <Q 1...>\n" \
"1212 2020-12-17 18:59:29 +0100 wiopi: Mobile (wip)\n" \
"1211 2020-12-17 16:39:52 +0100 z21dlg: check hostname option added -> protver 0 or 1\n" \
"1210 2020-12-17 13:04:14 +0100 wiopi: WIP\n" \
"1209 2020-12-16 21:24:01 +0100 z21: call only get hostname ny address in case protver > 0\n" \
"1208 2020-12-16 21:13:39 +0100 z21: revert check hostname only if protver > 0\n" \
"1207 2020-12-16 12:01:19 +0100 wiopi: mobile (wip)\n" \
"1206 2020-12-16 10:13:13 +0100 wiopi: cam script\n" \
"1205 2020-12-15 20:51:34 +0100 z21: check ip with hostname only if protver > 0\n" \
"1204 2020-12-15 16:29:46 +0100 wiopi: Sound added\n" \
"1203 2020-12-15 13:40:35 +0100 wiopi: support for output pair\n" \
"1202 2020-12-15 12:17:30 +0100 wiopi: show RailCom on display\n" \
"1201 2020-12-15 11:23:50 +0100 wio: send CV commands to all nodes\n" \
"1200 2020-12-15 11:00:36 +0100 userial: not available on aarch64\n" \
"1199 2020-12-15 10:57:05 +0100 wio: broadcast CV commands\n" \
"1198 2020-12-15 09:25:41 +0100 wiopi: RailCom reporting\n" \
"1197 2020-12-15 08:20:05 +0100 rocs: userial baud rate aliasing added\n" \
"1196 2020-12-14 19:26:41 +0100 guiframe: added missing getItem from AUI panel\n" \
"1195 2020-12-14 16:00:38 +0100 wiopi: RailCom added: no luck because 250000 baud is not available\n" \
"1194 2020-12-14 12:12:26 +0100 rocs: serial speed 250000 check\n" \
"1193 2020-12-14 08:32:56 +0100 wiopi: sensor added\n" \
"1192 2020-12-13 14:59:23 +0100 wiopi: IO signal support\n" \
"1191 2020-12-13 13:21:33 +0100 wiopi: stepper reporting added\n" \
"1190 2020-12-13 12:28:16 +0100 wiopi: stepper support (WIP)\n" \
"1189 2020-12-13 10:14:08 +0100 wrapper: trace level find default off\n" \
"1188 2020-12-13 08:39:50 +0100 wio: added missing stepper reporting parameter\n" \
"1187 2020-12-12 16:12:40 +0100 wiopi: led signal support\n" \
"1186 2020-12-12 15:39:08 +0100 wiopi: signal support\n" \
"1185 2020-12-12 14:26:14 +0100 wiopi: crash fix in case WLAN is disconnected\n" \
"1184 2020-12-12 13:14:11 +0100 wiopi: clock show alternating time, date, temp.\n" \
"1183 2020-12-12 09:45:05 +0100 wiopi: clock added HT16K33\n" \
"1182 2020-12-11 17:56:09 +0100 wiopi: show connection speed on the statusbar 5G or 2G\n" \
"1181 2020-12-11 16:11:08 +0100 wiopi: show added\n" \
"1180 2020-12-11 15:03:38 +0100 wiopi: show status bar\n" \
"1179 2020-12-11 08:58:34 +0100 wiopi: set options fix, show rfid on display\n" \
"1178 2020-12-10 18:52:59 +0100 cbus: do SoD on startup also with address zero\n" \
"1177 2020-12-10 12:53:42 +0100 wiopi: display analog clock\n" \
"1176 2020-12-10 10:20:23 +0100 wiopi: display AMP support added\n" \
"1175 2020-12-10 07:45:25 +0100 wio: typo fix\n" \
"1174 2020-12-10 07:43:49 +0100 wio: text must also be escaped in HEXA\n" \
"1173 2020-12-09 11:55:29 +0100 rocguiinidlg: label AUI fix\n" \
"1172 2020-12-09 11:04:51 +0100 rocview: MDI removed\n" \
"1171 2020-12-09 10:36:20 +0100 wiopi: ID12 added\n" \
"1170 2020-12-08 19:30:13 +0100 guiframe: check AUI before MDI\n" \
"1169 2020-12-08 15:03:02 +0100 wiopi: display refresh\n" \
"1168 2020-12-08 12:17:52 +0100 wiopi: display (WIP)\n" \
"1167 2020-12-07 10:51:01 +0100 locdialog: added missing action states\n" \
"1166 2020-12-07 09:05:41 +0100 wiopi: servo delay and reporting added\n" \
"1165 2020-12-06 18:21:52 +0100 cleanup unused decoder file format\n" \
"1164 2020-12-06 15:54:42 +0100 wiopi: IO events\n" \
"1163 2020-12-06 15:03:50 +0100 wiopi: alive thread added\n" \
"1162 2020-12-06 12:30:17 +0100 wiopi: rascii struct correction\n" \
"1161 2020-12-06 12:26:36 +0100 rocs: rascii struct member correction\n" \
"1160 2020-12-06 12:14:37 +0100 wiopi: added missinf wiringpi setup\n" \
"1159 2020-12-06 11:18:00 +0100 rocs: rascii include fix\n" \
"1158 2020-12-06 11:16:18 +0100 rocs: rascii include fix\n" \
"1157 2020-12-06 11:02:46 +0100 rocs: rascii added\n" \
"1156 2020-12-06 09:10:33 +0100 node: not remove an attr in case the value is set to NULL\n" \
"1155 2020-12-06 08:51:45 +0100 wio: added missing recipient id, 0=broadcast, in the clock command\n" \
"1154 2020-12-06 08:29:21 +0100 wiopi: use internal rascii frame\n" \
"1153 2020-12-05 16:39:02 +0100 tttrackdialog: grid connect value increased to 26\n" \
"1152 2020-12-05 14:57:07 +0100 wiopi: WIP\n" \
"1151 2020-12-04 23:02:03 +0100 attr: set simple type improvements\n" \
"1150 2020-12-04 22:26:16 +0100 attr: memory usage optimisation\n" \
"1149 2020-12-04 13:54:23 +0100 wiopi: scan IO\n" \
"1148 2020-12-04 13:33:58 +0100 wiopi: servo pos fix\n" \
"1147 2020-12-04 11:47:17 +0100 wiopi: WIP\n" \
"1146 2020-12-03 15:46:59 +0100 wiopi: support for PCA9622\n" \
"1145 2020-12-03 14:53:19 +0100 wiopi: PCA9785 support\n" \
"1144 2020-12-03 12:26:10 +0100 wiopi: WIP\n" \
"1143 2020-12-03 09:06:42 +0100 wiopi: WIP\n" \
"1142 2020-12-02 14:35:24 +0100 wiopi: WIP\n" \
"1141 2020-12-02 12:11:55 +0100 wiopi: WIP\n" \
"1140 2020-12-02 12:11:16 +0100 wio: use strtobyte with initialized buffer\n" \
"1139 2020-12-02 11:10:24 +0100 dir.svg: removed alt-g\n" \
"1138 2020-12-01 10:34:39 +0100 guiapp: stay offline in case of local file\n" \
"1137 2020-12-01 09:37:20 +0100 z21: disregard domain inhost name in case its not specified in the rocrail.ini\n" \
"1136 2020-11-30 19:29:57 +0100 wsocket: workaround for win32 and getnameinfo\n" \
"1135 2020-11-30 15:58:35 +0100 rocs makefile: adde missing library for win32\n" \
"1134 2020-11-30 15:41:06 +0100 translations: type fix\n" \
"1133 2020-11-30 13:09:12 +0100 z21: compare hostname none case sensitive\n" \
"1132 2020-11-30 12:33:18 +0100 z21,socket: get hostname by ip\n" \
"1131 2020-11-30 09:53:22 +0100 control: crash fix on send declist to digints\n" \
"1130 2020-11-30 09:13:19 +0100 socket: compiler warning\n" \
"1129 2020-11-30 09:07:12 +0100 z21: check for hostname by ip addr.\n" \
"1128 2020-11-30 08:18:55 +0100 control: container for digints\n" \
"1127 2020-11-29 19:12:20 +0100 control: revert digint struct\n" \
"1126 2020-11-29 15:34:44 +0100 dccpp: removed wait loop on init sensors\n" \
"1125 2020-11-29 13:16:56 +0100 router: reduceV option added to override the default value of routes\n" \
"1124 2020-11-29 08:11:37 +0100 guiapp: replaced deprecated MacOpenFile\n" \
"1123 2020-11-28 17:58:13 +0100 guiapp: revert removing MacOpenFile\n" \
"1122 2020-11-28 13:15:31 +0100 xmlscript: resolve fndesc\n" \
"1121 2020-11-28 11:47:42 +0100 dccpp: response flag in structure\n" \
"1120 2020-11-28 11:36:10 +0100 dccpp: wait for response after init a sensor\n" \
"1119 2020-11-28 11:24:10 +0100 rdp: removed from rocutils.xml\n" \
"1118 2020-11-28 10:29:34 +0100 revert rdp: wrong level\n" \
"1117 2020-11-28 08:10:11 +0100 guiapp: save and restore the client size instead of the system dependent frame size in case of Apple\n" \
"1116 2020-11-26 21:27:57 +0100 rocs: free op loaded library on delete\n" \
"1115 2020-11-26 21:27:09 +0100 control: use a structure to save loaded DigInt data\n" \
"1114 2020-11-26 21:26:10 +0100 mjpeg: variable correction\n" \
"1113 2020-11-26 11:04:36 +0100 block: ignore events in case the block is virtual\n" \
"1112 2020-11-25 19:30:18 +0100 rocguiinidlg: fix for label\n" \
"1111 2020-11-25 17:21:51 +0100 optional ARCH make parameter added\n" \
"1110 2020-11-25 12:24:03 +0100 loc: debug trace correction\n" \
"1109 2020-11-25 11:14:48 +0100 loc: partial fix for sync events and commands\n" \
"1108 2020-11-24 12:55:37 +0100 WIP rdp\n" \
"1107 2020-11-24 11:48:38 +0100 item: disable close in case of virtual block\n" \
"1106 2020-11-24 09:53:43 +0100 model: fix for deleteing closed routes and blocks\n" \
"1105 2020-11-23 10:41:19 +0100 wiodlg: type s88 added\n" \
"1104 2020-11-21 15:04:03 +0100 infodialog: show cpu arch\n" \
"1103 2020-11-21 12:16:18 +0100 rocs makefile: security issue macOS\n" \
"1102 2020-11-21 12:15:22 +0100 rocs makefile: security issue for macOS\n" \
"1101 2020-11-19 10:26:59 +0100 dccpp: timing improvement at connect\n" \
"1100 2020-11-19 08:59:33 +0100 dccpp: initial commands serialized\n" \
"1099 2020-11-18 08:18:26 +0100 dccpp: wait until a befor setup\n" \
"1098 2020-11-18 08:07:20 +0100 dccpp: wait until a befor setup\n" \
"1097 2020-11-17 22:10:39 +0100 dccpp: forget the big sleep and run from start\n" \
"1096 2020-11-17 14:53:49 +0100 dccpp: init sequence changes\n" \
"1095 2020-11-17 09:51:58 +0100 rocdigs.xml: new data memeber for dccpp\n" \
"1094 2020-11-17 08:26:58 +0100 dccpp: init sensors after the status is requested\n" \
"1093 2020-11-16 07:41:39 +0100 dccpp: disable DTR in sublib serial\n" \
"1092 2020-11-16 07:19:25 +0100 car,locdialog: purchased and value fix on export\n" \
"1091 2020-11-15 09:47:35 +0100 guiapp: save and restore the client size instead of the system dependent frame size\n" \
"1090 2020-11-15 08:24:33 +0100 car,locdialog: only import if the ID is not already in the table\n" \
"1089 2020-11-14 14:43:07 +0100 car,loco: import/export as is with comman\n" \
"1088 2020-11-14 14:34:28 +0100 cardlg,locdialog: remove ampersand on export\n" \
"1087 2020-11-14 11:04:46 +0100 throttledlg: only show loco ID in titlebar\n" \
"1086 2020-11-14 10:51:43 +0100 cardlg,locdialog: export purchased and value too\n" \
"1085 2020-11-14 10:01:32 +0100 replaced makefile variable MACOSX with MACOS\n" \
"1084 2020-11-14 08:43:50 +0100 locdialog,cardlg: flatten data on export\n" \
"1083 2020-11-13 08:21:46 +0100 cardlg: import/export in csv format added\n" \
"1082 2020-11-13 07:58:29 +0100 locdialog: import/export in csv format\n" \
"1081 2020-11-11 14:57:01 +0100 item: force switch command in case ctrl is pressed\n" \
"1080 2020-11-11 14:54:35 +0100 wio: fix for switch delay in case > 255\n" \
"1079 2020-11-11 07:43:21 +0100 dccpp: init sensors after receiving th <i> string\n" \
"1078 2020-11-10 15:19:29 +0100 wiodlg: deep sleep command added\n" \
"1077 2020-11-10 12:07:45 +0100 signaldialog: layout improvements\n" \
"1076 2020-11-10 10:49:40 +0100 item: save enter state in stageblock\n" \
"1075 2020-11-07 14:32:34 +0100 revert\n" \
"1074 2020-11-07 08:12:27 +0100 translation merge: nl, fr, de, sl\n" \
"1073 2020-11-07 07:55:14 +0100 mvtrack: fix for using cm\n" \
"1072 2020-11-03 16:07:08 +0100 loc: rfid history\n" \
"1071 2020-11-01 09:26:55 +0100 loc,wio: report with CALC level the read RFID\n" \
"1070 2020-11-01 07:52:57 +0100 wio: type LED added for accessory events\n" \
"1069 2020-10-31 19:06:22 +0100 start.html: FR correction\n" \
"1068 2020-10-31 15:01:33 +0100 wiolib: reset simulated sensor\n" \
"1067 2020-10-31 13:52:51 +0100 start.html update for IT and FR\n" \
"1066 2020-10-31 13:38:12 +0100 control: support for environment variables in Rocweb/andRoc SVG paths\n" \
"1065 2020-10-30 22:29:06 +0100 zipper: start.html added\n" \
"1064 2020-10-30 19:34:11 +0100 devices: list up first 10 devices in case of no device check\n" \
"1063 2020-10-30 15:41:21 +0100 wio: support for A0 value on alive events\n" \
"1062 2020-10-30 08:45:51 +0100 rocrailinidialog: disable properties and modify in case of wio lib\n" \
"1061 2020-10-30 07:53:53 +0100 wio: support library added\n" \
"1060 2020-10-29 21:16:28 +0100 mbus: show ID in case of commands\n" \
"1059 2020-10-29 14:37:45 +0100 rocprodlg: save programming mode on OK\n" \
"1058 2020-10-29 11:49:54 +0100 rocguiinidlg: fix for option resetspeeddir\n" \
"1057 2020-10-29 07:34:23 +0100 app: support environment variables in the protpath\n" \
"1056 2020-10-28 08:49:06 +0100 block: fix for slave action\n" \
"1055 2020-10-27 07:44:03 +0100 timedactions: usage button added to show usage in the index\n" \
"1054 2020-10-25 17:50:05 +0100 item: regard showID in stage\n" \
"1053 2020-10-25 11:17:53 +0100 wio: report node stop reason\n" \
"1052 2020-10-24 19:28:13 +0200 app: replaced warning with info level for merging options\n" \
"1051 2020-10-23 18:18:45 +0200 guiframe: fix for update train image\n" \
"1050 2020-10-23 11:00:32 +0200 model: fix for looking up a car for zero length ID which results in NOP in XMLScript\n" \
"1049 2020-10-22 12:47:12 +0200 fback: broadcast rfid to clients\n" \
"1048 2020-10-22 10:55:35 +0200 item: show last read RFID in the status bar\n" \
"1047 2020-10-22 07:45:36 +0200 desktoplink.sh: only copy it to applications incase parameter $1 is set\n" \
"1046 2020-10-21 15:24:57 +0200 renderer: text ps adjust only in case text ps is set > 0\n" \
"1045 2020-10-21 15:17:56 +0200 rocguiinidlg: text pointsize adjust range extended\n" \
"1044 2020-10-21 15:13:29 +0200 renderer: pointsize adjust es percentage\n" \
"1043 2020-10-21 11:14:22 +0200 svg text pointsize adjust added\n" \
"1042 2020-10-19 19:03:07 +0200 wio: fix for output and LEDScript\n" \
"1041 2020-10-19 10:30:22 +0200 mbus: default auto switch delay active\n" \
"1040 2020-10-19 07:43:52 +0200 wio: output bri to max. in case of LEDScript activation\n" \
"1039 2020-10-19 07:35:53 +0200 wio: use output/carfun addr > 100 as trigger for LEDScript\n" \
"1038 2020-10-18 18:54:27 +0200 cardlg,car: fix for function color in case of ledaddr == 0 and ledcount > 0\n" \
"1037 2020-10-18 17:18:42 +0200 text: do not cat NULL variable strings\n" \
"1036 2020-10-18 13:37:43 +0200 wio: LEDScript support\n" \
"1035 2020-10-17 13:34:58 +0200 router: typo fix\n" \
"1034 2020-10-17 13:28:59 +0200 car: add bus(node-id) to all commands\n" \
"1033 2020-10-17 10:31:51 +0200 operator: shiftloco command added\n" \
"1032 2020-10-16 22:16:22 +0200 cardlg: function LED color brightness added\n" \
"1031 2020-10-16 12:13:49 +0200 car,wio: color function option\n" \
"1030 2020-10-15 17:15:02 +0200 wiodlg: mobile car option added\n" \
"1029 2020-10-15 16:47:54 +0200 wio: remove raw attribute before sending the VDirF command\n" \
"1028 2020-10-14 21:23:31 +0200 stc: version correction from 2.0 to 2.1\n" \
"1027 2020-10-14 17:00:46 +0200 wio: send loco a command on WIO Control events instead of field events\n" \
"1026 2020-10-14 11:41:23 +0200 canservice: mbus namespace\n" \
"1025 2020-10-14 11:35:33 +0200 rocnetnode: mbus namespace\n" \
"1024 2020-10-14 09:04:03 +0200 mbus,rcan,bidib: namespace conflicts resolved\n" \
"1023 2020-10-13 20:45:48 +0200 virtual: removed writehex emu\n" \
"1022 2020-10-13 19:30:56 +0200 wio: fix for activation OTA\n" \
"1021 2020-10-10 17:14:48 +0200 mbus: bps 57600 added\n" \
"1020 2020-10-09 21:31:12 +0200 stc: serial tt control\n" \
"1019 2020-10-09 19:28:53 +0200 cleanup old build scripts\n" \
"1018 2020-10-09 17:48:20 +0200 removed un used files\n" \
"1017 2020-10-09 17:40:52 +0200 zipper: removed obsolete startrocrail.sh\n" \
"1016 2020-10-09 16:59:29 +0200 switch: default active switch time\n" \
"1015 2020-10-09 09:04:23 +0200 desktoplink.sh: copy link also to /usr/share/applications in case of super user privilege\n" \
"1014 2020-10-08 17:56:32 +0200 makefile correction for copy desktoplink.sh\n" \
"1013 2020-10-08 15:36:17 +0200 rocview.sh: fix for server path\n" \
"1012 2020-10-08 08:21:15 +0200 make install: use $HOME/Rocrail as target instead of /opt/rocrail\n" \
"1011 2020-10-07 17:25:00 +0200 loc: decoupler flag added for query in XMLScript\n" \
"1010 2020-10-03 10:03:33 +0200 xmlscript: condition comparator contains ~ added\n" \
"1009 2020-10-01 08:05:09 +0200 clntcon,wio: use reported nodeid for targetting commands\n" \
"1008 2020-09-29 19:19:25 +0200 locdialog: CAM options for use in the throttle dialog\n" \
"1007 2020-09-28 10:20:57 +0200 switch: action state fix\n" \
"1006 2020-09-25 21:53:23 +0200 model: regard only active locos in case of checking restored\n" \
"1005 2020-09-25 10:36:50 +0200 model: check if all locos did restore before starting the auto mode (optional, default off)\n" \
"1004 2020-09-22 08:47:06 +0200 ahome: report all event childs\n" \
"1003 2020-09-22 08:13:01 +0200 wio: stop loco in case of empty battery\n" \
"1002 2020-09-19 14:55:44 +0200 tt: compiler fix for leopard\n" \
"1001 2020-09-18 15:23:40 +0200 basedlg: format rfid sensor address\n" \
"1000 2020-09-18 11:10:27 +0200 wio: use also the 32bit RFID as number to identify sensors\n" \
"999 2020-09-18 07:47:06 +0200 switch: fix for save protect ID pointer\n" \
"998 2020-09-17 08:14:32 +0200 block: generateinatenter command added\n" \
"997 2020-09-15 10:23:13 +0200 scheduledialog: layout fix\n" \
"996 2020-09-15 10:04:08 +0200 fix for double translation 'run around'\n" \
"995 2020-09-15 08:24:04 +0200 replaced headshunt with runaround\n" \
"994 2020-09-14 13:52:14 +0200 operator: headshunt option added\n" \
"993 2020-09-14 11:17:09 +0200 xmlscript: check condition for NULL\n" \
"992 2020-09-14 09:32:29 +0200 location: enable select shortest block added\n" \
"991 2020-09-14 08:19:32 +0200 model: fix for checking the location for shortest block\n" \
"990 2020-09-14 08:03:31 +0200 location: select shotest block option added\n" \
"989 2020-09-12 11:19:26 +0200 actions: test parameter\n" \
"988 2020-09-11 10:14:50 +0200 xmlscript: reslove function command\n" \
"987 2020-09-11 08:36:01 +0200 basedlg: show cmd:param in case desc is empty\n" \
"986 2020-09-11 07:55:37 +0200 text: find loco for car\n" \
"985 2020-09-10 15:08:40 +0200 operatordlg: added missing translation for assigned locomotive\n" \
"984 2020-09-10 10:17:10 +0200 translation merge: cs, sl, fr, sr_latin, de, nl\n" \
"983 2020-09-09 12:21:17 +0200 action,xmlscript: add car properties\n" \
"982 2020-09-09 11:11:37 +0200 action: use caller parameter as local\n" \
"981 2020-09-09 10:30:48 +0200 action,xmlscript: parameter option added\n" \
"980 2020-09-09 08:30:25 +0200 car: extended actions\n" \
"979 2020-09-09 08:22:10 +0200 car: actions extended\n" \
"978 2020-09-08 14:48:34 +0200 car: action support added (wip)\n" \
"977 2020-09-08 10:19:49 +0200 SLIM-COLOR: small blocks symbol with shunt border\n" \
"976 2020-09-08 10:15:53 +0200 small block shunting\n" \
"975 2020-09-06 10:55:38 +0200 tt: reject movement events for detecting the wanted position\n" \
"974 2020-09-06 10:27:14 +0200 vendors: FichtelBahn added\n" \
"973 2020-09-05 08:51:23 +0200 DB-Signals: semaphores with shunting\n" \
"972 2020-09-05 08:26:58 +0200 router: use long long to be able to check 33 functions on 32 bit systems\n" \
"971 2020-09-04 15:26:10 +0200 aboxdlg: support for general URL links\n" \
"970 2020-09-04 13:36:52 +0200 router: fix for checking functions\n" \
"969 2020-09-04 10:18:36 +0200 bidib: only send wanted function group instead of all\n" \
"968 2020-09-04 08:46:58 +0200 renderer: fix for semaphore shunting-shunt\n" \
"967 2020-09-04 08:00:35 +0200 action: shift function option support by '!' prefix\n" \
"966 2020-09-03 15:56:22 +0200 rascii,serial: not set DSR to avoid resetting the Arduino\n" \
"965 2020-09-03 10:56:40 +0200 bidib: query inputs option added\n" \
"964 2020-09-03 10:29:48 +0200 bidib: only query input ports in case the node has flat addressing\n" \
"963 2020-09-03 08:47:32 +0200 rascii: fix for Arduino reset waiting\n" \
"962 2020-09-02 13:10:15 +0200 bidib: fix for query inputs only\n" \
"961 2020-08-31 10:30:05 +0200 tt: reportedpos attribute added\n" \
"960 2020-08-30 08:33:41 +0200 tt: reported position state support\n" \
"959 2020-08-30 07:40:00 +0200 wiodlg: disable subtype in case of none RCAN modules\n" \
"958 2020-08-29 09:51:27 +0200 cc: flush UART buffer after connect\n" \
"957 2020-08-29 08:17:04 +0200 wiodlg: fix for setting I/O\n" \
"956 2020-08-28 08:47:07 +0200 tt: set bridge position in regval on action\n" \
"955 2020-08-26 15:04:32 +0200 planpanel: fix for rotating by dialog\n" \
"954 2020-08-25 08:35:49 +0200 ledbutton: fix for reset bitmap\n" \
"953 2020-08-24 10:02:53 +0200 throttle: select train member on shift and select to show functions\n" \
"952 2020-08-23 08:21:56 +0200 tt,bidib: fix for rotation direction\n" \
"951 2020-08-22 07:42:11 +0200 tt: next/prev track onestep\n" \
"950 2020-08-21 14:35:34 +0200 bidib,onestep: support for position events in 1.5 degrees\n" \
"949 2020-08-21 07:59:38 +0200 tt,item,planpanel: fix for state change for unknown sensor events\n" \
"948 2020-08-20 10:29:49 +0200 tt,bidib: stepcontrol report position support\n" \
"947 2020-08-20 09:02:54 +0200 tt: support for stepcontrol\n" \
"946 2020-08-19 05:23:46 +0200 item: set tooltip in text slider\n" \
"945 2020-08-17 10:05:10 +0200 tt: check animation under/overflow\n" \
"944 2020-08-16 14:56:12 +0200 tt,extcan: support for tt animation\n" \
"943 2020-08-15 12:12:51 +0200 rcan: support for SoD of 32 I/O in one frame\n" \
"942 2020-08-14 10:52:18 +0200 rcan/utils: trace fixes\n" \
"941 2020-08-14 08:01:49 +0200 mvtrack: convert kmh to mph if wanted\n" \
"940 2020-08-13 14:30:13 +0200 rcan/utils: fix for extended frame ID\n" \
"939 2020-08-12 10:36:40 +0200 loc: disable secondnextblock in case of engine type automobile\n" \
"938 2020-08-11 10:45:50 +0200 ucan: sleep 10ms in case write buffer full\n" \
"937 2020-08-11 10:29:19 +0200 socketcan: 10 retries in case no buffer space left on write\n" \
"936 2020-08-10 15:44:50 +0200 ledbutton: fix for scaling icon\n" \
"935 2020-08-10 10:59:54 +0200 extcan: only dump CAN frames in case of trace level develop\n" \
"934 2020-08-06 10:59:01 +0200 cbus: removed debug tracing\n" \
"933 2020-08-06 10:39:09 +0200 model: fix for reset modular plan title\n" \
"932 2020-08-05 08:04:00 +0200 feedbackdialog: show switch usage too\n" \
"931 2020-08-04 08:11:38 +0200 loc: check if reverse Vmax > 0 before using it\n" \
"930 2020-08-03 17:27:22 +0200 loc: send Rmax to the digint in case the loco runs reverse\n" \
"929 2020-08-03 08:55:12 +0200 wio: fix for system commands\n" \
"928 2020-08-01 08:12:52 +0200 wio: trace correction\n" \
"927 2020-07-31 13:57:45 +0200 route,model: set locked route command added\n" \
"926 2020-07-31 08:09:12 +0200 clock,meter: set time also in case of freeze status\n" \
"925 2020-07-31 07:42:00 +0200 rcan: traces for sublibs added\n" \
"924 2020-07-29 19:07:56 +0200 rcan/ascii: error parsing added\n" \
"923 2020-07-26 08:32:36 +0200 switch: new commands for channel tune test\n" \
"922 2020-07-25 19:03:34 +0200 rcan: fix for renaming the cbus subnode in rcan\n" \
"921 2020-07-24 15:01:17 +0200 desktoplink.cmd: create also shortcut in the Rocrail directory\n" \
"920 2020-07-24 10:19:42 +0200 rcan: decoder check support added\n" \
"919 2020-07-24 07:29:01 +0200 desktoplink.cmd: use the SpecialFolders command to retrieve the Desktop path\n" \
"918 2020-07-23 17:49:03 +0200 desktoplink.sh: use temp. filename without the use of environment variables\n" \
"917 2020-07-23 12:23:10 +0200 ecos: add view/get/query for the link manager 20\n" \
"916 2020-07-23 12:11:47 +0200 ecos: add ecoslink view 20\n" \
"915 2020-07-23 10:40:39 +0200 rcan: name config added\n" \
"914 2020-07-21 11:35:03 +0200 control: fix for realtime clock\n" \
"913 2020-07-20 08:00:46 +0200 tt: support rotation speed for WIO type\n" \
"912 2020-07-19 15:42:29 +0200 canservice: reuse port\n" \
"911 2020-07-19 10:45:53 +0200 WIO fixes\n" \
"910 2020-07-18 22:37:14 +0200 translation merge: de, nl, fr\n" \
"909 2020-07-18 16:39:13 +0200 rcan/tcp: fix for copy CAN EID\n" \
"908 2020-07-16 16:18:29 +0200 wio: option 3 added\n" \
"907 2020-07-16 16:13:58 +0200 zipper.sh: removed rocctrl\n" \
"906 2020-07-16 14:32:51 +0200 wio: options byte 3 added\n" \
"905 2020-07-16 09:20:41 +0200 wio/rcan: extended subtype added\n" \
"904 2020-07-15 17:57:57 +0200 cbus: crash fix for double posting same frame (double free)\n" \
"903 2020-07-15 09:51:44 +0200 rcan: tracelevel corrections\n" \
"902 2020-07-14 12:11:45 +0200 rocrailinidialog: uudp added\n" \
"901 2020-07-14 10:08:45 +0200 uudp: save client ip:port\n" \
"900 2020-07-14 08:25:06 +0200 UUDP library added\n" \
"899 2020-07-14 07:17:22 +0200 operatordlg: fix for OK in case of modal\n" \
"898 2020-07-14 06:58:20 +0200 RocControl removed\n" \
"897 2020-07-13 16:49:47 +0200 extcan: dcc state event\n" \
"896 2020-07-13 07:11:30 +0200 rcan: avoid cbus bootloader conflict\n" \
"895 2020-07-12 19:30:55 +0200 wrapper: new canprot attribute\n" \
"894 2020-07-12 19:29:56 +0200 rcan: protocol choise; default RCAN+CBUS\n" \
"893 2020-07-12 19:15:42 +0200 trainpanel: open operator dialog read/write on properties\n" \
"892 2020-07-12 14:16:30 +0200 cbusnodedlg: correction for WIO subtype\n" \
"891 2020-07-12 11:10:59 +0200 tt: stop calibrate command support added\n" \
"890 2020-07-11 17:26:42 +0200 rcan: support for environment events\n" \
"889 2020-07-11 15:31:00 +0200 tt: wio lights support added\n" \
"888 2020-07-11 14:54:37 +0200 tt: use one function for next/prev track\n" \
"887 2020-07-11 14:37:14 +0200 tt: wio next/prev support added\n" \
"886 2020-07-11 08:26:08 +0200 tt: fix for wio type\n" \
"885 2020-07-10 18:29:39 +0200 car: fix for mapped function reporting\n" \
"884 2020-07-10 15:46:04 +0200 extcan: rfid processing ack\n" \
"883 2020-07-10 09:52:38 +0200 got: use sectionID+1 as seonsor addr\n" \
"882 2020-07-09 07:09:14 +0200 car: added timed function support\n" \
"881 2020-07-08 15:57:23 +0200 rcan: support for SYS_STATE events and responses\n" \
"880 2020-07-08 09:16:29 +0200 vscode project settings\n" \
"879 2020-07-08 08:53:41 +0200 vscode settings\n" \
"878 2020-07-08 08:30:32 +0200 rcan: process mobile drive acks\n" \
"877 2020-07-07 14:30:14 +0200 Rocweb: update function lables on shift\n" \
"876 2020-07-07 13:49:30 +0200 Rocweb: Shift button added\n" \
"875 2020-07-07 09:54:07 +0200 guiframe: programming menu reorg\n" \
"874 2020-07-07 06:36:49 +0200 rcan: csnodeid added to redirect zero bus mobile commands to WIO-Prog2\n" \
"873 2020-07-06 16:39:30 +0200 cbus: show loco session number in trace\n" \
"872 2020-07-06 15:40:37 +0200 rcan: splitup cbus&rcan in separate files\n" \
"871 2020-07-06 08:03:28 +0200 got: section mode support on port 29910\n" \
"870 2020-07-05 15:30:27 +0200 tracelevel DEVELOP added, fix for cbusnodedlg include\n" \
"869 2020-07-05 14:49:37 +0200 rocnetnode,rcan: include fix\n" \
"868 2020-07-05 11:23:01 +0200 rcan: instruction codes structurred\n" \
"867 2020-07-04 15:42:34 +0200 rcan,extcan: text and sound support\n" \
"866 2020-07-04 10:17:24 +0200 extcan: dump CANFrame function\n" \
"865 2020-07-04 09:57:09 +0200 rcan: fix for sending text\n" \
"864 2020-07-04 09:01:48 +0200 rcan,extcan: text command added\n" \
"863 2020-07-03 22:21:47 +0200 remove vscode from source tree\n" \
"862 2020-07-03 18:04:49 +0200 extcan: filter out echo and ack\n" \
"861 2020-07-03 13:02:19 +0200 rcan: ignore ack messages\n" \
"860 2020-07-03 12:34:54 +0200 ignore CAN echo (socketCAN)\n" \
"859 2020-07-03 12:23:02 +0200 rcan: acknowledge WIO sensor events\n" \
"858 2020-07-03 10:55:27 +0200 wiodlg: type added\n" \
"857 2020-07-02 18:00:04 +0200 rcan: cleanup codes\n" \
"856 2020-07-02 17:22:38 +0200 rcan: removed bootloader opcode\n" \
"855 2020-07-02 15:27:41 +0200 rcan: cleanup opcodes\n" \
"854 2020-07-02 14:26:27 +0200 trace: fix for async file writing in case of no file\n" \
"853 2020-07-02 11:46:21 +0200 rcan: fix for formatting extended frames with dlc=0\n" \
"852 2020-07-01 16:11:53 +0200 rcan: use extended frames for WIO-CAN\n" \
"851 2020-07-01 09:05:51 +0200 importantchanges update\n" \
"850 2020-07-01 07:53:59 +0200 naming corrections\n" \
"849 2020-06-30 22:50:51 +0200 rcan: include correction\n" \
"848 2020-06-30 22:44:24 +0200 naming corrections\n" \
"847 2020-06-30 19:17:59 +0200 VSC tasks (WIP)\n" \
"846 2020-06-30 17:52:25 +0200 VSC tasks (WIP)\n" \
"845 2020-06-30 17:30:54 +0200 VSC files\n" \
"844 2020-06-30 16:22:04 +0200 cbusdlg: enable fonfof\n" \
"843 2020-06-30 11:09:16 +0200 signaldialog: new translation 'Reset by sensor' added\n" \
"842 2020-06-30 09:06:09 +0200 installwx: use apt-get to allow wildcards\n" \
"841 2020-06-29 16:34:52 +0200 zoomdlg: button size fix\n" \
"840 2020-06-29 14:26:50 +0200 WIO-CAN: WIP\n" \
"839 2020-06-28 15:53:04 +0200 signal: reset sensor added\n" \
"838 2020-06-28 10:02:00 +0200 translation merge: en_GB, zh_CN, fr, hu, de, nl\n" \
"837 2020-06-28 09:09:28 +0200 cbus: WIO programming and bidi support\n" \
"836 2020-06-27 17:28:40 +0200 car: revert extra function broadcast\n" \
"835 2020-06-27 14:34:07 +0200 cbus: extcan DCC support (WIP)\n" \
"834 2020-06-26 15:30:32 +0200 seltab: show tracksensors in case of position event\n" \
"833 2020-06-26 14:54:39 +0200 seltab: WIO-Step goto track support\n" \
"832 2020-06-26 14:23:49 +0200 seltabdlg: decoder track column added\n" \
"831 2020-06-26 13:19:55 +0200 seltab: WIO-Step support\n" \
"830 2020-06-26 06:45:14 +0200 posseldlg: pause button for track picking\n" \
"829 2020-06-25 16:53:12 +0200 posseldlg: load existing tracks\n" \
"828 2020-06-25 15:51:37 +0200 item: clone new track nodes\n" \
"827 2020-06-25 15:28:00 +0200 WIO-Step: calibration\n" \
"826 2020-06-24 16:03:02 +0200 posseldlg: dialog for positioning tracks\n" \
"825 2020-06-23 22:01:33 +0200 roads: new symbols for ghost and closed blocks (Wolfgang)\n" \
"824 2020-06-23 17:36:42 +0200 loc: fix for restoring gotoBlock -> bk type only\n" \
"823 2020-06-23 16:57:42 +0200 WIO motor support: WIP\n" \
"822 2020-06-23 12:14:46 +0200 acdecdlg,textdlg,variabledlg: index find added\n" \
"821 2020-06-23 10:46:07 +0200 z21: wait until the network/z21 is available before proceed\n" \
"820 2020-06-23 10:21:23 +0200 operatordlg: index filter added\n" \
"819 2020-06-23 10:11:42 +0200 WIO motor support: WIP\n" \
"818 2020-06-22 19:07:17 +0200 seltabtrackdlg: deocder tracknr added\n" \
"817 2020-06-22 15:32:55 +0200 wio stepper: wip\n" \
"816 2020-06-22 14:22:38 +0200 locdialog: search index field added\n" \
"815 2020-06-22 10:25:52 +0200 cbus: wio event opc added\n" \
"814 2020-06-21 11:55:31 +0200 router: disabled minocc/numBlock error\n" \
"813 2020-06-21 11:08:35 +0200 cbus,flim: trace correction for version\n" \
"812 2020-06-21 10:25:19 +0200 cbus: allow lower bps for sublib type USB\n" \
"811 2020-06-20 15:15:33 +0200 wiodlg: CAN-ID field added\n" \
"810 2020-06-20 14:34:32 +0200 wiodlg: CANIO, reboot command after set and OK\n" \
"809 2020-06-20 11:25:00 +0200 CANIO: set config\n" \
"808 2020-06-20 08:10:53 +0200 throttle: debug trace\n" \
"807 2020-06-20 08:09:58 +0200 car: broadcast the origin function before the mapped one\n" \
"806 2020-06-19 15:59:35 +0200 CANIO: WIP\n" \
"805 2020-06-18 10:13:49 +0200 z21: if addr == 0 use port value\n" \
"804 2020-06-18 08:55:44 +0200 loconet: extra traces for function group 1 and 2\n" \
"803 2020-06-18 08:53:52 +0200 cbusnodedlg: labe and description correction for WIO\n" \
"802 2020-06-18 08:09:16 +0200 rocrailinidialog,z21: swap gates option\n" \
"801 2020-06-17 19:27:05 +0200 z21: fix for BinState 28 border -> 32\n" \
"800 2020-06-15 16:00:49 +0200 actionsctrldlg: fix for time units\n" \
"799 2020-06-15 15:59:56 +0200 wio: use revision number instead of version\n" \
"798 2020-06-15 10:07:35 +0200 actionsctrldlg: added time unit labels\n" \
"797 2020-06-15 07:57:13 +0200 operatordlg: open car throttle with dispatching maps\n" \
"796 2020-06-15 07:45:19 +0200 action: added missing loco state condition <stop>\n" \
"795 2020-06-14 12:12:22 +0200 time unit label corrections\n" \
"794 2020-06-14 10:41:49 +0200 loconet: extra trace for functions\n" \
"793 2020-06-13 14:16:37 +0200 item: fix for status text for crossing blocks\n" \
"792 2020-06-13 11:05:40 +0200 rocview: time labels\n" \
"791 2020-06-13 10:27:26 +0200 guiapp: fix for merging operator events in case of child nodes\n" \
"790 2020-06-12 21:33:25 +0200 guestlocodlg: wx assert fix\n" \
"789 2020-06-12 14:55:37 +0200 cbus: fix for extcan switch commands\n" \
"788 2020-06-12 10:13:48 +0200 cbus: ascii tracelevel corrections\n" \
"787 2020-06-11 14:26:26 +0200 stagedlg,seltabdlg: time unit labels\n" \
"786 2020-06-11 10:46:32 +0200 item: show fromblock in destination block in the statusbar\n" \
"785 2020-06-11 09:44:39 +0200 cbus: extcan support for WIO\n" \
"784 2020-06-10 11:20:28 +0200 item: show loco destination in statusbar\n" \
"783 2020-06-09 16:16:12 +0200 cbusdlg: option 500000 bps added\n" \
"782 2020-06-09 15:49:20 +0200 cbus: added sublib LAWICEL / SLCAN\n" \
"781 2020-06-09 10:33:43 +0200 guiframe: fix for selecting a loco from block by mouse pointer\n" \
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
