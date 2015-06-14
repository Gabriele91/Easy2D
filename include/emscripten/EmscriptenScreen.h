#ifndef EMSCREEN_H
#define EMSCREEN_H

#include <SDL\SDL.h>
#include <Config.h>
#include <EmscriptenApp.h>
#include <EString.h>
#include <Screen.h>

namespace Easy2D
{

class EmscriptenScreen : Screen
{

public:

    EmscriptenScreen();
    virtual ~EmscriptenScreen();

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
    virtual void createWindow(  const char* appname,
                                uint width,
                                uint height,
                                uint freamPerSecond,
                                bool fullscreen,
                                Screen::TypeBuffers type = COLOR32DEPTH24STENCIL8,
                                Screen::AntiAliasing dfAA = NOAA);
    /*
    * close window
    */
    virtual void closeWindow();

private:
    ///////////////////////////////////
    //SDL
    SDL_Surface		*screen;
    String          appname;
    ///////////////////////////////////
    void __initOpenGL();
    void __createWindow();
    void __deleteWindow();
    ///////////////////////////////////
    //datas:
    uint nativeWidth;
    uint nativeHeight;
    uint screenWidth;
    uint screenHeight;
    uint freamPerSecond;
    bool fullscreen;
    bool showmouse;
    //friends class
    friend class EmscriptenApp;
};


};

#endif
