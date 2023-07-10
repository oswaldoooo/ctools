rootpath=/Users/oswaldo/dev/cpp/lib
jsoncpp=/opt/homebrew/Cellar/jsoncpp/1.9.5
boost=/opt/homebrew/Cellar/boost/1.82.0_1
all:buildtools buildmarsha
	echo installall
.PHONY:buildtools
buildtools:
	g++ -o lib/libtools.so -std=c++17 tools/tools.cpp tools/cinsecure.cpp error/error.cpp -I.. -I$(rootpath)
.PHONY:buildmarsha
buildmarsha:
	g++ -o lib/libmarshal.so -shared -fPIC show.cc -I$(jsoncpp)/include -L$(jsoncpp)/lib -I.. -ljsoncpp -std=c++17
.PHONY:buildtest
buildtest:
	g++ -o test app.cc show.cc -L$(jsoncpp)/lib -ljsoncpp -I$(jsoncpp)/include -I$(boost)/include -I.. -std=c++17 -g
.PHONY:bsclient
bsclient:
	g++ -o bsclient app.cc show.cc -L$(jsoncpp)/lib -ljsoncpp -I$(jsoncpp)/include -I$(boost)/include -I. -I.. -std=c++17
.PHONY:clean
clean:
	rm lib/*