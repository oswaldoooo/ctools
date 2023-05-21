#!/bin/bash
g++ -o libtools.so -std=c++17 tools.cpp ../error/error.cpp cinsecure.cpp -I$USRINCLUDE -shared -fPIC
mv libtools.so ../lib