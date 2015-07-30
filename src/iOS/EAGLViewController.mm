#import "EAGLViewController.h"
#import "EAGLView.h"
#include <RenderContext.h>
//using Easy2D name space
using namespace Easy2D;
//include input
#define ___PUBLIC_FOR_IOS_CLASS
#include <InputiOS.h>
#define inputios ((InputiOS*)Application::instance()->getInput())

//utility
static inline Easy2D::Mat4 RotTransform(CGFloat angle)
{
    float s=sinf(angle);
    float c=cosf(angle);
    Easy2D::Mat4 rot({
                      c,s,0,0,
                     -s,c,0,0,
                      0,0,1,0,
                      0,0,0,1
                     });
    return rot;
}

//implementation
@implementation EAGLViewController


- (void)dealloc
{
    [super dealloc];
}

- (EAGLViewController*)init
{
    [super init];
    memset(finger,0, sizeof(UITouch*)*MAX_TOUCHES);
    //first state
    orientation         = Screen::Orientation::PORTRAIT;
    onlyPortrait        = false;
    onlyLandscape       = false;
    previousOrientation = UIDeviceOrientationPortrait;
    //set rotation
    [[self view] transform] = CGAffineTransformMakeRotation(0);
    //set rotation
    Easy2D::RenderContext::setDisplay(Mat4::IDENTITY);
    //hide status bar
    [self disableStatusBar];
    //reg change view
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(viewChanging:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];

    //return this
    return self;
}

