#include "inet.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
int listentcp(char* addr, int port)
{
    struct sockaddr_in sc;
    sc.sin_family = AF_INET;
    sc.sin_port = htons(port);
    sc.sin_addr.s_addr = (addr == NULL) ? INADDR_ANY : inet_addr(addr);
    int sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid < 0) return -1;
    if (bind(sid, (struct sockaddr*)&sc, sizeof(sc)) < 0) return -2;
    if (listen(sid, 0) < 0) return -3;
    return sid;
}

int listenudp(char* addr, int port)
{
    struct sockaddr_in sc;
    sc.sin_family = AF_INET;
    sc.sin_port = htons(port);
    sc.sin_addr.s_addr = (addr == NULL) ? INADDR_ANY : inet_addr(addr);
    int sid = socket(AF_INET, SOCK_DGRAM, 0);
    if (sid < 0) return -1;
    if (bind(sid, (struct sockaddr*)&sc, sizeof(sc)) < 0) return -2;
    return sid;
}
int listenunix(char* addr)
{
    struct sockaddr_un su;
    su.sun_family = AF_UNIX;
    memset(su.sun_path, 0, 104);
    memmove(su.sun_path, addr, strlen(addr));
    int sid = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sid < 0) return -1;
    if (bind(sid, (struct sockaddr*)&su, sizeof(su)) < 0) return -2;
    if (listen(sid, 0) < 0) return -3;
    return sid;
}