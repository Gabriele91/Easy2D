#include <AndroidScreen.h>
#include <AndroidMain.h>
#include <RenderContext.h>
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
    ,showmouse(true)
{
    //get size screen
    nativeWidth =0;
    nativeHeight =0;
}
///////////////////////////////////////////////////////////
void eglPrintError(EGLint eg)
{
    String err;
    switch(eg)
    {
    case EGL_SUCCESS:
    case EGL_NONE:
        return;

    case EGL_NOT_INITIALIZED:
        err = "EGL_NOT_INITIALIZED";
        break;
    case EGL_BAD_ACCESS:
        err = "EGL_BAD_ACCESS";
        break;
    case EGL_BAD_ALLOC:
        err = "EGL_BAD_ALLOC";
        break;
    case EGL_BAD_ATTRIBUTE:
        err = "EGL_BAD_ATTRIBUTE";
        break;
    case EGL_BAD_CONTEXT:
        err = "EGL_BAD_CONTEXT";
        break;
    case EGL_BAD_CONFIG:
        err = "EGL_BAD_CONFIG";
        break;
    case EGL_BAD_CURRENT_SURFACE:
        err = "EGL_BAD_CURRENT_SURFACE";
        break;
    case EGL_BAD_DISPLAY:
        err = "EGL_BAD_DISPLAY";
        break;
    case EGL_BAD_SURFACE:
        err = "EGL_BAD_SURFACE";
        break;
    case EGL_BAD_MATCH:
        err = "EGL_BAD_MATCH";
        break;
    case EGL_BAD_PARAMETER:
        err = "EGL_BAD_PARAMETER";
        break;
    case EGL_BAD_NATIVE_PIXMAP:
        err = "EGL_BAD_NATIVE_PIXMAP";
        break;
    case EGL_BAD_NATIVE_WINDOW:
        err = "EGL_BAD_NATIVE_WINDOW";
        break;
    case EGL_CONTEXT_LOST:
        err = "EGL_CONTEXT_LOST";
        break;
    };
    DEBUG_MESSAGE( "EGL Error : " << err );
}

///////////////////////////////////////////////////////////
static const EGLint attribsEGL[] =
{
    //type surface
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    //buffer type
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    //zbuffer
    EGL_DEPTH_SIZE, 16,
    //context type
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    /* FSAA
    EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
    EGL_SAMPLES, 4,
    */
    EGL_NONE
};
const EGLint attrib_listEGL [] =
{
    EGL_CONTEXT_CLIENT_VERSION, 2, //OpenGL ES 2
    EGL_NONE
};
//FROM TEGRA SDK
static bool defaultEGLChooser(EGLDisplay disp, EGLConfig& bestConfig)
{

    EGLint count = 0;
    if (!eglGetConfigs(disp, NULL, 0, &count))
    {
        DEBUG_MESSAGE("defaultEGLChooser cannot query count of all configs");
        return false;
    }

    DEBUG_MESSAGE("Config count = "<< count);

    EGLConfig* configs = new EGLConfig[count];
    if (!eglGetConfigs(disp, configs, count, &count))
    {
        DEBUG_MESSAGE("defaultEGLChooser cannot query all configs");
        return false;
    }

    int bestMatch = 1<<30;
    int bestIndex = -1;

    int i;
    for (i = 0; i < count; i++)
    {
        int match = 0;
        EGLint surfaceType = 0;
        EGLint blueBits = 0;
        EGLint greenBits = 0;
        EGLint redBits = 0;
        EGLint alphaBits = 0;
        EGLint depthBits = 0;
        EGLint stencilBits = 0;
        EGLint renderableFlags = 0;

        eglGetConfigAttrib(disp, configs[i], EGL_SURFACE_TYPE, &surfaceType);
        eglGetConfigAttrib(disp, configs[i], EGL_BLUE_SIZE, &blueBits);
        eglGetConfigAttrib(disp, configs[i], EGL_GREEN_SIZE, &greenBits);
        eglGetConfigAttrib(disp, configs[i], EGL_RED_SIZE, &redBits);
        eglGetConfigAttrib(disp, configs[i], EGL_ALPHA_SIZE, &alphaBits);
        eglGetConfigAttrib(disp, configs[i], EGL_DEPTH_SIZE, &depthBits);
        eglGetConfigAttrib(disp, configs[i], EGL_STENCIL_SIZE, &stencilBits);
        eglGetConfigAttrib(disp, configs[i], EGL_RENDERABLE_TYPE, &renderableFlags);
        DEBUG_MESSAGE("Config["<<i<<"]: R"<<redBits<<
                                       "G"<<greenBits<<
                                       "B"<<blueBits<<
                                       "A"<<alphaBits<<
                                       " D"<<depthBits<<
                                       "S"<<stencilBits<<
                                       " Type="<<surfaceType<<
                                       " Render="<<renderableFlags);

        if ((surfaceType & EGL_WINDOW_BIT) == 0)
            continue;
        if ((renderableFlags & EGL_OPENGL_ES2_BIT) == 0)
            continue;
        if (depthBits < 16)
            continue;
        if ((redBits < 5) || (greenBits < 6) || (blueBits < 5))
            continue;

        int penalty = depthBits - 16;
        match += penalty * penalty;
        penalty = redBits - 5;
        match += penalty * penalty;
        penalty = greenBits - 6;
        match += penalty * penalty;
        penalty = blueBits - 5;
        match += penalty * penalty;
        penalty = alphaBits;
        match += penalty * penalty;
        penalty = stencilBits;
        match += penalty * penalty;

        if ((match < bestMatch) || (bestIndex == -1))
        {
            bestMatch = match;
            bestIndex = i;
            DEBUG_MESSAGE("Config["<<i<<"] is the new best config "<<configs[i]);
        }
    }

    if (bestIndex < 0)
    {
        delete[] configs;
        return false;
    }
    
    DEBUG_MESSAGE("Select Config["<<bestIndex<<"]");
    bestConfig = configs[bestIndex];
    delete[] configs;

    return true; 
}

