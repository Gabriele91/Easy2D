//
//  UIManager.cpp
//  ComponentGui
//
//  Created by Gabriele Di Bari on 19/05/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include <stdafx.h>
#include <RenderContext.h>
#include <Application.h>
#include <Screen.h>
#include <UIManager.h>

using namespace Easy2D;
using namespace Easy2D::Ui;

/**
 * Widget Hendler
 */
void  WidgetHandler::addTo(WidgetManager* argmanager)
{
    manager=argmanager;
    manager->addComponent(this);
    
}
void WidgetHandler::removeFrom(WidgetManager* argmanager)
{
    DEBUG_ASSERT(manager==argmanager);
    manager->removeComponent(this);
}
WidgetHandler::~WidgetHandler()
{
    if(manager) removeFrom(manager);
}

/**
 * WidgetManager
 */
WidgetManager::WidgetManager()
:inputManager(*this)
{
}
WidgetManager::~WidgetManager()
{
	for (auto widget : widgetMap)
	{
		//dt
		widget.second->manager = nullptr;
	}
}
void WidgetManager::callOnOver(const Vec2& value)
{
    if(focusObj)
    {
        if(focusObj->getEventBox().isIntersection(value))
            focusObj->onOver(value);
        else
            focusObj->onNotOver(value);
    }
}
void WidgetManager::callOnClick(const Vec2& value)
{
    for(auto widget:widgetMap)
    {
        if(widget.second->getEventBox().isIntersection(value))
        {
            updateFocus(widget.second);
            widget.second->onClick(value);
            return;
        }
    }
    updateFocus(nullptr);
}
void WidgetManager::callOnDoubleClick(const Vec2& value)
{
    if(focusObj)
    {
        if(focusObj->getEventBox().isIntersection(value))
            focusObj->onDoubleClick(value);
    }
}
void WidgetManager::callOnDown(const Vec2& value)
{
    if(focusObj) focusObj->onDown(value);
}
void WidgetManager::callOnRelease(const Vec2& value)
{
    if(focusObj) focusObj->onRelease(value);
}
void WidgetManager::callOnTextInput(const String& text)
{
    if (focusObj) focusObj->onTextInput(text);
}
void WidgetManager::callOnKeyPress(Key::Keyboard key)
{
    if (focusObj) focusObj->onKeyPress(key);
}
void WidgetManager::callOnKeyDown(Key::Keyboard key)
{
    if (focusObj) focusObj->onKeyDown(key);
}
void WidgetManager::callOnKeyRelease(Key::Keyboard key)
{
    if (focusObj) focusObj->onKeyRelease(key);
}
void WidgetManager::updateFocus(WidgetHandler* widget)
{
    if(focusObj!=widget)
    {
        if(widget)   widget->onFocus();
        if(focusObj) focusObj->onLostFocus();
        focusObj =  widget;
    }
}
void WidgetManager::addComponent(WidgetHandler* input)
{
    widgetMap.insert({input,input});
}
void WidgetManager::removeComponent(WidgetHandler* input)
{
    widgetMap.erase(input);
}
void WidgetManager::drawDebugWidgets() const
{
    for (auto widget : widgetMap)
    {
        OBBox2 box=widget.second->getEventBox();
        RenderContext::drawFillBox(box, Color(255,0,0,128));
    }
}
/**
 * InputManager
 */
void WidgetManager::InputManager::init()
{
    auto input=Application::instance()->getInput();
    input->addHandler((Input::MouseHandler*)this);
    input->addHandler((Input::FingersHandler*)this);
    input->addHandler((Input::KeyboardHandler*)this);
    //init state
    ckstate=UNCLICK;
}
void WidgetManager::InputManager::release()
{
    auto input=Application::instance()->getInput();
    input->removeHandler((Input::MouseHandler*)this);
    input->removeHandler((Input::FingersHandler*)this);
    input->removeHandler((Input::KeyboardHandler*)this);
}
Vec2 WidgetManager::InputManager::fromScreen(const Vec2& mouse) const
{
	//get values
	Screen& screen = *Application::instance()->getScreen();
	//View matrix/get mouse pos
	return
	RenderContext::getView().getInverse().mul2D(
		Vec2(mouse.x - screen.getSize().x*0.5f,
		    -mouse.y + screen.getSize().y*0.5f)
	);
}
//keyboard
void WidgetManager::InputManager::onTextInput(const String& text)
{
    manager.callOnTextInput(text);
}
void WidgetManager::InputManager::onKeyPress(Key::Keyboard key)
{
    manager.callOnKeyPress(key);
}
void WidgetManager::InputManager::onKeyDown(Key::Keyboard key)
{
    manager.callOnKeyDown(key);
}
void WidgetManager::InputManager::onKeyRelease(Key::Keyboard key)
{
    manager.callOnKeyRelease(key);
}
//mouse
void  WidgetManager::InputManager::onMouseMove(Vec2 mousePosition)
{
    manager.callOnOver(fromScreen(mousePosition));
}
void WidgetManager::InputManager::onMousePress(Vec2 mousePosition, Key::Mouse button)
{
    switch (ckstate)
    {
        case CLICKED:
            if(timer.getGetCounter() < doubleClickTime)
            {
                manager.callOnDoubleClick(fromScreen(mousePosition));
                break;
            }
            ckstate = UNCLICK;
        //else
        case UNCLICK:
            manager.callOnClick(fromScreen(mousePosition));
            timer.start();
            break;
        default: break;
    }
}
void  WidgetManager::InputManager::onMouseDown(Vec2 mousePosition, Key::Mouse button)
{
    if (ckstate == UNCLICK)
    {
        //callback
        manager.callOnDown(fromScreen(mousePosition));
    }
}
void WidgetManager::InputManager::onMouseRelease(Vec2 mousePosition, Key::Mouse button)
{
    if (ckstate == UNCLICK)
    {
        //callback
        manager.callOnRelease(fromScreen(mousePosition));
        //change state
        ckstate = CLICKED;
    }
    else if (ckstate == CLICKED)
    {
        ckstate = UNCLICK;
    }
}
//fingers
void  WidgetManager::InputManager::onFingerMove(Vec3 touchPosition, Key::Finger FingerID)
{
    //filter
    if( crtFinger !=  FingerID ) return;
    //event
    onMouseMove(touchPosition.xy());
}
void  WidgetManager::InputManager::onFingerDown(Vec3 touchPosition, Key::Finger FingerID)
{
    //filter
    if( crtFinger !=  FingerID ) return;
    //event
    onMouseDown(touchPosition.xy(),Key::BUTTON_LEFT);
}
void  WidgetManager::InputManager::onFingerPress(Vec3 touchPosition, Key::Finger FingerID)
{
    //set focus to a finger
    if( crtFinger ==  Key::FINGER_NULL ){ crtFinger = FingerID; }
    //filter
    if( crtFinger !=  FingerID ) return;
    //event
    onMousePress(touchPosition.xy(),Key::BUTTON_LEFT);
    
}
void  WidgetManager::InputManager::onFingerRelease(Vec3 touchPosition, Key::Finger FingerID)
{
    //filter
    if( crtFinger !=  FingerID ) return;
    //event
    onMouseRelease(touchPosition.xy(),Key::BUTTON_LEFT);
    //delete focus to a finger
    crtFinger = Key::FINGER_NULL ;
}


