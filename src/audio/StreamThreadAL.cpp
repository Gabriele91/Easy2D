#include <stdafx.h>
#include <StreamThreadAL.h>

///////////////////////
using namespace Easy2D;
///////////////////////

/**
 * Streaming sound thread implementation
 */

void StreamThreadAL::push(StreamTask *ssound)
{
    mutex.lock();
    soundmap[ssound]=ssound;
    mutex.unlock();
}

void StreamThreadAL::erase(StreamTask *ssound)
{
    mutex.lock();
    auto it=soundmap.find(ssound);
    if(it!=soundmap.end())
        soundmap.erase(it);
    mutex.unlock();
}

int StreamThreadAL::run()
{
    while(isrunning)
    {
        mutex.lock();
        for (auto sound:soundmap)
            sound.second->update();
        mutex.unlock();
        sleepThread(SOUND_THREAD_SLEEP);
    }
    return 0;
}

StreamThreadAL::~StreamThreadAL()
{
    isrunning=false;
};

