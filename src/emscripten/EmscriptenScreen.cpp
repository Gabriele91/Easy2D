#include <EmscriptenScreen.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>
#include <RenderContext.h>
///////////////////////
using namespace Easy2D;
///////////////////////

#define WINDOW_ATTRIBUTE ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
EmscriptenScreen::EmscriptenScreen()
    :nativeWidth(0)
    ,nativeHeight(0)
    ,screenWidth(0)
    ,screenHeight(0)
    ,freamPerSecond(0)
    ,fullscreen(false)
    ,showmouse(true)
{
    //get size screen
    nativeWidth = 0;//XWidthOfScreen(XDefaultScreenOfDisplay(display));
    nativeHeight = 0;//XHeightOfScreen(XDefaultScreenOfDisplay(display));
}

void EmscriptenScreen::__initOpenGL()
{
    //set default state
    RenderContext::setDefaultRenderState();
    //find errors:
    CHECK_GPU_ERRORS();

}
///////////////////////////////////////////////////////////
/**
* open a window
*/
void EmscriptenScreen::createWindow(const char* argappname,
                                    uint width,
                                    uint height,
                                    uint bites,
                                    uint freamPerSecond,
                                    bool fullscreen,
                                    AntiAliasing dfAA)
{

    appname=argappname;
    screenWidth= width;
    screenHeight= height;
    nativeWidth = screenWidth;
    nativeHeight = screenHeight;

    this->freamPerSecond=freamPerSecond;
    //SDL values
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    screen = SDL_SetVideoMode( width, height, bites, SDL_OPENGL | SDL_HWSURFACE );
    // connect the gl-context to the window
    acquireContext();
    //init openGL2
    __initOpenGL();
}
/**
* close window
*/
void EmscriptenScreen::closeWindow()
{
    SDL_VideoQuit();
}
/**
* EmscriptenScreen destructor
*/
EmscriptenScreen::~EmscriptenScreen()
{
    closeWindow();
}
/**
* set fullscreen mode
*/
void EmscriptenScreen::setFullscreen(bool argfullscreen)
{
    /*void*/
}
/**
* return screen width
*/
uint EmscriptenScreen::getWidth()
{
    return screenWidth;
}
/**
* return screen height
*/
uint EmscriptenScreen::getHeight()
{
    return screenHeight;
}
/**
* return screen native width
*/
uint EmscriptenScreen::getNativeWidth()
{
    return nativeWidth;
}
/**
* return screen  native height
*/
uint EmscriptenScreen::getNativeHeight()
{
    return nativeHeight;
}
/**
* return screen orientation
*/
EmscriptenScreen::Orientation EmscriptenScreen::getOrientation()
{
    return Screen::LANDSCAPE;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void EmscriptenScreen::setCursor(bool show)
{
    SDL_ShowCursor(show);
}
/**
* set position cursor
*/
void EmscriptenScreen::setPositionCursor(const Vec2& pos)
{
    SDL_WarpMouse(pos.x,pos.y);
}
/**
* return if cursor is shown or hidden
*/
bool EmscriptenScreen::getCursor()
{
    return showmouse;
}
/**
* return if fullscreen is enable return true
*/
bool EmscriptenScreen::isFullscreen()
{
    return fullscreen;
}
/**
* return frame rate
*/
uint EmscriptenScreen::getFrameRate()
{
    return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void EmscriptenScreen::acquireContext()
{
    /*void*/
}
/**
* swap the buffers
*/
void EmscriptenScreen::swap()
{
    SDL_GL_SwapBuffers();
}