void AndroidScreen::__setupScreen()
{
    //SET ANDROID WINDOW
    EGLint _w, _h, dummy, format;
    //get display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    DEBUG_ASSERT( display );
    eglInitialize(display, 0, 0);
    //set OpenGL ES2 configuration
#if 0
    //eglChooseConfig(display, attribsEGL, &config, 1, &numConfigs);
    //eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
#else
    //select default
    DEBUG_ASSERT(defaultEGLChooser(display,config));
#endif

    ANativeWindow_setBuffersGeometry(getAndroidApp()->window, 0, 0, format);
}
void AndroidScreen::__createSurface()
{
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
void AndroidScreen::__createContext()
{
    //create gl context
    //OpenGL ES 2.0 //1, EGL_NONE
    context = eglCreateContext(display, config, NULL, attrib_listEGL);
    DEBUG_ASSERT( context );
}
void AndroidScreen::__isResized()
{
    //get WIDTH,HEIGHT
    EGLint eglWidth,eglHeight;
    eglQuerySurface(display, surface, EGL_WIDTH, &eglWidth);
    eglQuerySurface(display, surface, EGL_HEIGHT, &eglHeight);
    nativeWidth = screenWidth =eglWidth;
    nativeHeight = screenHeight =eglHeight;

    switch(getOrientation())
    {
    case PORTRAIT:
    case PORTRAIT_REVERSE:
    case SENSOR_PORTRAIT:
        nativeWidth = screenWidth = Math::min(eglWidth,eglHeight);
        nativeHeight = screenHeight = Math::max(eglWidth,eglHeight);
        break;
    case LANDSCAPE:
    case LANDSCAPE_REVERSE:
    case SENSOR_LANDSCAPE:
        nativeWidth = screenWidth = Math::max(eglWidth,eglHeight);
        nativeHeight = screenHeight = Math::min(eglWidth,eglHeight);
        break;
    }

    DEBUG_MESSAGE( "Resized surface:" << screenWidth << "x" << screenHeight );
}

void AndroidScreen::__disableContext()
{
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}
void AndroidScreen::__deleteContext()
{
    if (context != EGL_NO_CONTEXT)
    {
        //delete context
        RenderContext::releaseContext();
        //delete OpenGL Context
        eglDestroyContext(display, context);
        context = EGL_NO_CONTEXT;
    }
}
void AndroidScreen::__deleteSurface()
{
    if (surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(display, surface);
        surface = EGL_NO_SURFACE;
    }
}

bool AndroidScreen::__isAValidContext()
{
    //get is a invalid context
    if(context == EGL_NO_CONTEXT) return false;
    if(!eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        //start found erros
        bool eglLostContext=false;
        //get errors
        for (EGLint eg = eglGetError(); eg != EGL_SUCCESS && eg != EGL_NONE ; eg = eglGetError())
        {
            eglPrintError(eg);
            eglLostContext|= eg == EGL_CONTEXT_LOST ;
            eglLostContext|= eg == EGL_BAD_CONTEXT ;
        }
        //if is lost
        if( eglLostContext )
        {
            eglDestroyContext(display, context);
            context = EGL_NO_CONTEXT;
            return false;
        }
    }
    //
    return true;
}
void AndroidScreen::__initStateOpenGLES()
{
    //get function pointer
    initOpenGLES();
    //find errors:
    CHECK_GPU_ERRORS();
    //set default state
    RenderContext::initContext();
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
                                 AntiAliasing dfAA)
{

    // appname=argappname;
    // screenWidth= width;  //ignore
    // screenHeight= height;//ignore
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
    #ifndef OPENGL_ES2
    if(dfAA!=NOAA)
    {
        glEnable( GL_MULTISAMPLE );
    }
    #endif
}
/**
* close window
*/
void AndroidScreen::closeWindow()
{
    // and a cheesy fade exit
    if (display != EGL_NO_DISPLAY)
    {
        //destroy
        __disableContext();
        __deleteContext();
        __deleteSurface();
        //terminate
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
}

/**
* AndroidScreen destructor
*/
AndroidScreen::~AndroidScreen()
{
    closeWindow();
}
/**
* set fullscreen mode
*/
void AndroidScreen::setFullscreen(bool argfullscreen)
{
    //void
}
/**
* return screen width
*/
uint AndroidScreen::getWidth()
{
    return screenWidth;
}
/**
* return screen height
*/
uint AndroidScreen::getHeight()
{
    return screenHeight;
}
/**
* return screen native width
*/
uint AndroidScreen::getNativeWidth()
{
    return nativeWidth;
}
/**
* return screen  native height
*/
uint AndroidScreen::getNativeHeight()
{
    return nativeHeight;
}
/**
* return screen orientation
*/
AndroidScreen::Orientation AndroidScreen::getOrientation()
{
    AndroidScreen::Orientation  orientation=PORTRAIT;
    switch(getAndroidScreenOrientation())
    {
    case 0:
        orientation=PORTRAIT;
        break;
    case 1:
        orientation=PORTRAIT_REVERSE;
        break;

    case 2:
        orientation=LANDSCAPE;
        break;
    case 3:
        orientation=LANDSCAPE_REVERSE;
        break;

    case 4:
        orientation=SENSOR_PORTRAIT;
        break;
    case 5:
        orientation=SENSOR_LANDSCAPE;
        break;
    }
    return orientation;
}
/**
* set screen orientation
*/
void AndroidScreen::setOrientation(Orientation orientation)
{
    switch(orientation)
    {
    case PORTRAIT:
        setAndroidScreenOrientation(0);
        break;
    case PORTRAIT_REVERSE:
        setAndroidScreenOrientation(1);
        break;
    case LANDSCAPE:
        setAndroidScreenOrientation(2);
        break;
    case LANDSCAPE_REVERSE:
        setAndroidScreenOrientation(3);
        break;
    case SENSOR_PORTRAIT:
        setAndroidScreenOrientation(4);
        break;
    case SENSOR_LANDSCAPE:
        setAndroidScreenOrientation(5);
        break;
    }
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void AndroidScreen::setCursor(bool show)
{
    showmouse=show;
    if(!showmouse)
    {
    }
    else
    {
    }
}
/**
* set position cursor
*/
void AndroidScreen::setPositionCursor(const Vec2& pos)
{
}
/**
* return if cursor is shown or hidden
*/
bool AndroidScreen::getCursor()
{
    return showmouse;
}
/**
* return if fullscreen is enable return true
*/
bool AndroidScreen::isFullscreen()
{
    return fullscreen;
}
/**
* return frame rate
*/
uint AndroidScreen::getFrameRate()
{
    return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void AndroidScreen::acquireContext()
{
    // connect the glx-context to the window
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
    {
        DEBUG_MESSAGE("Error eglMakeCurrent");
    }
}
/**
* swap the buffers
*/
void AndroidScreen::swap()
{
    eglSwapBuffers( display, surface);
}
