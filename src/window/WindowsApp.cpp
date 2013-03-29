#include "stdafx.h"
#include <WindowsApp.h>
#include <WindowsScreen.h>
#include <WindowsInput.h>
///////////////////////
using namespace Easy2D;

WindowsApp::WindowsApp()
		   :Application(){
	screen=(Screen*)new WindowsScreen();
	input=(Input*)new WindowsInput();
}

WindowsApp::~WindowsApp(){
	delete input;
	delete screen;
}

String WindowsApp::appDataDirectory(){
	return "";
}

String WindowsApp::appWorkingDirectory(){
	return "";
}

String WindowsApp::appResourcesDirectory(){
	return "";
}	

void WindowsApp::loop(){	
	while( !input->getClose() ){
		update(0);
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