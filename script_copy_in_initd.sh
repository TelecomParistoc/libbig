#!/bin/sh
#
#loop
#
#Starts robot loop
#

case "$1" in
	start)
		echo "Starting loop..."
		/etc/init.d/loop
		;;
	stop)
		echo "Stopping loop..."
		killproc /etc/init.d/loop
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
		statusproc /etc/init.d/loop
		;;
	*)
		echo "Usage: $0 {start|stop|reload|restart|status}"
		exit 1
		;;
esac
