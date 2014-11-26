#include <stdafx.h>
#include <Scene.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Scene::Scene(const String& app,
			 unsigned int width,
			 unsigned int height,
		 	 unsigned int bites,
			 unsigned int freamPerSecond,
			 bool fullscreen,
			 int dfAA) 
			 :isStarted(false) 
{
	//init context
	Application::instance()
		->getScreen()
		->createWindow(app.c_str(),
		width,
		height,
		bites,
		freamPerSecond,
		fullscreen,
		(Screen::AntiAliasing)dfAA);
	//init render
	Render::init();
}
Scene::Scene(bool initRender) :isStarted(false)
{
	//init render
	if (initRender)
	{
		Render::init();
	}
}
//utility methos
Easy2D::Screen* Scene::getScreen()
{
    return Application::instance()->getScreen();
}
Easy2D::Audio* Scene::getAudio()
{
    return Application::instance()->getAudio();
}
Easy2D::Input* Scene::getInput()
{
    return Application::instance()->getInput();
}
Easy2D::Game* Scene::getGame()
{
    return Application::instance()->getGame();
}
Easy2D::ResourcesGroup* Scene::getResourcesGroup(const String& name)
{
    return Application::instance()->getResourcesGroup(name);
}