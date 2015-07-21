//
//  UIManager.h
//  ComponentGui
//
//  Created by Gabriele Di Bari on 19/05/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef UIWIDGETMANAGER_H
#define UIWIDGETMANAGER_H

#include <Config.h>
#include <Input.h>
#include <Timer.h>


namespace Easy2D
{
namespace Ui
{

class WidgetManager;
class WidgetHandler;


class WidgetHandler
{
protected:
    
	friend class WidgetManager;
    WidgetManager* manager{ nullptr };
    void addTo(WidgetManager* manager);
    void removeFrom(WidgetManager* manager);
    
public:
    
    virtual ~WidgetHandler();
    virtual OBBox2  getEventBox(){ return OBBox2(); }
    virtual uint    getZ()       { return 0; }
    virtual void onOver(const Vec2& value){}
    virtual void onNotOver(const Vec2& value){}
    virtual void onClick(const Vec2& value){}
    virtual void onDoubleClick(const Vec2& value){}
    virtual void onDown(const Vec2& value){}
    virtual void onRelease(const Vec2& value){}
    virtual void onKeyPress(Key::Keyboard key){}
    virtual void onKeyDown(Key::Keyboard key){}
    virtual void onKeyRelease(Key::Keyboard key){}
    virtual void onTextInput(const String& input){}
    
    virtual void onFocus() { }
    virtual void onLostFocus() { }
    
};
    
struct CompWidgetHandler
{
    bool operator() (const WidgetHandler* left,
                     const WidgetHandler* right) const
    {
        auto vleft=((WidgetHandler*)left)->getZ();
        auto vright=((WidgetHandler*)right)->getZ();
        if(vleft==vright)
        {
            return left > right;
        }
        return vleft > vright ;
    }
};
    
class WidgetManager
{
private:
    std::map< WidgetHandler*,   WidgetHandler*,  CompWidgetHandler >  widgetMap;
    void addComponent(WidgetHandler* input);
    void removeComponent(WidgetHandler* input);
    WidgetHandler*  focusObj{ nullptr };
    
    void callOnOver(const Vec2& value);
    void callOnClick(const Vec2& value);
    void callOnDoubleClick(const Vec2& value);
    void callOnDown(const Vec2& value);
    void callOnRelease(const Vec2& value);
    void callOnTextInput(const String& input);
    void callOnKeyPress(Key::Keyboard key);
    void callOnKeyDown(Key::Keyboard key);
    void callOnKeyRelease(Key::Keyboard key);
    void updateFocus(WidgetHandler* widget);
    
    class InputManager : public Input::MouseHandler,
                         public Input::FingersHandler,
                         public Input::KeyboardHandler
    {
        //double click state
        enum ClickState
        {
            UNCLICK,
            CLICKED,
        };
        ClickState   ckstate { UNCLICK };
        Timer        timer;
        bool         isUnlocked { true };
        const double doubleClickTime= 500.0;
        //widget input...
        WidgetManager& manager;
        //aux
		Vec2 fromScreen(const Vec2& mouse) const;
        
        
    public:
        
        InputManager(WidgetManager& manager)
        :manager(manager){}
        //init/release
        void init();
        void release();
        //keyboard
        void onTextInput(const String& text);
        void onKeyPress(Key::Keyboard key);
        void onKeyDown(Key::Keyboard key);
        void onKeyRelease(Key::Keyboard key);
        //mouse
        void onMouseMove(Vec2 mousePosition);
        void onMouseDown(Vec2 mousePosition, Key::Mouse button);
        void onMousePress(Vec2 mousePosition, Key::Mouse button);
        void onMouseRelease(Vec2 mousePosition, Key::Mouse button);
        //lock/unlock events
        void lock()
        {
            isUnlocked=false;
        }
        
        void unlock()
        {
            isUnlocked=true;
        }
        
        bool isLocked() const
        {
            return !isUnlocked;
        }
    };
    InputManager inputManager;
    
    //friend class;
    friend class WidgetHandler;
    friend class InputManager;
    
public:
    
    WidgetManager();
	~WidgetManager();
    
    void init()
    {
        inputManager.init();
    }
    
    void lock()
    {
        inputManager.lock();
    }
    
    void unlock()
    {
        inputManager.unlock();
    }
    
    bool isLocked() const
    {
        return inputManager.isLocked();
    }
    
    void release()
    {
        inputManager.release();
    }
    
    WidgetHandler* getObjectFocused() const
    {
        return focusObj;
    }
    
    void drawDebugWidgets() const;
    //Object* createText()
    //Object* createLabel()
    //Object* createButton()
    //Object* createInputText()
};

};
};
#endif 
