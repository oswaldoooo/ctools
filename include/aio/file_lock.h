#ifndef _filelock_hpp
#define _filelock_hpp
#include <fcntl.h>
#include <cstddef>
#include <vector>
struct keypair{
	public:
	int a;
	int b;
	keypair(int a[2]){
		this->a=a[0];
		this->b=a[1];	
	}
};
namespace aio{
class filemutex{
	private:
		bool lockbyself;
		char* lockname=NULL;
		int lfd=0;
		std::vector<struct keypair> pos;	
		void load();
	public:
		filemutex(const char* lockname);
		filemutex(const char* lockname,off_t begin,off_t length);
		bool move_lock(off_t begin,off_t length);
		~filemutex();

};
class file{
	private:
		filemutex fm;
		void open(const char*filename,mode_t mode,int perm,off_t begin,off_t length);//base this init
	public:
		file(const char*filename,mode_t mode);//open with whole lock
		file(const char*filename,mode_t mode,int perm);
		file(const char*filename,mode_t mode,off_t begin,off_t length);//open with block lock
		~file();
};
};
#endif
