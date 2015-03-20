#ifndef AUDIO_H
#define AUDIO_H

#include <Config.h>
#include <Math3D.h>
#include <Application.h>

namespace Easy2D
{

class Audio
{


public:

	enum BufferType
	{
		STATIC = 1,
		STREAM = 2
	};
	enum Channels
	{
		MONO = 1,
		STEREO = 2
	};
    enum SempleBit
    {
		SEMPLE8BIT = 1,
		SEMPLE16BIT = 2,
		SEMPLE16BEBIT = 3
    };

	class SoundBuffer
	{
		public:
		virtual ~SoundBuffer(){};
		///return type of buffer
		virtual BufferType getType() = 0;
		///get format buffer
		virtual int getFormat() = 0;
		///get sound length
		virtual float getLength() = 0;
	};
    class SoundEmitter
    {
    protected:

		friend class Audio;
		virtual void __volumeFromManager(float volume) = 0;
		virtual void __volume2dFromManager(float volume) = 0;

	public:
        ///destroy sound rerouce
		virtual ~SoundEmitter() {};
		///set buffer
		virtual void setBuffer(SoundBuffer* buffer) = 0;
        ///enable loop
        virtual void enableLoop()=0;
        ///disable loop
        virtual void disableLoop()=0;
        ///play sound
        virtual void play()=0;
        ///play sound in loop mode
        virtual void loop()=0;
        ///stop sound
        virtual void stop()=0;
        //set dound in pause
        virtual void pause()=0;
        //set volume (0...1)
        virtual void volume(float volume)=0;
        //get volume
        virtual float volume()=0;
        virtual float realVolume()=0;
        //remaining time
        virtual float remainingTime()=0;
        //playback time
        virtual float playbackTime()=0;
        //global time duration
        virtual float duration()=0;
        //get sound states
        virtual bool isPause()=0;
        virtual bool isPlay()=0;
        virtual bool isStop()=0;
        virtual bool isLoop()=0;
		//2d sound settings
		virtual bool is2D() = 0;
		virtual void set2D(bool enable) = 0;
		virtual void setPosition(const 	Vec2& position) = 0;
		virtual void setRadius(float radius) = 0;
		virtual Vec2 getPosition() = 0;
		virtual float getRadius() = 0;

    };
	class SoundListener
	{
	protected:

		friend class Audio;

	public:
		virtual ~SoundListener(){}
		///set listener position
		virtual void setPosition(const 	Vec2& position) = 0;
		///set listener max volume
		virtual void setVolume(float volume) = 0;
		///get listener position
		virtual Vec2 getPosition() = 0;
		///get listener volume
		virtual float getVolume() = 0;

	};

    /**
    * init audio engine
    */
    Audio();
    /**
    * dealloc audio engine
    */
	virtual ~Audio();
	/**
	* create a emitter
	*/
	virtual SoundListener* createListener() = 0;
	/**
	* create a emitter
	*/
	virtual SoundEmitter* createEmitter() = 0;
    /**
     * create a sound buffer
     */
	virtual SoundBuffer* createBuffer(void *raw,
									  size_t size,
									  size_t sempleRate,
									  Channels channels,
									  SempleBit sempleBit)=0;
    /**
     * create a stream sound buffer
     */
	virtual SoundBuffer* createStreamBuffer(Application::ResouceStream* resource,
										    size_t offset,
										    size_t size,
										    size_t sempleRate,
										    Channels channels,
										    SempleBit sempleBit)=0;
    /**
    * change volume to all sounds
    */
    virtual void setVolume(float volume);
    /**
    * change volume to all sounds (in pecentual)
    */
	virtual void changeVolume(float volume);
	/**
	* subscription of the sound
	*/
	virtual void subscriptionSound(SoundEmitter *sound);
	/**
	* unsubscription of the sound
	*/
	virtual void unsubscriptionSound(SoundEmitter *sound);
	/**
	* subscription of the sound 2d
	*/
	virtual void subscriptionSound2D(SoundEmitter *sound);
	/**
	* unsubscription of the sound 2d
	*/
	virtual void unsubscriptionSound2D(SoundEmitter *sound);
    /**
    * for each sounds
    */
	virtual void foreachSounds(DFUNCTION<void(SoundEmitter*)> callback);
	/**
	* for each sounds 2D
	*/
	virtual void foreachSounds2D(DFUNCTION<void(SoundEmitter*)> callback);
	/**
	* update audio resources
	*/
	virtual void update(float dt){};

protected:

    ///a generic context (for default audio device)
	void *context;
	///sound subscription map
	DUNORDERED_MAP<SoundEmitter*, SoundEmitter*> soundmap;
	///sound subscription sound2D map
	DUNORDERED_MAP<SoundEmitter*, SoundEmitter*> sound2Dmap;

};

};

#endif