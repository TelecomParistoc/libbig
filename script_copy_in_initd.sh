#!/bin/sh
#
#loop
#
#Starts robot loop
#

### BEGIN INIT INFO
# Provides:          LaunchLoop.sh
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Starts robot loop
# Description:       This service starts robot loop
### END INIT INFO

case "$1" in
	start)
		echo "Starting loop..."
		/var/apps/loop /var/apps/robot_main &
		;;
	stop)
		echo "Stopping loop..."
		killproc /var/apps/loop
		;;
	reload)
		echo "Reloading loop..."
		killall -HUP loop
		;;
	restart)
		$0 stop
		sleep 1
		$0 start
		;;
	status)
		statusproc /var/apps/loop
		;;
	*)
		echo "Usage: $0 {start|stop|reload|restart|status}"
		exit 1
		;;
esac
