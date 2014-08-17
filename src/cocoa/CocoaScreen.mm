//
//  CocoaScreen.mm
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#include <CocoaScreen.h>
#include <Cocoa/Cocoa.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSScreen.h>
#include <RenderContext.h>
using namespace Easy2D;
#define TOCOCOAWINDOW Easy2DWindow * window=(Easy2DWindow *)(cocoaWindow);
#define RELOADCOCOAWINDOW  window=(Easy2DWindow *)(cocoaWindow);
#define TOCOCOACONTEXT NSOpenGLContext * openGLContext=(NSOpenGLContext *)(cocoaGLContext);
#define RELOADTOCOCOACONTEXT  openGLContext=(NSOpenGLContext *)(cocoaGLContext);
#define TOCOCOAINFO CocoaInfo *cocoaInfo=(CocoaInfo*)this->cocoaInfo;


///////////////////////////////////////////////////////////////////////////////
// WINDOW CLASS
@interface  Easy2DWindow : NSWindow
{
    @public
    bool enableSendAppOnClose;
}

- (id) init;
//! These are needed for borderless/fullscreen windows 
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
//! In addition to closing the window, also terminate the app.
- (void) close;
//! @return yes
- (BOOL) acceptsFirstResponder;
@end

@implementation Easy2DWindow

- (id) init{
    self=[super init];
    enableSendAppOnClose=true;
    return self;
}

- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}

- (BOOL) acceptsFirstResponder {
    return YES;
}

