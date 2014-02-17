#include <stdafx.h>
#include <Audio.h>
#include <EString.h>
#include <Debug.h>
#include <Application.h>
#include <Thread.h>
#include <Mutex.h>
#include <Math3D.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//deault api audio is openal
#ifdef PLATFORM_MAC_OS_X
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif
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
class BaseSoundAL : public Audio::SoundInterface{
protected:
	//cource file
	ALuint source;
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
	BaseSoundAL(ALuint source,
			    int format,
			    float lenSound,
			    float audioVolume):
    source(source),
    format(format),
    lenSound(lenSound),
    soundVolume(1.0),
    audioVolume(audioVolume)
	{
		Application::instance()->getAudio()->subscriptionSound(this);
	}
	///destroy sound rerouce
	virtual ~BaseSoundAL(){
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
class StaticSoundAL : public BaseSoundAL{
protected:
    
	//buffer file
    ALuint buffer;
    
public:
	///constructor
	StaticSoundAL(ALuint source,
                  ALuint buffer,
                  int format,
                  float lenSound,
                  float audioVolume):
    BaseSoundAL(source,format,lenSound,audioVolume),
    buffer(buffer)
	{
	}
    
	///destroy sound rerouce
	virtual ~StaticSoundAL(){
        if(isPlay()) stop();
		alDeleteBuffers(1,&buffer);
	};
};


/*************
 * STREAMING *
 *************/

#define AL_STREAME_SICURITY_FACTOR 2
#define AL_STREAM_BUFFER_SIZE  (size_t)(4410*32*AL_STREAME_SICURITY_FACTOR)
#define AL_STREAM_N_BUFFERS 2
#define AL_MAX_ONE_SEC_SIZE (16*2*44100*AL_STREAME_SICURITY_FACTOR)
#define AL_STREAM_THREAD_SLEEP_S ((double)(AL_STREAM_BUFFER_SIZE) / (double)(AL_MAX_ONE_SEC_SIZE))
#define AL_STREAM_THREAD_SLEEP_MS (AL_STREAM_THREAD_SLEEP_S*1000.0)
/**
 * class declaretion
 */
class StreamSoundAL;
class StreamThreadAL;
/**
 * Streaming sound thread
 */
class StreamThreadAL : public Thread {

    DUNORDERED_MAP<StreamSoundAL*,StreamSoundAL*> soundmap;
    Mutex mutex;
    bool isrunning;
    
public:
    
    StreamThreadAL():Thread(Thread::TERMINATE_JOIN),isrunning(true){}
    
    void push(StreamSoundAL *ssound){
        mutex.lock();
        soundmap[ssound]=ssound;
        mutex.unlock();
    }
    
    void erase(StreamSoundAL *ssound){
        mutex.lock();
            auto it=soundmap.find(ssound);
            if(it!=soundmap.end())
                soundmap.erase(it);
        mutex.unlock();
    }
    
    virtual int run();
    
	virtual ~StreamThreadAL(){
         isrunning=false;
	};

};
/**
 * Streaming sound
 */
class StreamSoundAL : public BaseSoundAL{
    
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
	virtual ~StreamSoundAL(){
        if(isPlay()) stop();
        //unschedule
        streamThread->erase(this);
        //delete buffers
		alDeleteBuffers(AL_STREAM_N_BUFFERS,buffers);
	};
    //overload play
	virtual void play() {
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
	virtual void loop(){
	    if(!isPlay()) play();
		if(!isLoop()) looping=true;
	}
    ///enable loop
	virtual void enableLoop(){
		looping=true;
	}
	///disable loop
	virtual void disableLoop(){
        looping=false;
	}
    ///playback Time
	virtual float playbackTime(){
        int bytesBufferRead;
		alGetSourcei(source, AL_BYTE_OFFSET , &bytesBufferRead);
        float  result=((float)(Math::min(lastRead+bytesBufferRead,sizeData))/sizeSemple);
		return result;
	}
	///global time duration
	virtual float duration(){
		return lenSound;
	}
    ///is in looping mode!?
	virtual bool isLoop(){
		return looping ;
	}
    
    //restat steam
    void restart(){
        restartStream();
        leftRead=0;
        lastRead=0;
    }
    //stream buffer
    size_t read(ALuint alBuffer){
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
    void update();
};

/**
 * Streaming sound thread implementation
 */
int StreamThreadAL::run(){
    while(isrunning){
        mutex.lock();
        for (auto sound:soundmap)
            sound.second->update();
        mutex.unlock();
        sleepThread(AL_STREAM_THREAD_SLEEP_MS);
    }
    return 0;
}
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

/*
* context struct OpenAL
*/
struct OpenALContext{
	ALCdevice *device;
	ALCcontext *context;
	float globalVolume;
    StreamThreadAL streamThread;
	OpenALContext():device(NULL),context(NULL),globalVolume(1.0){}
};
#define CONTEXT ((OpenALContext*)(this->context))
/**
* init audio engine
*/
Audio::Audio(){
	//alloc the OpenAL context
	this->context=(void*)new OpenALContext();
    CONTEXT->streamThread.start();
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
  delete CONTEXT;
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
	//return a StaticSoundAL class object
	return new StaticSoundAL(source,buffer,format,timeLongSong,CONTEXT->globalVolume);
}
/**
*
*/
Audio::SoundInterface* Audio::createStreamSound(DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream,
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
    return new StreamSoundAL(&CONTEXT->streamThread,//stream thread
                             source,                //souce
                             readStream,            //readStream callback
                             restartStream,         //restart steam callback
                             size,                  //global size
                             sempleRate,            //semple rate
                             sizeSemple,            //size of a semple
                             format,                //OpenAL buffer type
                             timeLongSong,          //len sound
                             CONTEXT->globalVolume  //volume
                             );
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