#ifndef STATICBUFFERAL_H
#define STATICBUFFERAL_H

#include <Config.h>
#include <AudioAL.h>

namespace Easy2D
{


class StaticBufferAL : public Audio::SoundBuffer
{
protected:

    //buffer file
	ALuint buffer;
	//format buffer
	int format;
	//buffer len in second
	float lenSound;


public:
    ///constructor
	StaticBufferAL(ALuint buffer,
                   int format,
                   float lenSound);
    ///destroy sound rerouce
	virtual ~StaticBufferAL();
	///return type of buffer
	virtual Audio::BufferType getType()
	{
		return Audio::STATIC;
	};
	///get format buffer
	virtual int getFormat()
	{
		return format;
	}
	///get sound length
	virtual float getLength()
	{
		return lenSound;
	}
	///attach a source
	void attach(ALint source);
};

};
#endif