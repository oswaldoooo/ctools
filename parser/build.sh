#!/bin/bash
if [[ "$CODELAB" != "" ]];then
	CTOOLS="$CODELAB/ctools"
	g++ -o $CTOOLS/lib/libparser.so -std=c++17 -lstdc++ -L$CTOOLS/lib -ltools parser.cpp -I$CTOOLS -shared -fPIC
elif [[ "$CTOOLS" != "" ]];then
	g++ -o $CTOOLS/lib/libparser.so -std=c++17 -lstdc++ -L$CTOOLS/lib -ltools parser.cpp -I$CTOOLS -shared -fPIC
else
	echo "codelab envrionment variable not set"
fi