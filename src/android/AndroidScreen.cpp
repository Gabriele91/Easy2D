#include <AndroidScreen.h>
#include <AndroidMain.h>
///////////////////////
using namespace Easy2D;
///////////////////////
AndroidScreen::AndroidScreen()
	:nativeWidth(0)
	,nativeHeight(0)
	,screenWidth(0)
	,screenHeight(0)
	,freamPerSecond(0)
	,fullscreen(false)
	,showmouse(true){
    //get size screen
	nativeWidth =0;
	nativeHeight =0;
}
///////////////////////////////////////////////////////////
//typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
static bool isExtensionSupported(const char *extList, const char *extension){

  const char *start;
  const char *where, *terminator;

  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if ( where || *extension == '\0' )
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for ( start = extList; ; ) {
    where = strstr( start, extension );

    if ( !where )
      break;

    terminator = where + strlen( extension );

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}
///////////////////////////////////////////////////////////
void AndroidScreen::__initStateOpenGLES(){
	//get function pointer
    initOpenGLES();
	//enable culling
    glEnable( GL_CULL_FACE );        
    glCullFace( GL_BACK );        
    //default status for blending
    glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
    glBlendFunc( GL_ONE , GL_ZERO ); 
	//disable light
	glDisable(GL_LIGHTING);
	//enable texturing	
	glEnable( GL_TEXTURE_2D );
    //always active!
    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//find errors:
	CHECK_GPU_ERRORS();
}
void AndroidScreen::__createScreen(){
	 const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    }; 
    //SET ANDROID WINDOW
    EGLint _w, _h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    //get display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    DEBUG_ASSERT( display );
    eglInitialize(display, 0, 0);
    //set openGL configuration
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(getAndroidApp()->window, 0, 0, format);
    //create a surface, and openGL context	
    surface = eglCreateWindowSurface(display, config,getAndroidApp()->window, NULL);
	//openGL ES 1.0 //1, EGL_NONE
    const EGLint attrib_list [] = {EGL_CONTEXT_CLIENT_VERSION, 1,  EGL_NONE}; 
    context = eglCreateContext(display, config, NULL, attrib_list);
    DEBUG_ASSERT( context );
    //set corrunt openGL context
    DEBUG_ASSERT (eglMakeCurrent(display, surface, surface, context));
    //get WIDTH,HEIGHT
	EGLint eglWidth,eglHeight;
    eglQuerySurface(display, surface, EGL_WIDTH, &eglWidth);
    eglQuerySurface(display, surface, EGL_HEIGHT, &eglHeight);
	nativeWidth = screenWidth =eglWidth;
	nativeHeight = screenHeight =eglHeight;
	//show message
	DEBUG_MESSAGE( "Open window:" << screenWidth << "x" << screenHeight );
}
///////////////////////////////////////////////////////////
/**
* open a window
*/
void AndroidScreen::createWindow(const char* argappname,
                                  uint width,
                                  uint height,
                                  uint bites,
                                  uint freamPerSecond,
                                  bool fullscreen,
								  AntiAliasing dfAA){

    // appname=argappname;
    screenWidth= width;
    screenHeight= height;
    this->freamPerSecond=freamPerSecond;
	//init screen
	__createScreen();
	__initStateOpenGLES();
    //enable AA
    if(dfAA!=NOAA)
        glEnable( GL_MULTISAMPLE );
}
/**
* close window
*/
void AndroidScreen::closeWindow(){
	// and a cheesy fade exit
	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT) {
		    eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE) {
		    eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	//free ptrs
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}
/**
* AndroidScreen destructor
*/
AndroidScreen::~AndroidScreen(){
	closeWindow();
}
/**
* set fullscreen mode
*/
void AndroidScreen::setFullscreen(bool argfullscreen){
	//void
}
/**
* return screen width
*/
uint AndroidScreen::getWidth(){
	return screenWidth;
}
/**
* return screen height
*/
uint AndroidScreen::getHeight(){
	return screenHeight;
}
/**
* return screen native width
*/
uint AndroidScreen::getNativeWidth(){
	return nativeWidth;
}
/**
* return screen  native height
*/
uint AndroidScreen::getNativeHeight(){
	return nativeHeight;
}
/**
* return screen orientation
*/
AndroidScreen::Orientation AndroidScreen::getOrientation(){
	return Screen::LANDSCAPE_RIGHT;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void AndroidScreen::setCursor(bool show){
	showmouse=show;
	if(!showmouse){
	}
	else{
	}
}
/**
* set position cursor
*/
void AndroidScreen::setPositionCursor(const Vec2& pos){
}
/**
* return if cursor is shown or hidden
*/
bool AndroidScreen::getCursor(){
	return showmouse;
}
/**
* return if fullscreen is enable return true
*/
bool AndroidScreen::isFullscreen(){
	return fullscreen;
}
/**
* return frame rate
*/
uint AndroidScreen::getFrameRate(){
	return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void AndroidScreen::acquireContext(){
    // connect the glx-context to the window
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        DEBUG_MESSAGE("Error eglMakeCurrent");
    }
}
/**
* swap the buffers
*/
void AndroidScreen::swap(){
   eglSwapBuffers( display, surface);
}
