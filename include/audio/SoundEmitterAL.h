#ifndef SOUNDEMITTERAL_H
#define SOUNDEMITTERAL_H

#include <Config.h>
#include <Audio.h>

namespace Easy2D
{
class AudioAL;
class StaticBufferAL;
class StreamBufferAL;
class StreamContextAL;

class SoundEmitterAL : public Audio::SoundEmitter
{
protected:
    //cource file
	ALuint source;
	Audio::SoundBuffer* buffer;
	StreamContextAL*    context;
    //real volume
    float soundVolume;
    float audioVolume;
	float listenerVolume;
	//type
	bool sound2D;
	Vec2 position;
	float radius;
    //setted from audio class
	friend class Audio;
	friend class AudioAL;
	virtual void  volume_from_manager(float volume);
	virtual void  volume_2D_from_manager(float volume);
	virtual float __getListenerVolume()
	{
		return listenerVolume;
	}
	AudioAL* getAudio()
	{
		return ((AudioAL*)Application::instance()->get_audio());
	}

public:
    ///constructor
	SoundEmitterAL();
    ///destroy sound rerouce
	virtual ~SoundEmitterAL();
	///set buffer
	virtual void set_buffer(Audio::SoundBuffer* buffer);
    ///enable loop
    virtual void enable_loop();
    ///disable loop
    virtual void disable_loop();
    ///play sound
    virtual void play();
    ///play sound in loop mode
    virtual void loop();
    ///stop sound
    virtual void stop();
    //set dound in pause
    virtual void pause();
    //set volume (0...1)
    virtual void volume(float volume);
    //get volume
    virtual float volume();
    virtual float real_volume();
    //remaining time
    virtual float remaining_time();
    //playback Time
    virtual float playback_time();
    //global time duration
    virtual float duration();
    //get sound states
    virtual bool is_pause();
    virtual bool is_play();
    virtual bool is_stop();
    virtual bool is_loop();
	//todo
	virtual bool is2D();
	virtual void set2D(bool enable);
	virtual void setPosition(const 	Vec2& position);
	virtual void setRadius(float radius);
	virtual Vec2 getPosition();
	virtual float getRadius();
	//id OpenAL
	ALint getSource()
	{
		return source;
	}
};

};

#endif