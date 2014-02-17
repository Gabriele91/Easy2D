#include <stdafx.h>
#include <Sound.h>
#include <Application.h>
#include <Debug.h>
#include <Table.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Sound::Sound(ResourcesManager<Sound> *rsmr,
             const String& pathfile)
            :Resource(rsmr,pathfile)
            ,pResource(NULL)
            ,offsetStartStream(0)
{
	reloadable=true;
}
///destroy sound rerouce
Sound::~Sound(){
	//release resource
	release();
}

//load wav

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

struct InfoSound{
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
/**
 * Read info from header
 */
InfoSound InfoWav(Application::ResouceStream *pResource){
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
/**
 * Read all sound
 */
static Audio::SoundInterface* soundLoadWav(const Utility::Path& path){
	//get raw file
	void *data=NULL; size_t len=0;
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

//load methods
bool Sound::load(){
    
    bool stream=false;
    String path=getPath();
    
    if(getPath().getExtension()=="e2d"){
        //load file
        void *data=NULL; size_t len=0;
        Application::instance()->loadData(rpath,data,len);
        String filestring((char*)data);
        free(data);
        //deserialize
        Table soundTable;
        soundTable.deserialize(filestring);
        //get path
        path=getPath().getDirectory()+'/'+soundTable.getString("file");
        stream=soundTable.getString("mode","stream")=="stream";
    }
    
    if(!stream){
        //static
        //load sound
        iSound=soundLoadWav(path);
    }
    else{
        //stream
        //open stream
        pResource=Application::instance()->getResouceStream(path);
        //get info
        InfoSound infoSound=InfoWav(pResource);
        //save info
        offsetStartStream=infoSound.rawPos;
        //crate a stream sound
        iSound=Application::instance()->getAudio()->
        createStreamSound(
        [this](uchar* buffer,size_t size) ->size_t {
            //debug
            DEBUG_ASSERT(size);
            DEBUG_ASSERT(buffer);
            //load resource
            return pResource->read(buffer, 1, size);
        },
        [this](){
            //restart stream
            pResource->seek(offsetStartStream, Application::Seek::SET);
        },
        infoSound.rawSize,
        infoSound.sempleRate,
        infoSound.cannels,
        infoSound.sempleBit);
    }
    //is loaded
	loaded=true;
    return true;
}
bool Sound::unload(){
    //unload resounce
    delete iSound;
    //is a stream resource?
    if(pResource){
        delete pResource;
        pResource=NULL;
        offsetStartStream=0;
    }
    //is unloaded
    loaded=false;
    return true;
}