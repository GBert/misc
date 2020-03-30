#!/bin/bash

EXEC=./make_amalgamation.bash
CPP=amalgamation.cpp

rm $CPP

echo "#!/bin/bash
echo \$1
if [ \"x\" = \"x\$1\" ] ; then
	exit
fi

cat \$1 >> $CPP
" > $EXEC

chmod u+x $EXEC

cat $EXEC

for dir in . DataModel Hardware Logger Network Storage Utils WebServer ; do
	find $dir -maxdepth 1 -type f -name "*.cpp" -exec $EXEC {} \;
done

rm $EXEC
