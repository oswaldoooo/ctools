#include "io.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
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