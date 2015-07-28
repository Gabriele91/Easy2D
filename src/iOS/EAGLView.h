#ifndef EAGLVIEW_H
#define EAGLVIEW_H

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

/*
 This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
 The view content is basically an EAGL surface you render your OpenGL scene into.
 Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
 */
@interface EAGLView : UIView
{
    
@private
    /* The pixel dimensions of the backbuffer */
    GLint backingWidth;
    GLint backingHeight;
    
    /* context */
    EAGLContext *context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer;
    GLuint viewFramebuffer;
    
    /* OpenGL name for the depth/stencil buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;
    GLuint stencilRenderbuffer;
    
    /* loop update */
    CADisplayLink *displayLink;
    NSTimeInterval animationInterval;
}

@property(nonatomic) NSTimeInterval animationInterval;

- (void)startAnimation;
- (void)stopAnimation;
- (void)setAnimationInterval:(NSTimeInterval)time;
- (void)drawView;
- (void)makeCurrent;
- (void)unsafeSwap;
- (float)getDeltaTime;
- (void) resetDefaultAnimationInterval;
- (unsigned int)getWidth;
- (unsigned int)getHeight;
- (unsigned int)getOrientation;
- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end

#endif