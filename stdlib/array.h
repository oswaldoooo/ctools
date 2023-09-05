#pragma once
#include <stddef.h>
#define DEFALUT_SIZE 10
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
struct array {
    size_t capsize;
    void* head;
    size_t curr_size;
    size_t sigsize;
};
void newarray(struct array*, size_t);
void push_back(struct array*, void*);
void* get_ele(struct array*, size_t);
void pop(struct array*);
void freearray(struct array*);
#ifdef __cplusplus
}
#endif