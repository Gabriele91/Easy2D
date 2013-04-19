#include "stdafx.h"
#include <Timer.h>
#include <LinuxApp.h>
#include <LinuxScreen.h>
#include <LinuxInput.h>
#include <Debug.h>
#include <sys/time.h>
///////////////////////
using namespace Easy2D;

LinuxApp::LinuxApp()
		   :Application(){
	screen=(Screen*)new LinuxScreen();
	input=(Input*)new LinuxInput();
	//not exit form loop
	doexit=false;
}

LinuxApp::~LinuxApp(){
	//delete screen
	delete screen;
	screen=NULL;
	//delete input
	delete input;
	input=NULL;
}

bool LinuxApp::loadData(const String& path,void*& ptr,size_t &len){
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

String LinuxApp::appDataDirectory(){
	return "";
}

String LinuxApp::appWorkingDirectory(){
	char cCurrentPath[PATH_MAX];
	if(!getcwd(cCurrentPath, sizeof(cCurrentPath))){
		DEBUG_MESSAGE("Error get working directory: "<<errno);
	}
	return cCurrentPath;
}

String LinuxApp::appResourcesDirectory(){
	return appWorkingDirectory();
}

void LinuxApp::exit(){
	doexit=true;
}

void LinuxApp::loop(){
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
		//update
		update(dt);
		//update opengl
		screen->swap();
    }
}

void LinuxApp::exec(Game *ptrGame){
	game=ptrGame;
	game->start();
	//setup input
	//to do fix
	((LinuxInput*)input)->__setDisplay(((LinuxScreen*)screen)->display);
	//
	//
	loop();
	game->end();
}

void LinuxApp::update(float dt){
	input->update();
	game->run(dt);
}

bool LinuxApp::onlyPO2(){
	return true;
}