- (void) close {
    //close app
    if(enableSendAppOnClose)
        [NSApp terminate:self];    
    // If the app refused to terminate, this window should still close.
    [super close];
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
/*
NSScreen* getNSScreen(){
    // CG -> Cocoa mumbo jumbo, for getting the proper screen
    NSDictionary* screenDictionary = [[NSScreen mainScreen] deviceDescription];
    NSNumber* screenID = [screenDictionary objectForKey:@"NSScreenNumber"];
    CGDirectDisplayID display = [screenID unsignedIntValue];
    
    
    NSArray *screens = [NSScreen screens];
    size_t i, count = [screens count];
    for (i = 0; i != count; i++) {
        CGDirectDisplayID id =
        (CGDirectDisplayID)[[[[screens objectAtIndex:i] deviceDescription] objectForKey:@"NSScreenNumber"] longValue];
        if (id == display) break;
    }
    if (i >= count) i = 0;
    
    return [screens objectAtIndex:i];
    
}
*/

struct CocoaInfo
{
    CGDisplayModeRef saveMode;
    CGDisplayModeRef fullscreenMode;
};
struct ScreenMode
{
    size_t width;
    size_t height;
    size_t bitsPerPixel;
};

size_t displayBitsPerPixelForMode(CGDisplayModeRef mode)
{
	
	size_t depth = 0;
	
	CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
	if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 32;
    else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        depth = 16;
    else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        depth = 8;
    
    return depth;
}
CGDisplayModeRef bestMatchForMode( ScreenMode screenMode )
{
	
	bool exactMatch = false;
	
    // Get a copy of the current display mode
	CGDisplayModeRef displayMode = CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
	
    // Loop through all display modes to determine the closest match.
    // CGDisplayBestModeForParameters is deprecated on 10.6 so we will emulate it's behavior
    // Try to find a mode with the requested depth and equal or greater dimensions first.
    // If no match is found, try to find a mode with greater depth and same or greater dimensions.
    // If still no match is found, just use the current mode.
    CFArrayRef allModes = CGDisplayCopyAllDisplayModes(kCGDirectMainDisplay, NULL);
    for(int i = 0; i < CFArrayGetCount(allModes); i++)
    {
		CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(allModes, i);
        
		if(displayBitsPerPixelForMode( mode ) != screenMode.bitsPerPixel)
			continue;
		
		if((CGDisplayModeGetWidth(mode) >= screenMode.width) &&
           (CGDisplayModeGetHeight(mode) >= screenMode.height) &&
           (CGDisplayModeGetWidth(displayMode) >= CGDisplayModeGetWidth(mode))&&
           (CGDisplayModeGetHeight(displayMode) >= CGDisplayModeGetHeight(mode)))
		{
			displayMode = mode;
			exactMatch = true;
		}
	}
	
    // No depth match was found
    if(!exactMatch)
	{
		for(int i = 0; i < CFArrayGetCount(allModes); i++)
		{
			CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(allModes, i);
			if(displayBitsPerPixelForMode( mode )  >= screenMode.bitsPerPixel)
				continue;
			
            
            if((CGDisplayModeGetWidth(mode) >= screenMode.width) &&
               (CGDisplayModeGetHeight(mode) >= screenMode.height) &&
               (CGDisplayModeGetWidth(displayMode) >= CGDisplayModeGetWidth(mode))&&
               (CGDisplayModeGetHeight(displayMode) >= CGDisplayModeGetHeight(mode)))
			{
				displayMode = mode;
			}
		}
	}
    
	return displayMode;
}
size_t currentDisplayBitsPerPixel(){
    return displayBitsPerPixelForMode(CGDisplayCopyDisplayMode(kCGDirectMainDisplay));
}


void CocoaScreen::__openWindow(int w,int h,const char *title,bool fullscreen)
{
    
    //GC
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //size window
    NSRect frame;
    //mask window
    int mask;
	//save info
    this->fullscreen=fullscreen;
    if(fullscreen)
    {
        frame=[[NSScreen mainScreen] frame];
        mask=NSBorderlessWindowMask ;
    }
    else
    {
        frame.origin.x = 0;
        frame.origin.y = 0;
        frame.size.width = w;
        frame.size.height = h;
        mask=NSTitledWindowMask | NSMiniaturizableWindowMask | NSClosableWindowMask;
    }
    
    //window
    NSWindow * window = [[Easy2DWindow alloc]
                         initWithContentRect: frame
                         styleMask: mask
                         backing: NSBackingStoreBuffered
                         defer: false];
    
    //set center of screen
    if(!fullscreen)
    {
        [window center];
    }
    else
    {
        // Set the window level to be above the menu bar
        [window setLevel:NSMainMenuWindowLevel+1];
        // Perform any other window configuration you desire
        [window setOpaque:YES];
        [window setHidesOnDeactivate:YES];
    }
    //  Set view
	NSView *contentView = [[Easy2DView alloc] initWithFrame:frame];
	[window setContentView: contentView];
    [window makeFirstResponder:contentView];
	[contentView release];
    if(fullscreen){
        [contentView setNeedsDisplay:YES];
    }
    //set title
	[window setReleasedWhenClosed:false];
    NSString *titleNS = [NSString stringWithUTF8String:title];
    [window setTitle:titleNS];
    
    //keyboard
	[window makeKeyAndOrderFront:nil];
    
    //GC release
	[pool release];
    
    //set screen size
    screenWidth=frame.size.width;
    screenHeight=frame.size.height;
    
    //save object
    cocoaWindow=(void*)window;
    
    //add listener
    if(onCocoaWindowCreated)
        onCocoaWindowCreated(cocoaWindow);
    
}
void CocoaScreen::__closeWindow()
{
    
    TOCOCOAWINDOW
    [window close];
    
    //delete listener
    if(onCocoaWindowClose)
        onCocoaWindowClose(cocoaWindow);
    
    [window release];
    

}
void CocoaScreen::__createContext(int msaa)
{
    //COCOA OPENGL CONTEXT
    NSOpenGLContext *openGLContext=NULL;
    
    NSOpenGLPixelFormatAttribute attributes[32];    
    int i=0;
    attributes[i++] = NSOpenGLPFANoRecovery;
    //attributes[i++] = NSOpenGLPFAAccelerated;
    attributes[i++] = NSOpenGLPFADoubleBuffer;
    attributes[i++] = NSOpenGLPFAColorSize;
    attributes[i++] = 32.0;
    attributes[i++] = NSOpenGLPFADepthSize;
    attributes[i++] = 32.0;
    //msaa
    if(msaa!=NOAA){
        attributes[i++]=NSOpenGLPFASampleBuffers;
        attributes[i++]=1;
        attributes[i++]=NSOpenGLPFASamples;
        attributes[i++]=static_cast<NSOpenGLPixelFormatAttribute>(msaa);
    }
    
    attributes[i] = 0;
    
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
void CocoaScreen::__deleteContext()
{
    TOCOCOACONTEXT
    [NSOpenGLContext clearCurrentContext];
    [openGLContext clearDrawable];
    [openGLContext release];
}
void CocoaScreen::__initOpenGL()
{
    //set default state
    RenderContext::setDefaultRenderState();
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
,onCocoaWindowCreated(NULL)
{
    NSRect screenRect;
    NSArray *screens=[NSScreen screens];
    size_t cntScreens=[screens count];
    for(size_t i=0;i!=cntScreens;++i){
        screenRect=[[screens objectAtIndex:i] frame];
    }
    nativeWidth=screenRect.origin.x+screenRect.size.width;
    nativeHeight=screenRect.origin.y+screenRect.size.height;
    //alloc hide info
    cocoaInfo=malloc(sizeof(CocoaInfo));
}
CocoaScreen::~CocoaScreen(){
    __deleteContext();
    __closeWindow();
    //dealloc info
    free(cocoaInfo);
}

/**
 * return screen width
 */
uint CocoaScreen::getWidth()
{
	return screenWidth;
}
/**
 * return screen height
 */
uint CocoaScreen::getHeight()
{
	return screenHeight;
}
/**
 * return screen native width
 */
uint CocoaScreen::getNativeWidth()
{
	return nativeWidth;
}
/**
 * return screen  native height
 */
uint CocoaScreen::getNativeHeight()
{
	return nativeHeight;
}
/**
 * return screen orientation
 */
Screen::Orientation CocoaScreen::getOrientation()
{
	return Screen::LANDSCAPE;
}
/**
 * return frame rate
 */
uint CocoaScreen::getFrameRate()
{
	return freamPerSecond;    
}

/**
 * set the specified thread's current rendering context
 */
void CocoaScreen::acquireContext()
{
    
    TOCOCOACONTEXT
    
	[openGLContext makeCurrentContext];
}
/**
 * swap the buffers
 */
void CocoaScreen::swap()
{
    //TOCOCOACONTEXT
    glSwapAPPLE();
	//[openGLContext flushBuffer];
}
/**
 * show or hide mouse cursor
 * @param show, set true if you want show cursor otherwise false
 */
void CocoaScreen::setCursor(bool show)
{
    if (show) CGDisplayShowCursor(kCGDirectMainDisplay);
    else CGDisplayHideCursor(kCGDirectMainDisplay);
}
/**
 * set position cursor
 */
void CocoaScreen::setPositionCursor(const Vec2& pos)
{
    
    TOCOCOAWINDOW
    NSView* view=window.contentView;
    //calc inverse top view
    float screenH=window.screen.frame.origin.y+window.screen.frame.size.height;
    float windowH=window.frame.origin.y+window.frame.size.height;
    float viewTop=window.frame.size.height-view.frame.size.height;
    //calc mouse point in screen
    CGPoint cgpoint;
    cgpoint.x =   window.frame.origin.x + Math::max(Math::min(pos.x,getWidth()*1.0f),0.0f);
    cgpoint.y =   (screenH-windowH+viewTop) + Math::max(Math::min(pos.y,getHeight()*1.0f),1.0f);
    //send event
    CGEventRef event = CGEventCreateMouseEvent(NULL,
                                               kCGEventMouseMoved,
                                               cgpoint,
                                               /*we don't care about this : */0);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    
}
/**
 * return if cursor is shown or hidden
 */
bool CocoaScreen::getCursor()
{
	return CGCursorIsVisible();;
}
/**
 * enable or disable full screen
 */
void CocoaScreen::setFullscreen(bool fullscreen)
{
    if(this->fullscreen!=fullscreen){
        
        TOCOCOAWINDOW
        TOCOCOACONTEXT
        TOCOCOAINFO
        
        //delete listener
        if(onCocoaWindowClose)
            onCocoaWindowClose(cocoaWindow);
        //disable close event (NSApplication close)
        window->enableSendAppOnClose=false;
        
        if(fullscreen){
            //save
            cocoaInfo->saveMode=CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
            //search new mode
            cocoaInfo->fullscreenMode=bestMatchForMode({wantWidth,wantHeight,wantPixel});
            //set new mode
            DEBUG_ASSERT_MGS_REPLACE(
                        CGDisplaySetDisplayMode(kCGDirectMainDisplay, cocoaInfo->fullscreenMode,NULL)==kCGErrorSuccess,
                        "CocoaScreen setFullScreen: can't change display mode"
            );
        }
        else{
            //old mode
            DEBUG_ASSERT_MGS_REPLACE(
                        CGDisplaySetDisplayMode(kCGDirectMainDisplay, cocoaInfo->saveMode,NULL)==kCGErrorSuccess,
                        "CocoaScreen setFullScreen: can't change display mode"
            );
        }
        
        //open new window
        NSString *titleNS = [window title];
        __closeWindow();
        __openWindow(wantWidth, wantHeight, [titleNS UTF8String] , fullscreen);
       
        //set context
        RELOADCOCOAWINDOW
        [openGLContext setView:[window contentView]];
        [openGLContext update];
        [[window contentView] display];
        
        acquireContext();
        
    }
}
/**
 * return if fullscreen is enable return true
 */
bool CocoaScreen::isFullscreen()
{
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
                          AntiAliasing dfAA)
{
    this->fullscreen=fullscreen;
    this->cocoaGLContext=NULL;
    this->cocoaWindow=NULL;
    this->freamPerSecond=freamPerSecond;
    this->wantWidth=width;
    this->wantHeight=height;
    
    TOCOCOAINFO
    //save
    cocoaInfo->saveMode=CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
    //display mode
    if(fullscreen)
    {
        //search new mode
        cocoaInfo->fullscreenMode=bestMatchForMode({wantWidth,wantHeight,bites});
        //set new mode
        DEBUG_ASSERT_MGS_REPLACE(
        CGDisplaySetDisplayMode(kCGDirectMainDisplay, cocoaInfo->fullscreenMode,NULL)==kCGErrorSuccess,
        "CocoaScreen setFullScreen: can't change display mode"
        );
    }
    //set default pixel
    this->wantPixel=(int)currentDisplayBitsPerPixel();
    
    //open window
    __openWindow(width,height,appname,fullscreen);
    //create context
    __createContext(dfAA);
    //set context to window   
    TOCOCOAWINDOW
    TOCOCOACONTEXT
    [openGLContext setView:[window contentView]];
	[openGLContext update];
    [[window contentView] display];
    //set context
    acquireContext();    
    //init openGL
    __initOpenGL();
    //enable AA
    if(dfAA!=NOAA)
        glEnable( GL_MULTISAMPLE );
}
/*
 * close window
 */
void CocoaScreen::closeWindow()
{
    __closeWindow();    
}