///////////////////////
#import "EAGLView.h"
#import "EAGLViewController.h"
///////////////////////
#include <ScreeniOS.h>
#include <RenderContext.h>
///////////////////////
using namespace Easy2D;
#define view ((EAGLView*)(eaglView))
#define controller ((EAGLViewController*)(eaglViewController))
///////////////////////
ScreeniOS::ScreeniOS()
:eaglView(nullptr)
,eaglViewController(nullptr)
{
    //none
}
void ScreeniOS::setEAGLView(void *eaglView)
{
    this->eaglView=eaglView;
}
void ScreeniOS::setEAGLViewController(void *eaglViewController)
{
    this->eaglViewController=eaglViewController;
}
/**
* open a window
*/
void ScreeniOS::createWindow(const char* argappname,
                              uint width,
                              uint height,
                              uint freamPerSecond,
                              bool fullscreen,
                              TypeBuffers type,
                              AntiAliasing dfAA)
{
    //set time intervall
    [view setAnimationInterval:1.0f/freamPerSecond];
}
/**
* close window
*/
void ScreeniOS::closeWindow()
{
    // none
}

/**
* AndroidScreen destructor
*/
ScreeniOS::~ScreeniOS()
{
    closeWindow();
}
/**
* set fullscreen mode
*/
void ScreeniOS::setFullscreen(bool argfullscreen)
{
    //void
}
/**
* return screen width
*/
uint ScreeniOS::getWidth()
{
    return [view getWidth];
}
/**
* return screen height
*/
uint ScreeniOS::getHeight()
{
    return [view getHeight];
}
/**
* return screen native width
*/
uint ScreeniOS::getNativeWidth()
{
    return getWidth();
}
/**
* return screen  native height
*/
uint ScreeniOS::getNativeHeight()
{
    return getHeight();
}
/**
* return screen orientation
*/
ScreeniOS::Orientation ScreeniOS::getOrientation()
{
    
    return [controller getOrientation];
}
/**
* set screen orientation
*/
void ScreeniOS::setOrientation(Orientation orientation)
{
    [controller setOrientation: orientation];
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void ScreeniOS::setCursor(bool show)
{
    //none
}
/**
* set position cursor
*/
void ScreeniOS::setPositionCursor(const Vec2& pos)
{
    //none
}
/**
* return if cursor is shown or hidden
*/
bool ScreeniOS::getCursor()
{
    return false;
}
/**
* return if fullscreen is enable return true
*/
bool ScreeniOS::isFullscreen()
{
    return true;
}
/**
* return frame rate
*/
uint ScreeniOS::getFrameRate()
{
    return 1.0f/[view getDeltaTime];
}
/**
* set the specified thread's current rendering context
*/
void ScreeniOS::acquireContext()
{
    [view makeCurrent];
}
/**
* swap the buffers
*/
void ScreeniOS::swap()
{
    [view unsafeSwap];
}
