#include "stdafx.h"
#include <AngleScreen.h>
#include <AngleInput.h>
#include <Debug.h>
#include <Math3D.h>
#include <RenderContext.h>
#include <OpenGLAngle.h>
///////////////////////
using namespace Easy2D;
#ifndef ERROR_BAD_LENGTH
#define ERROR_BAD_LENGTH 20 //by google
#endif
#define E2D_WINDOW_STYLE  (WS_BORDER | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
static bool checkEGLError()
{
	EGLint error=eglGetError();
	if (error != EGL_SUCCESS) Debug::message() << "EGL Error: ";
	switch (error)
	{
		case EGL_NOT_INITIALIZED: Debug::message() << "EGL_NOT_INITIALIZED\n"; break;
		case EGL_BAD_ACCESS: Debug::message() << "EGL_BAD_ACCESS\n"; break;
		case EGL_BAD_ALLOC: Debug::message() << "EGL_BAD_ALLOC\n"; break;
		case EGL_BAD_ATTRIBUTE: Debug::message() << "EGL_BAD_ATTRIBUTE\n"; break;
		case EGL_BAD_CONTEXT: Debug::message() << "EGL_BAD_CONTEXT\n"; break;
		case EGL_BAD_CONFIG: Debug::message() << "EGL_BAD_CONFIG\n"; break;
		case EGL_BAD_CURRENT_SURFACE: Debug::message() << "EGL_BAD_CURRENT_SURFACE\n"; break;
		case EGL_BAD_DISPLAY: Debug::message() << "EGL_BAD_DISPLAY\n"; break;
		case EGL_BAD_SURFACE: Debug::message() << "EGL_BAD_SURFACE\n"; break;
		case EGL_BAD_MATCH: Debug::message() << "EGL_BAD_MATCH\n"; break;
		case EGL_BAD_PARAMETER: Debug::message() << "EGL_BAD_PARAMETER\n"; break;
		case EGL_BAD_NATIVE_PIXMAP: Debug::message() << "EGL_BAD_NATIVE_PIXMAP\n"; break;
		case EGL_BAD_NATIVE_WINDOW: Debug::message() << "EGL_BAD_NATIVE_WINDOW\n"; break;
		case EGL_CONTEXT_LOST: Debug::message() << "EGL_CONTEXT_LOST\n"; break;
		case EGL_SUCCESS:
		default: return true; break;
	}
	return false;
}
#ifdef _DEBUG
	#define EGL_DEBUG_ASSERT_REPLACE( x ) if(!(x)){ DEBUG_ASSERT(checkEGLError()); }
#else
	#define EGL_DEBUG_ASSERT_REPLACE( x ) x
#endif

