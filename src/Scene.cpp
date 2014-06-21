#include <stdafx.h>
#include <Scene.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////

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