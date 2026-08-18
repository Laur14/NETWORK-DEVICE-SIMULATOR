#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rg){
    int err;
    rg->count = 0;
    rg->head = 0;
    rg->tail = 0;

    err = pthread_mutex_init(&rg->mutex,NULL);
    if(err != 0)
        fprintf(stderr,"mutex_fail_on_ring%s\n",strerror(err));

    err = pthread_cond_init(&rg->not_empty,NULL);
    if(err != 0)
        fprintf(stderr,"not_empty_cond_fail_on_ring%s\n",strerror(err));

    err = pthread_cond_init(&rg->not_full,NULL);   
    if(err != 0)
        fprintf(stderr,"not_full_fail_on_ring%s\n",strerror(err));
    
}
void push_ring_buf(ring_buffer_t *rb,packet_t *pk){
    int err = pthread_mutex_lock(&rb->mutex);
    if(err != 0)
        fprintf(stderr,"eroare_la_blocare_mutex_ring_buf%s\n",strerror(err));

    while(rb->count == RING_BUFF_SIZE){
        err = pthread_cond_wait(&rb->not_full,&rb->mutex);

        if(err != 0)
            fprintf(stderr,"eroare_la_asteptate_ring_buf%s\n",strerror(err));
    }

    memcpy(&rb->buffer[rb->head],pk,sizeof(packet_t));

    rb->head = (rb->head+1)%RING_BUFF_SIZE;
    rb->count++;

    err = pthread_cond_signal(&rb->not_empty);
    if(err != 0)
        fprintf(stderr,"eroare_la_semnal_cond_ring_buf%s\n",strerror(err));

    err = pthread_mutex_unlock(&rb->mutex);
    if(err != 0)
        fprintf(stderr,"eroare_la_deblocare_mutex_ring_buf%s\n",strerror(err));
}
void pop_ring_buf(ring_buffer_t* rb,packet_t* pk){
    int err = pthread_mutex_lock(&rb->mutex);
    if(err != 0)
        fprintf(stderr,"mutex lock err pb pop %s\n",strerror(err));
        
    while(rb->count == 0)
    {
        err = pthread_cond_wait(&rb->not_empty,&rb->mutex);
        if(err !=0)
            fprintf(stderr,"cond wait rb pop %s\n",strerror(err));
    }

    memcpy(pk,&rb->buffer[rb->tail],sizeof(packet_t));
    rb->count--;
    rb->tail = (rb->tail+1)% RING_BUFF_SIZE;

    err = pthread_cond_signal(&rb->not_full);
    if(err != 0)
        fprintf(stderr,"signal err rb pop %s\n",strerror(err));

    err = pthread_mutex_unlock(&rb->mutex);
    if(err != 0)
        fprintf(stderr,"mutex ulock err rb pop %s\n",strerror(err));
}