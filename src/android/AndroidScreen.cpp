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
void eglPrintError(EGLint eg){
	String err;
	switch(eg)
	{
		case EGL_SUCCESS: case EGL_NONE: return;

		case EGL_NOT_INITIALIZED:        err = "EGL_NOT_INITIALIZED";        break;
		case EGL_BAD_ACCESS:             err = "EGL_BAD_ACCESS";             break;
		case EGL_BAD_ALLOC:              err = "EGL_BAD_ALLOC";              break;
		case EGL_BAD_ATTRIBUTE:          err = "EGL_BAD_ATTRIBUTE";          break;
		case EGL_BAD_CONTEXT:            err = "EGL_BAD_CONTEXT";            break;
		case EGL_BAD_CONFIG:             err = "EGL_BAD_CONFIG";             break;
		case EGL_BAD_CURRENT_SURFACE:    err = "EGL_BAD_CURRENT_SURFACE";    break;
		case EGL_BAD_DISPLAY:            err = "EGL_BAD_DISPLAY";            break;
		case EGL_BAD_SURFACE:            err = "EGL_BAD_SURFACE";            break;
		case EGL_BAD_MATCH:              err = "EGL_BAD_MATCH";              break;
		case EGL_BAD_PARAMETER:          err = "EGL_BAD_PARAMETER";          break;
		case EGL_BAD_NATIVE_PIXMAP:      err = "EGL_BAD_NATIVE_PIXMAP";      break;
		case EGL_BAD_NATIVE_WINDOW:      err = "EGL_BAD_NATIVE_WINDOW";      break;
		case EGL_CONTEXT_LOST:           err = "EGL_CONTEXT_LOST";           break;
	};
	DEBUG_MESSAGE( "EGL Error : " << err );
}

///////////////////////////////////////////////////////////
static const EGLint attribsEGL[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_NONE
}; 
const EGLint attrib_listEGL [] = {
	EGL_CONTEXT_CLIENT_VERSION, 1,  
	EGL_NONE
}; 

void AndroidScreen::__setupScreen(){
    //SET ANDROID WINDOW
    EGLint _w, _h, dummy, format;
	//get display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    DEBUG_ASSERT( display );
    eglInitialize(display, 0, 0);
    //set openGL configuration
    eglChooseConfig(display, attribsEGL, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(getAndroidApp()->window, 0, 0, format);
}
void AndroidScreen::__createSurface(){   
    //create a surface, and openGL context	
    surface = eglCreateWindowSurface(display, config,getAndroidApp()->window, NULL);
    //get WIDTH,HEIGHT
	EGLint eglWidth,eglHeight;
    eglQuerySurface(display, surface, EGL_WIDTH, &eglWidth);
    eglQuerySurface(display, surface, EGL_HEIGHT, &eglHeight);
	nativeWidth = screenWidth =eglWidth;
	nativeHeight = screenHeight =eglHeight;
	//show message
	DEBUG_MESSAGE( "Open surface:" << screenWidth << "x" << screenHeight );
}
void AndroidScreen::__createContext(){
	//create gl context
	//openGL ES 1.0 //1, EGL_NONE
	context = eglCreateContext(display, config, NULL, attrib_listEGL);
	DEBUG_ASSERT( context );
}

bool AndroidScreen::__isAValidContext(){
	//get is a invalid context
	if(context == EGL_NO_CONTEXT) return false;
	if(!eglMakeCurrent(display, surface, surface, context) == EGL_FALSE){	
		//start found erros
		bool eglLostContext=false;
		//get errors
		for (EGLint eg = eglGetError(); eg != EGL_SUCCESS && eg != EGL_NONE ; eg = eglGetError()){
			eglPrintError(eg);
			eglLostContext|= eg == EGL_CONTEXT_LOST ;
			eglLostContext|= eg == EGL_BAD_CONTEXT ;
		}
		//if is lost
		if( eglLostContext ){		
			eglDestroyContext(display, context);
			context = EGL_NO_CONTEXT;	
			return false;
		}
	}
	//
	return true;
}
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
	//get window, 
	//create surface
	//create opengl es context
	__setupScreen();
	__createSurface();
	__createContext();
	//init opengl
	acquireContext();
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
	switch(AConfiguration_getOrientation(getAndroidApp()->config)){
		case ACONFIGURATION_ORIENTATION_PORT: return Orientation::PORTRAIT;
		case ACONFIGURATION_ORIENTATION_LAND: return Orientation::LANDSCAPE_RIGHT;
		default: return Orientation::LANDSCAPE_RIGHT;
	}
}
/**
* set screen orientation
*/
void AndroidScreen::setOrientation(Orientation orientation){
		switch(orientation){
			case PORTRAIT:
			case PORTRAIT_REVERSE:
				AConfiguration_setOrientation(getAndroidApp()->config, ACONFIGURATION_ORIENTATION_PORT);
			break;

			case LANDSCAPE_LEFT:
			case LANDSCAPE_RIGHT:
				AConfiguration_setOrientation(getAndroidApp()->config, ACONFIGURATION_ORIENTATION_LAND);			
			break;
		}
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
