#!/bin/sh

full_name=$1
tar -C / -xzf $full_name home/developer/src/mrsystem/www/uci.css
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/mrsystem.hlp
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/mrsystem.cfg
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/daemons.def
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/daemons.hlp
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/mrsystem.def
tar -C / -xzf $full_name home/developer/src/mrsystem/www/sysconf/daemons.cfg
tar -C / -xzf $full_name home/developer/src/mrsystem/www/uci.php
tar -C / -xzf $full_name home/developer/src/mrsystem/www/valchanged.sh
tar -C / -xzf $full_name home/developer/src/mrsystem/client_slcan/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_slcan/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/etc/init.d/mrsystem
tar -C / -xzf $full_name home/developer/src/mrsystem/client_gpios88/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_gpios88/s88.patch
tar -C / -xzf $full_name home/developer/src/mrsystem/client_gpios88/startmrgpios88
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/cane1b.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/sendeth.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/cand1b.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/mr_can.h
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/sendtoeth.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_can/m_cs2ms2.h
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/config.h
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_sint.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_gint.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_sstr.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_cmdline.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_intern.h
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_gstr.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/config/cfg_init.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/encode_can.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/cmd_get_can_bootldr.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/cmd_set_track_proto.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/cmd_set_system_status_val.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/mr_ipc.h
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/cmd_get_system_status_val.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/cmd_set_can_bootldr.c
tar -C / -xzf $full_name home/developer/src/mrsystem/libs/mr_ipc/decode_can.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cc/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cc/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms24l/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms24l/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cs2sl/cs2sl.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cs2sl/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cs2sl/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_zentrale/zentrale.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_zentrale/zentrale.h
tar -C / -xzf $full_name home/developer/src/mrsystem/client_zentrale/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_zentrale/canmember.h
tar -C / -xzf $full_name home/developer/src/mrsystem/client_zentrale/states.c
tar -C / -xzf $full_name home/developer/src/mrsystem/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/doku/konfig.htm
tar -C / -xzf $full_name home/developer/src/mrsystem/doku/erata.htm
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cs2eth/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_cs2eth/cs2eth.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms2/ms2.h
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms2/Makefile
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms2/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_ms2/ms2.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_log/main.c
tar -C / -xzf $full_name home/developer/src/mrsystem/client_log/log.c
tar -C / -xzf $full_name home/developer/src/railroad/can2udp/src/a20.h
tar -C / -xzf $full_name home/developer/src/railroad/can2udp/src/s88udp-bpi.c
tar -C / -xzf $full_name home/developer/src/railroad/can2udp/src/s88test-bpi.c
tar -C / -xzf $full_name home/developer/src/railroad/can2udp/src/s88udp-bpi.h
tar -C / -xzf $full_name home/developer/src/railroad/can2udp/src/a20.c
tar -C / -xzf $full_name home/developer/src/railroad/.git
sudo -u developer make -C /home/developer/src/mrsystem clean
rm /home/developer/src/mrsystem/lib/*
rm /home/developer/src/mrsystem/lib/include/*
sudo -u developer make -C /home/developer/src/mrsystem
make -C /home/developer/src/mrsystem install_apps
tar -C / -xzf $full_name var/www/uci.css
tar -C / -xzf $full_name var/www/sysconf/mrsystem.hlp
tar -C / -xzf $full_name var/www/sysconf/mrsystem.def
tar -C / -xzf $full_name var/www/uci.php
tar -C / -xzf $full_name var/www/valchanged.sh
tar -C / -xzf $full_name var/www/sysconf/daemons.hlp
tar -C / -xzf $full_name var/www/sysconf/daemons.cfg
tar -C / -xzf $full_name var/www/sysconf/daemons.def
echo "wakeup_s88 \"0\"" >> /etc/mrsystem
echo "gpio_s88 \"\"" >> /etc/mrsystem
echo "wakeup_s88 0" >> /var/www/sysconf/mrsystem.cfg
echo "gpio_s88 \"\"" >> /var/www/sysconf/mrsystem.cfg
tar -C / -xzf $full_name etc/init.d/mrsystem
