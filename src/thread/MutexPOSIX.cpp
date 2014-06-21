#include <stdafx.h>
#include "Mutex.h"
using namespace Easy2D;

Mutex::Mutex()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex,&attr);
}
Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutex);
}
bool Mutex::lock(bool block)
{
    if(block)
        return pthread_mutex_lock(&mutex) == 0;
    else
        return pthread_mutex_trylock(&mutex) == 0;
}
void Mutex::unlock()
{
    pthread_mutex_unlock(&mutex);
}
