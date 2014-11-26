#import "EAGLViewController.h"
#include <RenderContext.h>
//using Easy2D name space
using namespace Easy2D;
//PORTRAIT
#define ORI_TOP    Easy2D::Math::torad(0.0f)
#define ORI_DOWN   Easy2D::Math::torad(180.0f)
//LANDSCAPE
#define ORI_LEFT   Easy2D::Math::torad(-90.0f)
#define ORI_RIGHT  Easy2D::Math::torad(+90.0f)
//#define ENABLE_IPHONE_IDIOMA
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
    //default mode
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        maskOrientation= UIInterfaceOrientationMaskAll;
    else /* iphone */
        maskOrientation= UIInterfaceOrientationMaskAllButUpsideDown;
    //start on portrait
    preferOrientation = UIInterfaceOrientationPortrait;
    //default disable
    autoOrientation=0;
    //set rotation
    [[self view] transform] = CGAffineTransformMakeRotation(angleOrientation);
    //set rotation
    Easy2D::RenderContext::setDisplay(RotTransform(angleOrientation));
    //return this
    return self;
}


//oretation
- (Screen::Orientation)  getOrientation
{
    UIDeviceOrientation currentOrientation = [[UIDevice currentDevice] orientation];
    if(currentOrientation==UIDeviceOrientationPortrait) return Screen::Orientation::PORTRAIT;
    else if(currentOrientation==UIDeviceOrientationPortraitUpsideDown) return Screen::Orientation::PORTRAIT_REVERSE;
    else if(currentOrientation==UIDeviceOrientationLandscapeLeft) return Screen::Orientation::LANDSCAPE;
    else if(currentOrientation==UIDeviceOrientationLandscapeRight) return Screen::Orientation::LANDSCAPE_REVERSE;
    //default portrait
    return  Screen::Orientation::PORTRAIT;
}

- (void)setOrientation:(Easy2D::Screen::Orientation)type;
{
    switch (type)
    {
        case Easy2D::Screen::SENSOR_PORTRAIT:
            maskOrientation= UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
        break;
        
        case Easy2D::Screen::SENSOR_LANDSCAPE:
            maskOrientation= UIInterfaceOrientationLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
        break;
        
        default:
            //default rotation
            #ifdef ENABLE_IPHONE_IDIOMA
                if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
                    maskOrientation= UIInterfaceOrientationMaskAll;
                else  /* iphone */
                    maskOrientation= UIInterfaceOrientationMaskAllButUpsideDown;
            #else
                maskOrientation= UIInterfaceOrientationMaskAll;
            #endif
        break;
    }
    //default disable
    autoOrientation=0;
    //selete rotation
    switch (type)
    {
        case  Screen::Orientation::PORTRAIT:
            preferOrientation=UIInterfaceOrientationPortrait;
            angleOrientation=ORI_TOP;
        break;
        
        case  Screen::Orientation::PORTRAIT_REVERSE:
            preferOrientation=UIInterfaceOrientationPortraitUpsideDown;
            angleOrientation=ORI_DOWN;
        break;
        
        case  Screen::Orientation::LANDSCAPE:
            preferOrientation=UIInterfaceOrientationLandscapeLeft;
            angleOrientation=ORI_LEFT;
        break;
        
        case  Screen::Orientation::LANDSCAPE_REVERSE:
            preferOrientation=UIInterfaceOrientationLandscapeRight;
            angleOrientation=ORI_RIGHT;
        break;
        
        case  Screen::Orientation::SENSOR_PORTRAIT:
            //auto orientation on
            autoOrientation=Screen::Orientation::SENSOR_PORTRAIT;
            //portrait oretation
            angleOrientation=ORI_TOP;
            //default
            preferOrientation=UIInterfaceOrientationPortrait;
            //else
            if([self getOrientation]==Screen::Orientation::PORTRAIT_REVERSE)
            {
                preferOrientation=UIInterfaceOrientationPortraitUpsideDown;
                angleOrientation=ORI_DOWN;
            }
        break;
            
        case Screen::Orientation::SENSOR_LANDSCAPE:
            //auto orientation on
            autoOrientation=Screen::Orientation::SENSOR_LANDSCAPE;
            //landscape oretation
            angleOrientation=ORI_LEFT;
            //default
            preferOrientation=UIInterfaceOrientationLandscapeLeft;
            //else
            if([self getOrientation]==Screen::Orientation::LANDSCAPE_REVERSE)
            {
                preferOrientation=UIInterfaceOrientationLandscapeRight;
                angleOrientation=ORI_RIGHT;
            }
            break;
        default:  break;
    }
    //set rotation
    Easy2D::RenderContext::setDisplay(RotTransform(angleOrientation));
}

- (BOOL)shouldAutorotate
{
    switch (autoOrientation)
    {
        case Screen::Orientation::SENSOR_PORTRAIT:
        {
            //get orientation
            UIDeviceOrientation  _orientation=[[UIDevice currentDevice] orientation];
            //select case
            if (_orientation==UIDeviceOrientationPortrait || _orientation==UIDeviceOrientationFaceUp)
            {
                Easy2D::RenderContext::setDisplay(RotTransform(ORI_TOP));
            }
            else if(_orientation==UIDeviceOrientationPortraitUpsideDown || _orientation==UIDeviceOrientationFaceDown)
            {
                Easy2D::RenderContext::setDisplay(RotTransform(ORI_DOWN));
            }
        }
        break;
        
        case Screen::Orientation::SENSOR_LANDSCAPE:
        {
            //get orientation
            UIDeviceOrientation  _orientation=[[UIDevice currentDevice] orientation];
            //select case
            if (_orientation==UIDeviceOrientationLandscapeLeft)
            {
                Easy2D::RenderContext::setDisplay(RotTransform(ORI_LEFT));
            }
            else if(_orientation==UIDeviceOrientationLandscapeRight)
            {
                Easy2D::RenderContext::setDisplay(RotTransform(ORI_RIGHT));
            }
        }
        break;
        
        default:
        /* Easy2D::RenderContext::setDisplay(RotTransform(angleOrientation)); */
        break;
    }
    return false;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return maskOrientation;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return preferOrientation;
}


//touches event

- (CGPoint)touchLocation:(UITouch *)touch shouldNormalize:(BOOL)normalize
{
    CGPoint point = [touch locationInView: [self view]];
    //normalize? No (denormalize in android).
    return point;
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
        //get touch
        CGPoint locationInView = [self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==nil)
            {
                finger[i]=touch;
                //fire start touch
                inputios->__callOnFingerPress(Easy2D::Vec3(locationInView.x,locationInView.y,0.0f),
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
        //get touch
        CGPoint locationInView = [self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==touch)
            {
                //fire move touch
                inputios->__callOnFingerMove(Easy2D::Vec3(locationInView.x,locationInView.y,0.0f),
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
        //get touch
        CGPoint locationInView = [self touchLocation:touch shouldNormalize:YES];
        //set finger
        for(int i=0;i!=MAX_TOUCHES;++i)
        {
            if(finger[i]==touch)
            {
                //fire end touch
                inputios->__callOnFingerRelease(Easy2D::Vec3(locationInView.x,locationInView.y,0.0f),
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