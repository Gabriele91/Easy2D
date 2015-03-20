#ifndef WINDOWSSCREEN_H
#define WINDOWSSCREEN_H

#include <Config.h>
#include <String.h>
#include <Screen.h>
#include <WindowsApp.h>
#include <WindowsInput.h>

namespace Easy2D
{

class WindowsScreen : Screen
{

public:

    WindowsScreen();
    virtual ~WindowsScreen();

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
    virtual void setOrientation(Orientation orientation)
    {
        //void
    }
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
    virtual void setPositionCursor(const Vec2& pos);
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
    /**
    * close window
    */
    virtual void closeWindow();

private:
    //window methods
    void __initWindow(const char* appname,unsigned int bites,AntiAliasing dfAA);
    void __initOpenGL();
    void __destroyWindow();

    ///window application instance
    HINSTANCE hInstance;
    /// handle window
    HWND hWind;
    /// handle device context
    HDC hDevCxt;
    /// handle OpenGL rendering context
    HGLRC hGLCxt;

    //datas:
    uint nativeWidth;
    uint nativeHeight;
    uint screenWidth;
    uint screenHeight;
    uint freamPerSecond;
    bool fullscreen;
    bool showmouse;
    //friends class
    friend class WindowsApp;
    friend class WindowsInput;
};


};

#endif