#pragma once
enum type_info { INTEGER = 1,
    UINTEGER,
    BOOLEAN,
    IPADDR };
_Bool verify(const char* src, unsigned short type_info);
_Bool Atoi(const char* src, int* dst);
