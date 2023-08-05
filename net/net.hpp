#pragma once
#include "netkits.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctools/error/error.hpp>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>
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

inline pipe& operator>>(pipe& origin, std::string& target)
{
    // printf("start read from pipe\n");
    target = origin.read_tostr();
    return origin;
}
inline pipe& operator<<(pipe& origin, std::string data)
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
namespace context {
    static int top_id = 0;
    inline std::vector<ctools::Server> task_array;
}
}
namespace ctools {
namespace net {

// static size_t buffer_size = 10 << 10;
#define buffer_size 10 << 10
    enum class socktype { TCP = 999,
        UDP = 888,
        UNIX = 777 };
    void listen_and_serve(const socktype sc, int* task_id, const int port = 8000, const char* addr = "");
    void ProCon(const int conid, const int task_id);
    inline void ProCon(const int conid, const int task_id)
    {
        Server& ser = context::task_array[task_id];
        ser.register_con(conid);
        ser.Do();
        ser.safety_exit();
    }
    inline void listen_and_serve(const socktype sc, int* task_id, const int port, const char* addr)
    {
        *task_id = context::top_id++;
        int sock_type = (sc == socktype::UDP) ? SOCK_DGRAM : SOCK_STREAM;
        int sock_type_ = (sc == socktype::UNIX) ? AF_UNIX : AF_INET;
        struct sockaddr* sa;
        size_t sizelen;
        if (sc != socktype::UNIX) {
            struct sockaddr_in sce;
            sce.sin_family = AF_INET;
            sce.sin_addr.s_addr = INADDR_ANY;
            sce.sin_port = htons(port);
            sa = (struct sockaddr*)&sce;
            sizelen = sizeof(struct sockaddr_in);
        } else {
            struct sockaddr_un su;
            su.sun_family = AF_UNIX;
            if (strlen(addr) < 1) throw std::logic_error("not set address");
            sa = (struct sockaddr*)&su;
            sizelen = sizeof(struct sockaddr_un);
        }
        int sid = socket(sock_type_, sock_type, 0);
        if (bind(sid, sa, sizelen) == -1) throw std::logic_error("bind failed");
        if (listen(sid, 0) == -1) throw std::logic_error("listen failed");
        int conid;
        while (true) {
            conid = accept(conid, NULL, NULL);
            if (*task_id >= context::task_array.size()) throw std::logic_error("not bind the server class");
            context::task_array[*task_id].Do();
        }
    }
}
}
