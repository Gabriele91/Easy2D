#include <stdafx.h>
#include <AudioAL.h>
#include <Application.h>
#include <StaticBufferAL.h>
#include <StreamBufferAL.h>
#include <SoundEmitterAL.h>
#include <SoundListenerAL.h>
#include <EString.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
/*
 OpeanAL helpers
 */
static String ualGetALErrorString(ALenum err)
{
    switch(err)
    {
    case AL_NO_ERROR:
        return String("AL_NO_ERROR");
        break;

    case AL_INVALID_NAME:
        return String("AL_INVALID_NAME");
        break;

    case AL_INVALID_ENUM:
        return String("AL_INVALID_ENUM");
        break;

    case AL_INVALID_VALUE:
        return String("AL_INVALID_VALUE");
        break;

    case AL_INVALID_OPERATION:
        return String("AL_INVALID_OPERATION");
        break;

    case AL_OUT_OF_MEMORY:
        return String("AL_OUT_OF_MEMORY");
        break;
    };
    return String("AL_?????_ERROR");
}
static String ualGetALCErrorString(ALCenum err)
{
    switch(err)
    {
    case ALC_NO_ERROR:
        return String("ALC_NO_ERROR");
        break;

    case ALC_INVALID_ENUM:
        return String("ALC_INVALID_ENUM");
        break;

    case ALC_INVALID_VALUE:
        return String("ALC_INVALID_VALUE");
        break;

    case ALC_INVALID_CONTEXT:
        return String("ALC_INVALID_CONTEXT");
        break;

    case ALC_INVALID_DEVICE:
        return String("ALC_INVALID_DEVICE");
        break;

    case ALC_OUT_OF_MEMORY:
        return String("ALC_OUT_OF_MEMORY");
        break;
    };
    return String("AL_?????_ERROR");
}
static void ualGetDevices(std::vector<String>& Devices)
{
    // Vidage de la liste
    Devices.clear();

    // RÈcupÈration des devices disponibles
    const ALCchar* DeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

    if (DeviceList)
    {
        // Extraction des devices contenus dans la chaÓne renvoyÈe
        while (strlen(DeviceList) > 0)
        {
            Devices.push_back(DeviceList);
            DeviceList += strlen(DeviceList) + 1;
        }
    }
}
/**
 * init audio engine
 */
AudioAL::AudioAL()
	:device(nullptr)
	,context(nullptr)
	,globalVolume(1.0)
	,doUpdate(false)
{
    //alloc the OpenAL context
    streamThread.start();
    //get devices
    std::vector<String> devices;
    ualGetDevices(devices);
    //debug
    DEBUG_CODE(
        DEBUG_MESSAGE("Inizialization OpenAL Device");
        for(size_t i=0; i!=devices.size(); ++i)
{
    DEBUG_MESSAGE( "Audio device:"<<devices[i] );
    }
    )
    // Clear Error Code (so we can catch any new errors)
    alGetError();
    // Initialization
    alcMakeContextCurrent(NULL);
    // select the "preferred device"
    device = alcOpenDevice(alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER));
    //debug
    DEBUG_MESSAGE("alcOpenDevice = "<<device);
    DEBUG_ASSERT_MSG( device, "OpenAL can't open device");
    DEBUG_ASSERT_MGS_REPLACE(device, "OpenALc open device error:" << ualGetALCErrorString(alcGetError(device)) );
    //create OpenAL Context
    context = alcCreateContext(device,NULL);
    DEBUG_ASSERT_MSG( context,"OpenALc create context error:" << ualGetALCErrorString(alcGetError(device)) );
    //enable context
    DEBUG_ASSERT_MGS_REPLACE(alcMakeContextCurrent(context), "OpenALc make current error:" << ualGetALCErrorString(alcGetError(device)) );
    //debug
    DEBUG_CODE(
        int error=alGetError();
        DEBUG_ASSERT_MSG( alGetError() == AL_NO_ERROR , "openAL error:" << ualGetALErrorString(error) );
    )
}
/**
 * dealloc audio engine
 */
