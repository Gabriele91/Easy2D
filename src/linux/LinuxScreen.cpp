#include <LinuxScreen.h>
///////////////////////
using namespace Easy2D;
///////////////////////

#define WINDOW_ATTRIBUTE ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
LinuxScreen::LinuxScreen()
	:nativeWidth(0)
	,nativeHeight(0)
	,screenWidth(0)
	,screenHeight(0)
	,freamPerSecond(0)
	,fullscreen(false)
	,showmouse(true){
	//connetect to display
    display =XOpenDisplay(NULL);
    DEBUG_ASSERT(display);
    //get size screen
	nativeWidth = XWidthOfScreen(XDefaultScreenOfDisplay(display));
	nativeHeight = XHeightOfScreen(XDefaultScreenOfDisplay(display));
}
///////////////////////////////////////////////////////////
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
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
void LinuxScreen::__createGLXContext(uint bites,AntiAliasing dfAA){
	///////////////////////////////////////////////////////////
    //SETUP openGL
    bitesOpenGL=bites<24?bites:24;
	///////////////////////////////////////////////////////////
    //SET BUFFERS
    int bufferOpenGL[]={  GLX_RGBA,                          //[0]
                          GLX_DEPTH_SIZE, bitesOpenGL,       //[1] [2]
                          GLX_DOUBLEBUFFER,                  //[3]
                          GLX_SAMPLE_BUFFERS  , 1,           //[4] [5] // <-- MSAA
                          GLX_SAMPLES         , dfAA,        //[6] [7] // <-- MSAA
                          None };
    //no msaa
    if(dfAA<MSAAx2||dfAA>MSAAx64)
        bufferOpenGL[4]=None;
    //setup color map
    visual  = glXChooseVisual(display, screen,  bufferOpenGL );
    if (visual  == NULL){
        bufferOpenGL[3]=None;//diable GLX_DOUBLEBUFFER
        visual  = glXChooseVisual(display, screen,  bufferOpenGL );
        doubleBuffered = false;
        DEBUG_MESSAGE("singlebuffered rendering will be used, no doublebuffering available");
    }
    else{
        doubleBuffered = true;
        DEBUG_MESSAGE("doublebuffered rendering available");
    }
    DEBUG_ASSERT(visual);
	///////////////////////////////////////////////////////////
	//get openGL version
	int glxMajor, glxMinor;
    glXQueryVersion(display, &glxMajor, &glxMinor);
	DEBUG_MESSAGE("openGL rendering :"<<glxMajor<<"."<<glxMinor);
    ///////////////////////////////////////////////////////////////////////
    // create a GLX context
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    ///////////////////////////////////////////////////////////////////////
    int n = 0, modeNum = 0;
    //Get a framebuffer config using the default attributes
    GLXFBConfig framebufferConfig = (*glXChooseFBConfig(display, DefaultScreen(display), 0, &n));
    ///////////////////////////////////////////////////////////////////////
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( display, DefaultScreen( display ) );
    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||   !glXCreateContextAttribsARB ){
        DEBUG_MESSAGE( "glXCreateContextAttribsARB() not found, old-style GLX context" );
        context = glXCreateNewContext( display, framebufferConfig, GLX_RGBA_TYPE, 0, True );
      }
      else{
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 1,  //[0] [1]
            GLX_CONTEXT_MINOR_VERSION_ARB, 4,  //[2] [3]
            None
        };
        //create context
        context = glXCreateContextAttribsARB( display,
                                              framebufferConfig ,
                                              0,
                                              GL_TRUE,
                                              context_attribs);
      }

    DEBUG_ASSERT(context);
	///////////////////////////////////////////////////////////
    //COLOR MAP WINDOW
    Colormap cmap;
    // create a color map
    cmap = XCreateColormap(display,
                           RootWindow(display, visual->screen),
                           visual ->visual,
                           AllocNone);
    //set att....
    winAttr.colormap = cmap;
    winAttr.border_pixel = 0;
	///////////////////////////////////////////////////////////
}
void LinuxScreen::__deleteGLXContext(){

    DEBUG_ASSERT(context);

	if( !glXMakeCurrent(display, None, NULL)){
		DEBUG_MESSAGE("Could not release drawing context.\n");
	}
	// destroy the context
	glXDestroyContext(display, context);
	context = NULL;

}
void LinuxScreen::__initOpenGL(){
    //init openGL2
    initOpenGL2();
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
void LinuxScreen::__createFullScreenWindow(){
        //set fullscreen=true
        fullscreen=true;
        //set fullscreen
        XF86VidModeModeInfo **modes;
        int modeNum, bestMode=-1;
        //get info
        XF86VidModeGetAllModeLines(display,screen, &modeNum, &modes);
        // save desktop-resolution before switching modes
        desktopMode = *modes[0];
        //look for mode with requested resolution
        for (int i = 0; i < modeNum; i++)
            if ((modes[i]->hdisplay == screenWidth) && (modes[i]->vdisplay == screenHeight))
                bestMode = i;
        //////////////////////////////////////////////////////////////////////////////
        DEBUG_ASSERT_MSG(bestMode!=-1,"error : full-screen resolution not supported");
        //////////////////////////////////////////////////////////////////////////////
        //witch to fullscreen
        XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
        XF86VidModeSetViewPort(display, screen, 0, 0);
        nativeWidth = modes[bestMode]->hdisplay;
        nativeHeight = modes[bestMode]->vdisplay;
        DEBUG_MESSAGE("resolution : "<<nativeWidth<<"x"<<nativeHeight);
        XFree(modes);
        //set window attributes
        winAttr.override_redirect = True;
        winAttr.event_mask = ExposureMask |
                             FocusChangeMask |
                             KeyPressMask |
                             KeyReleaseMask |
                             ButtonPressMask |
                             ButtonReleaseMask |
                             PointerMotionMask |
                             StructureNotifyMask |
                             WINDOW_ATTRIBUTE ;
        window = XCreateWindow(display,
                               RootWindow(display, visual ->screen),
                               0, 0,
                               nativeWidth,
                               nativeHeight, 0,
                               bitesOpenGL,
                               InputOutput,
                               visual->visual,
                               CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect | CWOverrideRedirect,
                               &(winAttr));
        XWarpPointer(display, None, window, 0, 0, 0, 0, 0, 0);
        XMapRaised(display, window);
        XGrabKeyboard(display, window, 1, GrabModeAsync,GrabModeAsync, CurrentTime);
        XGrabPointer(display, window, 1, ButtonPressMask,GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
}
void LinuxScreen::__createWindow(){
        //set fullscreen=false
        fullscreen=false;
        //create a window in window mode
        winAttr.override_redirect = True;
        winAttr.event_mask = ExposureMask |
                             FocusChangeMask |
                             KeyPressMask |
                             KeyReleaseMask |
                             ButtonPressMask |
                             ButtonReleaseMask |
                             PointerMotionMask |
                             StructureNotifyMask |
                             WINDOW_ATTRIBUTE ;
        window =
        XCreateWindow(display, RootWindow(display, visual ->screen),
                        0, 0,
                        screenWidth,
                        screenHeight,
                        0,
                        visual ->depth,
                        InputOutput,
                        visual ->visual,
                        CWBorderPixel | CWColormap | CWEventMask,
                        &(winAttr));
        //only set window title and handle wm_delete_events if in windowed mode
        Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", 1);
        XSetWMProtocols(display, window, &wmDelete, 1);
        XSetStandardProperties(display,
                               window,
                               appname,
                               appname,
                               None,
                               NULL,
                               0,
                               NULL);
        XMapRaised(display, window);
        //disable resize
        XSizeHints size_hints;
        size_hints.flags = PSize | PMinSize | PMaxSize;
        size_hints.min_height = size_hints.max_height = screenHeight;
        size_hints.min_width  = size_hints.max_width  = screenWidth;
        XSetWMNormalHints(display,window,&size_hints);
}
void LinuxScreen::__deleteWindow(){
    // switch back to original desktop resolution if we were in fullscreen
    if( fullscreen ){
        XF86VidModeSwitchToMode(display, screen, &(desktopMode));
        XF86VidModeSetViewPort(display, screen, 0, 0);
    }
    XCloseDisplay(display);
}
///////////////////////////////////////////////////////////
/**
* open a window
*/
void LinuxScreen::createWindow(const char* argappname,
                                  uint width,
                                  uint height,
                                  uint bites,
                                  uint freamPerSecond,
                                  bool fullscreen,
								  AntiAliasing dfAA){

    appname=argappname;
    screenWidth= width;
    screenHeight= height;
    this->freamPerSecond=freamPerSecond;
    //x11 values
    //get screen
    screen = DefaultScreen(display);
	//create openGL context
	__createGLXContext(bites,dfAA);
    //set fullscreen
    if(fullscreen)
		__createFullScreenWindow();
	else
		__createWindow();
    // connect the glx-context to the window
    glXMakeCurrent(display, window, context);
    //init openGL2
    __initOpenGL();
    //enable AA
    if(dfAA!=NOAA)
        glEnable( GL_MULTISAMPLE );
}
/**
* close window
*/
void LinuxScreen::closeWindow(){
	__deleteGLXContext();
	__deleteWindow();
}
/**
* LinuxScreen destructor
*/
LinuxScreen::~LinuxScreen(){
	closeWindow();
}
/**
* set fullscreen mode
*/
void LinuxScreen::setFullscreen(bool argfullscreen){
	if(fullscreen!=argfullscreen){
		__deleteWindow();
		if(fullscreen)
			__createFullScreenWindow();
		else
			__createWindow();
	}
}
/**
* return screen width
*/
uint LinuxScreen::getWidth(){
	return screenWidth;
}
/**
* return screen height
*/
uint LinuxScreen::getHeight(){
	return screenHeight;
}
/**
* return screen native width
*/
uint LinuxScreen::getNativeWidth(){
	return nativeWidth;
}
/**
* return screen  native height
*/
uint LinuxScreen::getNativeHeight(){
	return nativeHeight;
}
/**
* return screen orientation
*/
LinuxScreen::Orientation LinuxScreen::getOrientation(){
	return Screen::LANDSCAPE;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void LinuxScreen::setCursor(bool show){
	showmouse=show;
	if(!showmouse){
        Cursor invisibleCursor;
        Pixmap bitmapNoData;
        XColor black;
        static char noData[] = { 0,0,0,0,0,0,0,0 };
        black.red = black.green = black.blue = 0;

        bitmapNoData = XCreateBitmapFromData(display, window, noData, 8, 8);
        invisibleCursor = XCreatePixmapCursor(display, bitmapNoData, bitmapNoData,
                                             &black, &black, 0, 0);
        XDefineCursor(display,window, invisibleCursor);
        XFreeCursor(display, invisibleCursor);
	}
	else{
	    /*
	    Cursor cursor;
        cursor=XCreateFontCursor(display,XC_left_ptr);
        XDefineCursor(display, window, cursor);
        XFreeCursor(display, cursor);
	    */
        XUndefineCursor(display, window);
	}
}
/**
* set position cursor
*/
void LinuxScreen::setPositionCursor(const Vec2& pos){
    XWarpPointer(display, None, window, 0, 0, 0, 0, pos.x, pos.y);
    XSync(display, False);
    XFlush(display);
}
/**
* return if cursor is shown or hidden
*/
bool LinuxScreen::getCursor(){
	return showmouse;
}
/**
* return if fullscreen is enable return true
*/
bool LinuxScreen::isFullscreen(){
	return fullscreen;
}
/**
* return frame rate
*/
uint LinuxScreen::getFrameRate(){
	return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void LinuxScreen::acquireContext(){
    // connect the glx-context to the window
    glXMakeCurrent(display, window, context);
}
/**
* swap the buffers
*/
void LinuxScreen::swap(){
	glXSwapBuffers( display, window );
}
