#pragma once

#include <queue>
#include <mutex>


typedef struct ST_Queue_
{

    std::queue<void *> * p_queue;
    std::mutex mutex;

    void enqueue(void *value);
    bool empty();
    void * dequeue();
    // void wait_and_pop(void *&value);

} ST_Queue, *PST_Queue;
