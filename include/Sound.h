#ifndef SOUND_H
#define SOUND_H

#include <Config.h>
#include <Audio.h>
#include <Resource.h>
#include <Application.h>

namespace Easy2D
{

class Sound : public Resource<Sound>
{

    //sound instance
    Audio::SoundBuffer *sbuffer;

public:

    Sound(ResourcesManager<Sound> *rsmr=NULL,
          const String& pathfile="");
    ///destroy sound rerouce
    virtual ~Sound();
    //load methods
    virtual bool load();
    virtual bool unload();
	//get sound buffer
	Audio::SoundBuffer* getBuffer()
	{
		return sbuffer;
	}

};

};

#endif