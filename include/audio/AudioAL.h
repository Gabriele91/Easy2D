#ifndef AUDIOAL_H
#define AUDIOAL_H

#include <Config.h>
#include <Audio.h>
#include <StreamThreadAL.h>

namespace Easy2D
{
class StreamBufferAL;
class StaticBufferAL;
class SoundEmitterAL;
class SoundListenerAL;

class AudioAL : public Audio
{
protected:

	////////////////////////////////////////////////////////////
    ALCdevice *device;
    ALCcontext *context;
    float globalVolume;
    StreamThreadAL streamThread;
	////////////////////////////////////////////////////////////
	bool  doUpdate;
	DUNORDERED_MAP<SoundListener*, SoundListener*> listeners;
	void requiredUpdate();
	friend class SoundListenerAL;
	friend class SoundEmitterAL;

public:

    /**
     * init audio engine
     */
    AudioAL();
    /**
     * dealloc audio engine
     */
	virtual ~AudioAL();
	/**
	* create a emitter
	*/
	virtual SoundListener* createListener();
	/**
	* create a emitter
	*/
	virtual SoundEmitter* createEmitter();
    /**
     * create a sound buffer
     */
	virtual SoundBuffer* createBuffer(void *raw,
                                      size_t size,
                                      size_t sempleRate,
                                      Channels channels,
                                      SempleBit sempleBit);
    /**
     * create a stream sound buffer
     */
	virtual SoundBuffer* createStreamBuffer(Application::ResouceStream* resource,
										    size_t offset,
										    size_t size,
										    size_t sempleRate,
										    Channels channels,
										    SempleBit sempleBit);

    /**
     * change volume to all sounds (in pecentual)
     */
	virtual void changeVolume(float volume);
	/**
	* update audio resources
	*/
	virtual void update(float dt);
	/**
	* get audio stream thread
	*/
	StreamThreadAL& getStreamThreadAL()
	{
		return streamThread;
	}
	/**
	* subscription of the listener 2d
	*/
	void subscriptionListener2D(SoundListener *listener);
	/**
	* unsubscription of the listener 2d
	*/
	void unsubscriptionListener2D(SoundListener *listener);
};

};

#endif