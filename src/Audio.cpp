#include <stdafx.h>
#include <Audio.h>
#include <EString.h>
#include <Debug.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//deault api audio is openal
#include <AL/al.h>
#include <AL/alc.h>
/*
OpeanAL helpers
*/
static String ualGetALErrorString(ALenum err){
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
static String ualGetALCErrorString(ALCenum err){
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
static void ualGetDevices(std::vector<String>& Devices){
    // Vidage de la liste
    Devices.clear();

    // Récupération des devices disponibles
    const ALCchar* DeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

    if (DeviceList)
    {
        // Extraction des devices contenus dans la chaîne renvoyée
        while (strlen(DeviceList) > 0)
        {
            Devices.push_back(DeviceList);
            DeviceList += strlen(DeviceList) + 1;
        }
    }
}

/**
* OpenAL Sound
*/
class SoundAL : public Audio::SoundInterface{
protected:
	//cource file
	ALuint source;
	//buffer file
    ALuint buffer;
	//format buffer
	int format;
	//buffer len in second
	float lenSound;
	//real volume
	float soundVolume;
	float audioVolume;
	//settet from Audio siggleton
	friend class Audio;
	virtual void __volumeFromManager(float volume){
		audioVolume=volume;
		alSourcef(source, AL_GAIN, audioVolume*audioVolume);
	}

public:
	///constructor
	SoundAL(ALuint source,
			ALuint buffer,
			int format,
			float lenSound,
			float audioVolume):source(source),
							   buffer(buffer),
							   format(format),
							   lenSound(lenSound),
							   soundVolume(1.0),
							   audioVolume(audioVolume)
	{
		Application::instance()->getAudio()->subscriptionSound(this);
	}
	///destroy sound rerouce
	virtual ~SoundAL(){
		alDeleteBuffers(1,&buffer);
		alDeleteSources(1,&source);
		Application::instance()->getAudio()->unsubscriptionSound(this);
	};
	///enable loop
	virtual void enableLoop(){
		alSourcei(source, AL_LOOPING,  AL_TRUE );
	}
	///disable loop
	virtual void disableLoop(){
		alSourcei(source, AL_LOOPING,  AL_FALSE );
	}
	///play sound
	virtual void play(){
		alSourceRewind(source); //alSourcei(source, AL_BYTE_OFFSET, 0);
		alSourcePlay (source);
	}
	///play sound in loop mode
	virtual void loop(){
	    if(!isPlay()) play();
		if(!isLoop()) alSourcei(source, AL_LOOPING,  AL_TRUE  );
	}
	///stop sound
	virtual void stop(){
	    if(isPlay()){
			alSourcei(source, AL_LOOPING,  AL_FALSE );
			alSourceStop(source);
		}
	}
	//set dound in pause
	virtual void pause(){
	    if(isPlay())
			alSourcePause(source);
	}
	//set volume (0...1)
	virtual void volume(float volume){
		soundVolume=volume;
		alSourcef(source, AL_GAIN, volume);
	}
	//get volume
	virtual float volume(){
		return soundVolume;
	}
	virtual float realVolume(){
		return audioVolume*audioVolume;
	}
	//remaining time
	virtual float remainingTime(){
		return duration()-playbackTime();
	}
	//playback Time
	virtual float playbackTime(){
		float result = 0.0;
		alGetSourcef(source, AL_SEC_OFFSET , &result);
		return result;
	}
	//global time duration
	virtual float duration(){
		return lenSound;
	}
	//get sound states
	virtual bool isPause(){
		ALint status;
		alGetSourcei (source, AL_SOURCE_STATE, &status);
		return status == AL_PAUSED;
	}
	virtual bool isPlay(){
		ALint status;
		alGetSourcei (source, AL_SOURCE_STATE, &status);
		return status == AL_PLAYING;
	}
	virtual bool isStop(){
		ALint status;
		alGetSourcei (source, AL_SOURCE_STATE, &status);
		return ( status != AL_PLAYING && status != AL_PAUSED );
	}
	virtual bool isLoop(){	
		ALint looping;
		alGetSourcei (source, AL_LOOPING, &looping);
		return looping != 0 ;
	}
}; 
/*
* context struct OpenAL
*/
struct OpenALContext{
	ALCdevice *device;
	ALCcontext *context;
	float globalVolume;
	OpenALContext():device(NULL),context(NULL),globalVolume(1.0){}
};
#define CONTEXT ((OpenALContext*)(this->context))
/**
* init audio engine
*/
Audio::Audio(){
	//alloc the OpenAL context
	this->context=(void*)new OpenALContext();
	//get devices
	std::vector<String> devices;
	ualGetDevices(devices);
	//debug
	DEBUG_CODE(
		DEBUG_MESSAGE("Inizialization OpenAL Device");
		for(size_t i=0;i!=devices.size();++i){
			DEBUG_MESSAGE( "Audio device:"<<devices[i] );
		}
	)
    // Clear Error Code (so we can catch any new errors)
    alGetError();
	// Initialization
	alcMakeContextCurrent(NULL);
	// select the "preferred device"
    CONTEXT->device = alcOpenDevice(alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER));
	//debug
	DEBUG_MESSAGE("alcOpenDevice = "<<CONTEXT->device);
	DEBUG_ASSERT_MSG( CONTEXT->device, "OpenAL can't open device");
	DEBUG_ASSERT_MGS_REPLACE(CONTEXT->device, "OpenALc open device error:" << ualGetALCErrorString(alcGetError(CONTEXT->device)) );
	//create OpenAL Context
	CONTEXT->context = alcCreateContext(CONTEXT->device,NULL);
	DEBUG_ASSERT_MSG( CONTEXT->context,"OpenALc create context error:" << ualGetALCErrorString(alcGetError(CONTEXT->device)) );
	//enable context
	DEBUG_ASSERT_MGS_REPLACE(alcMakeContextCurrent(CONTEXT->context), "OpenALc make current error:" << ualGetALCErrorString(alcGetError(CONTEXT->device)) );
	//debug
	DEBUG_CODE(
		int error=alGetError();
		DEBUG_ASSERT_MSG( alGetError() == AL_NO_ERROR , "openAL error:" << ualGetALErrorString(error) );
	)
}
/**
* dealloc audio engine
*/
Audio::~Audio(){
  //delete
  alcMakeContextCurrent(NULL);
  alcDestroyContext (CONTEXT->context);
  //debug
  DEBUG_ASSERT_MSG(alcGetError (CONTEXT->device) == ALC_NO_ERROR,
				   "OpenAL error destroy context: " << ualGetALErrorString(alcGetError (CONTEXT->device)));
  //close the device
#if 1
  alcCloseDevice (CONTEXT->device);
#else
  DEBUG_ASSERT_MGS_REPLACE( alcCloseDevice (CONTEXT->device)  , 
						   "OpenAL error close device: " << ualGetALErrorString(alcGetError (CONTEXT->device)) );
#endif  
  //dealloc context
  delete this->context;
  this->context=NULL;
}
/**
* subscription of the sound
*/
void Audio::subscriptionSound(Audio::SoundInterface *sound){
	soundmap[sound]=sound;
}
/**
* unsubscription of the sound
*/
void Audio::unsubscriptionSound(Audio::SoundInterface *sound){
	auto it=soundmap.find(sound);
	if(it!=soundmap.end())
		soundmap.erase(it);
}
/**
* create a sound buffer
*/
Audio::SoundInterface* Audio::createSound(void *raw,
										  size_t size,
										  size_t sempleRate,
										  Audio::Channels channels,
										  Audio::SempleBit sempleBit)
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
	//create a source
 	ALuint source = AL_NONE;
	alGenSources(1, &source);
	alSourcei (source, AL_BUFFER, buffer);
	//calc semple size (SampleRate * NumChannels * BitsPerSample/8)
	size_t sizeSemple= sempleRate * channels * sempleBit;
	float timeLongSong= (float)size/(float)sizeSemple;
	//return a SoundAL class object
	return new SoundAL(source,buffer,format,timeLongSong,CONTEXT->globalVolume);
}
/**
* change volume to all sounds
*/
void Audio::setVolume(float volume){
	for(auto sound:soundmap){
		sound.second->volume(volume);
	}
}
/**
* change volume to all sounds (in pecentual)
*/
void Audio::changeVolume(float volume){
	CONTEXT->globalVolume=volume;
	for(auto sound:soundmap){
		sound.second->__volumeFromManager(volume);
	}
}
/**
* for each sounds
*/
void Audio::foreachSounds(DFUNCTION<void(Audio::SoundInterface*)> callback){
	for(auto sound:soundmap){
		callback(sound.second);
	}
}