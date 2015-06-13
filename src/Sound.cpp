#include <stdafx.h>
#include <Sound.h>
#include <Application.h>
#include <WavLoader.h>
#include <AiffLoader.h>
#include <Debug.h>
#include <Table.h>

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
Sound::~Sound()
{
    //release resource
    release();
}


//load methods
bool Sound::load()
{

    bool stream=false;
    Utility::Path path=getPath();

    if(getPath().getExtension()=="e2d")
    {
        //load file
        void *data=NULL;
        size_t len=0;
        Application::instance()->load_data(rpath,data,len);
        String filestring((char*)data);
        free(data);
        //deserialize
        Table soundTable;
        soundTable.deserialize(filestring);
        //get path
        path=getPath().getDirectory()+'/'+soundTable.getString("file");
        stream=soundTable.getString("mode","stream")=="stream";
    }

    if(!stream)
    {
        //static
        //load sound
		if (path.getExtension() == "aiff")
		{
			sbuffer = AiffLoader::load(path);
		}
		else if (path.getExtension() == "wav")
		{
			sbuffer = WavLoader::load(path);
		}
		DEBUG_CODE(else
		{
			DEBUG_ASSERT_MSG(0, "Sound format not supported");
		});
    }
    else
    {
        //stream
        //open stream
		auto pResource = Application::instance()->get_resouce_stream(path);
		//stream
		//load sound
		if (path.getExtension() == "aiff")
		{
			//get info
			AiffLoader::InfoSound infoSound = AiffLoader::get_info(pResource);
			//crate a stream sound
			sbuffer = Application::instance()->get_audio()->createStreamBuffer(pResource,
																			  infoSound.m_raw_pos,
																			  infoSound.m_raw_size,
																			  infoSound.m_semple_rate,
																			  infoSound.m_cannels,
																			  infoSound.m_semple_bit);
		}
		else if (path.getExtension() == "wav")
		{
			//get info
			WavLoader::InfoSound infoSound = WavLoader::getInfo(pResource);
			//crate a stream sound
			sbuffer = Application::instance()->get_audio()->createStreamBuffer(pResource,
																			  infoSound.rawPos,
																			  infoSound.rawSize,
																			  infoSound.sempleRate,
																			  infoSound.cannels,
																			  infoSound.sempleBit);
		}
		DEBUG_CODE(else
		{
			DEBUG_ASSERT_MSG(0, "Sound format not supported");
		});
    }
    //is loaded
    loaded=true;
    return true;
}
bool Sound::unload()
{
    //unload resounce
	delete sbuffer;
    //is unloaded
    loaded=false;
    return true;
}