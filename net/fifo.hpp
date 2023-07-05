#pragma once
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#define default_buffer_size 1<<10
namespace ctools {
enum class mod{read=999,write=998};
class FIFO{
private:
    mod md;
    int fid;
    size_t buffer_size=default_buffer_size;
    char *buffer;
    bool isclose;
public:
    FIFO(const char *fifo_path,mod md_){
        md=md_;
        if (md==mod::read){
            fid=open(fifo_path, O_RDONLY);
        }else{
            fid=open(fifo_path, O_WRONLY);
        }
        if (fid<0){
            if(mkfifo(fifo_path, 0660)<0) throw std::logic_error("open fifo failed");
            if (md==mod::read){
                fid=open(fifo_path, O_RDONLY);
            }else{
                fid=open(fifo_path, O_WRONLY);
            }
            if (fid<0) throw std::logic_error("open fifo failed");
        }
        if(md==mod::read) buffer=new char[buffer_size];
    }
    std::string read(){
        if(md!=mod::read) throw std::logic_error("not read mode");
        if (strlen(buffer)>0) memset(buffer, 0, sizeof(char)*strlen(buffer));
        int ok=::read(fid, buffer,buffer_size);
        if (ok<0){
            throw std::logic_error("read from fifo failed");
        }
        return buffer;
    }
    void write(const char *data){
        if(md==mod::read) throw std::logic_error("fifo is read mode");
        if(::write(fid, data, sizeof(char)*strlen(data))<0)throw std::logic_error("write to fifo failed");
    }
    void close()noexcept{
        if(isclose)return;
        ::close(fid);
        if(md==mod::read)delete [] buffer;
        isclose=true;
    }
};
FIFO& operator<<(FIFO& origin,const char *data){
    origin.write(data);
    return origin;
}
FIFO& operator>>(FIFO& origin,char *buffer){
    std::string ans=origin.read();
    memmove(buffer, ans.c_str(), sizeof(char)*ans.length());
    return origin;
}
template <typename T> void free(T *);
}
