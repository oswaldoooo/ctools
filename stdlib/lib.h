#pragma once
#include <stddef.h>
#ifndef OS
#define OS 1
#endif
#define PRE_MEM_DIR ".mem_control"
void* cmalloc(unsigned long, unsigned*);
void cfree(void*, size_t, unsigned);
static unsigned err_code;
enum error_leve {
    CREATE_FD_FAILED,
    TRUNCATE_FAILED,
    MM_FAILED,
    LINK_FAILED
};
#if OS == 1
// need in bsd or darwin, don't support for linux
void release_unused_shm();
#endif