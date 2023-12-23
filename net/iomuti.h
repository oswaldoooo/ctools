#pragma once
#include <stddef.h>
#define IOIN 1
#define IOUT 2
#define IOERR 4
struct io_event {
    int fd;
    int rflag;
};
int iomutil_create(unsigned int maxsize);
int iomutil_add(int ioid, int fd, int flags);
int iomutil_del(int ioid, int fd);
int iomuti_wait(int ioid, struct io_event* queue, size_t maxsize, int timeout);