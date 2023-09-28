#include "io.h"
#include <stdlib.h>
#include <string.h>
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

bool verify(const char* src, unsigned short types_info)
{
    switch (types_info) {
    case INTEGER: {
        int tians;
        return Atoi(src, &tians);
    } break;
    case UINTEGER: {
        int tians;
        return (Atoi(src, &tians) && tians >= 0);
    } break;
    case BOOLEAN: {
        return (!strncmp(src, "true", 4) || !strncmp(src, "false", 5) || (strlen(src) == 1) && (src[0] == '1' || src[0] == '0'));
    } break;
    case IPADDR: {
        unsigned short pos = 0;
        unsigned short tival = 0;
        unsigned short total = 0;
        if (strlen(src) < 7 || strlen(src) > 15) return false;
        for (size_t i = 0; i < strlen(src); i++) {
            if (src[i] == '.') {
                pos = 0;
                tival = 0;
                total++;
            } else if (src[i] >= '0' && src[i] <= '9' && pos < 3) {
                pos++;
                tival *= 10;
                tival += src[i] - '0';
                if (tival >= 256) return false;
            } else {
                return false;
            }
        }
        return total == 3;
    } break;
    default:
        return false;
    }
    return true;
}
bool Atoi(const char* src, int* dst)
{
    bool neg = false;
    if (strlen(src) < 1) return 0;
    if (src[0] == '+' && strlen(src) > 1) {
        src++;
    } else if (src[0] == '-' && strlen(src) > 1) {
        src++;
        neg = true;
    }
    *dst = 0;
    for (size_t i = 0; i < strlen(src); i++) {
        if (src[i] >= '0' && src[i] <= '9') {
            (*dst) *= 10;
            (*dst) += src[i] - '0';
        } else {
            return false;
        }
    }
    if (neg) *dst = -(*dst);
    return true;
}
