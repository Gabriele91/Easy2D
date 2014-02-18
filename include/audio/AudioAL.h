#ifndef AUDIOAL_H
#define AUDIOAL_H

#include <Config.h>
#include <Audio.h>
#include <StreamThreadAL.h>

namespace Easy2D {

	class AudioAL : public Audio {
    protected:
        
        ALCdevice *device;
        ALCcontext *context;
        float globalVolume;
        StreamThreadAL streamThread;

	public:

		/**
         * init audio engine
         */
		AudioAL();
		/**
         * dealloc audio engine
         */
		virtual ~AudioAL();
		/**
         * create a sound buffer
         */
		virtual SoundInterface* createSound(void *raw,
											size_t size,
											size_t sempleRate,
											Channels channels,
											SempleBit sempleBit);
		/**
         * create a stream sound buffer
         */
		virtual SoundInterface* createStreamSound(DFUNCTION<size_t(uchar* buffer,size_t sizeBuffer)> readStream,
                                                  DFUNCTION<void(void)> restartStream,
                                                  size_t size,
                                                  size_t sempleRate,
                                                  Channels channels,
                                                  SempleBit sempleBit);

		/**
         * change volume to all sounds (in pecentual)
         */
		virtual void changeVolume(float volume);
        
	};

};

#endif