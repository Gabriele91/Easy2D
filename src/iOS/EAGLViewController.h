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
    //mask
    NSUInteger maskOrientation;
    UIInterfaceOrientation preferOrientation;
    NSUInteger autoOrientation;
    float angleOrientation;
}

- (EAGLViewController*)init;

//oretation
- (Easy2D::Screen::Orientation)  getOrientation;
- (void)setOrientation:(Easy2D::Screen::Orientation)type;

- (BOOL)shouldAutorotate;
- (NSUInteger)supportedInterfaceOrientations;
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation;
//event
- (CGPoint)touchLocation:(UITouch *)touch shouldNormalize:(BOOL)normalize;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;

@end

#endif
