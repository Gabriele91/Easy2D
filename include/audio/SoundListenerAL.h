#ifndef SOUNDLISTENER_H
#define SOUNDLISTENER_H

#include <Config.h>
#include <AudioAL.h>

namespace Easy2D
{

class SoundListenerAL : public Audio::SoundListener
{
protected:

	friend class AudioAL;
	Vec2 position;
	float  volume;
	AudioAL* getAudio()
	{
		return ((AudioAL*)Application::instance()->get_audio());
	}

public:
	SoundListenerAL():volume(1.0)
	{
		getAudio()->subscriptionListener2D(this);
		getAudio()->requiredUpdate();
	}
	virtual ~SoundListenerAL()
	{
		getAudio()->unsubscriptionListener2D(this);
		getAudio()->requiredUpdate();
	}
	///set listener position
	virtual void setPosition(const 	Vec2& position)
	{
		if (this->position == position) return;
		this->position = position;
		getAudio()->requiredUpdate();
	}
	///set listener max volume
	virtual void setVolume(float volume)
	{
		if (this->volume == volume) return;
		this->volume = volume;
		getAudio()->requiredUpdate();
	}
	///get listener position
	virtual Vec2 getPosition()
	{
		return position;
	}
	///get listener volume
	virtual float getVolume()
	{
		return volume;
	}

};

};

#endif