#include <stdafx.h>
#include <Camera.h>
#include <Input.h>
#include <Screen.h>
#include <Application.h>

using namespace Easy2D;

Vec2 Camera::getSMouse()
{
    //get input
    Input& input=*Application::instance()->getInput();
    Screen& screen=*Application::instance()->getScreen();
    //get mouse pos
    Vec2 relative(input.getMouse().x-screen.getSize().x*0.5,
                  screen.getSize().y*0.5-input.getMouse().y);

    return relative;
}
Vec2 Camera::getWMouse()
{
    //mouse to world
    return getGlobalMatrix().getInverse().mul2D(getSMouse());
}