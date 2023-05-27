#!/bin/bash
if [[ "$CODELAB" != "" ]];then
	CTOOLS="$CODELAB/ctools"
	g++ -o $CTOOLS/lib/libtools.so -std=c++17 tools.cpp $CTOOLS/error/error.cpp cinsecure.cpp -I$CTOOLS -shared -fPIC
elif [[ "$CTOOLS" != "" ]];then
	g++ -o $CTOOLS/lib/libtools.so -std=c++17 tools.cpp $CTOOLS/error/error.cpp cinsecure.cpp -I$CTOOLS -shared -fPIC
else
	echo "codelab envrionment variable not set"
fi