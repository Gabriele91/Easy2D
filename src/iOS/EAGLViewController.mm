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
    lastOrientation     = UIDeviceOrientationPortrait;
    //set rotation
    [[self view] transform] = CGAffineTransformMakeRotation(0);
    //set rotation
    Easy2D::RenderContext::setDisplay(Mat4::IDENTITY);
    //hide status bar
    [self disableStatusBar];
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


- (Easy2D::Screen::Orientation)  getOrientation
{
    switch (lastOrientation)
    {
        default:
        case UIDeviceOrientationFaceUp:
        case UIDeviceOrientationPortrait: return Screen::Orientation::PORTRAIT;
        
        case UIDeviceOrientationFaceDown:
        case UIDeviceOrientationPortraitUpsideDown: return Screen::Orientation::PORTRAIT_REVERSE;
            
        case UIDeviceOrientationLandscapeLeft: return Screen::Orientation::LANDSCAPE;
        case UIDeviceOrientationLandscapeRight: return Screen::Orientation::LANDSCAPE_REVERSE;
    }
}

- (UIInterfaceOrientation) getInterfaceOrientationValue: (Screen::Orientation) screenOrientation
{
    switch (screenOrientation)
    {
        case Screen::Orientation::PORTRAIT:          return  UIInterfaceOrientationPortrait;
        case Screen::Orientation::PORTRAIT_REVERSE:  return  UIInterfaceOrientationPortraitUpsideDown;
        case Screen::Orientation::LANDSCAPE:         return  UIInterfaceOrientationLandscapeLeft;
        case Screen::Orientation::LANDSCAPE_REVERSE: return  UIInterfaceOrientationLandscapeRight;
            //default unknown
        default: return  UIInterfaceOrientationUnknown;
    }
}

- (BOOL) isLandscape
{
    return lastOrientation == UIDeviceOrientationLandscapeLeft ||
           lastOrientation == UIDeviceOrientationLandscapeRight;
}

- (BOOL) isPortrait
{
    return lastOrientation == UIDeviceOrientationPortrait ||
           lastOrientation == UIDeviceOrientationPortraitUpsideDown||
           lastOrientation == UIDeviceOrientationUnknown ||
           lastOrientation == UIDeviceOrientationFaceUp  ||
           lastOrientation == UIDeviceOrientationFaceDown;
}

- (void)setOrientation:(Easy2D::Screen::Orientation)type;
{
    if(type == Screen::SENSOR_LANDSCAPE)
    {
        onlyLandscape = true;
        onlyPortrait  = false;
        orientation   = type;
        //if is in portrait
        if([self isPortrait])
        {
            //force rotation
            NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
    }
    else if(type == Screen::SENSOR_PORTRAIT)
    {
        onlyLandscape = false;
        onlyPortrait  = true;
        orientation   = type;
        //if is in landscape
        if([self isLandscape])
        {
            //force rotation
            NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
            [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
        }
    }
    else
    {
        onlyLandscape = false;
        onlyPortrait  = false;
        orientation   = type;
        //force rotation
        NSNumber *value = [NSNumber numberWithInt:[self getInterfaceOrientationValue:type]];
        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    }
}

////////////////////////////////
//ROTATION
-(BOOL)shouldAutorotate
{
    //save last rotation
    lastOrientation =  [[UIDevice currentDevice] orientation];
    //turn
    return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return [self shouldAutorotate];
}

- (NSUInteger)supportedInterfaceOrientations
{
    if(onlyPortrait)
        return UIInterfaceOrientationMaskPortrait |
               UIInterfaceOrientationMaskPortraitUpsideDown;
    if(onlyLandscape)
        return UIInterfaceOrientationMaskLandscapeLeft |
               UIInterfaceOrientationMaskLandscapeRight;
    else
        return UIInterfaceOrientationMaskAll;
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
    return Vec2(point.x*scale,point.y*scale);
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