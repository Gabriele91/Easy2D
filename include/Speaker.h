#ifndef SPEAKER_H
#define SPEAKER_H

#include <Config.h>
#include <Audio.h>
#include <Component.h>
#include <Sound.h>

namespace Easy2D
{

class Speaker : public Component
{

	Audio::SoundEmitter* emitter;
	Sound::ptr resource;
	bool playOnAttach{ false };
	//sound state
	enum State
	{
		SPK_NONE,
		SPK_PLAY,
		SPK_LOOP
	};
	State state{ SPK_NONE };

public:

	Speaker(Sound::ptr soundResource = nullptr, bool is2d = false);
	virtual ~Speaker();
	/////////////////////////////////////
	void setSound(Sound::ptr sound);
	Sound::ptr getSound();
	/////////////////////////////////////
	///enable loop
	void enable_loop();
	///disable loop
	void disable_loop();
	///play sound
	void play();
	///play sound in loop mode
	void loop();
	///stop sound
	void stop();
	//set dound in pause
	void pause();
	//set volume (0...1)
	void volume(float volume);
	//get volume
	float volume();
	float real_volume();
	//remaining time
	float remaining_time();
	//playback time
	float playback_time();
	//global time duration
	float duration();
	//get sound states
	bool is_pause();
	bool is_play();
	bool is_stop();
	bool is_loop();
	/////////////////////////////////////
	void set2D(bool enable);
	bool is2D();
	void setRadius(float radius);
	float getRadius();
	void setPlayOnAttach(bool playonattach);
	bool getPlayOnAttach();
	/////////////////////////////////////
	//virtual void onChangedMatrix()
	virtual void onRun(float dt);
	virtual void onSetScene(Scene* scene);
	virtual void onScenePause();
	virtual void onSceneResume();
	/////////////////////////////////////
	//component
	ADD_COMPONENT_METHOS(Speaker)
	//serialize/deserialize
	virtual void serialize(Table& table);
	virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(Speaker, "Speaker")


};

#endif