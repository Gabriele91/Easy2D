#include <stdafx.h>
#include <AiffLoader.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
typedef uint ID;
typedef uchar extended;
#define FORM        ((ID)0x4d524f46)      /* "FORM" */
#define AIFF        ((ID)0x46464941)      /* "AIFF" */
#define SSND        ((ID)0x444e5353)      /* "SSND" */
#define COMM        ((ID)0x4d4d4f43)      /* "COMM" */
//32bit
#define SIZE_AiffChunkFORM 12
#define SIZE_AiffChunk     8
typedef int long32;
typedef unsigned int ulong32;

ASPACKED(struct AiffChunkFORM {
	ID		chunkID;
	long32  chunkSize;
	ID      formType;
	char    chunks[];
});
ASPACKED(struct AiffChunk{
	ID	    chunkID;
	long32  chunkSize;
});
ASPACKED(struct AiffChunkSSND {
	ID             chunkID;
	long32         chunkSize;
	ulong32        offset;
	ulong32        blockSize;
	unsigned char  soundData[];
});
ASPACKED(struct AiffChunkCOMM {
	ID             chunkID;
	long32         chunkSize;
	short          numChannels;
	ulong32        numSampleFrames;
	short          sampleSize;
	extended       sanebuf[10];	
});
static uint saneToInt32(extended sanebuf[])
{
	if ((sanebuf[0] & 0x80) || (sanebuf[0] <= 0x3F) || (sanebuf[0] > 0x40)
		|| (sanebuf[0] == 0x40 && sanebuf[1] > 0x1C))
		return 0;

	return ((sanebuf[2] << 23) | (sanebuf[3] << 15) | (sanebuf[4] << 7)
		| (sanebuf[5] >> 1)) >> (29 - sanebuf[1]);
}
static ushort endianUint16Conversion(ushort word)
{
	return ((word >> 8) & 0x00FF) | ((word << 8) & 0xFF00);
}
static uint   endianUint32Conversion(uint dword)
{
	return ((dword >> 24) & 0x000000FF) | ((dword >> 8) & 0x0000FF00) | ((dword << 8) & 0x00FF0000) | ((dword << 24) & 0xFF000000);
}
static void endingConvChunk(AiffChunk& chunk)
{
	chunk.chunkSize = endianUint32Conversion(chunk.chunkSize);
}
static void endingConvSSND(AiffChunkSSND& ssnd)
{
	ssnd.chunkSize = endianUint32Conversion(ssnd.chunkSize);
	ssnd.offset    = endianUint32Conversion(ssnd.offset);
	ssnd.blockSize = endianUint32Conversion(ssnd.blockSize);
}
static void endingConvCOMM(AiffChunkCOMM& comm)
{
	comm.chunkSize       = endianUint32Conversion(comm.chunkSize);
	comm.numChannels     = endianUint16Conversion(comm.numChannels);
	comm.numSampleFrames = endianUint32Conversion(comm.numSampleFrames);
	comm.sampleSize      = endianUint16Conversion(comm.sampleSize);
}
static void endingConvSoundBuff16(uchar* buff,size_t size)
{
	while (size) 
	{
		ushort* buffer = (ushort*)buff;
		(*buffer) = endianUint16Conversion(*buffer);
		buff += 2;
		size -= 2;
	}
}

