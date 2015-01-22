#! /bin/sh
### BEGIN INIT INFO
# Provides:          can2lan
# Required-Start:    $network
# Required-Stop:     $network
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: M*rklin CAN to LAN gateway
# Description:       transfers CAN frames to TCP/UDP packets
#                    and vice versa
### END INIT INFO
#
# Author:	Gerhard Bertelsmann <info@gerhard-bertelsmann.de>
#

PATH=/bin:/usr/bin:/sbin:/usr/sbin
DAEMON=/usr/bin/can2lan
PIDFILE=/var/run/can2lan.pid
OPTIONS="-s -c /var/www/html/cs2"
test -x $DAEMON || exit 0

. /lib/lsb/init-functions

case "$1" in
  start)
        log_daemon_msg "Starting deferred execution scheduler" "can2lan"
        start_daemon -p $PIDFILE $DAEMON $OPTIONS
        log_end_msg $?
    ;;
  stop)
        log_daemon_msg "Stopping deferred execution scheduler" "can2lan"
        killproc -p $PIDFILE $DAEMON
        log_end_msg $?
    ;;
  force-reload|restart)
    $0 stop
    $0 start
    ;;
  status)
    status_of_proc -p $PIDFILE $DAEMON can2lan && exit 0 || exit $?
    ;;
  *)
    echo "Usage: /etc/init.d/can2lan {start|stop|restart|force-reload|status}"
    exit 1
    ;;
esac

exit 0