//disable status bar
-(void)disableStatusBar
{
    //disable status bar
    if ([self respondsToSelector:@selector(setNeedsStatusBarAppearanceUpdate)])
    {
        // iOS 7
        [self prefersStatusBarHidden];
        [self performSelector:@selector(setNeedsStatusBarAppearanceUpdate)];
    }
    else
    {
        // iOS 6
        [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
    }
}

//no status bar
-(BOOL)prefersStatusBarHidden
{
    return YES;
}

- (unsigned int)getWidth
{
    EAGLView* view = (EAGLView*)self.view;
    return [view getWidth];
}

- (unsigned int)getHeight
{
    EAGLView* view = (EAGLView*)self.view;
    return [view getHeight];
}

//Orientation
- (Screen::Orientation)  getOrientation
{
    //current orientation
    UIDeviceOrientation currentOrientation = [[UIDevice currentDevice] orientation];
    switch (currentOrientation)
    {
        case UIDeviceOrientationPortrait: return Screen::Orientation::PORTRAIT;
        case UIDeviceOrientationPortraitUpsideDown: return Screen::Orientation::PORTRAIT_REVERSE;
        case UIDeviceOrientationLandscapeLeft: return Screen::Orientation::LANDSCAPE;
        case UIDeviceOrientationLandscapeRight: return Screen::Orientation::LANDSCAPE_REVERSE;
        //default portrait
        default: return  Screen::Orientation::PORTRAIT;
    }
}

- (UIDeviceOrientation) getOrientationValue: (Screen::Orientation) screenOrientation
{
    switch (screenOrientation)
    {
        case Screen::Orientation::PORTRAIT:          return  UIDeviceOrientationPortrait;
        case Screen::Orientation::PORTRAIT_REVERSE:  return  UIDeviceOrientationPortraitUpsideDown;
        case Screen::Orientation::LANDSCAPE:         return  UIDeviceOrientationLandscapeLeft;
        case Screen::Orientation::LANDSCAPE_REVERSE: return  UIDeviceOrientationLandscapeRight;
        //default unknown
        default: return  UIDeviceOrientationUnknown;
    }
}

- (BOOL) isPortrait: (UIDeviceOrientation) lOrientation
{
    return  lOrientation == UIDeviceOrientationPortrait ||
            lOrientation == UIDeviceOrientationPortraitUpsideDown;
}

- (BOOL) isLandscape: (UIDeviceOrientation) lOrientation
{
    return lOrientation == UIDeviceOrientationLandscapeLeft ||
    lOrientation == UIDeviceOrientationLandscapeRight;
}

- (BOOL) lastIsLandscape
{
    return [self isLandscape: previousOrientation];
}

- (BOOL) lastIsPortrait
{
    return [self isPortrait: previousOrientation];
}

- (void)setOrientation:(Easy2D::Screen::Orientation)type;
{
    if(type == Screen::SENSOR_LANDSCAPE)
    {
        onlyLandscape = true;
        onlyPortrait  = false;
        orientation   = type;
        if([self lastIsPortrait])  [self setViewOrientation:UIDeviceOrientationLandscapeLeft];
    }
    else if(type == Screen::SENSOR_PORTRAIT)
    {
        onlyLandscape = true;
        onlyPortrait  = false;
        orientation   = type;
        if([self lastIsLandscape]) [self setViewOrientation:UIDeviceOrientationPortrait];
    }
    else
    {
        onlyLandscape = false;
        onlyPortrait  = false;
        orientation   = type;
        [self setViewOrientation:[self getOrientationValue:type]];
    }
}

- (void)setViewOrientation: (UIDeviceOrientation) currentOrientation
{
    //ignore cases
    if (previousOrientation==currentOrientation) return;
    else if ( (![self isLandscape:currentOrientation]) &&
              (![self isPortrait:currentOrientation]) ) return;
    else if ([self isLandscape:currentOrientation] && onlyPortrait) return;
    else if ([self isPortrait:currentOrientation] && onlyLandscape) return;
    //or change
    CATransform3D transform = CATransform3DIdentity;
    //rotation
    if (UIDeviceOrientationLandscapeLeft == currentOrientation)
        transform = CATransform3DMakeRotation(M_PI/2, 0, 0, 1);
    else if (UIDeviceOrientationLandscapeRight == currentOrientation)
        transform = CATransform3DMakeRotation(-M_PI/2, 0, 0, 1);
    else if (UIDeviceOrientationPortraitUpsideDown == currentOrientation)
        transform = CATransform3DMakeRotation(-M_PI, 0, 0, 1);
    //get bound
    CGRect  frame = [[UIScreen mainScreen] bounds];
    CGPoint center= CGPointMake(CGRectGetMidX(frame),CGRectGetMidY(frame));
    //resize
    if([self lastIsLandscape] && [self isPortrait: currentOrientation])
    {
        //became portrait
        float width =std::min(frame.size.width,  frame.size.height);
        float height=std::max(frame.size.width,  frame.size.height);
        frame.size.width = width;
        frame.size.height = height;
        [[self view] setBounds:frame];
    }
    else if([self lastIsPortrait] && [self isLandscape: currentOrientation])
    {
        //became landscape
        float width =std::max(frame.size.width,  frame.size.height);
        float height=std::min(frame.size.width,  frame.size.height);
        frame.size.width = width;
        frame.size.height = height;
        [[self view] setBounds:frame];
    }
    //applay rotation
    [self view].transform=CATransform3DGetAffineTransform(transform);
    //center of screen
    [[[self view] layer] setPosition:center];
    //save state
    previousOrientation = currentOrientation;
}


- (void) movingToBackground: (NSNotification *) notification
{
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
}

- (void) movingToForeground: (NSNotification *) notification
{
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
}

- (void)viewChanging:(NSNotification*)notification
{
    [self setViewOrientation: [[UIDevice currentDevice] orientation]];
}
////////////////////////////////
//DISABLE ROTATION
-(BOOL)shouldAutorotate
{
    return NO;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return NO;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationPortrait;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationPortrait;
}
////////////////////////////////
- (Vec2)touchLocation:(UITouch *)touch shouldNormalize:(BOOL)normalize
{
    CGPoint point = [touch locationInView: [self view]];
    //normalize? No (denormalize in android).
    float scale=[[UIScreen mainScreen] scale];
#if 0
    unsigned int width=0;
    unsigned int height=0;
    width =[[UIScreen mainScreen] bounds].size.height*scale;
    height=[[UIScreen mainScreen] bounds].size.width*scale;
    //cases
    switch (previousOrientation)
    {
        default:
        case UIDeviceOrientationPortrait:           return Vec2(point.x*scale      , point.y*scale);
        case UIDeviceOrientationPortraitUpsideDown: return Vec2(point.x*scale      , height-point.y*scale);
        case UIDeviceOrientationLandscapeLeft:      return Vec2(width-point.y*scale, point.x*scale);
        case UIDeviceOrientationLandscapeRight:     return Vec2(      point.y*scale, point.x*scale);;
    }
#else
    return Vec2(point.x*scale,point.y*scale);
#endif
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    //list touch
    NSEnumerator *enumerator = [touches objectEnumerator];
    //for each
    for (UITouch *touch = (UITouch*)[enumerator nextObject];
         touch;
         touch = (UITouch*)[enumerator nextObject])
    {
        //get input
        Vec2 location=[self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==nil)
            {
                finger[i]=touch;
                //fire start touch
                inputios->__callOnFingerPress(Easy2D::Vec3(location,0.0f),
                                              (Easy2D::Key::Finger)(Easy2D::Key::Finger::FINGER1+i));
                break;
            }
        }
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    //list touch
    NSEnumerator *enumerator = [touches objectEnumerator];
    //for each
    for (UITouch *touch = (UITouch*)[enumerator nextObject];
         touch;
         touch = (UITouch*)[enumerator nextObject])
    {
        //get input
        Vec2 location=[self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==touch)
            {
                //fire move touch
                inputios->__callOnFingerMove(Easy2D::Vec3(location,0.0f),
                                             (Easy2D::Key::Finger)(Easy2D::Key::Finger::FINGER1+i));
                break;
            }
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{    //list touch
    NSEnumerator *enumerator = [touches objectEnumerator];
    //for each
    for (UITouch *touch = (UITouch*)[enumerator nextObject];
         touch;
         touch = (UITouch*)[enumerator nextObject])
    {
        //get input
        Vec2 location=[self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==touch)
            {
                //fire end touch
                inputios->__callOnFingerRelease(Easy2D::Vec3(location,0.0f),
                                                (Easy2D::Key::Finger)(Easy2D::Key::Finger::FINGER1+i));
                //
                finger[i]=nil;
                break;
            }
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    //fire event: end touches
    [self touchesEnded: touches withEvent: event];
}

@end