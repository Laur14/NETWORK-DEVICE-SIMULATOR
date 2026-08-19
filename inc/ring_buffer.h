#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <string.h>
#include <stdio.h>
#include "util.h"
#include <pthread.h>

#define RING_BUFF_SIZE 64

typedef struct {
    packet_t buffer[RING_BUFF_SIZE];
    size_t head;
    size_t tail;
    size_t count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} ring_buffer_t;

typedef struct{
    ring_buffer_t* in;
    ring_buffer_t* out;
}worker_args_t;

void ring_buffer_init(ring_buffer_t *rb);
void push_ring_buf(ring_buffer_t *rb,packet_t *pk);
void pop_ring_buf(ring_buffer_t* rb,packet_t* pk);
#endif