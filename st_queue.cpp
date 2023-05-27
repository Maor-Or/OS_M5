#include <iostream>
#include "st_queue.hpp"
using namespace std;

void ST_Queue::enqueue(void *value)
{
    std::lock_guard<std::mutex> lock(mutex);
    p_queue->push(value);

    void* ptr = p_queue->front();
    unsigned int* value2 =reinterpret_cast<unsigned int*>(ptr);
    // unsigned int x = static_cast<unsigned int>(p_queue->front());
    // printf("enqued : %u \n",value);
    cout << "value enqued: "<< *(value2) << endl;
}

bool ST_Queue::empty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return p_queue->empty();
}

void *ST_Queue::dequeue()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (p_queue->empty())
    {
        printf("returning nullptr\n");
        return nullptr;
    }
    void *task = p_queue->front();
    printf("returning task %u \n",task);
    p_queue->pop();
    return task;
}