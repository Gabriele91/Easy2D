#include <stdafx.h>
#include <Screen.h>
#include <Game.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;

Game::Game(const String& app,
		   unsigned int width,
		   unsigned int height,
		   unsigned int bites,
		   unsigned int freamPerSecond,
		   bool fullscreen,
		   int dfAA){
	Application::instance()
		->getScreen()
		->createWindow(app.c_str(),
					   width,
					   height,
					   bites,
					   freamPerSecond,
					   fullscreen,
					   (Screen::AntiAliasing)dfAA);
}

