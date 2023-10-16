#ifndef net_h
#define net_h
#define TCP 0x01
#define UDP 0x02
#define UNIX 0x03
#include <pthread.h>
#include <stddef.h>
struct chan {
    int core[2];
    pthread_mutex_t mutex;
    size_t size;
};
int newchan(struct chan* src, size_t size);
int closechan(struct chan* src);
int sendchan(struct chan* src, const void* v);
int revchan(struct chan* src, void* dst);
void transform(const char*, int[4]);

#endif
