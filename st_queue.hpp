#pragma once

#include <queue>
#include <mutex>
#include <pthread.h>


typedef struct ST_Queue_
{

    std::queue<void *> * p_queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    void enqueue(void *value);
    bool empty();
    void * dequeue();

} ST_Queue, *PST_Queue;
