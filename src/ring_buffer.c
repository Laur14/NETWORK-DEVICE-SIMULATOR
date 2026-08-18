#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rg){
     int err;
    rg->count = 0;
    rg->head = 0;
    rg->tail = 0;

    err = pthread_mutex_init(&rg->mutex,NULL);
    if(err != 0)
        fprintf(stderr,"mutex fail on ring ",strerror(err));

    err = pthread_cond_init(&rg->not_empty,NULL);
    if(err != 0)
        fprintf(stderr,"not empty cond fail on ring",strerror(err));

    err = pthread_cond_init(&rg->not_full,NULL);   
    if(err != 0)
        fprintf(stderr,"not full fail on ring",strerror(err));
    
}