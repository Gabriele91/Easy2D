#ifndef STREAMSOUNDAL_H
#define STREAMSOUNDAL_H

#include <Config.h>
#include <BaseSoundAL.h>
#include <StreamThreadAL.h>

namespace Easy2D
{

#define AL_STREAM_N_BUFFERS 2
#define AL_STREAM_BUFFER_SIZE  (size_t)(4410*32*2)

class StreamSoundAL : public BaseSoundAL,public StreamTask
{

protected:

    //stream thread
    StreamThreadAL* streamThread;
    //buffers
    ALuint buffers[AL_STREAM_N_BUFFERS];
    //function pointer
    DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream;
    DFUNCTION<void(void)> restartStream;
    size_t sizeData;
    size_t sempleRate;
    size_t sizeSemple;
    size_t leftRead;
    size_t lastRead;
    bool looping;


public:
    ///constructor
    StreamSoundAL(StreamThreadAL* streamThread,
                  ALuint source,
                  DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream,
                  DFUNCTION<void(void)> restartStream,
                  size_t size,
                  size_t sempleRate,
                  size_t sizeSemple,
                  int   format,
                  float lenSound,
                  float audioVolume);
    ///destroy sound rerouce
    virtual ~StreamSoundAL();
    //overload play
    virtual void play();

    //overload loop
    virtual void loop();
    ///enable loop
    virtual void enableLoop();
    ///disable loop
    virtual void disableLoop();
    ///playback Time
    virtual float playbackTime();
    ///global time duration
    virtual float duration();
    ///is in looping mode!?
    virtual bool isLoop();

    //restat steam
    void restart();
    //stream buffer
    size_t read(ALuint alBuffer);
    ///update
    void update();
};

};

#endif