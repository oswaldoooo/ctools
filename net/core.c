#include "parse.h"
#include <stdio.h>
#include <string.h>
void parse_packet(const u_char* src, struct packet_header* header)
{
    memmove(header->src_host, src + 26, 4);
    memmove(header->dst_host, src + 30, 4);
    header->src_port = src[34] * 256 + src[35];
    header->dst_port = src[36] * 256 + src[37];
}
void parse_netadd(char* dst, u_char* origin)
{
    snprintf(dst, 15, "%d.%d.%d.%d", origin[0], origin[1], origin[2], origin[3]);
}