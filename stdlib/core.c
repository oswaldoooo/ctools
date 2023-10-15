#include "io.h"
#include "math.h"
#include "poll.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
int print(FILE* fl, const char* src, const char* filename, const int line, const char* funname)
{
    if (src == NULL || strlen(src) == 0) return -1;
    time_t ti = time(NULL);
    char* out = ctime(&ti);
    out[strlen(out) - 1] = 0;
    return (src[strlen(src) - 1] != '\n') ? fprintf(fl, "%s %s:%s:%d %s\n", out, filename, funname, line, src) : fprintf(fl, "%s %s:%s:%d %s", out, filename, funname, line, src);
}
const char* format_byte_tostr(uint64_t data)
{
    const char* levelname[5] = { "B", "KB", "MB", "GB", "TB" };
    memset(global_mem, 0, 512);
    if (data < 1024) {
        snprintf(global_mem, 512, "%lld B", data);
        return global_mem;
    }
    uint64_t leave;
    uint8_t level = 0;
    while (data > 0 && level < 5) {
        leave = data % 1024;
        data = data / 1024;
        if (leave > 0) snprintf(global_mem, 512, "%s %lld %s", global_mem, leave, levelname[level]);
        level++;
    }
    if (data > 0) {
        snprintf(global_mem, 512, "%s %lld PB", global_mem, data);
    }
    return global_mem;
}
const char* formatInt(int data)
{
    memset(global_mem, 0, 512);
    char ti[100];
    memset(ti, 0, 100);
    unsigned level = 0;
    short isnegative_number = 0;
    if (data < 0) {
        data = -data;
        isnegative_number = 1;
    }
    while (data > 0) {
        ti[level] = data % 10 + '0';
        data = data / 10;
        level++;
    }
    int mid;
    for (unsigned short i = 0; i < level / 2; i++) {
        mid = ti[i];
        ti[i] = ti[level - i - 1];
        ti[level - i - 1] = mid;
    }
    if (isnegative_number == 1) {
        global_mem[0] = '-';
        memmove(global_mem + 1, ti, level);
    } else {
        memmove(global_mem, ti, level);
    }
    return global_mem;
}


// poll enhanced
#define DEFAULT_POLL_SIZE 30
struct poll_pool {
    unsigned curr_top;
    unsigned max_top;
    struct pollfd* src;
};
static struct poll_pool polls_pool[10];
static unsigned polls_top = 0;
int poll_create(uint32_t max)
{
    max = (max == 0) ? DEFAULT_POLL_SIZE : max;
    polls_pool[polls_top].curr_top = 0;
    polls_pool[polls_top].max_top = max;
    polls_pool[polls_top].src = malloc(sizeof(struct pollfd) * max);
    return polls_top++;
}
int poll_add(int pollid, int fd, short events)
{
    if (pollid >= polls_top) return -1;
    struct poll_pool* pl;
    pl = &polls_pool[pollid];
    if (pl->curr_top >= pl->max_top) return -2;
    pl->src[pl->curr_top].fd = fd;
    pl->src[pl->curr_top].events = events;
    pl->curr_top++;
    return 0;
}
int poll_del(int pollid, int fd)
{
    if (pollid >= polls_top) return -1;
    struct poll_pool* pl;
    pl = &polls_pool[pollid];
    if (pl->curr_top == 0) return -2;
    for (size_t i = 0; i < pl->curr_top; i++) {
        if (pl->src[i].fd == fd) {
            memmove(pl->src + i * sizeof(struct pollfd), pl->src + (i + 1) * sizeof(struct pollfd), (pl->curr_top - i - 1) * sizeof(struct pollfd));
            break;
        }
    }
    pl->curr_top--;
    return 0;
}
int poll_wait(int pollid, struct pollfd* dst, size_t max, unsigned timeout)
{
    if (pollid >= polls_top) return -1;
    struct poll_pool* pl;
    pl = &polls_pool[pollid];
    int to = (timeout == 0) ? -1 : timeout;
    poll(pl->src, pl->curr_top, to);
    unsigned start = 0;
    for (size_t i = 0; i < pl->curr_top; i++) {
        if (pl->src[i].fd > 0 && start < max && (pl->src[i].events & pl->src[i].revents)) {
            dst[start++] = pl->src[i];
        }
    }
    return start;
}
int poll_free(int pollid)
{
    if (pollid >= polls_top) return -1;
    struct poll_pool* pl;
    pl = &polls_pool[pollid];
    if (pollid < polls_top - 1) {
        memmove(polls_pool + pollid * sizeof(struct poll_pool), polls_pool + (pollid + 1) * sizeof(struct poll_pool), (polls_top - pollid - 1) * sizeof(struct poll_pool));
    }
    free(pl->src);
    polls_top--;
    return 0;
}

long Pow(long src, uint8_t step)
{
    long ans = 1;
    if (src == 0) return 0;
    if (step == 0) return ans;
    for (size_t i = 0; i < step; i++) {
        ans *= src;
    }
    return ans;
}
u_int64_t Uint64(u_char* src, size_t length)
{
    length = (length > 8) ? 8 : length;
    length = (strlen((char*)src) < length) ? strlen((char*)src) : length;
    u_int32_t ans = 0;
    if (length == 0) return ans;
    for (size_t i = 0; i < length; i++) {
        ans += src[i] * Pow(256, length - i - 1);
    }
    return ans;
}
u_int32_t Uint32(u_char* src, size_t length)
{
    length = (length > 4) ? 4 : length;
    length = (strlen((char*)src) < length) ? strlen((char*)src) : length;
    u_int32_t ans = 0;
    if (length == 0) return ans;
    for (size_t i = 0; i < length; i++) {
        ans += src[i] * Pow(256, length - i - 1);
    }
    return ans;
}
u_int16_t Uint16(u_char* src, size_t length)
{
    length = (length > 2) ? 2 : length;
    length = (strlen((char*)src) < length) ? strlen((char*)src) : length;
    u_int32_t ans = 0;
    if (length == 0) return ans;
    for (size_t i = 0; i < length; i++) {
        ans += src[i] * Pow(256, length - i - 1);
    }
    return ans;
}

