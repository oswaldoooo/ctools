#!/bin/bash
clang++ -o libtools.so -std=c++17 tools.cpp ../error/error.cpp -I$USRINCLUDE -shared -fPIC