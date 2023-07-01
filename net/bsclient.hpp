#include "boost/asio.hpp"
#include "boost/asio/buffer.hpp"
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/asio/ip/udp.hpp"
#include "boost/system/detail/error_code.hpp"
#include "netkits.h"
#include "parser/marshaler.h"
#include "show.h"
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
using namespace boost::asio;
typedef std::string string;

namespace ctools {
namespace net {
    enum class socktype { TCP = 999,
        UDP = 888,
        UNIX = 777 };
    struct request {
        string type;
        string path;
        string content;
        request() = delete;
        request(const char* typed)
            : type(typed)
        {
        }
    };
}
}

// add the request's marshal
template <>
string ctools::marshal(struct ctools::net::request* req)
{
    Json::Value jv;
    Json::FastWriter writer;
    jv["type"] = req->type;
    jv["path"] = req->path;
    jv["content"] = req->content;
    return writer.write(jv);
}

namespace ctools {
class Bsclient : public Client {
private:
    int* msgpipe;
    int* anspipe;
    size_t buffer_size = 1 << 10;

public:
    bool close = false;
    bool needwait = true;
    ip::tcp::endpoint end;
    io_service io;
    ip::tcp::socket* sock;
    Bsclient(string address, int port)
    {
        msgpipe = new int[2];
        anspipe = new int[2];
        if (pipe(msgpipe) == -1) throw std::logic_error("create msgpipe failed");
        if (pipe(anspipe) == -1) throw std::logic_error("create anspipe failed");
        end = ip::tcp::endpoint(ip::address::from_string(address), port);
        sock = new ip::tcp::socket(io);
    }
    Bsclient(string address, int port, int* msgpipe, int* anspipe)
    {
        msgpipe = msgpipe;
        anspipe = anspipe;
        end = ip::tcp::endpoint(ip::address::from_string(address), port);
        sock = new ip::tcp::socket(io);
    }
    int msgRPipe()
    {
        return msgpipe[0];
    }
    int msgWPipe()
    {
        return msgpipe[1];
    }
    int ansRPipe()
    {
        return anspipe[0];
    }
    int ansWPipe()
    {
        return anspipe[1];
    }
    void connect()
    {
        sock->connect(end);
    }
    bool isclose() override
    {
        return close;
    }
    bool needwaitreturn() override
    {
        return needwait;
    }
    void getback() override
    {
    }
    void write_some(std::string* data, boost::system::error_code* ec) override
    {
        this->sock->write_some(boost::asio::buffer(*data), *ec);
    }
    void read_some(char* outbuffer, boost::system::error_code* ec) override
    {
        this->sock->read_some(buffer(outbuffer, this->buffer_size), *ec);
    }
};

/*
support tcp,udp,unix socket(will be update not far)
*/
class Bstclient : public Client {
private:
    int* msgpipe;
    int* anspipe;
    string path;
    size_t buffer_size = 1 << 10;
    net::socktype st;
    struct net::request req = "json";

public:
    bool close = false;
    bool needwait = true;
    ip::tcp::endpoint tend;
    ip::udp::endpoint uend;
    io_service io;
    ip::tcp::socket* tsock;
    ip::udp::socket* usock;
    Bstclient(string url)
    {
#define reg_exp "^bs(t|u|)://(.*):([\\d]{1,5})/(.*)"
        std::regex expression(reg_exp);
        std::smatch match;
        if (!std::regex_search(url, match, expression)) throw std::logic_error("bs url is not vaild");
        int port = atoi(match[3].str().c_str());
        if (match[1].str().compare("t") == 0) {
            st = net::socktype::TCP;
            tend = ip::tcp::endpoint(ip::address::from_string(match[2].str()), port);
            tsock = new ip::tcp::socket(io);
        } else if (match[1].str().compare("u") == 0) {
            st = net::socktype::UDP;
            uend = ip::udp::endpoint(ip::address::from_string(match[2].str()), port);
            usock = new ip::udp::socket(io);
        } else
            throw std::logic_error("not support type");
        req.path = match[4].str();
        msgpipe = new int[2];
        anspipe = new int[2];
        if (pipe(msgpipe) == -1) throw std::logic_error("create msgpipe failed");
        if (pipe(anspipe) == -1) throw std::logic_error("create anspipe failed");
    }
    Bstclient(string url, int* msgpipe, int* anspipe)
    {
#define reg_exp "^bs(t|u|)://(.*):([\\d]{1,5})/(.*)"
        std::regex expression(reg_exp);
        std::smatch match;
        if (!std::regex_search(url, match, expression)) throw std::logic_error("bs url is not vaild");
        int port = atoi(match[3].str().c_str());
        if (match[1].str().compare("t") == 0) {
            st = net::socktype::TCP;
            std::cout << "regesiter with tcp" << std::endl;
            tend = ip::tcp::endpoint(ip::address::from_string(match[2].str()), port);
            tsock = new ip::tcp::socket(io);
        } else if (match[1].str().compare("u") == 0) {
            st = net::socktype::UDP;
            std::cout << "register with udp" << std::endl;
            uend = ip::udp::endpoint(ip::address::from_string(match[2].str()), port);
            usock = new ip::udp::socket(io);
        } else
            throw std::logic_error("not support type");
        req.path = match[4].str();
        this->msgpipe = msgpipe;
        this->anspipe = anspipe;
    }
    int msgRPipe()
    {
        return msgpipe[0];
    }
    int msgWPipe()
    {
        return msgpipe[1];
    }
    int ansRPipe()
    {
        return anspipe[0];
    }
    int ansWPipe()
    {
        return anspipe[1];
    }
    void connect()
    {
        if (st == net::socktype::TCP) {
            tsock->connect(tend);
        } else if (st == net::socktype::UDP) {
            usock->connect(uend);
        } else
            throw std::out_of_range("type out of support range");
    }