AiffLoader::InfoSound AiffLoader::getInfo(Application::ResouceStream *pResource)
{
	/* none */
	InfoSound infoSound;
	AiffChunkFORM  form;
	AiffChunkSSND  ssnd;
	AiffChunkCOMM  comm;
    
    pResource->seek(0, Application::Seek::SET);
	pResource->read(&form, SIZE_AiffChunkFORM, 1);
    
	bool   isaiff    = (form.formType == AIFF);
	bool   foundSSND = false;
	bool   foundCOMM = false;
	size_t start = 0;
	//read blocks
	do
	{
		AiffChunk chunk;
		if(!pResource->read(&chunk, SIZE_AiffChunk, 1)) break;
		endingConvChunk(chunk);
		//no size?
		if (!chunk.chunkSize) break;
		//select block
		switch (chunk.chunkID)
		{
		case SSND:
			foundSSND = true;
			if (!pResource->read(&ssnd.offset, sizeof(unsigned long)*2, 1)) break;
			endingConvSSND(ssnd);
			start = pResource->tell();
			break;
		case COMM:
			foundCOMM = true;
			if (!pResource->read(&comm.numChannels, sizeof(AiffChunkCOMM) - sizeof(AiffChunk), 1)) 
				break;
			endingConvCOMM(comm);
			break;
		default: break;
		}
	} 
	while (isaiff && (!foundSSND || !foundCOMM));
	DEBUG_ASSERT_MSG(isaiff, "OpenALReadAiff: not have Aiff header");
	DEBUG_ASSERT_MSG(foundCOMM, "OpenALReadAiff: not have COMM header");
	DEBUG_ASSERT_MSG(foundSSND, "OpenALReadAiff: not have SSND header");
	//num of channels
	Audio::Channels channels;
	if (comm.numChannels == 1) channels = Audio::Channels::MONO;
	else if (comm.numChannels == 2) channels = Audio::Channels::STEREO;
	DEBUG_CODE(else{
		DEBUG_ASSERT_MSG(true, "OpenALReadAiff:number of channels not valid");
	});
	Audio::SempleBit sempleBit;
	if (comm.sampleSize == 8) sempleBit = Audio::SempleBit::SEMPLE8BIT;
	else if (comm.sampleSize == 16) sempleBit = Audio::SempleBit::SEMPLE16BEBIT;
	DEBUG_CODE(else{
		DEBUG_ASSERT_MSG(true, "OpenALReadAiff: number of bit not valid");
	});
	//calc len of file audio
	size_t audioLen = comm.numChannels * comm.numSampleFrames * (comm.sampleSize / 8);
	uint frequency  = saneToInt32(comm.sanebuf);
	//set info
	infoSound.cannels    = channels;
	infoSound.rawPos     = start;
	infoSound.rawSize    = audioLen;
	infoSound.sempleBit  = sempleBit;
	infoSound.sempleRate = frequency;
	//calc time
	size_t sempleSize = frequency * comm.numChannels * (comm.sampleSize / 8);
	infoSound.time    = (float)audioLen / (float)sempleSize;
	//return
	return infoSound;
}
Audio::SoundBuffer* AiffLoader::load(const Utility::Path& path)
{
	//get raw file
	void *data = NULL;
	size_t len = 0;
	Application::instance()->loadData(path, data, len);
	//headers
	AiffChunkFORM*   form = nullptr;
	AiffChunkSSND*   ssnd = nullptr;
	AiffChunkCOMM*   comm = nullptr;
	//bools
	//init
	form = (AiffChunkFORM*)data;
	bool  isaiff = form->formType == AIFF;
	bool  foundSSND = false;
	bool  foundCOMM = false;
	uchar *current = (uchar*)form->chunks;
	do
	{
		AiffChunk* chunk = (AiffChunk*)(current);
		endingConvChunk(*chunk);
		//no size?
		if (!chunk->chunkSize) break;
		//next block
		current += chunk->chunkSize + SIZE_AiffChunk;
		//select block
		switch (chunk->chunkID)
		{
		case SSND:
			foundSSND = true;
			ssnd = (AiffChunkSSND*)chunk;
			endingConvSSND(*ssnd);
		break;
		case COMM:
			foundCOMM = true;
			comm = (AiffChunkCOMM*)chunk;
			endingConvCOMM(*comm);
		break;
		default: break;
		}
	}
	while (isaiff && (!foundSSND || !foundCOMM));
	DEBUG_ASSERT_MSG(isaiff, "OpenALReadAiff: not have Aiff header");
	DEBUG_ASSERT_MSG(foundCOMM, "OpenALReadAiff: not have COMM header");
	DEBUG_ASSERT_MSG(foundSSND, "OpenALReadAiff: not have SSND header");
	//num of channels
	Audio::Channels channels;
	if      (comm->numChannels == 1) channels = Audio::Channels::MONO;
	else if (comm->numChannels == 2) channels = Audio::Channels::STEREO;
	DEBUG_CODE(else{
		DEBUG_ASSERT_MSG(true, "OpenALReadAiff:number of channels not valid");
	});
	Audio::SempleBit sempleBit;
	if      (comm->sampleSize == 8 ) sempleBit = Audio::SempleBit::SEMPLE8BIT;
	else if (comm->sampleSize == 16) sempleBit = Audio::SempleBit::SEMPLE16BIT;
	DEBUG_CODE(else{
		DEBUG_ASSERT_MSG(true, "OpenALReadAiff: number of bit not valid");
	});
	//calc len of file audio
	size_t audioLen = comm->numChannels * comm->numSampleFrames * (comm->sampleSize / 8);
	uint frequency  = saneToInt32(comm->sanebuf);
	uchar* start    = ssnd->soundData + ssnd->offset ;
	//BE->LE
	if (comm->sampleSize == 16)
		endingConvSoundBuff16(start, audioLen);
	/**
	* return a sound instance
	*/
	auto e2dbuffer = Application::instance()->getAudio()->createBuffer(start,
																	   audioLen,
																	   frequency,
																	   channels,
																	   sempleBit);
	//dealloc
	free(data); data = nullptr;
	//return
	return e2dbuffer;
}
