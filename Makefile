all:lib buildtools buildmarsha
	echo installall
lib:
	mkdir lib
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
ifile:
	g++ -o ifile ifile.cc -std=c++17 -lgflags
precheck:ifile
	cp cmd.txt .. && touch precheck && cd .. && ctools/ifile ctools
installlib:
	apt install libjsoncpp-dev -y && apt install libgflags-dev -y && apt install libyaml-cpp-dev -y && touch installlib
.PHONY:install
install:precheck lib buildtools buildmarsha
	bash install.sh
.PHONY:clean
clean:
	rm lib/*