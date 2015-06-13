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
			Audio::SempleBit m_semple_bit;
			Audio::Channels m_cannels;
			size_t m_semple_rate;
			size_t m_raw_size;
			size_t m_raw_pos;
			float m_time;
		};

		static InfoSound get_info(Application::ResouceStream *pResource);
		static Audio::SoundBuffer* load(const Utility::Path& path);

	};

};

#endif