#include <stdafx.h>
#include <Camera.h>
#include <Input.h>
#include <Screen.h>
#include <Application.h>

using namespace Easy2D;
//costructor
Camera::Camera()
{
    updateProjection();
}
//
Vec2 Camera::getSMouse()
{
    //get input
    Input& input=*Application::instance()->getInput();
    Screen& screen=*Application::instance()->getScreen();
    //get mouse pos
    Vec2 relative(input.getMouse().x-screen.getSize().x*0.5f,
                  screen.getSize().y*0.5f-input.getMouse().y);

    return relative;
}
Vec2 Camera::getWMouse()
{
    //mouse to world
    return getGlobalMatrix().getInverse().mul2D(getSMouse());
}
//projection
const Mat4& Camera::getProjection()
{
    return projection;
}
const Vec2& Camera::getViewport()
{
    return viewport;
}
const AABox2& Camera::getBoxViewport()
{
    return vpBox;
}
void Camera::updateProjection()
{
    updateProjection(Application::instance()->getScreen()->getSize());
}
void Camera::updateProjection(const Vec2& argViewport)
{
    //set viewport
    updateViewport(argViewport);
    //update projection is always the same
    projection.setOrtho(-viewport.x*0.5f,viewport.x*0.5f, -viewport.y*0.5f,viewport.y*0.5f, 1.0f,-1.0f);

}
void Camera::updateViewport(const Vec2& argViewport)
{
    //set viewport
    viewport=argViewport;
    //update box
    vpBox.setMax(Vec2(viewport.x* 0.5f,viewport.y* 0.5f));
    vpBox.setMin(Vec2(viewport.x*-0.5f,viewport.y*-0.5f));
}