AudioAL::~AudioAL()
{
    //break loop
    streamThread.breakLoop();
    streamThread.join();
    //delete
    alcMakeContextCurrent(NULL);
    alcDestroyContext (context);
    //debug
    DEBUG_ASSERT_MSG(alcGetError (device) == ALC_NO_ERROR,
                     "OpenAL error destroy context: " << ualGetALErrorString(alcGetError (device)));
    //close the device
#if 1
    alcCloseDevice (device);
#else
    DEBUG_ASSERT_MGS_REPLACE( alcCloseDevice (device)  ,
                              "OpenAL error close device: " << ualGetALErrorString(alcGetError (device)) );
#endif
}
/**
* create a emitter
*/
Audio::SoundListener* AudioAL::createListener()
{
	return new SoundListenerAL();
}
/**
* create a emitter
*/
Audio::SoundEmitter* AudioAL::createEmitter()
{
	return new SoundEmitterAL();
}
/**
 * create a sound buffer
 */
Audio::SoundBuffer* AudioAL::createBuffer(void *raw,
										  size_t size,
										  size_t sempleRate,
										  Channels channels,
										  SempleBit sempleBit)
{
    //get openAL format
    ALenum format;
    if(channels==Channels::MONO)
        format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    else
        format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    //create buffer
    ALuint buffer = AL_NONE;
    alGenBuffers(1, &buffer);
    //send file data
    alBufferData(buffer, format, raw, size, sempleRate);
    //calc semple size (SampleRate * NumChannels * BitsPerSample/8)
    size_t sizeSemple= sempleRate * channels * sempleBit;
    float timeLongSong= (float)size/(float)sizeSemple;
    //return a StaticSoundAL class object
	return new StaticBufferAL(buffer, format, timeLongSong);
}
/**
 * create a stream sound buffer
 */
Audio::SoundBuffer* AudioAL::createStreamBuffer(Application::ResouceStream* resource,
												size_t offset,
												size_t size,
												size_t sempleRate,
												Channels channels,
												SempleBit sempleBit)
{
    //get openAL format
    ALenum format;
    if(channels==Channels::MONO)
        format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    else
        format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	//big ending?
	bool isBE = sempleBit == SempleBit::SEMPLE16BEBIT;
    //calc semple size (SampleRate * NumChannels * BitsPerSample/8)
    size_t sizeSemple= sempleRate * channels * sempleBit;
    float timeLongSong= (float)size/(float)sizeSemple;
	//create buffer
	return new StreamBufferAL(resource,             //resource
							  offset,               //resource offset
                              size,                  //global size
                              sempleRate,            //semple rate
                              sizeSemple,            //size of a semple
                              format,                //OpenAL buffer type
                              isBE,                  //is big ending?
                              timeLongSong           //len sound
                              );
}

/**
 * change volume to all sounds (in pecentual)
 */
void AudioAL::changeVolume(float volume)
{
    globalVolume=volume;
    Audio::changeVolume(volume);
}
/**
* subscription of the listener 2d
*/
void AudioAL::subscriptionListener2D(SoundListener *listener)
{
	listeners[listener] = listener;
}
/**
* unsubscription of the listener 2d
*/
void AudioAL::unsubscriptionListener2D(SoundListener *listener)
{
	auto it = listeners.find(listener);
	if (it != listeners.end())
		listeners.erase(it);
}
/**
* required a update audio resources
*/
void AudioAL::requiredUpdate()
{
	doUpdate = true;
}
/**
* update audio resources
*/
void AudioAL::update(float dt)
{
	if (doUpdate)
	{
		foreachSounds2D([this](SoundEmitter* _emitter)
		{
			SoundEmitterAL* const emitter = (SoundEmitterAL*)_emitter;
			const Vec2  pos    = emitter->getPosition();
			const float radius = emitter->getRadius();
			emitter->__volume2dFromManager(0.0);
			//calc audio
			if  (radius>0.0)
			for (auto it : listeners)
			{
				float dist  = it.second->getPosition().distance(pos);
				float volume = (Math::max(radius - dist,0.0f) / radius) * it.second->getVolume();
				if (emitter->__getListenerVolume() < volume)
				{
					emitter->__volume2dFromManager(volume);
				}
			}
		});
		doUpdate = false;
	}
}