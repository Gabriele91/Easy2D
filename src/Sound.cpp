#include <stdafx.h>
#include <Sound.h>
#include <Application.h>
#include <WavLoader.h>
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
        iSound=WavLoader::load(path);
    }
    else{
        //stream
        //open stream
        pResource=Application::instance()->getResouceStream(path);
        //get info
        WavLoader::InfoSound infoSound=WavLoader::getInfo(pResource);
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