    bool isclose() override
    {
        return close;
    }
    bool needwaitreturn() override
    {
        return needwait;
    }
    void getback() override
    {
    }
    void write_some(std::string* data, boost::system::error_code* ec) override
    {
        // second encode here
        req.content = *data;
        string ms = marshal(&req);
        printf("write to server:[%s]\n", ms.c_str());
        if (st == net::socktype::TCP) {
            this->tsock->write_some(boost::asio::buffer(ms), *ec);
        } else if (st == net::socktype::UDP) {
            this->usock->send(boost::asio::buffer(*data));
        }
    }
    void read_some(char* outbuffer, boost::system::error_code* ec) override
    {
        if (st == net::socktype::TCP) {
            this->tsock->read_some(buffer(outbuffer, this->buffer_size), *ec);
        } else if (st == net::socktype::UDP) {
            this->usock->receive(buffer(outbuffer, this->buffer_size));
        }
    }
};

}

void dialtcp(ctools::Bsclient* cl, int, int);
void dialtcp(ctools::Bsclient* cl, int msgrpipe, int answpipe)
{
    char cachebuffer[1 << 10];
    boost::system::error_code ec;
    char outbuffer[1 << 10];
    // debug point
    string tosend;
    // cl->connect();
    string accp;
    while (!cl->isclose()) {
        ec.clear();
        if (strlen(outbuffer) > 0) memset(outbuffer, 0, sizeof(char) * strlen(outbuffer));
        if (strlen(cachebuffer) > 0) memset(cachebuffer, 0, strlen(cachebuffer) * sizeof(char));
        if (read(msgrpipe, cachebuffer, 1 << 10) == -1) {

        } else {
            tosend = cachebuffer;
            // debug point
            printf("accept from front %s\n", tosend.c_str());
            cl->sock->write_some(buffer(tosend), ec);
            if (ec) {
                std::cout << ec.what() << std::endl;
            } else {
                // debug point
                if (cl->needwaitreturn()) {
                    // cl->sock->read_some(buffer(outbuffer),ec);
                    printf("wait msg\n");
                    cl->sock->read_some(buffer(outbuffer), ec);
                    if (ec) {
                        std::cout << "error:" << ec.what() << std::endl;
                    } else {
                        // debug point
                        printf("write to front pipe,msg %s\n", outbuffer);
                        // write to pipe
                        if (write(answpipe, outbuffer, sizeof(char) * strlen(outbuffer)) == -1) {
                            printf("write to front failed\n");
                        }
                    }
                }
            }
        }
    }
}

void dial(ctools::Client* cl, int, int);
void dial(ctools::Client* cl, int msgrpipe, int answpipe)
{
    char cachebuffer[1 << 10];
    boost::system::error_code ec;
    char outbuffer[1 << 10];
    // debug point
    string tosend;
    // cl->connect();
    string accp;
    while (!cl->isclose()) {
        ec.clear();
        if (strlen(outbuffer) > 0) memset(outbuffer, 0, sizeof(char) * strlen(outbuffer));
        if (strlen(cachebuffer) > 0) memset(cachebuffer, 0, strlen(cachebuffer) * sizeof(char));
        if (read(msgrpipe, cachebuffer, 1 << 10) == -1) {

        } else {
            tosend = cachebuffer;
            // debug point
            printf("accept from front %s\n", tosend.c_str());
            // cl->sock->write_some(buffer(tosend),ec);
            cl->write_some(&tosend, &ec);
            if (ec) {
                std::cout << ec.what() << std::endl;
            } else {
                // debug point
                if (cl->needwaitreturn()) {
                    // cl->sock->read_some(buffer(outbuffer),ec);
                    printf("wait msg\n");
                    // cl->sock->read_some(buffer(outbuffer),ec);
                    cl->read_some(outbuffer, &ec);
                    printf("get msg %s\n", outbuffer);
                    if (ec) {
                        std::cout << "error:" << ec.what() << std::endl;
                    } else {
                        // debug point
                        printf("write to front pipe,msg %s\n", outbuffer);
                        // write to pipe
                        if (write(answpipe, outbuffer, sizeof(char) * strlen(outbuffer)) == -1) {
                            printf("write to front failed\n");
                        }
                    }
                }
            }
        }
    }
}
