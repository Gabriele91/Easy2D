//
//  CocoaApp.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//
////////////////////////////////////////
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "AppDelegate.h"
////////////////////////////////////////
#include <AppiOS.h>
#include <ScreeniOS.h>
#include <InputiOS.h>
#import <AudioAL.h>
#import <Timer.h>
#import <Audio.h>
//namespace
using namespace Easy2D;
#define delegate ((Easy2DApplication*)(appDelegate))
////////////////////////////////////////
/**
 * cocoa app costructor
 */
AppiOS::AppiOS(const String& name)
{
    //init variable
    appDelegate=nullptr;
    //init components
    screen=new ScreeniOS();
    input=new InputiOS();
    audio=new AudioAL();
    //save appname
    appname  = name;
    //////////////////////////////////////////////////////
    NSArray* nspaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	dataPath = [[nspaths objectAtIndex:0] UTF8String];
    //////////////////////////////////////////////////////
}

/**
 * Set application delegate
 */
void AppiOS::setUIApplicationDelegate(void* appdelg)
{
    //get application delegate
    appDelegate=appdelg;
    //init components
    ((ScreeniOS*)screen)->setEAGLView([delegate glView]);
    ((ScreeniOS*)screen)->setEAGLViewController([delegate glViewController]);
}
/**
 * destroy an window application
 */
AppiOS::~AppiOS()
{
    
    //delete audio
    delete audio;
    audio=NULL;
    
    //delete screen
	delete screen;
	screen=NULL;
    
	//delete input
	//delete input;
	input=NULL;
    
}
/**
 * load a binary file
 * @return succes
 */
bool AppiOS::loadData(const String& path,void*& ptr,size_t &len)
{
	//open
	FILE *pfile=fopen(path,"rb");
	DEBUG_MESSAGE("load file: "<<path);
	DEBUG_ASSERT_MSG(pfile,"error load file: "<<path);
    //get size
    fseek(pfile,0,SEEK_END);
    len=ftell(pfile);
    fseek(pfile,0,SEEK_SET);
    //read
    ptr=malloc(len*sizeof(char)+1);
    fread(ptr,len,1,pfile);
    (*((char*)ptr+len))='\0';
	//close
	fclose(pfile);
	return pfile!=NULL;
}
/**
 * where you can save files data
 * @return path
 */
String AppiOS::appDataDirectory()
{
    return dataPath;
}
/**
 * application root (read only)
 * @return path
 */
String AppiOS::appWorkingDirectory()
{
	char cCurrentPath[PATH_MAX];
	if(!getcwd(cCurrentPath, sizeof(cCurrentPath))){
		DEBUG_MESSAGE("Error get working directory: "<<errno);
	}
	return cCurrentPath;
}
/**
 * resources directory (read only)
 * @return path
 */
String AppiOS::appResourcesDirectory()
{
    return [[[NSBundle mainBundle] resourcePath] UTF8String];
}
/**
 * application exit method
 */
void AppiOS::exit()
{
/* unused ?? */
}
/**
 * application loop
 */
void AppiOS::loop()
{
/* unused */
}
/**
 * execute a instance application
 */
void AppiOS::exec(Game *game)
{
	//save
    mainInstance=game;
    //launch the iphone app
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    //start app
    UIApplicationMain( 0, nullptr, nil,   NSStringFromClass([Easy2DApplication class]) );
    //relase
    [pool release];
}
/**
 * application update
 */
void AppiOS::update(float dt)
{
	mainInstance->run(dt);
}
/**
 * return true if device supports only power of two texture
 */
bool AppiOS::onlyPO2()
{
    return true;
}