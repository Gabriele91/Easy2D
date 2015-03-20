#include <stdafx.h>
#include <StaticBufferAL.h>

///////////////////////
using namespace Easy2D;
///////////////////////


///constructor
StaticBufferAL::StaticBufferAL(ALuint buffer,
                               int format,
                               float lenSound)
							 :buffer(buffer)
							 ,format(format)
							 ,lenSound(lenSound)
{
}
///attach a source
void StaticBufferAL::attach(ALint source)
{
	alSourcei(source, AL_BUFFER, buffer);
}
///destroy sound rerouce
StaticBufferAL::~StaticBufferAL()
{
    alDeleteBuffers(1,&buffer);
};