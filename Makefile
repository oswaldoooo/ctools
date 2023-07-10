all:buildtools buildmarsha
	echo installall
.PHONY:buildtools
buildtools:
	g++ -o lib/libtools.so -std=c++17 tools/tools.cpp tools/cinsecure.cpp error/error.cpp -I.. -I.
.PHONY:buildmarsha
buildmarsha:
	g++ -o lib/libmarshal.so -shared -fPIC show.cc -I.. -ljsoncpp -std=c++17
.PHONY:buildtest
buildtest:
	g++ -o test app.cc show.cc -ljsoncpp -I.. -std=c++17 -g
.PHONY:bsclient
bsclient:
	g++ -o bsclient app.cc show.cc -ljsoncpp -I. -I.. -std=c++17
.PHONY:clean
clean:
	rm lib/*