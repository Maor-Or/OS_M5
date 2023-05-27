#pragma once
#include "st_queue.hpp"


typedef void*(*func)(void *);

typedef struct ActiveObject_ *PActiveObject;


ActiveObject_* CreateActiveObject(func p_func);
PST_Queue getQueue(PActiveObject p_ao);
void stop(PActiveObject p_ao);