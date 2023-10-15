#include "io.h"
#include "lib.h"
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define LINUX 0
#define BSD 1
static _Bool check = 0;
void* cmalloc(unsigned long size, unsigned* seed)
{
    const char* add = formatInt(*seed);
    int shmid;
    if (OS == LINUX) {
        shmid = shm_open(add, O_CREAT | O_RDWR, 0644);
        printf("do linux shm open\n");
    } else if (OS == BSD) {
        printf("do bsd shell open\n");
        if (!check) {
            struct stat st;
            if (stat(PRE_MEM_DIR, &st) == -1) {
                mkdir(PRE_MEM_DIR, 0711);
            }
            check = 1;
        }
        char filename[20];
        memset(filename, 0, 20);
        memmove(filename, PRE_MEM_DIR, strlen(PRE_MEM_DIR));
        filename[strlen(filename)] = '/';
        const char* seedstr = formatInt(*seed);
        strncat(filename, seedstr, strlen(seedstr));
        shmid = open(filename, O_CREAT | O_RDWR, 0600);
        if (shmid > 0) *seed = shmid;
    } else {
        fprintf(stderr, "don't support operating system %d\n", OS);
        exit(1);
    }
    if (shmid < 0) {
        err_code = CREATE_FD_FAILED;
        return NULL;
    }
    if (ftruncate(shmid, size) < 0) {
        err_code = TRUNCATE_FAILED;
        return NULL;
    }
    void* ans = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, shmid, 0);
    if (ans == MAP_FAILED) {
        shm_unlink(add);
        err_code = MM_FAILED;
        return NULL;
    }
    return ans;
}

void cfree(void* ptr, size_t size, unsigned seed)
{
    munmap(ptr, size);
    if (OS == LINUX) {
        shm_unlink(formatInt(seed));
    } else if (OS == BSD) {
        close(seed);
    }
}
#if OS == 1
void release_unused_shm()
{
    char command[50];
    memset(command, 0, 50);
    snprintf(command, 50, "rm -rf %s/*", PRE_MEM_DIR);
    system(command);
}
#endif