//window methods
void AngleScreen::__initWindow(const char* appname,TypeBuffers typeBufferOpenGL,AntiAliasing dfAA)
{

    DEBUG_MESSAGE( "Open window:" << screenWidth << "x" << screenHeight );

    hInstance = (HINSTANCE)GetModuleHandle(NULL);

    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hInstance = hInstance;
    wc.lpfnWndProc = AngleInput::WndProc;
    wc.lpszClassName = TEXT( "Easy2DWindow" );
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    // Register that class with the Windows O/S..
    RegisterClass(&wc);
    //calc siuze window (Adjust....)
    RECT windowRect;
    windowRect.left=(long)0;					// Set Left Value To 0
    windowRect.right=(long)screenWidth;			// Set Right Value To Requested Width
    windowRect.top=(long)0;						// Set Top Value To 0
    windowRect.bottom=(long)screenHeight;		// Set Bottom Value To Requested Height
    DWORD dwstyle = E2D_WINDOW_STYLE;
    AdjustWindowRect( &windowRect, dwstyle, true);
    //calc size window
    int rwidth = windowRect.right-windowRect.left;
    int rheight = windowRect.bottom-windowRect.top - ERROR_BAD_LENGTH;
    //calc center
    int wlft = (GetSystemMetrics(SM_CXSCREEN) - rwidth) / 2;
    int wtop = (GetSystemMetrics(SM_CYSCREEN) - rheight) / 2;
    //create a window
    hWind = CreateWindowA("Easy2DWindow",
                          appname,
                          dwstyle,
                          wlft, //x
                          wtop, //y
                          rwidth,//Window Width
                          rheight,//Calculate Window Height
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    DEBUG_ASSERT(hWind);
    //vars declaterion
    EGLint numConfigs=0;
    EGLint majorVersion=0;
    EGLint minorVersion=0;
	EGLConfig config;
    //Get Display
	display = eglGetDisplay(GetDC(hWind));
	//Init egl
	EGL_DEBUG_ASSERT_REPLACE(eglInitialize(display, &majorVersion, &minorVersion));
	// Get configs
	EGL_DEBUG_ASSERT_REPLACE(eglGetConfigs(display, NULL, 0, &numConfigs));
	//set attributes
    int colorBits   = (int)Screen::getColorBits(typeBufferOpenGL);
    int redBits     = 0;
    int greenBits   = 0;
    int blueBits    = 0;
    int alphaBits   = 0;
    int depthBits   = (int)Screen::getDepthBits(typeBufferOpenGL);
    int stencilBits = (int)Screen::getStencilBits(typeBufferOpenGL);
    switch(colorBits)
    {
        case 16:
        redBits     = 4;
        greenBits   = 4;
        blueBits    = 4;
        alphaBits   = 4;
        case 24:
        redBits     = 8;
        greenBits   = 8;
        blueBits    = 8;
        alphaBits   = 0;
        case 32:
        default:
        redBits     = 8;
        greenBits   = 8;
        blueBits    = 8;
        alphaBits   = 8;
        break;
    };    //type context
    EGLint attributes[32];    
    int i=0;
    //set type of surfece
    attributes[i++]   = EGL_SURFACE_TYPE;
    attributes[i++]   = EGL_WINDOW_BIT;
    //colors
    attributes[i++]   = EGL_RED_SIZE;
    attributes[i++]   = redBits;
    attributes[i++]   = EGL_GREEN_SIZE;
    attributes[i++]   = greenBits;
    attributes[i++]   = EGL_BLUE_SIZE;
    attributes[i++]   = blueBits;
    attributes[i++]   = EGL_ALPHA_SIZE;
    attributes[i++]   = alphaBits;
    //depth
    if(depthBits)
    {
        attributes[i++]   = EGL_DEPTH_SIZE;
        attributes[i++]   = depthBits;
    }
    //stencil
    if(stencilBits)
    {
        attributes[i++]   = EGL_STENCIL_SIZE;
        attributes[i++]   = stencilBits;
    }
    //msaa
    if(dfAA!=NOAA)
    {
        attributes[i++]=EGL_SAMPLE_BUFFERS;
        attributes[i++]=1;
        attributes[i++]=EGL_SAMPLES;
        attributes[i++]=static_cast<int>(dfAA);
    }
    //type
    attributes[i++] = EGL_RENDERABLE_TYPE;
    attributes[i++] = EGL_OPENGL_ES2_BIT;
    //
    attributes[i]=EGL_NONE; 
    ///////////////////////////////
    EGLint surfaceAttribList[] =
    {
       //EGL_POST_SUB_BUFFER_SUPPORTED_NV, EGL_TRUE,
       EGL_NONE, EGL_NONE
    };
	EGLint contextAttribs[] = 
	{ 
		EGL_CONTEXT_CLIENT_VERSION, 2, 
		EGL_NONE, EGL_NONE 
	};
	// Choose config
	EGL_DEBUG_ASSERT_REPLACE(eglChooseConfig(display, attributes, &config, 1, &numConfigs));
	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWind, surfaceAttribList);
	EGL_DEBUG_ASSERT_REPLACE(surface != EGL_NO_SURFACE);
	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	EGL_DEBUG_ASSERT_REPLACE(context != EGL_NO_CONTEXT);
    // Make the context current
	EGL_DEBUG_ASSERT_REPLACE(eglMakeCurrent(display, surface, surface, context));
	//init opengl context
	__initOpenGL();
    //return
}


void AngleScreen::__initOpenGL()
{
	//init OpenGL ES
	initOpenGLES();
    //set default state
	RenderContext::initContext();
    //find errors:
    CHECK_GPU_ERRORS();
}

void AngleScreen::__destroyWindow()
{
    //disable fullscreen
    if( fullscreen )
    {
        ChangeDisplaySettings( NULL, 0 );
        ShowCursor(TRUE);
    }
    //delete glcontext
    //wglMakeCurrent( NULL, NULL );
	eglMakeCurrent(0,0,0,0);//????
	//delete context
	if(context)
    {
		//delete context
		RenderContext::releaseContext();
		//delete OpenGL Context
		eglDestroyContext(display,context);
    }
    //destroy window
    if(hWind)
    {
        //Animate exit
        //AnimateWindow( hWind, 200, AW_HIDE | AW_BLEND );
        //release device context
		if(surface)
        {
			eglDestroySurface(display,surface);
        }
        //destroy window
        DEBUG_ASSERT_REPLACE(DestroyWindow(hWind));
    }
    //reset values
    fullscreen=false;
    hWind=NULL;
    //unregister class
    DEBUG_ASSERT_REPLACE(UnregisterClass("Easy2DWindow",hInstance));
}
//
AngleScreen::AngleScreen()
    :Screen()
    ,nativeWidth(0)
    ,nativeHeight(0)
    ,screenWidth(0)
    ,screenHeight(0)
    ,freamPerSecond(0)
    ,fullscreen(false)
    ,showmouse(true)
{

    setCursor(showmouse);
    nativeWidth = GetSystemMetrics( SM_CXSCREEN );
    nativeHeight = GetSystemMetrics( SM_CYSCREEN );

}
AngleScreen::~AngleScreen()
{
    closeWindow();
}

