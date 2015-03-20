#ifndef LINUXINPUT_H
#define LINUXINPUT_H
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/glx.h>
#include <Config.h>
#include <Math3D.h>
#include <Input.h>
#include <LinuxApp.h>

namespace Easy2D
{

class LinuxInput : Input
{

public:

    /**
    * get mouse position
    * @return mouse position
    */
    virtual Vec2 getMouse() const
    {
        return emouse.pos;
    }
    /**
    * get finger position
    * @param finger number
    * @return finger position
    */
    virtual Vec2 getFinger(Key::Finger id) const
    {
        return Vec2::ZERO;
    }
    /**
    * get accelerometer values
    * @return (x,y,z, azimuth, pitch, roll) accelerometer values
    */
    virtual const AccelerometerValues& getAccelerometer() const
    {
        return AccelerometerValues();
    }
    /**
    * update window event,
    * call this method in a loop
    */
    virtual void update();
    /**
    * Return true if window has focus
    * @return focus status
    */
    virtual bool getFocus() const
    {
        return ewindow.focus;
    }
    /**
    * Return true if keyboard button is down
    * @param key's id
    * @return status
    */
    virtual bool getKeyDown(Key::Keyboard id) const
    {
        return ekeyboard.status[id] ? true : false;
    }
    /**
    *  Return true if go up keyboard button
    * @param key's id
    * @return status
    */
    virtual bool getKeyUp(Key::Keyboard id) const
    {
        return !ekeyboard.status[id];
    }
    /**
    *  Return true if hit keyboard button
    * @param key's id
    * @return status
    */
    virtual bool getKeyHit(Key::Keyboard id) const
    {
        for(int i=0; i<10&&i<ekeyboard.nPress; ++i)
        {
            if( ekeyboard.hit[i]==id ) return true;
        }
        return false;
    }
    /**
    * Return true if mouse button is down
    * @param mouse button
    * @return status
    */
    virtual bool getMouseDown(Key::Mouse id) const
    {
        return emouse.status[id] ? true : false;
    }
    /**
    * Return true if mouse button is up
    * @param mouse button
    * @return status
    */
    virtual bool getMouseUp(Key::Mouse id) const
    {
        return !emouse.status[id];
    }
    /**
    * Return true if mouse button is hit
    * @param mouse button
    * @return status
    */
    virtual bool getMouseHit(Key::Mouse id) const
    {
        for(int i=0; i<10&&i<emouse.nPress; ++i)
        {
            if( emouse.hit[i]==id ) return true;
        }
        return false;
    }
    /**
    * Return scroll wheel delta
    * @return scroll delta
    */
    virtual float getScroll() const
    {
        return emouse.scroll;
    }
    /**
    * Return true if finger is down
    * @param finger number
    * @return status
    */
    virtual bool getFingerDown(Key::Finger id) const
    {
        return false;
    }
    /**
    * Return true if finger is up
    * @param finger number
    * @return status
    */
    virtual bool getFingerUp(Key::Finger id) const
    {
        return false;
    }
    /**
    * Return true if finger hit the screen
    * @param  finger number
    * @return status
    */
    virtual bool getFingerHit(Key::Finger id) const
    {
        return false;
    }
    /**
    * Return true if window minimized
    * @return window status
    */
    virtual bool getMinimized() const
    {
        return ewindow.minimized;
    }
    /**
    * Return true if window maximized
    * @return window status
    */
    virtual bool getMaximized() const
    {
        return ewindow.maximized;
    }
    /**
    * Return true if window is closed
    * @return window status
    */
    virtual bool getClose() const
    {
        return ewindow.close;
    }
    /**
    * Return true if window is resized
    * @return window status
    */
    virtual bool getResize() const
    {
        return ewindow.resize;
    }
    /**
    * window size (when it is resized)
    * @return window size
    */
    virtual Vec2 getResizeValues() const
    {
        return ewindow.windowResize;
    }


protected:
    //window
    struct EventWindow
    {
        bool focus,
             minimized,
             maximized,
             resize,
             close;
        Vec2 windowResize;
        void __init()
        {
            memset(this,0,sizeof(EventWindow));
        }
    } ewindow;
    //mouse
    struct EventMouse
    {
        int nPress,nDown;
        short scroll;
        Vec2 pos;
        char status[Key::MOUSEMAX];
        Key::Mouse hit[7];
        Key::Mouse down[10];

        void __init()
        {
            memset(this,0,sizeof(EventMouse));
        }
        void __mouseDown(Key::Mouse k)
        {
            status[k]=(char)(0x0001 | 0x0002*(status[k]&0x0001));
            down[nDown++]=k;
            nDown%=10;
        }
        void __mouseUp(Key::Mouse k)
        {
            if(status[k]== 0x0001 && nPress<7)
                hit[nPress++]=k;

            for(auto& kd:down) if(kd==k)
                    kd=Key::Mouse::MOUSE_NULL;

            status[k]=false;
        }
        void __clearHit()
        {
            memset(hit,false,7);
            nPress=0;
        }
        void __update(LinuxInput *self)
        {
            for(auto& kd:down)
                if(kd!=Key::Mouse::MOUSE_NULL)
                    self->__callOnMouseDown(pos,kd);
        }
    } emouse;
    //keyboard
    struct EventKeyboard
    {
        int nPress,nDown;
        Key::Keyboard hit[10];
        Key::Keyboard down[10];
        char status[Key::KEYBOARDMAX];

        void __init()
        {
            memset(this,0,sizeof(EventKeyboard));
        }
        void __keyboardDown(Key::Keyboard k)
        {
            status[k]=(char)(0x0001 | 0x0002*(status[k]&0x0001));
            down[nDown++]=k;
            nDown%=10;
        }
        void __keyboardUp(Key::Keyboard k)
        {
            if(status[k]== 0x0001 && nPress<7)
                hit[nPress++]=k;

            for(auto& kd:down) if(kd==k)
                    kd=Key::Keyboard::KEY_NULL;

            status[k]=false;
        }
        void __clearHit()
        {
            memset(hit,false,10);
            nPress=0;
        }
        void __update(LinuxInput *self)
        {
            for(auto& kd:down)
                if(kd!=Key::Keyboard::KEY_NULL)
                    self->__callOnKeyDown(kd);
        }
    } ekeyboard;
    //calls
    void __callOnKeyPress(Key::Keyboard key);
    void __callOnKeyRelease(Key::Keyboard key);
    void __callOnKeyDown(Key::Keyboard key);
    void __callOnMouseMove(Vec2 mousePosition);
    void __callOnMousePress(Vec2 mousePosition, Key::Mouse button);
    void __callOnMouseDown(Vec2 mousePosition, Key::Mouse button);
    void __callOnMouseRelease(Vec2 mousePosition, Key::Mouse button);
    void __callOnMouseScroll(short scrollDelta);
    //hide constructor
    LinuxInput();
    //linux:
    Display *display;
    void __setDisplay(Display *display);
    void __update(XEvent &event);
    bool _isKeyRepeat(XEvent &event);
    //friends class
    friend class LinuxApp;

};

};

#endif
