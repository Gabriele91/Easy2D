#ifndef STREAMBUFFERAL_H
#define STREAMBUFFERAL_H

#include <Config.h>
#include <Application.h>
#include <AudioAL.h>
#include <StreamThreadAL.h>

namespace Easy2D
{

class StreamBufferAL;
class StreamContextAL;
class SoundEmitterAL;

#define AL_STREAM_N_BUFFERS 2
#define AL_STREAM_BUFFER_SIZE  (size_t)(4410*32*2)

class StreamBufferAL : public Audio::SoundBuffer
{   

	friend class StreamContextAL;
	//resource stream
	Application::ResouceStream *resource;
	//info
	size_t offset;
	size_t sizeData;
	size_t sempleRate;
	size_t sizeSemple;
	bool looping;
	//format buffer
	int format;
	bool isBE;
	//buffer len in second
	float lenSound;

public:
	///constructor
	StreamBufferAL(Application::ResouceStream* resource,
				   size_t offset,
				   size_t size,
				   size_t sempleRate,
				   size_t sizeSemple,
				   int   format,
				   bool  isBE,
				   float lenSound);
	///destroy sound rerouce
	virtual~StreamBufferAL();
	///return type of buffer
	virtual Audio::BufferType get_type()
	{
		return Audio::STREAM;
	};
	///get format buffer
	virtual int get_format()
	{
		return format;
	}
	///get sound length
	virtual float get_length()
	{
		return lenSound;
	}
	///create a instance
	StreamContextAL* newContext(SoundEmitterAL* source);
	///read value
	size_t read(uchar* data, size_t from, size_t size);
};

class StreamContextAL : public StreamTask
{

protected:

	friend class StreamBufferAL;
    //stream thread
    StreamThreadAL* streamThread;
    //buffers
    ALuint buffers[AL_STREAM_N_BUFFERS];
	//stream buffer
	SoundEmitterAL* source;
	StreamBufferAL* sbuffer;
	//buffer info
	size_t leftRead;
	size_t lastRead;
	bool   looping;
	//constructor
	StreamContextAL(SoundEmitterAL* source, StreamBufferAL* sbuffer); 

public:
	///destroy sound context
	virtual ~StreamContextAL();
	///play
    void play();
    ///enable loop
    void enable_loop();
    ///disable loop
    void disable_loop();
    ///playback Time
    float playback_time();
    ///global time duration
    float duration();
    ///is in looping mode!?
    bool is_loop();
	//restat steam
    void restart();
    //stream buffer
    size_t read(ALuint alBuffer);
    ///update
    void update();
};

};

#endif