//calc size window
void setClientSize(HWND window, int width, int height)
{
    RECT offsetRect;
    //calc offset
    GetClientRect(window, &offsetRect);
    width -= offsetRect.right;
    height -= offsetRect.bottom;

    GetWindowRect(window, &offsetRect);
    width += (offsetRect.right - offsetRect.left);
    height += (offsetRect.bottom - offsetRect.top);

    //calc center
    int wlft = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int wtop = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    SetWindowPos(window, HWND_NOTOPMOST,wlft, wtop, width, height, SWP_SHOWWINDOW);
}
/**
* enable or disable full screen
*/
void AngleScreen::setFullscreen(bool prfullscreen)
{
    //set
    bool change=fullscreen!=prfullscreen;
    fullscreen=prfullscreen;
    //set window style
    DWORD style = fullscreen ? (WS_POPUP | WS_VISIBLE) : E2D_WINDOW_STYLE;
    SetWindowLong(hWind, GWL_STYLE, style);

    if( !fullscreen )
    {
        //disable fullscreen
        if(change)
            ChangeDisplaySettings( NULL, 0 );
        //set size window
        setClientSize(hWind,screenWidth,screenHeight);
    }
    else
    {
        DEVMODE dm;
        memset(&dm, 0, sizeof(dm));
        dm.dmSize = sizeof(dm);
        dm.dmPelsWidth = getWidth();
        dm.dmPelsHeight	= getHeight();
        dm.dmBitsPerPel	= 32;
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        LONG ret = ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

        DEBUG_ASSERT( (ret == DISP_CHANGE_SUCCESSFUL) );

        //WARNING MoveWindow can change backbuffer size
        MoveWindow(hWind, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, TRUE);
        //set focus on this screen
        SetFocus(hWind);
    }

    ShowCursor( !fullscreen );
}
/**
* return if fullscreen is enable return true
*/
bool AngleScreen::isFullscreen()
{
    return fullscreen;
}
/**
* return frame rate
*/
uint AngleScreen::getFrameRate()
{
    return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void AngleScreen::acquireContext()
{
    // Make the context current
    DEBUG_ASSERT_REPLACE( eglMakeCurrent(display, surface, surface, context));
}
/**
* swap the buffers
*/
void AngleScreen::swap()
{
	eglSwapBuffers(display,surface);
}
/**
* create window
*/
void AngleScreen::createWindow(const char* appname,
                                 uint width,
                                 uint height,
                                 uint setFreamPerSecond,
                                 bool prfullscreen,
                                 TypeBuffers type,
                                 AntiAliasing dfAA)
{
    DEBUG_ASSERT(appname);
    DEBUG_MESSAGE( "createWindow Easy2D Win32" );

    //set values
    screenWidth=Math::min(nativeWidth,width);
    screenHeight=Math::min(nativeHeight,height);
    freamPerSecond=setFreamPerSecond;
    //create window
    __initWindow(appname,type,dfAA);
    setFullscreen(prfullscreen);
    //
}
/**
* close window
*/
void AngleScreen::closeWindow()
{
    __destroyWindow();
}
/**
* return screen width
*/
uint AngleScreen::getWidth()
{
    return screenWidth;
}
/**
* return screen height
*/
uint AngleScreen::getHeight()
{
    return screenHeight;
}
/**
* return screen native width
*/
uint AngleScreen::getNativeWidth()
{
    return nativeWidth;
}
/**
* return screen  native height
*/
uint AngleScreen::getNativeHeight()
{
    return nativeHeight;
}
/**
* return screen orientation
*/
AngleScreen::Orientation AngleScreen::getOrientation()
{
    return Screen::LANDSCAPE;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void AngleScreen::setCursor(bool show)
{
    showmouse=show;
    ShowCursor(showmouse);
}
/**
* set position cursor
*/
void AngleScreen::setPositionCursor(const Vec2& pos)
{
    POINT mouse;
    mouse.x=(LONG)pos.x;
    mouse.y=(LONG)pos.y;
    ClientToScreen(hWind, &mouse);
    SetCursorPos(mouse.x,mouse.y);
}
/**
* return if cursor is shown or hidden
*/
bool AngleScreen::getCursor()
{
    return showmouse;
}
