#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h>       // close()
#include "util.h"
#include <pthread.h>
#include "ring_buffer.h"

ring_buffer_t ring;
void* load_rb_func(void *arg){
    int fd = *(int*)arg;
    packet_t pk;
    uint8_t buf[512];
    int flag=0;
    struct sockaddr_in dest;
    socklen_t len = sizeof(dest);

    for(;;){
        ssize_t n = recvfrom(fd, buf, sizeof(buf), flag, (struct sockaddr*)&dest, &len) ;

        if(n==-1){
            perror("recv");
            continue;
        }
        else{
            packet_deserialize(buf,n,&pk);
            push_ring_buf(&ring,&pk);
        }   
    }

}
void* deload_rb_func(void* arg){
    packet_t pk;
    for(;;)
    { 
        pop_ring_buf(&ring,&pk);
        printf("lungime buf %u  , dest ip %u\n",pk.length,pk.dst_ip);
    }
}
int main(){
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr ;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(5000);
    
    if(bind(fd,(struct sockaddr*)&addr,sizeof(addr))==-1)
    {
        perror("bind");
        return 1;
    }

    pthread_t load_rb;    
    pthread_t unload_rb;
    ring_buffer_init(&ring);
   
    int err = pthread_create(&load_rb,NULL,load_rb_func,&fd);

    if(err != 0)
        fprintf(stderr,"load_rb_error %s\n",strerror(err));

    err = pthread_create(&unload_rb,NULL,deload_rb_func,&fd);

    if(err != 0)
        fprintf(stderr,"deload_eb_error %s\n",strerror(err));
    pthread_join(load_rb,NULL);
    pthread_join(unload_rb,NULL);
    
    
    
    return 0;
}