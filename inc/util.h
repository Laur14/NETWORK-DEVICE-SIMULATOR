#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h> 
#define srcip 0
#define dstip 4
#define srcport 8
#define dstport 10
#define dprotocol 12
#define dlength 13
#define dPayload 15
#define udp_sim 2
char* protocol_costum(int a);

typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
    uint16_t length;
    uint8_t payload[256];
} packet_t;

void packet_deserialize(const uint8_t* buf,ssize_t lenght, packet_t* pk);

size_t packet_serialized(packet_t pk, uint8_t* buff);

#endif 