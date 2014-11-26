#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#import <UIKit/UIKit.h>

@class EAGLView;
@class EAGLViewController;

@interface Easy2DApplication : NSObject <UIApplicationDelegate>
{
    UIWindow *window;
    EAGLViewController *glViewController;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UIViewController *glViewController;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

#endif