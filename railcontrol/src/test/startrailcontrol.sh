#/bin/bash

echo Starting railcontrol
cd ..
./railcontrol test/testconfig.conf 2> /dev/null > /dev/null &
