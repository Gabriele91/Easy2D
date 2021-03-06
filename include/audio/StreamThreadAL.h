#ifndef STREAMTHREADAL_H
#define STREAMTHREADAL_H

#include <Thread.h>
#include <Mutex.h>

namespace Easy2D
{

class StreamTask
{

public:

    virtual void update()=0;

};

#define SOUND_THREAD_SLEEP 25 //25 ms

/**
 * Streaming sound thread
 */
class StreamThreadAL : public Thread
{

    UnorderedMap<StreamTask*,StreamTask*> soundmap;
    Mutex mutex;
    bool isrunning;

public:
    //init thread
    StreamThreadAL():Thread(Thread::TERMINATE_NONE),isrunning(true) {}
    //add a sound task
    void push(StreamTask *ssound);
    //delete a sound task
    void erase(StreamTask *ssound);
    //thread main
    virtual int run();
    //break loop
    void breakLoop(){ isrunning=false; }
    //at close
    virtual ~StreamThreadAL();

};

};

#endif
