#!/bin/bash
# test_c2_bus.sh - Stress test C2 bus by constantly writing a value to xram and
#                  reading it back
#
# NOTE: Requires bash for 'RANDOM' to work

C2_BUS_URI='c2drv:///dev/c2_bus'

if ! ./si4010prog -d "$C2_BUS_URI" reset; then
	echo "Unable to reset device, error code $?"
	exit 1
fi

# Initialize pseudo random test sequence
TESTVAL=`date | md5sum | cut -f 1 -d ' '`
TMPFILE='/tmp/c2_test.txt'

while true; do
	ADDR=$((RANDOM%(4096-16)))
	TESTVAL=`echo "$TESTVAL" | md5sum | cut -f 1 -d ' '`

	date
	if ! ./si4010prog -d "$C2_BUS_URI" "wxram:$ADDR,$TESTVAL"; then
		echo "Unable to write value, error code $?"
		exit 1
	fi

	if ! ./si4010prog -d "$C2_BUS_URI" "dxram:$ADDR,16" > "$TMPFILE"; then
		echo "Unable to readback value, error code $?"
		exit 1
	fi

	RESULT=`cat "$TMPFILE" | cut -d ' ' -f 4-5 | tr -d ' '`
	if [ X"$RESULT" != X"$TESTVAL" ]; then
		echo "Mismatch!"
		echo "Output:"
		cat "$TMPFILE"
		echo "expected: $TESTVAL"
		exit 1
	fi
done
