#ifndef SCREEIOS_H
#define SCREEIOS_H
#include <Config.h>
#include <AppiOS.h>
#include <EString.h>
#include <Screen.h>

namespace Easy2D
{

class ScreeniOS : Screen
{

public:

    ScreeniOS();
    virtual ~ScreeniOS();
    /**
     * Pass iOS view.
     */
    void setEAGLView(void *eaglView);
    /**
     * Pass iOS controller.
     */
    void setEAGLViewController(void *eaglViewController);
    
    /**
    * return screen width
    */
    virtual uint getWidth();
    /**
    * return screen height
    */
    virtual uint getHeight();
    /**
    * return screen native width
    */
    virtual uint getNativeWidth();
    /**
    * return screen  native height
    */
    virtual uint getNativeHeight();
    /**
    * return screen orientation
    */
    virtual Orientation getOrientation();
    /**
    * set screen orientation
    */
    virtual void setOrientation(Orientation orientation);
    /**
    * return frame rate
    */
    virtual uint getFrameRate();
    /**
    * set the specified thread's current rendering context
    */
    virtual void acquireContext();
    /**
    * swap the buffers
    */
    virtual void swap();
    /**
    * show or hide mouse cursor
    * @param show, set true if you want show cursor otherwise false
    */
    virtual void setCursor(bool show=true);
    /**
    * set position cursor
    */
    void setPositionCursor(const Vec2& pos);
    /**
    * return if cursor is shown or hidden
    */
    virtual bool getCursor();
    /**
    * enable or disable full screen
    */
    virtual void setFullscreen(bool fullscreen=true);
    /**
    * return if fullscreen is enable return true
    */
    virtual bool isFullscreen();

protected:

    /**
    * create window
    */
    virtual void createWindow(const char* appname,
                              uint width,
                              uint height,
                              uint bites,
                              uint freamPerSecond,
                              bool fullscreen,
                              AntiAliasing dfAA=NOAA);
    /*
    * close window
    */
    virtual void closeWindow();

    //datas:
    void *eaglView;
    void *eaglViewController;
    
    //friends class
    friend class AppiOS;
};


};

#endif
