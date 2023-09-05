#include "array.h"
#include <stdlib.h>
#include <string.h>
void newarray(struct array* src, size_t sigsize)
{
    src->capsize = DEFALUT_SIZE;
    src->curr_size = 0;
    src->head = malloc(sigsize * DEFALUT_SIZE);
    src->sigsize = sigsize;
}
void push_back(struct array* src, void* ele)
{
    if (src->curr_size >= src->capsize) {
        char* newarr = malloc(2 * src->capsize);
        memset(newarr, 0, 2 * src->capsize);
        memmove(newarr, src->head, src->curr_size * src->sigsize);
        free(src->head);
        src->head = newarr;
        src->capsize = 2 * src->capsize;
    }
    memmove((char*)src->head + (src->curr_size * src->sigsize), ele, src->sigsize);
    src->curr_size++;
}
void* get_ele(struct array* src, size_t pos)
{
    if (pos >= src->curr_size) return NULL;
    return src->head + pos * src->sigsize;
}
void pop(struct array* src)
{
    if (src->curr_size > 0) {
        src->curr_size--;
    }
}
void freearray(struct array* src)
{
    free(src->head);
}