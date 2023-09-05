#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#define log_fprint(x, y) print(x, y, __FILE__, __LINE__, __func__)
#define log_print(y) log_fprint(stdout, y)
#include <stdio.h>
#include <stdlib.h>
int print(FILE*, const char*, const char*, const int, const char*);
static char global_mem[512];
const char* format_byte_tostr(uint64_t);
const char* formatInt(int);
const char* itoa(int);
#ifdef __cplusplus
}
#endif