//
//  InputiOS.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 30/10/14.
//
//
#include <InputiOS.h>
#define input ((InputiOS*)Application::instance()->getInput())
using namespace Easy2D;
//init component
InputiOS::InputiOS()
{
    //void
}
//free component
InputiOS::~InputiOS()
{
    //void
}
///////////////////////////////////////////////////////////////////////////
//UPDATE 
void InputiOS::update()
{
    //reset input window
    ewindow.resize=false;
    //update hit
    efingers.__clearHit();
    //update down fingers
    efingers.__update(this);
}
///////////////////////////////////////////////////////////////////////////
// WINDOW EVENTS FIRED FROM IOS
void InputiOS::__callOnFocus(bool focus)
{
    //c-style event
    input->ewindow.focus=focus;
    //oop event
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onFocus(focus);
}
void InputiOS::__callOnClose()
{
    //c-style event
    input->ewindow.close=true;
    //oop
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onClose();
}
void InputiOS::__callOnResize(Vec2 size)
{
    //c-style
    input->ewindow.resize=true;
    input->ewindow.windowResize=size;
    //oop
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onResize(size);
}
///////////////////////////////////////////////////////////////////////////
//FINGERES EVENTS
void InputiOS::__callOnFingerDown(const Vec3& position, Key::Finger fid)
{
    //oop event
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerDown(position,fid);
}
///////////////////////////////////////////////////////////////////////////
/* FINGERES EVENT FIRED FROM IOS */
void InputiOS::__callOnFingerPress(const Vec3& position, Key::Finger fid)
{
    //c-style event
    input->efingers.__fingerDown(fid,position);
    //oop event
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerPress(position,fid);
}
void InputiOS::__callOnFingerMove(const Vec3& position, Key::Finger fid)
{
    //c-style event
    input->efingers.__fingerDown(fid,position);
    //oop event
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerMove(position,fid);
}
void InputiOS::__callOnFingerRelease(const Vec3& position, Key::Finger fid)
{
    //c-style event
    input->efingers.__fingerUp(fid,position);
    //oop event
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerRelease(position,fid);
}
///////////////////////////////////////////////////////////////////////////