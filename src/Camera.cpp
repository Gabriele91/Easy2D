#include <stdafx.h>
#include <Camera.h>
#include <Input.h>
#include <Screen.h>
#include <Application.h>

using namespace Easy2D;
//costructor
Camera::Camera():zoom(1.0f)
{
    updateProjection();
}
//

Vec2 Camera::getSMouse(const Vec2& mouse)
{
    //get values
    Screen& screen=*Application::instance()->getScreen();
    //get mouse pos
    Vec2 relative(mouse.x-screen.getSize().x*0.5f,
                  screen.getSize().y*0.5f-mouse.y);

    return relative*zoom;
}
Vec2 Camera::getWMouse(const Vec2& mouse)
{
    //mouse to world
    return getGlobalMatrix().getInverse().mul2D(getSMouse(mouse));
}
Vec2 Camera::getSMouse()
{
    //get input
    Input& input=*Application::instance()->getInput();
    //return value
    return getSMouse(input.getMouse());
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
void Camera::setZoom(float argZoom)
{
    zoom=argZoom;
    updateProjection(viewport);
}
float Camera::getZoom()
{
    return zoom;
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
    projection.setOrtho(-viewport.x*0.5f*zoom,viewport.x*0.5f*zoom, 
                        -viewport.y*0.5f*zoom,viewport.y*0.5f*zoom, 
                         1.0f,
                        -1.0f);
    //update box
    vpBox.setMax(Vec2(viewport.x* 0.5f,viewport.y* 0.5f)*zoom);
    vpBox.setMin(Vec2(viewport.x*-0.5f,viewport.y*-0.5f)*zoom);
}
void Camera::updateViewport(const Vec2& argViewport)
{
    //set viewport
    viewport=argViewport;
}