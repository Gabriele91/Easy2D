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

/**
* init audio engine
*/
Audio::Audio()
{
}
/**
* dealloc audio engine
*/
Audio::~Audio()
{
}
/**
 * change volume to all sounds
 */
void Audio::setVolume(float volume)
{
    for(auto sound:soundmap)
    {
        sound.second->volume(volume);
    }
}
/**
 * change volume to all sounds (in pecentual)
 */
void Audio::changeVolume(float volume)
{
    for(auto sound:soundmap)
    {
        sound.second->__volumeFromManager(volume);
    }
}
/**
* subscription of the sound
*/
void Audio::subscriptionSound(Audio::SoundEmitter *sound)
{
	soundmap[sound] = sound;
}
/**
* unsubscription of the sound
*/
void Audio::unsubscriptionSound(Audio::SoundEmitter *sound)
{
	auto it = soundmap.find(sound);
	if (it != soundmap.end())
		soundmap.erase(it);
}
/**
* subscription of the sound 2d
*/
void Audio::subscriptionSound2D(Audio::SoundEmitter *sound)
{
	sound2Dmap[sound] = sound;
}
/**
* unsubscription of the sound 2d
*/
void Audio::unsubscriptionSound2D(Audio::SoundEmitter *sound)
{
	auto it = sound2Dmap.find(sound);
	if (it != sound2Dmap.end())
		sound2Dmap.erase(it);
}
/**
* for each sounds
*/
void Audio::foreachSounds(DFUNCTION<void(Audio::SoundEmitter*)> callback)
{
	for (auto sound : soundmap)
	{
		callback(sound.second);
	}
}
/**
* for each sounds 2d
*/
void Audio::foreachSounds2D(DFUNCTION<void(Audio::SoundEmitter*)> callback)
{
	for (auto sound : sound2Dmap)
	{
		callback(sound.second);
	}
}