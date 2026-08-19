#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h>       // close()
#include "util.h"

int main(){
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
        perror("socket fail in generator");
    
    packet_t pk;
    pk.dst_ip = inet_addr("127.0.0.1");
    pk.dst_port = 5000;
    pk.src_ip = inet_addr("10.42.0.1");
    pk.src_port = 5000;
    pk.protocol = udp_sim;
    for(int i=0;i<10;i++)
        pk.payload[i] = i;
    pk.length = 10;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr =inet_addr("127.0.0.1");
    addr.sin_port = htons(5000);

    uint8_t buff[512];
    int flag=0;
   
    if(sendto(fd,buff,packet_serialized(pk,buff),flag,(struct sockaddr*)&addr,(socklen_t)sizeof(addr)) == -1)
        perror("eroare la send");
    else
        printf("send %u  to %s",pk.length,inet_ntoa(addr.sin_addr));

    close(fd);

    return 0;
}