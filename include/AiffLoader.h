#ifndef AIFFLOADER_H
#define AIFFLOADER_H

#include <Config.h>
#include <Audio.h>
#include <Utility.h>
#include <Application.h>

namespace Easy2D
{

	class AiffLoader
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
		static Audio::SoundBuffer* load(const Utility::Path& path);

	};

};

#endif