#include <stdio.h>
#include <string.h>
#include <sys/socket.h>   // socket(), bind(), recvfrom(), sendto()
#include <netinet/in.h>   // struct sockaddr_in, htons()
#include <arpa/inet.h>    // inet_addr() / inet_pton()
#include <unistd.h>       // close()
#include <pthread.h>

uint64_t cnt = 0;
pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;
void* couter_thread(void* arg)
{
    for(uint64_t i = 0;i<100000;i++)
    {
        pthread_mutex_lock(&mt);
        cnt++;
        pthread_mutex_unlock(&mt);
    }
    return NULL;
}
int main()
{   
    pthread_t fc1,fc2;
    int aux;
    pthread_create(&fc1,NULL,couter_thread,&aux);
    pthread_create(&fc2,NULL,couter_thread,&aux);
    pthread_join(fc1,NULL);
    
    pthread_join(fc2,NULL);
    printf("i: %lu\n",cnt);
    return 0;   
}