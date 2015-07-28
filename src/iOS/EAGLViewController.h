//
//  EAGLViewController.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 28/10/14.
//
//

#ifndef EAGLVIEWCONTROLLER_H
#define EAGLVIEWCONTROLLER_H

#import <UIKit/UIKit.h>
#include <Screen.h>
#define MAX_TOUCHES 10


@interface EAGLViewController : UIViewController
{
    //list of touch
    UITouch *finger[MAX_TOUCHES];
    //modes
    Easy2D::Screen::Orientation orientation;
    bool  onlyPortrait;
    bool  onlyLandscape;
    //mask
    UIDeviceOrientation previousOrientation;
}

- (EAGLViewController*)init;

//oretation
- (Easy2D::Screen::Orientation)  getOrientation;
- (void)setOrientation:(Easy2D::Screen::Orientation)type;

- (BOOL)shouldAutorotate;
- (BOOL)shouldAutorotateToInterfaceOrientation;
- (NSUInteger)supportedInterfaceOrientations;
//screen info
- (unsigned int)getWidth;
- (unsigned int)getHeight;
//event
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;

@end

#endif
