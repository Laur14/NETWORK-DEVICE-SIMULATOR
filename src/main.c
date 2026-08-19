#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h>       // close()
#include "util.h"
#include <pthread.h>
#include "ring_buffer.h"
#include <stdlib.h>

worker_args_t ring[3];
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
            push_ring_buf(ring[0].in,&pk);
        }   
    }

}
void* worker_forward_func(void* arg){
    worker_args_t wr = *(worker_args_t*)arg;
    packet_t pk;
    for(;;){
        pop_ring_buf(wr.in,&pk);
        push_ring_buf(wr.out,&pk);
    }
}
void* worker_firewall_func(void* arg){
    worker_args_t wr = *(worker_args_t*)arg;
    packet_t pk;
    for(;;){
        pop_ring_buf(wr.in,&pk);
        push_ring_buf(wr.out,&pk);
    }
}
void* worker_stats_func(void* arg){
    worker_args_t wr = *(worker_args_t*)arg;
    packet_t pk;
    for(;;){
        pop_ring_buf(wr.in,&pk);
        char ip_str[INET_ADDRSTRLEN];
        struct in_addr addr;
        addr.s_addr = htonl(pk.src_ip);
        inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
        printf("am ajuns la status ci %s\n", ip_str);
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
    ring_buffer_t* rb1 = (ring_buffer_t*)malloc(sizeof(ring_buffer_t));
    ring_buffer_t* rb2 = (ring_buffer_t*)malloc(sizeof(ring_buffer_t));
    ring_buffer_t* rb3 = (ring_buffer_t*)malloc(sizeof(ring_buffer_t));

    ring_buffer_init(rb1);
    ring_buffer_init(rb2);
    ring_buffer_init(rb3);

    ring[0].in = rb1;
    ring[0].out = rb2;
    ring[1].in = rb2;
    ring[1].out = rb3;
    ring[2].in = rb3;
    ring[2].out = NULL;

    pthread_t load_rb;    
    pthread_t forword;
    pthread_t firewall;
    pthread_t status;
    int err = pthread_create(&load_rb,NULL,load_rb_func,&fd);

    if(err != 0)
        fprintf(stderr,"load_rb_error %s\n",strerror(err));

    err = pthread_create(&forword,NULL,worker_forward_func,&ring[0]);
    if(err != 0)
        fprintf(stderr,"worker_forward_func %s\n",strerror(err));

    err = pthread_create(&firewall,NULL,worker_firewall_func,&ring[1]);
    if(err != 0)
        fprintf(stderr,"worker_firewall_func %s\n",strerror(err));

    err = pthread_create(&status,NULL,worker_stats_func,&ring[2]);
    if(err != 0)
        fprintf(stderr,"worker_stats_func %s\n",strerror(err));

    pthread_join(load_rb,NULL);
    pthread_join(forword,NULL);
    pthread_join(firewall,NULL);
    pthread_join(status,NULL);
   
    
    
    
    return 0;
}