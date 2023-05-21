#!/bin/bash
g++ -o libparser.so -std=c++17 -lstdc++ -L$USRLIB -ltools parser.cpp -I$USRINCLUDE -shared -fPIC
mv libparser.so ../lib