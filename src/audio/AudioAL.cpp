#include <stdafx.h>
#include <AudioAL.h>
#include <StaticSoundAL.h>
#include <StreamSoundAL.h>
#include <EString.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
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
AudioAL::AudioAL():device(NULL),context(NULL),globalVolume(1.0){
	//alloc the OpenAL context
    streamThread.start();
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
AudioAL::~AudioAL(){
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
 * create a sound buffer
 */
Audio::SoundInterface* AudioAL::createSound(void *raw,
                                            size_t size,
                                            size_t sempleRate,
                                            Channels channels,
                                            SempleBit sempleBit){
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
	//return a StaticSoundAL class object
	return new StaticSoundAL(source,buffer,format,timeLongSong,globalVolume);
}
/**
 * create a stream sound buffer
 */
Audio::SoundInterface* AudioAL::createStreamSound(DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream,
                                                  DFUNCTION<void(void)> restartStream,
                                                  size_t size,
                                                  size_t sempleRate,
                                                  Channels channels,
                                                  SempleBit sempleBit){
	//get openAL format
 	ALenum format;
	if(channels==Channels::MONO)
		format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	else
		format = sempleBit==SempleBit::SEMPLE8BIT ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	//calc semple size (SampleRate * NumChannels * BitsPerSample/8)
	size_t sizeSemple= sempleRate * channels * sempleBit;
	float timeLongSong= (float)size/(float)sizeSemple;
    //create a source
 	ALuint source = AL_NONE;
	alGenSources(1, &source);
    return new StreamSoundAL(&streamThread,         //stream thread
                             source,                //souce
                             readStream,            //readStream callback
                             restartStream,         //restart steam callback
                             size,                  //global size
                             sempleRate,            //semple rate
                             sizeSemple,            //size of a semple
                             format,                //OpenAL buffer type
                             timeLongSong,          //len sound
                             globalVolume           //volume
                             );
}

/**
 * change volume to all sounds (in pecentual)
 */
void AudioAL::changeVolume(float volume){
	globalVolume=volume;
    Audio::changeVolume(volume);
}