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
    CGRect screenBounds=[[UIScreen mainScreen] bounds];
    //alloc window
    window = [[UIWindow alloc] initWithFrame: screenBounds];
    //controller
    glViewController=[[[EAGLViewController alloc] init] autorelease];
    //view controller //input to do
    window.rootViewController = glViewController;
    //alloc view
    glView=[[EAGLView alloc]  initWithFrame: screenBounds];
    //set view
    [glViewController setView:glView];
    [window addSubview: glView];
    //view
    [window makeKeyAndVisible];
    //set ui application delegate
    ((AppiOS*)Application::instance())->setUIApplicationDelegate(self);
    //init framerate
    glView.animationInterval = 1.0 / 60.0;
    //create buffers
    [glView createFramebuffer];
    //init game
    [glView makeCurrent];
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
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        return UIInterfaceOrientationMaskAll;
    else  /* iphone */
        return UIInterfaceOrientationMaskAllButUpsideDown;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{
    glView.animationInterval = 1.0 / 60.0;
}


- (void)applicationWillTerminate:(UIApplication *)application
{
    Application::instance()->getGame()->end();
}

- (void)dealloc
{
    [window release];
    [glView release];
    [super dealloc];
}

@end