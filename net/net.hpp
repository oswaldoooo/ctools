#pragma once
#include "boost/asio/ip/tcp.hpp"
#include <boost/asio.hpp>
#include <boost/asio/basic_datagram_socket.hpp>
#include <cstdio>
#include <cstring>
#include <ctools/error/error.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
namespace ctools {
struct pipe {
    enum class pipe_code { read = 1000,
        write = 1001,
        both = 2001 };

private:
    int* core_pipe;
    size_t buffer_size = 1 << 10;
    char* out_buffer;

public:
    pipe()
    {
        core_pipe = new int[2];
        out_buffer = new char[buffer_size];
        if (::pipe(core_pipe) == -1) throw std::logic_error("create pipe failed");
    }
    pipe(struct pipe&& pl)
    {
        std::memmove(this->core_pipe, pl.core_pipe, 2 * sizeof(int));
        std::memmove(this->out_buffer, pl.out_buffer, 2 * sizeof(int));
        this->buffer_size = pl.buffer_size;
    }

    bool write(std::string data)
    {
        return (::write(core_pipe[1], data.c_str(), buffer_size) != -1);
    }
    const char* read()
    {
        if (::read(core_pipe[0], out_buffer, buffer_size) == -1) {
            return nullptr;
        }
        return out_buffer;
    }
    std::string read_tostr()
    {
        if (::read(core_pipe[0], out_buffer, buffer_size) == -1) {
            return NULL;
        }
        return out_buffer;
    }
    bool close(pipe_code pc = pipe_code::both)
    {
        if (pc == pipe_code::read) {
            return (::close(core_pipe[0]) != -1);
        } else if (pc == pipe_code::write) {
            return (::close(core_pipe[1]) != -1);
        } else if (pc == pipe_code::both) {
            return (::close(core_pipe[0]) != -1 && ::close(core_pipe[1]) != -1);
        } else {
            throw std::logic_error("close choose error");
        }
    }
    ~pipe()
    {
        delete[] core_pipe;
        delete[] out_buffer;
    }
};

pipe& operator>>(pipe& origin, std::string& target)
{
    // printf("start read from pipe\n");
    target = origin.read_tostr();
    return origin;
}
pipe& operator<<(pipe& origin, std::string data)
{
    // printf("start write to pipe\n");
    origin.write(data);
    return origin;
}

template <size_t N>
pipe& operator<<(pipe& origin, const char data[N])
{
    origin.write(data);
    return origin;
}

// the parent class all conn
class conn {
public:
    virtual error decode() = 0;
    virtual bool needsave() = 0;
    virtual void save(std::string) = 0;
    virtual error Do() = 0;
    virtual bool isclose() = 0;
    virtual error response() = 0;
    virtual error bad_response() = 0;
};
}

namespace ctools {
namespace net {
// static size_t buffer_size = 10 << 10;
#define buffer_size 10 << 10
    void DoIt(boost::asio::ip::tcp::socket* soc, bool* isclose, bool needwait)
    {
        char inbuffer[buffer_size];
        boost::system::error_code ec;
        while (!(*isclose)) {
            ec.clear();
            if (strlen(inbuffer) > 0) memset(inbuffer, 0, sizeof(char) * strlen(inbuffer));
            soc->read_some(boost::asio::buffer(inbuffer), ec);
        }
    }
    enum class socktype { TCP = 999,
        UDP = 888,
        UNIX = 777 };
    void listenandserve_tcp(socktype sc, std::string address, conn (*regfunc)(boost::asio::ip::tcp::socket), pipe* errpipe)
    {
        // todo compelete common listen and serve
    }
    void listenandserve_udp(socktype sc, std::string address, conn (*regfunc)(boost::asio::ip::udp::socket), pipe* errpipe)
    {
        // todo compelete common listen and serve
    }
}
}