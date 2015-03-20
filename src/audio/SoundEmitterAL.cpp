#include <stdafx.h>
#include <Application.h>
#include <SoundEmitterAL.h>
#include <StreamBufferAL.h>
#include <StaticBufferAL.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////

///global volume
void SoundEmitterAL::__volumeFromManager(float volume)
{
	audioVolume = volume;
	alSourcef(source, AL_GAIN, realVolume());
}
void SoundEmitterAL::__volume2dFromManager(float volume)
{
	listenerVolume = volume;
	alSourcef(source, AL_GAIN, realVolume());
}

///constructor
SoundEmitterAL::SoundEmitterAL()
    :source(0)
	,buffer(nullptr)
	,context(nullptr)
	,soundVolume(1.0)
	,audioVolume(1.0)
	,listenerVolume(1.0)
	,sound2D(false)
	,radius(0.0)
{
	alGenSources(1, &source);
    getAudio()->subscriptionSound(this);
}
///destroy sound rerouce
SoundEmitterAL::~SoundEmitterAL()
{
	getAudio()->unsubscriptionSound(this);
	if (is2D()) set2D(false);
	if (isPlay()) stop();
	if (context) delete context;
	if (buffer) alSourcei(source, AL_BUFFER, 0);
	alDeleteSources(1, &source);
};
///set buffer
void SoundEmitterAL::setBuffer(Audio::SoundBuffer* inbuffer)
{
	//DEBUG_ASSERT(inbuffer);
	//delete last context
	if(isPlay()) stop();
	//dt buffer
	alSourcei(source, AL_BUFFER, 0);
	//delete context
	if (context){ delete context; context = nullptr; }
	buffer = inbuffer;
	//is not null?
	if (!buffer) return;
	//create a context
	if (buffer->getType() == Audio::STREAM)
	{
		context=((StreamBufferAL*)buffer)->newContext(this);
	}
	else
	{
		((StaticBufferAL*)buffer)->attach(source);
	}
}
///enable loop
void SoundEmitterAL::enableLoop()
{
	if (context) context->enableLoop();
	else alSourcei(source, AL_LOOPING, AL_TRUE);
}
///disable loop
void SoundEmitterAL::disableLoop()
{
	if (context) context->disableLoop();
	else alSourcei(source, AL_LOOPING, AL_FALSE);
}
///play sound
void SoundEmitterAL::play()
{
	//play "buffer"
	if (context) context->play();
	//play source
    alSourceRewind(source); //alSourcei(source, AL_BYTE_OFFSET, 0);
    alSourcePlay (source);
}
///play sound in loop mode
void SoundEmitterAL::loop()
{
    if(!isPlay()) play();
	if(!isLoop()) enableLoop();
}
///stop sound
void SoundEmitterAL::stop()
{
    if(isPlay())
    {
        alSourcei(source, AL_LOOPING,  AL_FALSE );
        alSourceStop(source);
    }
}
//set dound in pause
void SoundEmitterAL::pause()
{
    if(isPlay()) 
		alSourcePause(source);
}
//set volume (0...1)
void SoundEmitterAL::volume(float volume)
{
    soundVolume=volume;
	alSourcef(source, AL_GAIN, realVolume());
}
//get volume
float SoundEmitterAL::volume()
{
    return soundVolume;
}
float SoundEmitterAL::realVolume()
{
	return soundVolume*audioVolume*( is2D() ? listenerVolume : 1.0f );
}
//remaining time
float SoundEmitterAL::remainingTime()
{
    return duration()-playbackTime();
}
//playback Time
float SoundEmitterAL::playbackTime()
{
	//get from context
	if (context) return context->playbackTime();
	//else
    float result = 0.0;
    alGetSourcef(source, AL_SEC_OFFSET , &result);
    return result;
}
//global time duration
float SoundEmitterAL::duration()
{
	return buffer->getLength();
}
//get sound states
bool SoundEmitterAL::isPause()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return status == AL_PAUSED;
}
bool SoundEmitterAL::isPlay()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return status == AL_PLAYING;
}
bool SoundEmitterAL::isStop()
{
    ALint status;
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    return ( status != AL_PLAYING && status != AL_PAUSED );
}
bool SoundEmitterAL::isLoop()
{
	//get from context
	if (context) return context->isLoop();
	//else
    ALint looping;
    alGetSourcei (source, AL_LOOPING, &looping);
    return looping != 0 ;
}
//sound
bool SoundEmitterAL::is2D()
{
	return sound2D; 
}
void SoundEmitterAL::set2D(bool enable) 
{ 
	if (is2D() == enable) return;

	if (enable)
	{
		getAudio()->subscriptionSound2D(this);
		getAudio()->requiredUpdate();
		//force update
		getAudio()->update(Application::instance()->getLastDeltaTime()); 
	}
	else if (is2D() && !enable)
	{
		getAudio()->unsubscriptionSound2D(this);
		getAudio()->requiredUpdate();
		listenerVolume = 1.0;
	}

	sound2D = enable;
}
void SoundEmitterAL::setPosition(const Vec2& position) 
{
	if (this->position == position) return;
	this->position = position;
	getAudio()->requiredUpdate();
}
void SoundEmitterAL::setRadius(float radius) 
{
	if (this->radius == radius) return;
	this->radius = radius;
	getAudio()->requiredUpdate();
}
Vec2 SoundEmitterAL::getPosition() 
{ 
	return position; 
}
float SoundEmitterAL::getRadius()
{
	return radius; 
};