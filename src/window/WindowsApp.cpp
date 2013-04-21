#include "stdafx.h"
#include <Timer.h>
#include <WindowsApp.h>
#include <WindowsScreen.h>
#include <WindowsInput.h>
#include <Debug.h>
//window
#include <direct.h>
//
///////////////////////
using namespace Easy2D;

WindowsApp::WindowsApp()
		   :Application(){
	screen=(Screen*)new WindowsScreen();
	input=(Input*)new WindowsInput();
	//do loop
	doexit=false;
}

WindowsApp::~WindowsApp(){
	//delete screen
	delete screen;
	screen=NULL;
	//delete input
	delete input;
	input=NULL;
}

bool WindowsApp::loadData(const String& path,void*& ptr,size_t &len){
	//open
	FILE *pfile=fopen(path,"rb");
	DEBUG_ASSERT_MSG(pfile,"load file: "<<path);
		//get size
		fseek(pfile,0,SEEK_END);
		len=ftell(pfile);
		fseek(pfile,0,SEEK_SET);
		//read
		ptr=malloc(len*sizeof(char));
		fread(ptr,len,1,pfile);
	//close
	fclose(pfile);
	return pfile!=NULL;
}

String WindowsApp::appDataDirectory(){
	return "";
}

String WindowsApp::appWorkingDirectory(){
	char cCurrentPath[MAX_PATH];
	if(!_getcwd(cCurrentPath, sizeof(cCurrentPath))){
		DEBUG_MESSAGE("Error get working directory: "<<errno);
	}
	return cCurrentPath;
}

String WindowsApp::appResourcesDirectory(){
	return appWorkingDirectory();
}	

void WindowsApp::exit(){
	PostQuitMessage(0);
	doexit=true;
}

void WindowsApp::loop(){	
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
			Sleep(sleepTime>10?1:0);
			millipass=timer.getGetCounter();
			sleepTime=msToSleep-millipass;
		}
		//calc dt
		dt=millipass/1000.0;
		timer.reset();
		//update
		update(dt);	
		//update opengl
		screen->swap();
    }
}

void WindowsApp::exec(Game *prgame){
	game=prgame;
	game->start();
	loop();
	game->end();
}

void WindowsApp::update(float dt){
	input->update();
	game->run(dt);
}

bool WindowsApp::onlyPO2(){
	return true;
}