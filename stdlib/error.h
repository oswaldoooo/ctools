#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <stddef.h>
struct error {
    char* core;
    size_t len;
};
void errorf(struct error* src, const char* words);
void join_error(struct error* src, struct error** target_error_list, size_t length);
void free_error(struct error* src);
void free_error_list(struct error** errlist, size_t length);
_Bool error_is_null(struct error* src);
#ifdef __cplusplus
}
#endif