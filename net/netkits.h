#pragma once
#include <cstdio>
#include <cstdlib>
namespace ctools {
class Client {
public:
    virtual bool isclose() = 0;
    virtual bool needwaitreturn() = 0;
    virtual void getback() = 0;
    // virtual void write_some(std::string*,boost::system::error_code*)=0;
    // virtual void read_some(char *outbuffer,boost::system::error_code*)=0;
};
}

namespace ctools {
class Server {
private:
    int server_id;

public:
    Server()
        : server_id(rand())
    {
    }
    virtual void register_con(const int conid) = 0;
    virtual void close_con() = 0;
    virtual bool write(const char* content) = 0;
    virtual bool read(char* buffer, size_t buffer_size) = 0;
    virtual void Do() = 0;
    virtual void safety_exit()
    {
        printf("server %d exit\n", server_id);
    }
};
class ServerImp : public Server {
};
}