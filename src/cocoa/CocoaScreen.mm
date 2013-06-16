//
//  CocoaScreen.mm
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#include <CocoaScreen.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSScreen.h>

using namespace Easy2D;
#define TOCOCOAWINDOW NSWindow * window=(NSWindow *)(cocoaWindow);
#define TOCOCOACONTEXT NSOpenGLContext * openGLContext=(NSOpenGLContext *)(cocoaGLContext);


///////////////////////////////////////////////////////////////////////////////
// WINDOW CLASS
@interface  Easy2DWindow : NSWindow
/* These are needed for borderless/fullscreen windows */
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
@end

@implementation Easy2DWindow
- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}
@end
///////////////////////////////////////////////////////////////////////////////
// VIEW CLASS
@interface Easy2DView : NSView
// The default implementation doesn't pass rightMouseDown to responder chain
- (void)rightMouseDown:(NSEvent *)theEvent;
@end

@implementation Easy2DView
- (void)rightMouseDown:(NSEvent *)theEvent
{
	[[self nextResponder] rightMouseDown:theEvent];
}
@end
///////////////////////////////////////////////////////////////////////////////


void CocoaScreen::__openWindow(int w,int h,const char *title){
    
    //GC
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //size window
    NSRect frame;
    frame.origin.x = 0;
    frame.origin.y = 0;
    frame.size.width = w;
    frame.size.height = h;
	
    //window
    NSWindow * window = [[Easy2DWindow alloc]
                          initWithContentRect: frame
                          styleMask: (NSTitledWindowMask | NSMiniaturizableWindowMask | NSClosableWindowMask)
                          backing: NSBackingStoreBuffered
                          defer: false];
	//disable GC
	[window setReleasedWhenClosed:false];
	[window center];
	[window makeKeyAndOrderFront:nil];
    //  Set content view
	NSView *contentView = [[Easy2DView alloc] initWithFrame:frame];
	[window setContentView: contentView];
	[contentView release];
    //set title
    NSString *titleNS = [NSString stringWithUTF8String:title];
    [window setTitle:titleNS];
    //GC release
	[pool release];
    
    //set screen size
    screenWidth=w;
    screenHeight=h;
    
    //save object
    cocoaWindow=(void*)window;
    
}
void CocoaScreen::__closeWindow(){
    
    TOCOCOAWINDOW
    [window release];

}
void CocoaScreen::__createContext(int msaa){
    //COCOA OPENGL CONTEXT
    NSOpenGLContext *openGLContext=NULL;
        
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFADoubleBuffer,	// double buffered
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)16, // 16 bit depth buffer
		
		//msaa
		//NSOpenGLPFASampleBuffers, 1,
		//NSOpenGLPFASamples, static_cast<NSOpenGLPixelFormatAttribute>(msaa),
        
		NSOpenGLPFANoRecovery,		
        (NSOpenGLPixelFormatAttribute)nil
    };
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    DEBUG_ASSERT(pixelFormat);

    openGLContext = [[NSOpenGLContext alloc]  initWithFormat:pixelFormat shareContext:nil];
    DEBUG_ASSERT(openGLContext);
    
    //swap interval    
    GLint value = 1;
    [openGLContext setValues:&value forParameter:NSOpenGLCPSwapInterval];
    
    //dealloc
    [pixelFormat release];
    
    /////////////////////////
    //return
    cocoaGLContext=(void*)openGLContext;


}
void CocoaScreen::__deleteContext(){
    TOCOCOACONTEXT
    [NSOpenGLContext clearCurrentContext];
    [openGLContext clearDrawable];
    [openGLContext release];
}

void CocoaScreen::__initOpenGL(){
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
/**
 * cocoa screen
 */
CocoaScreen::CocoaScreen()
:nativeWidth(0)
,nativeHeight(0)
,screenWidth(0)
,screenHeight(0)
,freamPerSecond(0)
,fullscreen(false)
,showmouse(true)
{
    NSRect screenRect;
    NSArray *screens=[NSScreen screens];
    unsigned cntScreens=[screens count];
    for(int i=0;i<cntScreens;++i){
        screenRect=[[screens objectAtIndex:i] visibleFrame];
    }
    nativeWidth=screenRect.size.width;
    nativeHeight=screenRect.size.height;
}
CocoaScreen::~CocoaScreen(){
    __deleteContext();
    __closeWindow();
}

/**
 * return screen width
 */
uint CocoaScreen::getWidth(){
	return screenWidth;
}
/**
 * return screen height
 */
uint CocoaScreen::getHeight(){
	return screenHeight;
}
/**
 * return screen native width
 */
uint CocoaScreen::getNativeWidth(){
	return nativeWidth;
}
/**
 * return screen  native height
 */
uint CocoaScreen::getNativeHeight(){
	return nativeHeight;
}
/**
 * return screen orientation
 */
Screen::Orientation CocoaScreen::getOrientation(){
	return Screen::LANDSCAPE;
}
/**
 * return frame rate
 */
uint CocoaScreen::getFrameRate(){
	return freamPerSecond;    
}

/**
 * set the specified thread's current rendering context
 */
void CocoaScreen::acquireContext(){
    
    TOCOCOACONTEXT
    
	[openGLContext makeCurrentContext];
    
}
/**
 * swap the buffers
 */
void CocoaScreen::swap(){
    
    TOCOCOACONTEXT
    
	[openGLContext flushBuffer];
    
}
/**
 * show or hide mouse cursor
 * @param show, set true if you want show cursor otherwise false
 */
void CocoaScreen::setCursor(bool show){

}
/**
 * set position cursor
 */
void CocoaScreen::setPositionCursor(const Vec2& pos){

}
/**
 * return if cursor is shown or hidden
 */
bool CocoaScreen::getCursor(){
	return showmouse;
}
/**
 * enable or disable full screen
 */
void CocoaScreen::setFullscreen(bool fullscreen){
}
/**
 * return if fullscreen is enable return true
 */
bool CocoaScreen::isFullscreen(){
	return fullscreen;
}

/**
 * create window
 */
void CocoaScreen::createWindow(const char* appname,
                          uint width,
                          uint height,
                          uint bites,
                          uint freamPerSecond,
                          bool fullscreen,
                          AntiAliasing dfAA){
    this->fullscreen=fullscreen;
    this->cocoaGLContext=NULL;
    this->cocoaWindow=NULL;
    this->freamPerSecond=freamPerSecond;
    //open window
    __openWindow(width,height,appname);
    //create context
    __createContext(dfAA);
    //set context to window   
    TOCOCOAWINDOW
    TOCOCOACONTEXT
    [openGLContext setView:[window contentView]];
	[openGLContext update];
    //set context
    acquireContext();    
    //init openGL2
    __initOpenGL();
    //enable AA
    if(dfAA!=NOAA)
        glEnable( GL_MULTISAMPLE );
}
/*
 * close window
 */
void CocoaScreen::closeWindow(){
    __closeWindow();    
}