#include <aio/file_lock.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
void aio::file::open(const char*filename,mode_t mode,int perm,off_t begin,off_t length){
	size_t filenamelen=strlen(filename);
	if (filenamelen>0){
		char filelockname[filenamelen+5];
		memmove(filelockname,filename,filenamelen);
		memmove(filelockname+filenamelen,".mux",4);
		filelockname[filenamelen+4]=0;

	}
}
aio::file::file(const char*filename,mode_t mode):fm(filename){

}
aio::file::file(const char*filename,mode_t mode,int perm):fm(filename){

}

aio::file::file(const char*filename,mode_t mode,off_t begin,off_t length):fm(filename,begin,length){

}
aio::file::~file(){}
aio::filemutex::filemutex(const char*lockname){
	size_t locknamelen=strlen(lockname);
	if (locknamelen>0){
		this->lockname=(char*)malloc(locknamelen+5);
		memmove(this->lockname,lockname,locknamelen);
		memmove(this->lockname,".mux",4);
		this->lockname[locknamelen+4]=0;
		this->lfd=open(this->lockname,O_CREAT|O_EXCL|O_RDWR,0600);
		if(this->lfd<0){
			throw std::logic_error("lock file failed");
		}
		close(this->lfd);
	}
}

aio::filemutex::filemutex(const char* lockname,off_t begin,off_t length){
	size_t locknamelen=strlen(lockname);
    if (locknamelen>0){
        this->lockname=(char*)malloc(locknamelen+5);
        memmove(this->lockname,lockname,locknamelen);
        memmove(this->lockname,".mux",4);
        this->lockname[locknamelen+4]=0;
        this->lfd=open(this->lockname,O_CREAT|O_RDWR,0600);
        if(this->lfd<0){
            throw std::logic_error("lock file failed");
        }
		//close(this->lfd);
		struct stat st;
		stat(this->lockname,&st);
		if(st.st_size==0){
			throw std::logic_error("lock file failed");
		}
		this->load();
		if (this->pos.size()>0){
			for(size_t i=0;i<this->pos.size();i++){
				if (this->pos[i].a<=begin&&this->pos[i].a+this->pos[i].b>=begin){
					//include this
					throw std::logic_error("lock file failed");
				}
			}
			//write to it
			char tibuffer[20];memset(tibuffer,0,20);
			lseek(this->lfd,0,0);
			snprintf(tibuffer,20,"%lld %lld ",begin,length);
			write(this->lfd,tibuffer,20);
		
		}
		close(this->lfd);
    }

}

void parsemap(char* buffer,std::vector<struct keypair>& vec){
	size_t bufferlen=strlen(buffer);
	if(bufferlen>0){
		int i=0;
		char ti[bufferlen+1];
		memset(ti,0,bufferlen+1);
		char* next=buffer;
		char* last=buffer;
		int curr=0;
		int cache[2]={-1,-1};
		while(i<bufferlen){
			next=strstr(last," ");
			if(next==NULL)break;
			if(next-last>0){
				memset(ti,0,strlen(ti));
				memmove(ti,last,next-last);
				if(cache[0]==-1)
					cache[0]=atoi(ti);
				else if(cache[1]==-1){
					cache[1]=atoi(ti);
					vec.push_back(cache);
					cache[0]=-1;
					cache[1]=-1;
				}
				//else


			}
			last=next+1;
		}
	}

}
void aio::filemutex::load(){
	struct stat st;
	if (stat(this->lockname,&st)<0) throw std::logic_error("get filename stat failed");
	if(st.st_size>0){
		char buffer[st.st_size+1];
		int lfd=open(this->lockname,O_RDWR|O_APPEND);
		if(lfd<=0)throw std::logic_error("open lockfile failed");
		if(read(this->lfd,buffer,st.st_size)>0){
			buffer[st.st_size]=0;
			this->pos.clear();
			parsemap(buffer,this->pos);

		}
		close(lfd);

	}

}
aio::filemutex::~filemutex(){
	if(this->lockname!=NULL)free(this->lockname);		
}
