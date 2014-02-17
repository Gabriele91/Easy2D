#ifndef SOUND_H
#define SOUND_H

#include <Config.h>
#include <Audio.h>
#include <Resource.h>
#include <Application.h>

namespace Easy2D {

	class Sound : public Resource<Sound> {
        
        //sound instance
        Audio::SoundInterface *iSound;
        //sound stream
        Application::ResouceStream *pResource;
        size_t offsetStartStream;
        
	public:

		Sound(ResourcesManager<Sound> *rsmr=NULL,
              const String& pathfile="");
        ///destroy sound rerouce
        virtual ~Sound();
		//load methods
		virtual bool load();
		virtual bool unload();
        //sound methos
        ///enable loop
        DFORCEINLINE void enableLoop(){
            iSound->enableLoop();
        }
        ///disable loop
        DFORCEINLINE void disableLoop(){
            iSound->disableLoop();
        }
        ///play sound
        DFORCEINLINE void play(){
            iSound->play();
        }
        ///play sound in loop mode
        DFORCEINLINE void loop(){
            iSound->loop();
        }
        ///stop sound
        DFORCEINLINE void stop(){
            iSound->stop();
        }
        //set dound in pause
        DFORCEINLINE void pause(){
            iSound->pause();
        }
        //set volume (0...1)
        DFORCEINLINE void volume(float volume){
            iSound->volume(volume);
        }
        //get volume
        DFORCEINLINE float volume(){
            return iSound->volume();
        }
        DFORCEINLINE float realVolume(){
            return iSound->realVolume();
        }
        //remaining time
        DFORCEINLINE float remainingTime(){
            return iSound->remainingTime();
        }
        //playback time
        DFORCEINLINE float playbackTime(){
            return iSound->playbackTime();
        }
        //global time duration
        DFORCEINLINE float duration(){
            return iSound->duration();
        }
        //get sound states
        DFORCEINLINE bool isPause(){
            return iSound->isPause();
        }
        DFORCEINLINE bool isPlay(){
            return iSound->isPlay();
        }
        DFORCEINLINE bool isStop(){
            return iSound->isStop();
        }
        DFORCEINLINE bool isLoop(){
            return iSound->isLoop();
        }

	};

};

#endif