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
OPTIONS="-c /var/www/html/config"
NAME=can2lan
DESC="M*rklin CAN to LAN gateway"
test -x $DAEMON || exit 0

. /lib/lsb/init-functions

case "$1" in
  start)
        echo -n "Starting $DESC: "
        start-stop-daemon --start --verbose --pidfile /var/run/$NAME.pid \
                --oknodo --exec $DAEMON -- $OPTIONS
        echo "$NAME."
	pidof can2lan > /var/run/$NAME.pid
    ;;
  stop)
        echo -n "Stopping $DESC: "
        start-stop-daemon --stop --verbose --pidfile /var/run/$NAME.pid \
                --oknodo --exec $DAEMON
        echo "$NAME."
    ;;
  force-reload|restart)
    $0 stop
    sleep 1
    $0 start
    ;;
  status)
    status_of_proc -p /var/run/$NAME.pid $DAEMON can2lan && exit 0 || exit $?
    ;;
  *)
    echo "Usage: /etc/init.d/can2lan {start|stop|restart|force-reload|status}"
    exit 1
    ;;
esac

exit 0
