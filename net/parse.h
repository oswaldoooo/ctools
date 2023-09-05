#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <sys/types.h>
struct packet_header {
    uint16_t src_port;
    uint16_t dst_port;
    u_char src_host[4];
    u_char dst_host[4];
    uint32_t len;
};
void parse_packet(const u_char*, struct packet_header*);
void parse_netadd(char*, u_char[4]);
#ifdef __cplusplus
}
#endif