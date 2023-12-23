#include "iomuti.h"
#include <sys/epoll.h>

int iomutil_create(unsigned int maxsize)
{
    return epoll_create(maxsize);
}
int iomutil_add(int ioid, int fd, int flags)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    if (flags & IOIN) {
        ev.events |= EPOLLIN;
    }
    if (flags & IOUT) {
        ev.events |= EPOLLOUT;
    }
    if (flags & IOERR) {
        ev.events |= EPOLLERR;
    }

    return epoll_ctl(ioid, EPOLL_CTL_ADD, fd, &ev);
}
int iomutil_del(int ioid, int fd)
{
    return epoll_ctl(ioid, EPOLL_CTL_DEL, fd, NULL);
}
int iomuti_wait(int ioid, struct io_event* queue, size_t maxsize, int timeout)
{
    struct epoll_event epoll_queue[maxsize];
    int size = epoll_wait(ioid, epoll_queue, maxsize, timeout);
    if (size > 0) {
        for (size_t i = 0; i < size; i++) {
            queue[i].fd = epoll_queue[i].data.fd;
            if (epoll_queue[i].events & EPOLLIN) {
                queue[i].rflag |= IOIN;
            }
            if (epoll_queue[i].events & EPOLLOUT) {
                queue[i].rflag |= IOUT;
            }
            if (epoll_queue[i].events & EPOLLERR) {
                queue[i].rflag |= IOERR;
            }
        }
    }
    return size;
}