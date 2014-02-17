#ifndef AUDIO_H
#define AUDIO_H

#include <Config.h>

namespace Easy2D {

	class Audio {

		
	public:

		enum Channels{
			MONO=1,
			STEREO=2
		};
		enum SempleBit{
			SEMPLE8BIT=1,
			SEMPLE16BIT=2
		};
	
		class SoundInterface{
		protected:
			
			friend class Audio;
			virtual void __volumeFromManager(float volume)=0;

		public:
			///destroy sound rerouce
			virtual ~SoundInterface(){};
			///enable loop
			virtual void enableLoop()=0;
			///disable loop
			virtual void disableLoop()=0;
			///play sound
			virtual void play()=0;
			///play sound in loop mode
			virtual void loop()=0;
			///stop sound
			virtual void stop()=0;
			//set dound in pause
			virtual void pause()=0;
			//set volume (0...1)
			virtual void volume(float volume)=0;
			//get volume
			virtual float volume()=0;
			virtual float realVolume()=0;
			//remaining time
			virtual float remainingTime()=0;
			//playback time
			virtual float playbackTime()=0;
			//global time duration
			virtual float duration()=0;
			//get sound states
			virtual bool isPause()=0; 
			virtual bool isPlay()=0;
			virtual bool isStop()=0; 
			virtual bool isLoop()=0;
		};

		/**
		* init audio engine
		*/
		Audio();
		/**
		* dealloc audio engine
		*/
		virtual ~Audio();
		/**
		* subscription of the sound
		*/
		virtual void subscriptionSound(SoundInterface *sound);
		/**
		* unsubscription of the sound
		*/
		virtual void unsubscriptionSound(SoundInterface *sound);
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
		* change volume to all sounds
		*/
		virtual void setVolume(float volume);
		/**
		* change volume to all sounds (in pecentual)
		*/
		virtual void changeVolume(float volume);
		/**
		* for each sounds
		*/
		virtual void foreachSounds(DFUNCTION<void(SoundInterface*)> callback);

		
	protected:

		///a generic context (for default audio device)
		void *context;
		///sound subscription map
		DUNORDERED_MAP<SoundInterface*,SoundInterface*> soundmap;

	};

};

#endif