#include <iostream>

#include "st_activeObject.hpp"

using namespace std;

typedef struct ActiveObject_
{
    int isStopThread;
    func ao_func;
    pthread_t ao_thread;
    PST_Queue ao_p_stq;

} ActiveObject, *PActiveObject;

void *threadFunction(void *p_ao);

PActiveObject CreateActiveObject(func p_func)
{
    // allocationg the Active object:
    PActiveObject p_ao = (PActiveObject)calloc(1, sizeof(ActiveObject));

    // allocating the Active object's attributes:
    // inintilizing the func pointer:
    p_ao->ao_func = p_func;

    // queue:
    p_ao->ao_p_stq = (PST_Queue)calloc(1, sizeof(ST_Queue)); // struct
    p_ao->ao_p_stq->p_queue = new queue<void *>();           // nested queue in struct
    pthread_cond_init(&(p_ao->ao_p_stq->cond), NULL);
    pthread_mutex_init(&(p_ao->ao_p_stq->mutex), NULL);

    // thread:
    p_ao->isStopThread = 0;
    int thread_create_result = pthread_create(&(p_ao->ao_thread), NULL, threadFunction, p_ao);
    if (thread_create_result != 0)
    {
        printf("Failed to create thread.\n");
        exit(1);
    }
    return p_ao;
}

PST_Queue getQueue(PActiveObject p_ao)
{
    if (p_ao == nullptr)
    {
        return nullptr;
    }

    return p_ao->ao_p_stq;
}

void stop(PActiveObject p_ao)
{
    struct ActiveObject_ *p_ao2 = (PActiveObject)p_ao;
    p_ao2->isStopThread = 1;
    pthread_join(p_ao2->ao_thread, NULL);
    pthread_cond_destroy(&(p_ao->ao_p_stq->cond));
    pthread_mutex_destroy(&(p_ao->ao_p_stq->mutex));
    delete p_ao2->ao_p_stq->p_queue;
    free(p_ao2->ao_p_stq);
    free(p_ao2);
}

// Function to be executed in the thread
void *threadFunction(void *p_ao)
{
    struct ActiveObject_ *p_ao2 = (PActiveObject)p_ao;
    void *task;

    if (p_ao2->isStopThread == 1)
    {
        return nullptr;
    }

    // going through the tasks in the queue:
    while ((p_ao2->isStopThread) == 0 &&
           (task = p_ao2->ao_p_stq->dequeue()) != nullptr)
    {
        p_ao2->ao_func(task);
    }

    return nullptr;
}