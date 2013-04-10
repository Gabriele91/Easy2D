#include "stdafx.h"
#include <WindowsScreen.h>
#include <WindowsInput.h>
#include <Debug.h>
#include <Math2D.h>
///////////////////////
using namespace Easy2D;
#define E2D_WINDOW_STYLE  (WS_BORDER | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
//window methods
void WindowsScreen::__initWindow(const char* appname,uint bites){
	
	DEBUG_MESSAGE( "Open window:" << screenWidth << "x" << screenHeight );

	hInstance = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hInstance = hInstance;         
	wc.lpfnWndProc = WindowsInput::WndProc;         
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
	int rheight = windowRect.bottom-windowRect.top;
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
	//get device context
	hDevCxt = GetDC( hWind );
	DEBUG_ASSERT(hDevCxt);
	// create PFD:
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER |							// Must Support Double Buffering
		PFD_SWAP_EXCHANGE,
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bites, 										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Resource
		0,											// Shift Bit Ignored
		0,											// No Accumulation Resource
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Resource (Depth Resource)  
		0,											// No Stencil Resource
		0,											// No Auxiliary Resource
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	int chosenPixelFormat=0;
	//ChooseAntiAliasingPixelFormat( chosenPixelFormat, 0 /* MSAA */);
	//DEBUG_ASSERT(chosenPixelFormat);	
	DEBUG_ASSERT_REPLACE( chosenPixelFormat=ChoosePixelFormat(hDevCxt, &pfd) );
	DEBUG_ASSERT_REPLACE( SetPixelFormat( hDevCxt, chosenPixelFormat, &pfd ) );
	//OpenGL Context
	hGLCxt = wglCreateContext( hDevCxt );
	DEBUG_ASSERT(hGLCxt);	
	DEBUG_ASSERT_REPLACE( wglMakeCurrent( hDevCxt, hGLCxt ) );
	//openGL 2 init
	initOpenGL2();
	//return
}
void WindowsScreen::__destroyWindow(){
	//disable fullscreen
	setFullscreen(0);	
	//delete glcontext
	wglMakeCurrent( NULL, NULL );
	if(hGLCxt){
		DEBUG_ASSERT_REPLACE(wglDeleteContext(hGLCxt));
	}
	//destroy window
	if(hWind){	
		//Animate exit
		AnimateWindow( hWind, 200, AW_HIDE | AW_BLEND );
		//release device context
		if(hDevCxt){
			DEBUG_ASSERT_REPLACE(ReleaseDC(hWind,hDevCxt));
		}
		//destroy window
		DEBUG_ASSERT_REPLACE(DestroyWindow(hWind));
	}
	//unregister class
	DEBUG_ASSERT_REPLACE(UnregisterClass("Easy2DWindow",hInstance));
}
//
WindowsScreen::WindowsScreen()
	:Screen()
	,nativeWidth(0)
	,nativeHeight(0)
	,screenWidth(0)
	,screenHeight(0)
	,freamPerSecond(0)
	,fullscreen(false)
	,showmouse(true){

	setCursor(showmouse);
	nativeWidth = GetSystemMetrics( SM_CXSCREEN );
	nativeHeight = GetSystemMetrics( SM_CYSCREEN );

}
WindowsScreen::~WindowsScreen(){
	closeWindow();
}

/**
* enable or disable full screen
*/
void WindowsScreen::setFullscreen(bool prfullscreen){
	//set 
	fullscreen=prfullscreen;
	//set window style
	DWORD style = fullscreen ? (WS_POPUP | WS_VISIBLE) : E2D_WINDOW_STYLE;
	SetWindowLong(hWind, GWL_STYLE, style);

	if( !fullscreen ){
		//disable fullscreen
		ChangeDisplaySettings( NULL, 0 );	
		//window adjast
		RECT windowRect;
		windowRect.left=(long)0;					// Set Left Value To 0
		windowRect.right=(long)screenWidth;			// Set Right Value To Requested Width
		windowRect.top=(long)0;						// Set Top Value To 0
		windowRect.bottom=(long)screenHeight;		// Set Bottom Value To Requested Height
		DWORD dwstyle = E2D_WINDOW_STYLE;	
		AdjustWindowRect( &windowRect, dwstyle, true);
		//calc size window
		int rwidth = windowRect.right-windowRect.left;
		int rheight = windowRect.bottom-windowRect.top;
		//calc center
		int wlft = (GetSystemMetrics(SM_CXSCREEN) - rwidth) / 2;
		int wtop = (GetSystemMetrics(SM_CYSCREEN) - rheight) / 2;
		//reset rect
		InvalidateRect( NULL, NULL, false );
		//set window into center of screen
		SetWindowPos(hWind, 
			         HWND_NOTOPMOST,
					 wlft, wtop, 
					 rwidth, rheight, 
				     SWP_SHOWWINDOW);

	}
	else{
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
	}

	ShowCursor( !fullscreen );
}
/**
* return if fullscreen is enable return true
*/
bool WindowsScreen::isFullscreen(){
	return fullscreen;
}
/**
* return frame rate
*/
uint WindowsScreen::getFrameRate(){
	return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void WindowsScreen::acquireContext(){
	wglMakeCurrent(hDevCxt,hGLCxt);
}
/**
* swap the buffers
*/
void WindowsScreen::swap(){
	SwapBuffers( hDevCxt );
}
/**
* create window
*/
void WindowsScreen::createWindow(const char* appname, 
								uint width, 
								uint height,
								uint bites, 
								uint setFreamPerSecond,
								bool prfullscreen){
	DEBUG_ASSERT(appname);
	DEBUG_ASSERT(bites);
	DEBUG_MESSAGE( "createWindow Easy2D Win32" );
	
	//set values
	screenWidth=Math::min(nativeWidth,width);
	screenHeight=Math::min(nativeHeight,height);
	freamPerSecond=setFreamPerSecond;
	//create window
	__initWindow(appname,bites);
	setFullscreen(prfullscreen);
	//
}
/**
* close window
*/
void WindowsScreen::closeWindow(){
	__destroyWindow();
}
/**
* return screen width
*/
uint WindowsScreen::getWidth(){
	return screenWidth;
}
/**
* return screen height
*/
uint WindowsScreen::getHeight(){
	return screenHeight;
}
/**
* return screen native width
*/
uint WindowsScreen::getNativeWidth(){
	return nativeWidth;
}
/**
* return screen  native height
*/
uint WindowsScreen::getNativeHeight(){
	return nativeHeight;
}
/**
* return screen orientation
*/
WindowsScreen::Orientation WindowsScreen::getOrientation(){
	return Screen::LANDSCAPE_RIGHT;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void WindowsScreen::setCursor(bool show){
	showmouse=show;
	ShowCursor(showmouse);
}
/**
* return if cursor is shown or hidden
*/
bool WindowsScreen::getCursor(){
	return showmouse;
}