#include "error.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// set value to error
void errorf(struct error* src, const char* words)
{
    unsigned long length = strlen(words);
    src->core = malloc(length);
    memmove(src->core, words, length);
}
// collect errors to one error
void join_error(struct error* src, struct error** target_error_list, size_t length)
{
    size_t total_length = length;
    size_t length_record[length];
    if (length > 0) {
        for (size_t i = 0; i < length; i++) {
            length_record[i] = strlen(target_error_list[i]->core);
            total_length += length_record[i];
        }
        src->core = malloc(total_length);
        size_t copy_length = 0;
        for (size_t i = 0; i < length; i++) {
            memmove(src->core + copy_length + i, target_error_list[i]->core, length_record[i]);
            copy_length += length_record[i];
            src->core[copy_length + i] = '\n';
        }
    }
}
void free_error(struct error* src)
{
    if (src->core != NULL) free(src->core);
}
void free_error_list(struct error** errlist, size_t length)
{
    if (length > 0) {
        for (size_t i = 0; i < length; i++) {
            if (errlist[i] != NULL) {
                free_error(errlist[i]);
                free(errlist[i]);
            }
        }
    }
}
_Bool error_is_null(struct error* src)
{
    return (src->core == NULL || strlen(src->core) < 1);
}