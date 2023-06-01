#include <iostream>
#include "st_queue.hpp"
using namespace std;

void ST_Queue::enqueue(void *value)
{    
    pthread_mutex_lock(&mutex);
   
    p_queue->push(value);
   
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

bool ST_Queue::empty()
{
    pthread_mutex_lock(&mutex);

    bool res = p_queue->empty();

    pthread_mutex_unlock(&mutex);
    return res;
}

void *ST_Queue::dequeue()
{
    pthread_mutex_lock(&mutex);
    while (p_queue->empty())
    {
        pthread_cond_wait(&cond,&mutex);
    }
    void *task = p_queue->front();
    p_queue->pop();
    pthread_mutex_unlock(&mutex);
    return task;
}