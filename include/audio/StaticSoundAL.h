#ifndef STATICSOUNDAL_H
#define STATICSOUNDAL_H

#include <Config.h>
#include <BaseSoundAL.h>

namespace Easy2D
{


class StaticSoundAL : public BaseSoundAL
{
protected:

    //buffer file
    ALuint buffer;

public:
    ///constructor
    StaticSoundAL(ALuint source,
                  ALuint buffer,
                  int format,
                  float lenSound,
                  float audioVolume);
    ///destroy sound rerouce
    virtual ~StaticSoundAL();
};

};

#endif