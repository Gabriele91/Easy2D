#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "EAGLView.h"
///////////////////////////
#include <Debug.h>
#include <RenderContext.h>
#include <AppiOS.h>
using namespace Easy2D;
///////////////////////////
#define USE_DEPTH_BUFFER 1
#define USE_STENCIL_BUFFER 1
// A class extension to declare private methods
@interface EAGLView ()
//private context
@property (nonatomic, retain) EAGLContext *context;

@end

// Implementation
@implementation EAGLView

@synthesize context;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    
    if ((self = [super initWithFrame:frame]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                                     [NSNumber numberWithBool:FALSE],
                                                     kEAGLDrawablePropertyRetainedBacking,
                                                     kEAGLColorFormatRGBA8,
                                                     kEAGLDrawablePropertyColorFormat,
                                                     nil];
        //create context
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        //if faild
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
        
        //init intervall
        animationInterval = 1.0 / 60.0;
        
        //enable multi touch
        self.multipleTouchEnabled=YES;
        
        //init link
        displayLink = nil;
        
    }
    return self;
}


- (void)makeCurrent
{
    //set current context
    [EAGLContext setCurrentContext:context];
}

- (void)unsafeSwap
{
    //swap
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

- (void)drawView
{
    //set current context
    [EAGLContext setCurrentContext:context];
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    
    //update/draw (callback game instance)
    Application::instance()->update(self.animationInterval);
    
    //get errors
    CHECK_GPU_ERRORS();
    //swap
    [self unsafeSwap];
}


- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}


- (BOOL)createFramebuffer
{
    //create frame buffer
    glGenFramebuffers(1, &viewFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    
    CHECK_GPU_ERRORS();
    
    //create render buffer
    glGenRenderbuffers(1, &viewRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    
    //get the storage from iOS so it can be displayed in the view
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    
    //get size
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    //attach color to frame buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer);
    
    CHECK_GPU_ERRORS();
    
    if (USE_DEPTH_BUFFER)
    {
        //create zbuffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        //create the storage for the buffer, optimized for depth values, same size as the colorRenderbuffer
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        //attach the depth buffer to our framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        CHECK_GPU_ERRORS();
    }
    
    if (USE_STENCIL_BUFFER)
    {
        //create stencil
        glGenRenderbuffers(1, &stencilRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderbuffer);
        //create the storage for the buffer, optimized for stencil values, same size as the colorRenderbuffer
        glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, backingWidth, backingHeight);
        //attach the stencil buffer to our framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderbuffer);
        
        CHECK_GPU_ERRORS();
    }
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
    //set default state 
    RenderContext::setDefaultRenderTarget(
    {
      viewFramebuffer,
      viewRenderbuffer,
      depthRenderbuffer,
      stencilRenderbuffer,
      USE_DEPTH_BUFFER,
      USE_STENCIL_BUFFER
    });
    //find errors:
    CHECK_GPU_ERRORS();
    
    return YES;
}


- (void)destroyFramebuffer
{
    //set context
    [EAGLContext setCurrentContext:context];
    //delete frame buffer
    glDeleteFramebuffers(1, &viewFramebuffer);
    viewFramebuffer = 0;
    //delete render buffer
    glDeleteRenderbuffers(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    //delete depth buffer
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    //delete stencil buffer
    if(stencilRenderbuffer)
    {
        glDeleteRenderbuffers(1, &stencilRenderbuffer);
        stencilRenderbuffer = 0;
    }
}


- (void)startAnimation
{
    if(displayLink==nil)
    {
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(drawView)];
    
        [displayLink setFrameInterval: animationInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
}


- (void)stopAnimation
{
    //if display exist
    if (displayLink != nil)
    {
        //invalidate loop
        [displayLink invalidate];
        //display to nil
        displayLink = nil;
    }
}


- (float)getDeltaTime
{
    return animationInterval;
}

- (unsigned int)getWidth
{
    return (unsigned int)backingWidth;
}

- (unsigned int)getHeight
{
    return (unsigned int)backingHeight;
}

- (unsigned int)getOretation
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if(orientation==UIInterfaceOrientationPortrait) return 0;
    else if(orientation==UIInterfaceOrientationPortraitUpsideDown) return 1;
    else if(orientation==UIInterfaceOrientationLandscapeLeft) return 2;
    else if(orientation==UIInterfaceOrientationLandscapeRight) return 3;
    //default portrait
    return 0;
}

- (void)setAnimationInterval:(NSTimeInterval)interval
{
    //set inserval
    animationInterval = interval;
    //if >0 re-init
    if (displayLink != nil)
    {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)dealloc
{
    //stop animation
    [self stopAnimation];
    //release context
    RenderContext::releaseContext();
    //disable context
    if ([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    //dealloc
    [context release];
    [super dealloc];
}

@end
