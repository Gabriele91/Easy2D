#include <stdafx.h>
#include <WavLoader.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//load wav

ASPACKED(struct RIFFHeader
{
    char chunkID[4];
    uint chunkSize;
    char format[4];
});

ASPACKED(struct FMTHeader
{
    char subchunk1ID[4];
    uint subchunk1Size;
    ushort audioFormat;
    ushort numChannels;
    uint sampleRate;
    uint byteRate;// SampleRate * NumChannels * BitsPerSample/8
    ushort blockAlign;//  NumChannels * BitsPerSample/8
    ushort bitsPerSample;
});

ASPACKED(struct RawHeader
{
    char subchunk2ID[4];
    uint subchunk2Size;
    char data[1];
});

struct InfoSound
{
    Audio::SempleBit sempleBit;
    Audio::Channels cannels;
    size_t sempleRate;
    size_t rawSize;
    size_t rawPos;
    float time;
};

/*
 * N.B. sizeof(RIFFHeader)+8+FMTHeader.subchunk1Size
 *      8 is sizeof(char subchunk1ID[4])+sizeof(uint subchunk1Size);
 */
#define str4cmp(s1,s2) (strncmp(s1,s2,4)==0)

WavLoader::InfoSound WavLoader::getInfo(Application::ResouceStream *pResource)
{
    //header
	RIFFHeader riffHeader;
    FMTHeader  fmtHeader;
    RawHeader  rawData;
    //read info
	pResource->read(&riffHeader, sizeof(RIFFHeader), 1);
	pResource->read(&fmtHeader, sizeof(FMTHeader), 1);
	pResource->seek(sizeof(RIFFHeader) + 8 + fmtHeader.subchunk1Size, Application::Seek::SET);
    pResource->read(&rawData, sizeof(RawHeader), 1);
    //is a wave, riff, fmt ,pcm file.
	bool isriff = str4cmp(riffHeader.chunkID, "RIFF");
	bool iswave = str4cmp(riffHeader.format, "WAVE");
	bool isfmt  = str4cmp(fmtHeader.subchunk1ID, "fmt ");
	bool ispcm = fmtHeader.audioFormat == 1;//pcm==1
    DEBUG_ASSERT_MSG(isriff,"OpenALReadWav: file must to be riff header");
    DEBUG_ASSERT_MSG(iswave,"OpenALReadWav: file must to be wave format");
    DEBUG_ASSERT_MSG(isfmt,"OpenALReadWav: file must to be ftm sound data's format");
    DEBUG_ASSERT_MSG(ispcm,"OpenALReadWav: file must to be PCM = 1 (i.e. Linear quantization)");
    //return
    InfoSound infoSound;
    //get cannels
	infoSound.cannels = fmtHeader.numChannels == 1 ?
                        Audio::Channels::MONO : Audio::Channels::STEREO;
    //get bits per semple
	infoSound.sempleBit  = fmtHeader.bitsPerSample == 8 ?
                           Audio::SempleBit::SEMPLE8BIT : Audio::SempleBit::SEMPLE16BIT;
    //get semple rate
	infoSound.sempleRate = fmtHeader.sampleRate;
    //get size raw data
    infoSound.rawSize=rawData.subchunk2Size;
    //get pos raw data
	infoSound.rawPos = sizeof(RIFFHeader) + 8 + fmtHeader.subchunk1Size + 8;//offsetof(RawHeader, data);
    //calc time
	size_t sizeSemple = infoSound.sempleRate * fmtHeader.numChannels * infoSound.sempleBit;
    infoSound.time= (float)infoSound.rawSize/(float)sizeSemple;

    return infoSound;
}

Audio::SoundBuffer* WavLoader::load(const Utility::Path& path)
{
    //get raw file
    void *data=NULL;
    size_t len=0;
    Application::instance()->load_data(path,data,len);
    //headers
	RIFFHeader *riffHeader;
	FMTHeader *fmtHeader;
    RawHeader *rawData;
    //point to file readed:
    char *cData=(char*)data;
	riffHeader = (RIFFHeader*)cData;
	fmtHeader = (FMTHeader*)(cData + sizeof(RIFFHeader));
	rawData = (RawHeader*)(cData + sizeof(RIFFHeader) + 8 + fmtHeader->subchunk1Size);
    //is a wave, riff, fmt ,pcm file.
	bool isriff = str4cmp(riffHeader->chunkID, "RIFF");
	bool iswave = str4cmp(riffHeader->format, "WAVE");
	bool isfmt  = str4cmp(fmtHeader->subchunk1ID, "fmt ");
	bool ispcm = fmtHeader->audioFormat == 1;//pcm==1
    DEBUG_ASSERT_MSG(isriff,"OpenALReadWav: file must to be riff header");
    DEBUG_ASSERT_MSG(iswave,"OpenALReadWav: file must to be wave format");
    DEBUG_ASSERT_MSG(isfmt,"OpenALReadWav: file must to be ftm sound data's format");
    DEBUG_ASSERT_MSG(ispcm,"OpenALReadWav: file must to be PCM = 1 (i.e. Linear quantization)");
    //get format
	Audio::Channels cannels = fmtHeader->numChannels == 1 ?
                              Audio::Channels::MONO : Audio::Channels::STEREO;

	Audio::SempleBit sbits = fmtHeader->bitsPerSample == 8 ?
                             Audio::SempleBit::SEMPLE8BIT : Audio::SempleBit::SEMPLE16BIT;
    /**
     * return a sound instance
     */
	auto e2dbuffer = Application::instance()->get_audio()->createBuffer(rawData->data,
																	   rawData->subchunk2Size,
																	   fmtHeader->sampleRate,
																	   cannels,
																	   sbits);
	//dealloc
	free(data); data = nullptr;
	//return
	return e2dbuffer;
}