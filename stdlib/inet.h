#pragma once
#ifdef __cplusplus
extern "C" {
#endif
int listentcp(char* addr, int port);
int listenudp(char* addr, int port);
int listenunix(char* addr);
#ifdef __cplusplus
}
#endif