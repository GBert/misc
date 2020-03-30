#/bin/bash

echo s | ncat localhost 2222

while [ `ps auxwf|grep ./railcontrol|wc -l` -gt 1 ] ; do
	echo Waiting on railcontrol exit
	sleep 1
done
rm /tmp/railcontrol.sqlite
echo Railcontrol stopped

