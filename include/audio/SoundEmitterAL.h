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
	virtual void  __volumeFromManager(float volume);
	virtual void  __volume2dFromManager(float volume);
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
	virtual void setBuffer(Audio::SoundBuffer* buffer);
    ///enable loop
    virtual void enableLoop();
    ///disable loop
    virtual void disableLoop();
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
    virtual float realVolume();
    //remaining time
    virtual float remainingTime();
    //playback Time
    virtual float playbackTime();
    //global time duration
    virtual float duration();
    //get sound states
    virtual bool isPause();
    virtual bool isPlay();
    virtual bool isStop();
    virtual bool isLoop();
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