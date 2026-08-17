#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h>       // close()
#include "util.h"
#include <pthread.h>

void* rx_thread_func(void *arg){
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
            printf("ip: %s , port: %u, marime: %zd\n",inet_ntoa(dest.sin_addr),ntohs(dest.sin_port),n);
            packet_deserialize(buf,n,&pk);
            printf("lungime : %u , protocol: %s\n",pk.length,protocol_costum(pk.protocol));
        }   
    }

}
int counter=0;
void* counter_thread(){
    for(uint64_t i=0;i<10000;i++){
        counter++;
    }
    return NULL;
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

    pthread_t rx_thread;
   
    int err = pthread_create(&rx_thread,NULL,rx_thread_func,&fd);

    if(err != 0)
        fprintf(stderr,"pthread_create_error %s\n",strerror(err));

    pthread_t rx_thread1;
    pthread_t rx_thread2;
    pthread_create(&rx_thread1,NULL,&counter_thread,NULL);  
    pthread_create(&rx_thread2,NULL,&counter_thread,NULL); 
    
    
    pthread_join(rx_thread,NULL);
    pthread_join(rx_thread1,NULL);
    pthread_join(rx_thread2,NULL);
    printf("counter: %d \n",counter);
    

    return 0;
}