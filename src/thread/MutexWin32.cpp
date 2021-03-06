#include <stdafx.h>
#include "Mutex.h"
using namespace Easy2D;

Mutex::Mutex()
{
    mutex= CreateMutex(NULL, FALSE, NULL);
}
Mutex::~Mutex()
{
    if(mutex)
        CloseHandle(mutex);
}
bool Mutex::lock(bool block)
{
    if(mutex)
        return ((bool)WaitForSingleObject(mutex, block ? INFINITE : 0) == WAIT_OBJECT_0);
    return false;
}
void Mutex::unlock()
{
    if(mutex)
        ReleaseMutex(mutex);
}
