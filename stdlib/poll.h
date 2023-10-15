#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
#include <sys/poll.h>
int poll_create(uint32_t max);
int poll_add(int pollid, int fd, short events);
int poll_del(int pollid, int fd);
int poll_wait(int, struct pollfd*, size_t, unsigned);
int poll_free(int pollid);
#ifdef __cplusplus
}
#endif