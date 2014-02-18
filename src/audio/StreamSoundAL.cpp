#include <stdafx.h>
#include <Math3D.h>
#include <StreamSoundAL.h>

///////////////////////
using namespace Easy2D;
///////////////////////

///constructor
StreamSoundAL::StreamSoundAL( StreamThreadAL* streamThread,
                              ALuint source,
                              DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream,
                              DFUNCTION<void(void)> restartStream,
                              size_t size,
                              size_t sempleRate,
                              size_t sizeSemple,
                              int   format,
                              float lenSound,
                              float audioVolume):
                                BaseSoundAL(source,format,lenSound,audioVolume),
                                streamThread(streamThread),
                                readStream(readStream),
                                restartStream(restartStream),
                                sizeData(size),
                                sempleRate(sempleRate),
                                sizeSemple(sizeSemple)
{
    /**
     *  create buffers
     */
    alGenBuffers(AL_STREAM_N_BUFFERS, buffers);
    //schedule update
    streamThread->push(this);
    //init
    leftRead=0;
    lastRead=0;
}
///destroy sound rerouce
StreamSoundAL::~StreamSoundAL(){
    if(isPlay()) stop();
    //unschedule
    streamThread->erase(this);
    //delete buffers
    alDeleteBuffers(AL_STREAM_N_BUFFERS,buffers);
};
//overload play
void StreamSoundAL::play() {
    /////////////////////////////////////////////////////////////////////
    //restat stream
    restart();
    /////////////////////////////////////////////////////////////////////
    //stream file/data
    for(size_t i=0;i!=AL_STREAM_N_BUFFERS;++i) read(buffers[i]);
    /////////////////////////////////////////////////////////////////////
    alSourceQueueBuffers(source, AL_STREAM_N_BUFFERS, buffers);
    BaseSoundAL::play();
}

//overload loop
void StreamSoundAL::loop(){
    if(!isPlay()) play();
    if(!isLoop()) looping=true;
}
///enable loop
void StreamSoundAL::enableLoop(){
    looping=true;
}
///disable loop
void StreamSoundAL::disableLoop(){
    looping=false;
}
///playback Time
float StreamSoundAL::playbackTime(){
    int bytesBufferRead;
    alGetSourcei(source, AL_BYTE_OFFSET , &bytesBufferRead);
    float  result=((float)(Math::min(lastRead+bytesBufferRead,sizeData))/sizeSemple);
    return result;
}
///global time duration
float StreamSoundAL::duration(){
    return lenSound;
}
///is in looping mode!?
bool StreamSoundAL::isLoop(){
    return looping ;
}

//restat steam
void StreamSoundAL::restart(){
    restartStream();
    leftRead=0;
    lastRead=0;
}
//stream buffer
size_t StreamSoundAL::read(ALuint alBuffer){
    //buffer
    uchar loadBuffer[AL_STREAM_BUFFER_SIZE];
    //stream file/data
    size_t readSize=readStream(loadBuffer,AL_STREAM_BUFFER_SIZE);
    //count a bites read
    leftRead+=readSize;
    //update buffer
    alBufferData(alBuffer, format, loadBuffer, readSize, sempleRate);
    //return size
    return readSize;
}
///update
/**
 * Streaming sound implementation
 */
void StreamSoundAL::update(){
    //
    if(!isPlay()&&!isLoop()) return;
    if(!isPlay()&&isLoop())/* wrong */{
        play();
    }
    //buffer info
    ALuint alBuffer;
    //loop
    int processed;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
    //
    while(processed--)
    {
        //get buffer
        alSourceUnqueueBuffers(source, 1, &alBuffer);
        //get last read
        ALint sizeReaded=0;
        alGetBufferi(alBuffer, AL_SIZE, &sizeReaded);
        lastRead+=sizeReaded;
        //stream file/data
        read(alBuffer);
        //in loop mode restart stream buffer
        if(sizeData<=leftRead && isLoop()) restart();
        //change buffer
        alSourceQueueBuffers(source, 1, &alBuffer);
    }
    
}