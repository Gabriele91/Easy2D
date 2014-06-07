#ifndef WAVLOADER_H
#define WAVLOADER_H

#include <Config.h>
#include <Audio.h>
#include <Utility.h>
#include <Application.h>

namespace Easy2D
{

class WavLoader
{

public:

    struct InfoSound
    {
        Audio::SempleBit sempleBit;
        Audio::Channels cannels;
        size_t sempleRate;
        size_t rawSize;
        size_t rawPos;
        float time;
    };

    static InfoSound getInfo(Application::ResouceStream *pResource);
    static Audio::SoundInterface* load(const Utility::Path& path);

};

};

#endif