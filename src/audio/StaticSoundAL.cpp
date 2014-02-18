#include <stdafx.h>
#include <StaticSoundAL.h>

///////////////////////
using namespace Easy2D;
///////////////////////


///constructor
StaticSoundAL::StaticSoundAL( ALuint source,
                              ALuint buffer,
                              int format,
                              float lenSound,
                              float audioVolume):
BaseSoundAL(source,format,lenSound,audioVolume),
buffer(buffer)
{
}

///destroy sound rerouce
StaticSoundAL::~StaticSoundAL(){
    if(isPlay()) stop();
    alDeleteBuffers(1,&buffer);
};