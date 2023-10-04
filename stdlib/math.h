#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/types.h>
u_int32_t Uint32(u_char* src, size_t length);
u_int16_t Uint16(u_char* src, size_t length);
u_int64_t Uint64(u_char* src, size_t length);
#ifdef __cplusplus
}
#endif