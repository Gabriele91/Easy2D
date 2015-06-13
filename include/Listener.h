#ifndef LISTENER_H
#define LISTENER_H

#include <Config.h>
#include <Audio.h>
#include <Component.h>
#include <Audio.h>

namespace Easy2D
{

class Listener : public Component
{

	Audio::SoundListener* listener;

public:

	Listener() :listener(Application::instance()->get_audio()->createListener())
	{
	}
	virtual ~Listener()
	{
		delete listener;
	}
	//set volume (0...1)
	DFORCEINLINE void volume(float volume)
	{
		listener->setVolume(volume);
	}
	//get volume
	DFORCEINLINE float volume()
	{
		return listener->getVolume();
	}
	/////////////////////////////////////	
	//virtual void onChangedMatrix()
	virtual void onRun(float dt)
	{
		listener->setPosition(getObject()->getPosition(true));
	}
	/////////////////////////////////////
	//component
	ADD_COMPONENT_METHOS(Listener)
	//serialize/deserialize
	virtual void serialize(Table& table)
	{
		table.set("volume", volume());
	}
	virtual void deserialize(const Table& table)
	{
		volume(table.getFloat("volume", volume()));
	}

};
REGISTERED_COMPONENT(Listener, "Listener")

};
#endif