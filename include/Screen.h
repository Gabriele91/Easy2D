#ifndef SCREEN_H
#define SCREEN_H

#include <Config.h>
#include <Math3D.h>

namespace Easy2D
{
////////////////////
class Scene;
class Application;
////////////////////
class E2D_DLL Screen
{

public:
    /**
    * Screen Anti Aliasing enum
    * @enum AntiAliasing
    */
    enum AntiAliasing
    {
        NOAA=0,
        MSAAx2=2,
        MSAAx4=4,
        MSAAx8=8,
        MSAAx16=16,
        MSAAx32=32,
        MSAAx64=64,
        CSAA=128,
        CSAAQ=256,
        BESTAA=512
    };
    /**
    * Screen orientation enum
    * @enum SceenOrientation
    */
    enum Orientation
    {
        ///@code
        /// like this:
        ///    _____
        ///   |     |
        ///   |     |
        ///   |     |
        ///   |__o__|
        ///
        ///@endcode
        PORTRAIT,
        ///@code
        /// like this:
        ///    _____
        ///   |  o  |
        ///   |     |
        ///   |     |
        ///   |_____|
        ///
        ///@endcode
        PORTRAIT_REVERSE,
        ///@code
        /// like this:
        ///    _____
        ///   |  o  |
        ///   |     |
        ///   |     |
        ///   |__o__|
        ///
        ///@endcode
        SENSOR_PORTRAIT,
        ///@code
        /// like this:
        /// ___________
        ///|           |
        ///|           |
        ///|_____o_____|
        ///
        ///@endcode
        LANDSCAPE,
        ///@code
        /// like this:
        /// ___________
        ///|     o     |
        ///|           |
        ///|___________|
        ///
        ///@endcode
        LANDSCAPE_REVERSE,
        ///@code
        /// like this:
        /// ___________
        ///|     o     |
        ///|           |
        ///|_____o_____|
        ///
        ///@endcode
        SENSOR_LANDSCAPE
    };
    /**
    * return screen width
    */
    virtual unsigned int get_width()=0;
    /**
    * return screen height
    */
    virtual unsigned int get_height()=0;
    /**
    * return screen native width
    */
    virtual unsigned int get_native_width()=0;
    /**
    * return screen  native height
    */
    virtual unsigned int get_native_height()=0;
    /**
    * return size of screen
    */
    virtual Vec2 get_size()
    {
        return Vec2((float)get_width(),(float)get_height());
    }
    /**
    * return native size of screen
    */
    virtual Vec2 get_native_size()
    {
        return Vec2((float)get_native_width(),(float)get_native_height());
    }
    /**
    * return screen orientation
    */
    virtual Orientation get_orientation()=0;
    /**
    * set screen orientation
    */
    virtual void set_orientation(Orientation orientation)=0;
    /**
    * return frame rate
    */
    virtual uint get_frame_rate()=0;
    /**
    * set the specified thread's current rendering context
    */
    virtual void acquire_context()=0;
    /**
    * swap the buffers
    */
    virtual void swap()=0;
    /**
    * show or hide mouse cursor
    * @param show, set true if you want show cursor otherwise false
    */
    virtual void set_cursor(bool show=true)=0;
    /**
     * set position cursor
     */
    virtual void set_position_cursor(const Vec2& pos)=0;
    /**
    * return if cursor is shown or hidden
    */
    virtual bool get_cursor()=0;
    /**
    * enable or disable full screen
    */
    virtual void set_fullscreen(bool fullscreen=true)=0;
    /**
    * return if fullscreen is enable return true
    */
    virtual bool is_fullscreen()=0;
    /**
    * virtual destructor
    */
    virtual ~Screen() {};

protected:
    /**
    * create window
    */
    virtual void create_window(const char* appname,
                              uint width,
                              uint height,
                              uint bites,
                              uint freamPerSecond,
                              bool fullscreen,
                              Screen::AntiAliasing dfAA)=0;
    /**
    * close window
    */
    virtual void close_window()=0;

    /**
    * Application is a friend class
    */
    friend class Application;
	friend class Scene;

};

};

#endif