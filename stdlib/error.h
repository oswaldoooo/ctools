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
void init_error(struct error* err);
void init_error_list(struct error** err, size_t length);
size_t err_len(struct error** src, size_t cap_len);
void errorf(struct error* src, const char* words);
void join_error(struct error* src, struct error** target_error_list, size_t length);
void free_error(struct error* src);
void free_error_list(struct error** errlist, size_t length);
_Bool error_is_null(struct error* src);
#ifdef __cplusplus
}
#endif