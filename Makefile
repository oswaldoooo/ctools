installdir=lib
all:lib buildtools buildmarsha
	echo installall
lib:libnet libstd
	@echo "library build success"
.PHONY:buildtools
buildtools:
	g++ -o ${installdir}/libtools.so -std=c++17 tools/tools.cpp tools/cinsecure.cpp error/error.cpp -I.. -I.
.PHONY:buildmarsha
buildmarsha:
	g++ -o ${installdir}/libmarshal.so -shared -fPIC show.cc -I.. -ljsoncpp -std=c++17
.PHONY:buildtest
buildtest:
	g++ -o bin/newtest lab/test.cc -std=c++17 -g
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
.PHONY:libnet
libnet:
	gcc -c net/core.c && ar -r ${installdir}/libnet.a core.o && gcc -fPIC -shared -o ${installdir}/libnet.so net/core.c
.PHONY:libstd
libstd:
	gcc -c stdlib/*.c arraylist/*.c && ar -r ${installdir}/libstd.a core.o
:PHONY:libarray
libarray:
	gcc -c arraylist/core.c && ar -r ${installdir}/libarray.a core.o
.PHONY:clean
clean:
	rm ${installdir}/*
.PHONY:clear
clear:
	rm *.o