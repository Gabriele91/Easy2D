//
//  CocoaApp.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#include <CocoaApp.h>
#include <CocoaScreen.h>
#include <CocoaInput.h>
#import <Cocoa/Cocoa.h>
#import <Timer.h>
#import <Audio.h>
//namespace
using namespace Easy2D;

////////////////////////////////////////
#define COCOAAPP NSApplication *applicationObject = (NSApplication*)cocoaApp;
/**
 * cocoa application class
 */
@interface Easy2DApplication : NSApplication
{
    
}

- (void)run;
- (void)terminate:(id)sender;

@end

@implementation Easy2DApplication


- (void)run
{
    //init loop cocoa
	[[NSNotificationCenter defaultCenter] postNotificationName:NSApplicationWillFinishLaunchingNotification object:NSApp];
	[[NSNotificationCenter defaultCenter] postNotificationName:NSApplicationDidFinishLaunchingNotification  object:NSApp];
    //is init    
    [self finishLaunching];
    //loop
    Application::instance()->loop();
	
}

- (void)terminate:(id)sender
{
	Application::instance()->exit();
}

@end
////////////////////////////////////////

////////////////////////////////////////
@interface Easy2DAppDelegate : NSObject
@end

@implementation Easy2DAppDelegate
//Set the working directory to the .app's parent directory
- (void) setupWorkingDirectory:(BOOL)shouldChdir
{
    if (shouldChdir)
    {
        char parentdir[MAXPATHLEN];
        CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
        CFURLRef url2 = CFURLCreateCopyDeletingLastPathComponent(0, url);
        if (CFURLGetFileSystemRepresentation(url2, 1, (UInt8 *)parentdir, MAXPATHLEN)) {
            chdir(parentdir);   // chdir to the binary app's parent 
        }
        CFRelease(url);
        CFRelease(url2);
    }
 }
@end
////////////////////////////////////////

/**
 * cocoa app costructor
 */
CocoaApp::CocoaApp(const String& name){
    
    NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];
    
    //set Current Process
    ProcessSerialNumber psn;    
	if (!GetCurrentProcess(&psn)) {
		TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		SetFrontProcess(&psn);
	}
    
    //create app instance 
    NSApplication *applicationObject = [Easy2DApplication sharedApplication];
    cocoaApp=(void*)applicationObject;
    
    //app delegate
    Easy2DAppDelegate* appDelegate=[[Easy2DAppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
        
    //free memory
    [pool release];
    
    
    //create component
    screen=(Screen*)new CocoaScreen();
    input=(Input*)new CocoaInput();
    audio=new Audio(); //default OpenAL
    //add listener event
    ((CocoaScreen*)screen)->onCocoaWindowCreated=[](void *nswindow){
        CocoaInput* input= (CocoaInput*)Application::instance()->getInput();
        input->__addCocoaListener(nswindow);
    };
    //delete listener event
    ((CocoaScreen*)screen)->onCocoaWindowClose=[](void *nswindow){
        CocoaInput* input= (CocoaInput*)Application::instance()->getInput();
        input->__closeCocoaListener();
    };
    
	//not exit form loop
	doexit=false;
    //save appname
    appname  = name;
    //////////////////////////////////////////////////////
    //create path
    NSArray* nspaths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	dataPath = [[nspaths objectAtIndex:0] UTF8String];
    dataPath+= "/" + appname;
    //create directory
    NSString *filePath=@(dataPath.c_str());
    [[NSFileManager defaultManager] createDirectoryAtPath:filePath withIntermediateDirectories:YES attributes:nil error:nil];
    [filePath release];
    //////////////////////////////////////////////////////
}

/**
 * destroy an window application
 */
CocoaApp::~CocoaApp(){
    //gc
    NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];
    
	//delete screen
	delete screen;
	screen=NULL;
    
	//delete input
	delete input;
	input=NULL;
    
    //delete app instance    
    COCOAAPP
    [[NSApp delegate] release];
    [applicationObject release];
    
    //free memory
    [pool release];
    
}
/**
 * load a binary file
 * @return succes
 */
bool CocoaApp::loadData(const String& path,void*& ptr,size_t &len){    
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
String CocoaApp::appDataDirectory(){
    return dataPath;
}
/**
 * application root (read only)
 * @return path
 */
String CocoaApp::appWorkingDirectory(){
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
String CocoaApp::appResourcesDirectory(){
    return [[[NSBundle mainBundle] resourcePath] UTF8String];
}
/**
 * application exit method
 */
void CocoaApp::exit(){
    doexit=true;
}
/**
 * application loop
 */
void CocoaApp::loop(){
    
    COCOAAPP

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
		screen->swap();
    }
}
/**
 * execute a instance application
 */
void CocoaApp::exec(Game *game){
	mainInstance=game;
    
	mainInstance->start();
    
    COCOAAPP
    
	if ([applicationObject respondsToSelector:@selector(run)])
	{
		[applicationObject  performSelectorOnMainThread:@selector(run)
                            withObject:nil
                            waitUntilDone:YES];
	}
    
    
	mainInstance->end();
}
/**
 * application update
 */
void CocoaApp::update(float dt){
	input->update();
	mainInstance->run(dt);
}
/**
 * return true if device supports only power of two texture
 */
bool CocoaApp::onlyPO2(){
    return false;
}