#include "parse.h"
#include "net.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
void parse_packet(const u_char* src, struct packet_header* header)
{
    memmove(header->src_host, src + 26, 4);
    memmove(header->dst_host, src + 30, 4);
    header->src_port = src[34] * 256 + src[35];
    header->dst_port = src[36] * 256 + src[37];
}
void parse_netadd(char* dst, u_char* origin)
{
    snprintf(dst, 15, "%d.%d.%d.%d", origin[0], origin[1], origin[2], origin[3]);
}
int newchan(struct chan* src, size_t size)
{
    if (pipe(src->core) < 0) return -1;
    if (pthread_mutex_init(&src->mutex, NULL) < 0) return -2;
    if (size <= 0)
        src->size = getpagesize();
    else
        src->size = size;
    return 0;
}
int closechan(struct chan* src)
{
    int ok = 0;
    ok += close(src->core[0]) + close(src->core[1]);
    ok += pthread_mutex_destroy(&src->mutex);
    return ok;
}
int sendchan(struct chan* src, const void* v)
{
    pthread_mutex_lock(&src->mutex);
    int ok = write(src->core[1], v, src->size);
    pthread_mutex_unlock(&src->mutex);
    return ok;
}
int revchan(struct chan* src, void* dst)
{
    pthread_mutex_lock(&src->mutex);
    int ok = read(src->core[0], dst, src->size);
    pthread_mutex_unlock(&src->mutex);
    return ok;
}
