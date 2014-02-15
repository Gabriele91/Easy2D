#include <stdafx.h>
#include <Sound.h>
#include <Application.h>
#include <Debug.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Sound::Sound(ResourcesManager<Sound> *rsmr,
             const String& pathfile)
            :Resource(rsmr,pathfile)
{
	reloadable=true;
}
///destroy sound rerouce
Sound::~Sound(){
	//release resource
	release();
}

//load wav
static Audio::SoundInterface* soundLoadWav(const Utility::Path& path){
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(path,data,len);
    
#define str4cmp(s1,s2) (strncmp(s1,s2,4)==0)
    
	ASPACKED(struct WavHeader{
		char chunkID[4];
		uint chunkSize;
		char format[4];
	});
	
	ASPACKED(struct WavRIFFHeader{
		char subchunk1ID[4];
		uint subchunk1Size;
		ushort audioFormat;
		ushort numChannels;
		uint sampleRate;
		uint byteRate;// SampleRate * NumChannels * BitsPerSample/8
		ushort blockAlign;//  NumChannels * BitsPerSample/8
		ushort bitsPerSample;
	});
    
	ASPACKED(struct RawHeader{
		char subchunk2ID[4];
		uint subchunk2Size;
		char data[1];
	});
	
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


//load methods
bool Sound::load(){
    //load sound
    iSound=soundLoadWav(getPath());
    //is loaded
	loaded=true;
    return true;
}
bool Sound::unload(){
    //unload resounce
    delete iSound;
    //is unloaded
    loaded=false;
    return true;
}