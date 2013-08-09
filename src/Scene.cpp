#include <stdafx.h>
#include <Scene.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Easy2D::Screen* Scene::getScreen(){
	return Application::instance()->getScreen();
}
Easy2D::Audio* Scene::getAudio(){
	return Application::instance()->getAudio();
}
Easy2D::Input* Scene::getInput(){
	return Application::instance()->getInput();
}
Easy2D::Game* Scene::getGame(){
	return Application::instance()->getGame();
}