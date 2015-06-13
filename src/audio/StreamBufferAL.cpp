#include <stdafx.h>
#include <Math3D.h>
#include <Debug.h>
#include <SoundEmitterAL.h>
#include <StreamBufferAL.h>

///////////////////////
using namespace Easy2D;
///////////////////////

///constructor
StreamBufferAL::StreamBufferAL(Application::ResouceStream* resource,
							   size_t offset,
							   size_t size,
							   size_t sempleRate,
							   size_t sizeSemple,
							   int   format,
							   bool  isBE,
							   float lenSound)
							  :resource(resource)
							  ,offset(offset)
							  ,sizeData(size)
						  	  ,sempleRate(sempleRate)
							  ,sizeSemple(sizeSemple)
							  ,format(format)
							  ,isBE(isBE)
							  ,lenSound(lenSound)
{
}
///destroy sound rerouce
StreamBufferAL::~StreamBufferAL()
{
	if (resource) delete resource;
};
///create a instance
StreamContextAL* StreamBufferAL::newContext(SoundEmitterAL* source)
{
	return new StreamContextAL(source,this);
}
///read value
size_t StreamBufferAL::read(uchar* data, size_t from, size_t size)
{
	//restart stream
	resource->seek(offset + from, Application::Seek::SET);
	//is big ending?
	if (isBE)
	{
		size_t readed = resource->read(data, 1, size);
		size_t size   = readed;
		//BE->LE
		while (size)
		{
			ushort* buffer = (ushort*)data;
			(*buffer) = (((*buffer) >> 8) & 0x00FF) | (((*buffer) << 8) & 0xFF00);
			data += 2;
			size -= 2;
		}
		return readed;
	}
	else
	{
		//load resource
		return resource->read(data, 1, size);
	}
}



//constructor
StreamContextAL::StreamContextAL(SoundEmitterAL* source,
								 StreamBufferAL* sbuffer)
								:source(source)
								,sbuffer(sbuffer)
{
	//create buffers
	alGenBuffers(AL_STREAM_N_BUFFERS, buffers);
	//schedule update
	((AudioAL*)(Application::instance()->get_audio()))->getStreamThreadAL().push(this); 
	//init
	leftRead = 0;
	lastRead = 0;
	looping  = false;
}
StreamContextAL::~StreamContextAL()
{
	if (source->is_play()) source->stop();
	//unschedule
	((AudioAL*)(Application::instance()->get_audio()))->getStreamThreadAL().erase(this);
	//delete buffers
	alDeleteBuffers(AL_STREAM_N_BUFFERS, buffers);
}
//overload play
void StreamContextAL::play()
{
    /////////////////////////////////////////////////////////////////////
    //restat stream
    restart();
    /////////////////////////////////////////////////////////////////////
    //stream file/data
    for(size_t i=0; i!=AL_STREAM_N_BUFFERS; ++i) read(buffers[i]);
    /////////////////////////////////////////////////////////////////////
	alSourceQueueBuffers(source->getSource(), AL_STREAM_N_BUFFERS, buffers);
}
///enable loop
void StreamContextAL::enable_loop()
{
    looping=true;
}
///disable loop
void StreamContextAL::disable_loop()
{
    looping=false;
}
///playback Time
float StreamContextAL::playback_time()
{
    int bytesBufferRead;
    alGetSourcei(source->getSource(), AL_BYTE_OFFSET , &bytesBufferRead);
	float  result = ((float)(Math::min(lastRead + bytesBufferRead, sbuffer->sizeData)) / sbuffer->sizeSemple);
    return result;
}
///is in looping mode!?
bool StreamContextAL::is_loop()
{
	return looping;
}
//restat steam
void StreamContextAL::restart()
{
    leftRead=0;
    lastRead=0;
}
//stream buffer
size_t StreamContextAL::read(ALuint alBuffer)
{
    //buffer
    uchar loadBuffer[AL_STREAM_BUFFER_SIZE];
    //stream file/data
	size_t remaning=Math::min(AL_STREAM_BUFFER_SIZE, size_t(sbuffer->sizeData - leftRead));
	size_t readSize = sbuffer->read(loadBuffer, leftRead, remaning);
    //count a bites read
    leftRead+=readSize;
    //update buffer
	alBufferData(alBuffer, sbuffer->format, loadBuffer, readSize, sbuffer->sempleRate);
    //return size
    return readSize;
}
///update
/**
 * Streaming sound implementation
 */
void StreamContextAL::update()
{
    //
	if ( source->is_pause()) return;
	if (!source->is_play() && !is_loop()) return;
	if (!source->is_play() && is_loop())/* wrong */
    {
		source->play();
    }
    //buffer info
    ALuint alBuffer;
    //loop
    int processed;
	alGetSourcei(source->getSource(), AL_BUFFERS_PROCESSED, &processed);
    //
    while(processed--)
    {
        //get buffer
		alSourceUnqueueBuffers(source->getSource(), 1, &alBuffer);
        //get last read
        ALint sizeReaded=0;
        alGetBufferi(alBuffer, AL_SIZE, &sizeReaded);
        lastRead+=sizeReaded;
        //stream file/data
        read(alBuffer);
        //in loop mode restart stream buffer
        if(sbuffer->sizeData<=leftRead && is_loop()) restart();
        //change buffer
		alSourceQueueBuffers(source->getSource(), 1, &alBuffer);
    }

}