#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#import <UIKit/UIKit.h>

@class EAGLView;
@class EAGLViewController;

/* delegate app */
@interface Easy2DApplication : UIResponder < UIApplicationDelegate >
{
    UIWindow *window;
    EAGLViewController *glViewController;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UIViewController *glViewController;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

/* hide calls */
@interface UIApplication (Easy2DApplication)
- (void)suspend;
- (void)terminateWithSuccess;
@end

/* close */
@interface UIApplication (close)
- (void)close;
@end

#endif