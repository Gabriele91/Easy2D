
#include "stdafx.h"
#include <Timer.h>
#include <AndroidApp.h>
#include <AndroidScreen.h>
#include <AndroidInput.h>
#include <AndroidMain.h>
#include <Audio.h>
#include <AudioAL.h>
#include <Debug.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
///////////////////////
using namespace Easy2D;

void AndroidApp::__reloadInstance(){	
		//CAST
		AndroidScreen* screen=((AndroidScreen*)(this->screen));
		//create surface
		screen->__createSurface();
		//is a valid gl es context?
		if(!screen->__isAValidContext()){
			//create opengl es context
			screen->__createContext();
			//init opengl
			screen->acquireContext();
			screen->__initStateOpenGLES();
			//reload gpu assets
			for(auto& rsg:this->groups)
				rsg.second->reloadGpuResouce();
		}
		//enable flip screen
		this->dodraw=true;
}

AndroidApp::AndroidApp(const String& appname)
		   :Application(){
	screen=(Screen*)new AndroidScreen();
	input=(Input*)new AndroidInput();
	audio=(Audio*)new AudioAL();//use OpenAL // todo use OpenSL ES
	//set android userdata
	setAndroidUserData((void*)this);

	//overload pause
	onConfigChange([](void *data){
		AndroidApp *self=((AndroidApp*)data);
		AndroidScreen* screen=((AndroidScreen*)(self->screen));
		//reset screen size
		screen->__isResized();
		screen->__disableContext();
		screen->__deleteSurface();
		//create surface
		self->__reloadInstance();
	});
	//overload pause
	onSaveStateAndroid([](void *data){
		AndroidApp *self=((AndroidApp*)data);
		//disable flip surface
		self->dodraw=false;
	});
	//set reload gpu data when is resume
	onInitAndroid([](void *data){
		AndroidApp *self=((AndroidApp*)data);
		//create surface
		self->__reloadInstance();
		//enable flip screen
		self->dodraw=true;
	});
	//not exit form loop
	doexit=false;
	//draw on surface
	dodraw=true;
}

AndroidApp::~AndroidApp(){
	//delete audio
	delete audio;
	audio=NULL;
	//delete screen
	delete screen;
	screen=NULL;
	//delete input
	delete input;
	input=NULL;
}

bool AndroidApp::loadData(const String& path,void*& ptr,size_t &len){
	//open asset
	AAsset *asset= AAssetManager_open(getAndroidApp()->activity->assetManager, 
									  path, 
									  AASSET_MODE_BUFFER);

	DEBUG_MESSAGE("load asset: "<<path);
	DEBUG_ASSERT_MSG(asset,"error load asset: "<<path);
	//get size
	len = AAsset_getLength(asset);
	//read
	ptr=malloc(len*sizeof(char)+1);
	AAsset_read(asset,ptr,len);
	(*((char*)ptr+len))='\0';
	//close asset
	AAsset_close(asset);
	//return is read
	return len!=0;
}

/**
* use asset manager for resource stream
*/
class AResouceStream : public Application::ResouceStream {

    AAsset *asset;
    
public:
    
    AResouceStream(const String& path){
        /**
         * Open file
         */
        asset=AAssetManager_open(getAndroidApp()->activity->assetManager, 
        						 path,
        						 AASSET_MODE_STREAMING);
        DEBUG_MESSAGE("open stream file: "<<path);
        DEBUG_ASSERT_MSG(asset,"error open stream file: "<<path);
    }
    
    virtual ~AResouceStream(){
        if(asset)
            close();
    }
    ///close file
    virtual void close(){
        AAsset_close(asset);
        asset=NULL;
    }
    ///read from file
    virtual size_t read(void * ptr, size_t size, size_t count){
        AAsset_read(asset,ptr,size*count);
    }
    ///seek from file
    virtual size_t seek (size_t offset, Application::Seek origin ){
        int cseek=0;
        switch (origin){
            case Application::Seek::CUR: cseek=SEEK_CUR; break;
            case Application::Seek::SET: cseek=SEEK_SET; break;
            case Application::Seek::END: cseek=SEEK_END; break;
            default:  break;
        }
        AAsset_seek (asset, offset, cseek);
    }
    ///returns the current value of the position indicator of the stream
    virtual size_t tell(){
        return size()-AAsset_getRemainingLength(asset);
    }
    ///get file size
    virtual size_t size(){
        return AAsset_getLength(asset);
    }
    ///return a uchar cast in int
    virtual int getc(){
        char c;
        read(&c, 1, 1);
        return c;
    }
    ///rewind from file
    virtual void rewind (){
        seek(0,Application::Seek::SET);
    }

};

Application::ResouceStream* AndroidApp::getResouceStream(const String& path){
    return new AResouceStream(path);
}

String AndroidApp::appDataDirectory(){
	return getAndroidApp()->activity->internalDataPath;
}

String AndroidApp::appWorkingDirectory(){
	return getAndroidApk();
}

String AndroidApp::appResourcesDirectory(){
	return appWorkingDirectory()+String("/assets");
}

void AndroidApp::exit(){
	doexit=true;
}

void AndroidApp::loop(){
	//
	Timer timer;
	double msToSleep=1000.0/(static_cast<double>(screen->getFrameRate()));
	double millipass=0;
	double dt=0;
	double sleepTime=0;
	//start timer
	timer.start();
	//set current context
	screen->acquireContext();
	//draw loop
	while( !input->getClose() && !doexit ) {
		//get timer values
		millipass=timer.getGetCounter();
		//calc dt and sleep time
		sleepTime=msToSleep-millipass;
		while(sleepTime>0 && sleepTime<60000 ){
		    //scheduler linux is faster then window
			usleep ((sleepTime>9?1:0)*1000);
			millipass=timer.getGetCounter();
			sleepTime=msToSleep-millipass;
		}
		//calc dt
		dt=millipass/1000.0;
		timer.reset();
        //save dt
        lastDeltaTime=dt;
		//update
		update(dt);
		//update opengl		
		if(dodraw) screen->swap();
    }
}

void AndroidApp::exec(Game *ptrGame){
	mainInstance=ptrGame;
	mainInstance->start();
	loop();
	mainInstance->end();
}

void AndroidApp::update(float dt){
	input->update();
	mainInstance->run(dt);
}

bool AndroidApp::onlyPO2(){
	return true;
}
