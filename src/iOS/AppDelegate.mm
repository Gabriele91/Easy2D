#import "AppDelegate.h"
#import "EAGLView.h"
#import "EAGLViewController.h"
///////////////////////////
#include <Debug.h>
#include <RenderContext.h>
#include <AppiOS.h>
using namespace Easy2D;
///////////////////////////
/**
 * cocoa application class implementation
 */
@implementation Easy2DApplication

@synthesize window;
@synthesize glViewController;
@synthesize glView;

- (id)init
{
    self = [super init];
    return self;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //scree bounds
    CGRect screenBound = [[UIScreen mainScreen] bounds];
    CGSize screenSize  = [[[UIScreen mainScreen] currentMode] size];
    //bound gl
    CGRect glBound; glBound.size = screenSize;
    //alloc window
    window = [[UIWindow alloc] initWithFrame: screenBound];
    //controller
    glViewController=[[[EAGLViewController alloc] init] autorelease];
    //view controller //input to do
    window.rootViewController = glViewController;
    //alloc view
    glView=[[EAGLView alloc]  initWithFrame: screenBound];
    //self resize
    window.autoresizingMask =
    glView.autoresizingMask =
    UIViewAutoresizingFlexibleLeftMargin |
    UIViewAutoresizingFlexibleWidth |
    UIViewAutoresizingFlexibleRightMargin |
    UIViewAutoresizingFlexibleTopMargin |
    UIViewAutoresizingFlexibleHeight |
    UIViewAutoresizingFlexibleBottomMargin;
    //set view
    [glViewController setView:glView];
    [window addSubview: glView];
    //display
    [window setNeedsDisplay];
    //view
    [window makeKeyAndVisible];
    //set ui application delegate
    ((AppiOS*)Application::instance())->setUIApplicationDelegate(self);
    //create buffers
    [glView createFramebuffer];
    //init game
    [glView makeCurrent];
    //init context
    RenderContext::initContext();
    //start
    Application::instance()->getGame()->start();
    //start animation
    [glView startAnimation];
    //return
    return TRUE;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
}

-(NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return UIInterfaceOrientationMaskAll;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
    Application::instance()->getGame()->pause();
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{
    Application::instance()->getGame()->restart();
    [glView resetDefaultAnimationInterval];
    [glView startAnimation];
}


- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
    Application::instance()->getGame()->end();
}

- (void)dealloc
{
    [window release];
    [glView release];
    [super dealloc];
}

@end

@implementation UIApplication (close)

- (void)close
{
    if ([self respondsToSelector:@selector(suspend)])
        [self suspend];
    // Checks if the current device supports background execution
    if ([[UIDevice currentDevice] respondsToSelector:@selector(isMultitaskingSupported)])
        [self performSelector:@selector(exit) withObject:nil afterDelay:0.9];
    else
        [self exit];
}

- (void)exit
{
    if ([self respondsToSelector:@selector(terminateWithSuccess)])
        [self terminateWithSuccess];
    else
        exit(EXIT_SUCCESS);
}

@end