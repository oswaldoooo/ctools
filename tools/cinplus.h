// #ifndef DEFAULT_BUFFER_SIZE
#include <cstring>
#include <ostream>
#define DEFAULT_BUFFER_SIZE 20<<10
// #endif
#ifndef cinplus_h
#define cinplus_h
#include <ctime>
#include <iostream>
#include <thread>
template<class T>
class cinplus{
private:
	unsigned int waittime=10;
	bool isinput;
	void wait(clock_t starttime);
	T val;
	char *content;
public:
	cinplus();
	~cinplus();
	cinplus(unsigned int wttime);
	bool get();
	bool getline(unsigned int container_size=DEFAULT_BUFFER_SIZE);
	T load();
	char *getstr();
};
template <class T>
cinplus<T>::cinplus(unsigned int wttime){
	waittime=wttime;
}
template <class T>
void cinplus<T>::wait(clock_t starttime){
	isinput=false;
	while(clock()-starttime<waittime*CLOCKS_PER_SEC&&!isinput){
		continue;
	}
}
template<class T>
cinplus<T>::~cinplus(){
}
template<class T>
bool cinplus<T>::get(){
	T origin;
	clock_t now=clock();
	std::thread the([origin, this]()mutable{
		if(std::cin>>origin) {
			isinput=true;
			val=origin;
		}
	});
	if(the.joinable()) the.detach();
	wait(now);
	if(the.joinable()) the.join();
	return isinput;
}
template<class T>
bool cinplus<T>::getline(unsigned int container_size){
	clock_t now=clock();
	std::thread the([container_size,this]()mutable{
		char origin[container_size];std::cin.clear();
		if(std::cin.getline(origin,container_size)) {
			content=origin;
			isinput=true;
		}
	});
	if(the.joinable()) {
		the.detach();
	}
	wait(now);

	if(the.joinable()) {
		the.join();
	}
	return isinput;
}
template <class T>
T cinplus<T>::load(){return val;}
template <class T>
char *cinplus<T>::getstr(){
	// std::flush(std::cout);
	if(isinput){
		std::cin.clear();std::cin.sync();
		return content;
	}
	throw "input is not input";
}
#endif