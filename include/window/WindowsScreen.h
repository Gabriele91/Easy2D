#ifndef WINDOWSSCREEN_H
#define WINDOWSSCREEN_H

#include <Config.h>
#include <String.h>
#include <Screen.h>
#include <WindowsApp.h>
#include <WindowsInput.h>

namespace Easy2D
{

class E2D_DLL WindowsScreen : Screen
{

public:

    WindowsScreen();
    virtual ~WindowsScreen();

    /**
    * return screen width
    */
    virtual uint get_width();
    /**
    * return screen height
    */
    virtual uint get_height();
    /**
    * return screen native width
    */
    virtual uint get_native_width();
    /**
    * return screen  native height
    */
    virtual uint get_native_height();
    /**
    * return screen orientation
    */
    virtual Orientation get_orientation();
    /**
    * set screen orientation
    */
    virtual void set_orientation(Orientation orientation)
    {
        //void
    }
    /**
    * return frame rate
    */
    virtual uint get_frame_rate();
    /**
    * set the specified thread's current rendering context
    */
    virtual void acquire_context();
    /**
    * swap the buffers
    */
    virtual void swap();
    /**
    * show or hide mouse cursor
    * @param show, set true if you want show cursor otherwise false
    */
    virtual void set_cursor(bool show=true);
    /**
    * set position cursor
    */
    virtual void set_position_cursor(const Vec2& pos);
    /**
    * return if cursor is shown or hidden
    */
    virtual bool get_cursor();
    /**
    * enable or disable full screen
    */
    virtual void set_fullscreen(bool fullscreen=true);
    /**
    * return if fullscreen is enable return true
    */
    virtual bool is_fullscreen();

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
                              AntiAliasing dfAA=NOAA);
    /**
    * close window
    */
    virtual void close_window();

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