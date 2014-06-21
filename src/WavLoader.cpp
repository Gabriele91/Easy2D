#include <stdafx.h>
#include <WavLoader.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//load wav

ASPACKED(struct WavHeader
{
    char chunkID[4];
    uint chunkSize;
    char format[4];
});

ASPACKED(struct WavRIFFHeader
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
 * N.B. sizeof(WavHeader)+8+wavRIFFHeader.subchunk1Size
 *      8 is sizeof(char subchunk1ID[4])+sizeof(uint subchunk1Size);
 */
#define str4cmp(s1,s2) (strncmp(s1,s2,4)==0)

WavLoader::InfoSound WavLoader::getInfo(Application::ResouceStream *pResource)
{
    //header
    WavHeader wavHeader;
    WavRIFFHeader wavRIFFHeader;
    RawHeader rawData;
    //read info
    pResource->read(&wavHeader, sizeof(WavHeader), 1);
    pResource->read(&wavRIFFHeader, sizeof(WavRIFFHeader), 1);
    pResource->seek(sizeof(WavHeader)+8+wavRIFFHeader.subchunk1Size, Application::Seek::SET);
    pResource->read(&rawData, sizeof(RawHeader), 1);
    //is a wave, riff, fmt ,pcm file.
    bool isriff=str4cmp(wavHeader.chunkID,"RIFF");
    bool iswave=str4cmp(wavHeader.format,"WAVE");
    bool isfmt=str4cmp(wavRIFFHeader.subchunk1ID,"fmt ");
    bool ispcm=wavRIFFHeader.audioFormat==1;//pcm==1
    DEBUG_ASSERT_MSG(isriff,"OpenALReadWav: file must to be riff header");
    DEBUG_ASSERT_MSG(iswave,"OpenALReadWav: file must to be wave format");
    DEBUG_ASSERT_MSG(isfmt,"OpenALReadWav: file must to be ftm sound data's format");
    DEBUG_ASSERT_MSG(ispcm,"OpenALReadWav: file must to be PCM = 1 (i.e. Linear quantization)");
    //return
    InfoSound infoSound;
    //get cannels
    infoSound.cannels= wavRIFFHeader.numChannels == 1 ?
                       Audio::Channels::MONO : Audio::Channels::STEREO;
    //get bits per semple
    infoSound.sempleBit=  wavRIFFHeader.bitsPerSample == 8 ?
                          Audio::SempleBit::SEMPLE8BIT : Audio::SempleBit::SEMPLE16BIT;
    //get semple rate
    infoSound.sempleRate=wavRIFFHeader.sampleRate;
    //get size raw data
    infoSound.rawSize=rawData.subchunk2Size;
    //get pos raw data
    infoSound.rawPos=sizeof(WavHeader)+8+wavRIFFHeader.subchunk1Size+8;//offsetof(RawHeader, data);
    //calc time
    size_t sizeSemple= infoSound.sempleRate * wavRIFFHeader.numChannels * infoSound.sempleBit;
    infoSound.time= (float)infoSound.rawSize/(float)sizeSemple;

    return infoSound;
}

Audio::SoundInterface* WavLoader::load(const Utility::Path& path)
{
    //get raw file
    void *data=NULL;
    size_t len=0;
    Application::instance()->loadData(path,data,len);
    //headers
    WavHeader *wavHeader;
    WavRIFFHeader *wavRIFFHeader;
    RawHeader *rawData;
    //point to file readed:
    char *cData=(char*)data;
    wavHeader=(WavHeader*)cData;
    wavRIFFHeader=(WavRIFFHeader*)(cData+sizeof(WavHeader));
    rawData=(RawHeader*)(cData+sizeof(WavHeader)+8+wavRIFFHeader->subchunk1Size);
    //is a wave, riff, fmt ,pcm file.
    bool isriff=str4cmp(wavHeader->chunkID,"RIFF");
    bool iswave=str4cmp(wavHeader->format,"WAVE");
    bool isfmt=str4cmp(wavRIFFHeader->subchunk1ID,"fmt ");
    bool ispcm=wavRIFFHeader->audioFormat==1;//pcm==1
    DEBUG_ASSERT_MSG(isriff,"OpenALReadWav: file must to be riff header");
    DEBUG_ASSERT_MSG(iswave,"OpenALReadWav: file must to be wave format");
    DEBUG_ASSERT_MSG(isfmt,"OpenALReadWav: file must to be ftm sound data's format");
    DEBUG_ASSERT_MSG(ispcm,"OpenALReadWav: file must to be PCM = 1 (i.e. Linear quantization)");
    //get format
    Audio::Channels cannels= wavRIFFHeader->numChannels == 1 ?
                             Audio::Channels::MONO : Audio::Channels::STEREO;

    Audio::SempleBit sbits=  wavRIFFHeader->bitsPerSample == 8 ?
                             Audio::SempleBit::SEMPLE8BIT : Audio::SempleBit::SEMPLE16BIT;
    /**
     * return a sound instance
     */
    return
        Application::instance()->getAudio()->createSound(rawData->data,
                rawData->subchunk2Size,
                wavRIFFHeader->sampleRate,
                cannels,
                sbits);
}