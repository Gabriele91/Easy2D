#ifndef BASESOUNDAL_H
#define BASESOUNDAL_H

#include <Config.h>
#include <Audio.h>

namespace Easy2D
{


class BaseSoundAL : public Audio::SoundInterface
{
protected:
    //cource file
    ALuint source;
    //format buffer
    int format;
    //buffer len in second
    float lenSound;
    //real volume
    float soundVolume;
    float audioVolume;
    //settet from Audio siggleton
    friend class Audio;
    virtual void __volumeFromManager(float volume);

public:
    ///constructor
    BaseSoundAL(ALuint source,
                int format,
                float lenSound,
                float audioVolume);
    ///destroy sound rerouce
    virtual ~BaseSoundAL();
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
};

};

#endif