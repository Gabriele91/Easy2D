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
class Screen
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
    * Type of default buffers
    * @enum TypeBuffers
    */
    enum TypeBuffers
    {

        COLOR32DEPTH32STENCIL32,
        COLOR32DEPTH32STENCIL24,
        COLOR32DEPTH32STENCIL16,
        COLOR32DEPTH32STENCIL8,
        COLOR32DEPTH32,
        ///////////////////////
        COLOR32DEPTH24STENCIL32,
        COLOR32DEPTH24STENCIL24,
        COLOR32DEPTH24STENCIL16,
        COLOR32DEPTH24STENCIL8,
        COLOR32DEPTH24,
        ///////////////////////
        COLOR32DEPTH16STENCIL32,
        COLOR32DEPTH16STENCIL24,
        COLOR32DEPTH16STENCIL16,
        COLOR32DEPTH16STENCIL8,
        COLOR32DEPTH16,
        ///////////////////////
        COLOR32DEPTH8STENCIL32,
        COLOR32DEPTH8STENCIL24,
        COLOR32DEPTH8STENCIL16,
        COLOR32DEPTH8STENCIL8,
        COLOR32DEPTH8,
        ///////////////////////
        COLOR32STENCIL32,
        COLOR32STENCIL24,
        COLOR32STENCIL16,
        COLOR32STENCIL8,
        COLOR32,

        //////////////////////////////////////////////

        COLOR16DEPTH32STENCIL32,
        COLOR16DEPTH32STENCIL24,
        COLOR16DEPTH32STENCIL16,
        COLOR16DEPTH32STENCIL8,
        COLOR16DEPTH32,
        ///////////////////////
        COLOR16DEPTH24STENCIL32,
        COLOR16DEPTH24STENCIL24,
        COLOR16DEPTH24STENCIL16,
        COLOR16DEPTH24STENCIL8,
        COLOR16DEPTH24,
        ///////////////////////
        COLOR16DEPTH16STENCIL32,
        COLOR16DEPTH16STENCIL24,
        COLOR16DEPTH16STENCIL16,
        COLOR16DEPTH16STENCIL8,
        COLOR16DEPTH16,
        ///////////////////////
        COLOR16DEPTH8STENCIL32,
        COLOR16DEPTH8STENCIL24,
        COLOR16DEPTH8STENCIL16,
        COLOR16DEPTH8STENCIL8,
        COLOR16DEPTH8,
        ///////////////////////
        COLOR16STENCIL32,
        COLOR16STENCIL24,
        COLOR16STENCIL16,
        COLOR16STENCIL8,
        COLOR16,
    };
    inline static uchar getColorBits(TypeBuffers type)
    {
        return (2-(type / COLOR16DEPTH32STENCIL32)) * 16;
    }
    inline static uchar getDepthBits(TypeBuffers type)
    {
        return (4-(type / 5)%5)*8;
    }
    inline static uchar getStencilBits(TypeBuffers type)
    {
        return (4 - (type % 5)) * 8;
    }
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
    virtual unsigned int getWidth()=0;
    /**
    * return screen height
    */
    virtual unsigned int getHeight()=0;
    /**
    * return screen native width
    */
    virtual unsigned int getNativeWidth()=0;
    /**
    * return screen  native height
    */
    virtual unsigned int getNativeHeight()=0;
    /**
    * return size of screen
    */
    virtual Vec2 getSize()
    {
        return Vec2((float)getWidth(),(float)getHeight());
    }
    /**
    * return native size of screen
    */
    virtual Vec2 getNativeSize()
    {
        return Vec2((float)getNativeWidth(),(float)getNativeHeight());
    }
    /**
    * return screen orientation
    */
    virtual Orientation getOrientation()=0;
    /**
    * set screen orientation
    */
    virtual void setOrientation(Orientation orientation)=0;
    /**
    * return frame rate
    */
    virtual uint getFrameRate()=0;
    /**
    * set the specified thread's current rendering context
    */
    virtual void acquireContext()=0;
    /**
    * swap the buffers
    */
    virtual void swap()=0;
    /**
    * show or hide mouse cursor
    * @param show, set true if you want show cursor otherwise false
    */
    virtual void setCursor(bool show=true)=0;
    /**
     * set position cursor
     */
    virtual void setPositionCursor(const Vec2& pos)=0;
    /**
    * return if cursor is shown or hidden
    */
    virtual bool getCursor()=0;
    /**
    * enable or disable full screen
    */
    virtual void setFullscreen(bool fullscreen=true)=0;
    /**
    * return if fullscreen is enable return true
    */
    virtual bool isFullscreen()=0;
    /**
    * virtual destructor
    */
    virtual ~Screen() {};

protected:
    /**
    * create window
    */
    virtual void createWindow(const char* appname,
                              uint width,
                              uint height,
                              uint freamPerSecond,
                              bool fullscreen,
                              TypeBuffers type=COLOR32DEPTH24STENCIL8,
                              Screen::AntiAliasing dfAA=NOAA)=0;
    /**
    * close window
    */
    virtual void closeWindow()=0;

    /**
    * Application is a friend class
    */
    friend class Application;
	friend class Scene;

};

};

#endif