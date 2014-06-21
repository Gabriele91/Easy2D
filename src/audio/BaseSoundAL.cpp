#include <stdafx.h>
#include <Application.h>
#include <BaseSoundAL.h>
///////////////////////
using namespace Easy2D;
///////////////////////

///global volume
void BaseSoundAL::__volumeFromManager(float volume)
{
    audioVolume=volume;
    alSourcef(source, AL_GAIN, audioVolume*audioVolume);
}

///constructor
BaseSoundAL::BaseSoundAL(ALuint source,int format,float lenSound,float audioVolume)
    :source(source),format(format),lenSound(lenSound),soundVolume(1.0),audioVolume(audioVolume)
{
    Application::instance()->getAudio()->subscriptionSound(this);
}
///destroy sound rerouce
BaseSoundAL::~BaseSoundAL()
{
    alDeleteSources(1,&source);
    Application::instance()->getAudio()->unsubscriptionSound(this);
};
///enable loop
void BaseSoundAL::enableLoop()
{
    alSourcei(source, AL_LOOPING,  AL_TRUE );
}
///disable loop
void BaseSoundAL::disableLoop()
{
    alSourcei(source, AL_LOOPING,  AL_FALSE );
}
///play sound
void BaseSoundAL::play()
{
    alSourceRewind(source); //alSourcei(source, AL_BYTE_OFFSET, 0);
    alSourcePlay (source);
}
///play sound in loop mode
void BaseSoundAL::loop()
{
    if(!isPlay()) play();
    if(!isLoop()) alSourcei(source, AL_LOOPING,  AL_TRUE  );
}
///stop sound
void BaseSoundAL::stop()
{
    if(isPlay())
    {
        alSourcei(source, AL_LOOPING,  AL_FALSE );
        alSourceStop(source);
    }
}
//set dound in pause
void BaseSoundAL::pause()
{
    if(isPlay())
        alSourcePause(source);
}
//set volume (0...1)
void BaseSoundAL::volume(float volume)
{
    soundVolume=volume;
    alSourcef(source, AL_GAIN, volume);
}
//get volume
float BaseSoundAL::volume()
{
    return soundVolume;
}
float BaseSoundAL::realVolume()
{
    return audioVolume*audioVolume;
}
//remaining time
float BaseSoundAL::remainingTime()
{
    return duration()-playbackTime();
}
//playback Time
float BaseSoundAL::playbackTime()
{
    float result = 0.0;
    alGetSourcef(source, AL_SEC_OFFSET , &result);
    return result;
}
//global time duration
float BaseSoundAL::duration()
{
    return lenSound;
}
//get sound states
bool BaseSoundAL::isPause()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return status == AL_PAUSED;
}
bool BaseSoundAL::isPlay()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return status == AL_PLAYING;
}
bool BaseSoundAL::isStop()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return ( status != AL_PLAYING && status != AL_PAUSED );
}
bool BaseSoundAL::isLoop()
{
    ALint looping;
    alGetSourcei (source, AL_LOOPING, &looping);
    return looping != 0 ;
}