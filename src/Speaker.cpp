#include <stdafx.h>
#include <Speaker.h>
#include <Object.h>
#include <ResourcesGroup.h>
/////////////////////////////////////
using namespace Easy2D;
/////////////////////////////////////

Speaker::Speaker(Sound::ptr soundResource, bool is2d)
		:emitter(Application::instance()
				 ->get_audio()
				 ->createEmitter())
{
	setSound(soundResource);
	set2D(is2d);
}
Speaker::~Speaker()
{
	delete emitter;
}
/////////////////////////////////////
void Speaker::setSound(Sound::ptr sound)
{
	resource = sound;
	if (resource)
	{
		if (!resource->isLoad()) resource->load();
		emitter->set_buffer(resource->getBuffer());
	}
	else emitter->set_buffer(nullptr);
}
Sound::ptr Speaker::getSound()
{
	return resource;
}
/////////////////////////////////////
///enable loop
void Speaker::enable_loop()
{
	emitter->enable_loop();
}
///disable loop
void Speaker::disable_loop()
{
	emitter->disable_loop();
}
///play sound
void Speaker::play()
{
	if (getScene())
	{
		emitter->play();
		state = SPK_NONE;
	}
	else if (state==SPK_NONE)
	{
		state = SPK_PLAY;
	}
}
///play sound in loop mode
void Speaker::loop()
{
	if (getScene())
	{
		emitter->loop();
		state = SPK_NONE;
	}
	else if (state == SPK_NONE)
	{
		state = SPK_LOOP;
	}
}
///stop sound
void Speaker::stop()
{
	emitter->stop();
	state = SPK_NONE;
}
//set dound in pause
void Speaker::pause()
{
	emitter->pause();
	state = SPK_NONE;
}
//set volume (0...1)
void Speaker::volume(float volume)
{
	emitter->volume(volume);
}
//get volume
float Speaker::volume()
{
	return emitter->volume();
}
float Speaker::real_volume()
{
	return emitter->real_volume();
}
//remaining time
float Speaker::remaining_time()
{
	return emitter->remaining_time();
}
//playback time
float Speaker::playback_time()
{
	return emitter->playback_time();
}
//global time duration
float Speaker::duration()
{
	return emitter->duration();
}
//get sound states
bool Speaker::is_pause()
{
	if (!getScene() && state != SPK_NONE)
	{ 
		return false;
	}

	return emitter->is_pause();
}
bool Speaker::is_play()
{
	if (!getScene() && state != SPK_NONE)
	{
		return true;
	}

	return emitter->is_play();
}
bool Speaker::is_stop()
{
	return emitter->is_stop();
}
bool Speaker::is_loop()
{
	if (!getScene() && state == SPK_LOOP)
	{
		return true;
	}
	return emitter->is_loop();
}
/////////////////////////////////////
void Speaker::set2D(bool enable)
{
	emitter->set2D(enable);
}
bool Speaker::is2D()
{
	return emitter->is2D();
}
void Speaker::setRadius(float radius)
{
	emitter->setRadius(radius);
}
float Speaker::getRadius()
{
	return emitter->getRadius();
}
void Speaker::setPlayOnAttach(bool playonattach)
{
	this->playOnAttach = playonattach;
}
bool Speaker::getPlayOnAttach()
{
	return this->playOnAttach;
}
/////////////////////////////////////
//virtual void onChangedMatrix()
void Speaker::onRun(float dt)
{
	if (is2D())
	{
		emitter->setPosition(getObject()->getPosition(true));
	}
}
void Speaker::onSetScene(Scene* scene)
{
	onSceneResume(); 
	//play on attach
	if (playOnAttach && !is_play())
	{
		if (is_loop())
			loop();
		else
			play();
	}
}
void Speaker::onScenePause()
{
	//////////////////////////////////////////////
	if (is_loop()) state = Speaker::SPK_LOOP;
	else if (is_play()) state = Speaker::SPK_PLAY;
	//////////////////////////////////////////////
	if (state != Speaker::SPK_NONE) emitter->pause();
}
void Speaker::onSceneResume()
{
	switch (state)
	{
	default: break;
	case Speaker::SPK_NONE:	break;
	case Speaker::SPK_PLAY: if (is_pause())    play(); break;
	case Speaker::SPK_LOOP: if (is_pause())    loop(); break;
	}
}
/////////////////////////////////////
//component
//serialize/deserialize
void Speaker::serialize(Table& table)
{
	//info sound
	table.set("sound", resource->getName());
	if (is_loop() || state == Speaker::SPK_LOOP) table.set("loop", 1.0);
	else if (is_play() || state == Speaker::SPK_PLAY) table.set("play", 1.0);
	table.set("volume", volume());
	//set play on attach
	table.set("playOnAttach", (float)playOnAttach);
	//info 2d
	if (is2D())
	{
		table.set("is2D", 1.0);
		table.set("radius", getRadius());
	}
}
void Speaker::deserialize(const Table& table)
{
	//get is 2D
	set2D(table.getFloat("is2D", (float)is2D()) != 0.0f);
	if (is2D()) setRadius(table.getFloat("radius", (float)getRadius()));
	//load sound
	auto rsmanager = table.getResourcesManager();
	DEBUG_ASSERT(rsmanager);
	auto rsgroup = rsmanager->getResourcesGroup();
	DEBUG_ASSERT(rsgroup);
	setSound(rsgroup->load<Sound>(table.getString("sound")));
	//set volume
	volume(table.getFloat("volume", volume()));
	//set play on attach
	setPlayOnAttach(table.getFloat("playOnAttach", (float)playOnAttach) != 0.0f);
	//get sound info
	if (table.getFloat("loop", (float)is_loop()) != 0.0f)
		loop();
	else if (table.getFloat("play", (float)is_play()) != 0.0f)
		play();
	//play on attach
	if (playOnAttach && getScene() && !is_play())
	{
		if (is_loop())
			loop();
		else
			play();
	}
}