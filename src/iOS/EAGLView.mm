#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "EAGLView.h"
///////////////////////////
#include <Debug.h>
#include <RenderContext.h>
#include <AppiOS.h>
#include <ScreeniOS.h>
using namespace Easy2D;
///////////////////////////
#define USE_DEPTH_BUFFER 1
#define USE_STENCIL_BUFFER 1
//include input
#define ___PUBLIC_FOR_IOS_CLASS
#include <InputiOS.h>
#define inputios ((InputiOS*)Application::instance()->getInput())

const char* getErrorFrameBuffer(GLuint error)
{
    switch (error)
    {
        case GL_FRAMEBUFFER_COMPLETE: return "GL_FRAMEBUFFER_COMPLETE";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";
        default: return "UNKNOW";
    }
}
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
        // Enable resize
        [self setAutoresizingMask: UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        //if retina
        if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2.0)
        {
            eaglLayer.contentsScale = 2.0f;
        }
        //init layer
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
    
    //get additional buffers
    ScreeniOS* screen = (ScreeniOS*) Application::instance()->getScreen();
    Screen::TypeBuffers typeBuffers = screen->getDefaultBuffers();
    bool useDepthBuffer = USE_DEPTH_BUFFER && Screen::getDepthBits(typeBuffers) != 0;
    bool useStencilBuffer = USE_STENCIL_BUFFER && Screen::getStencilBits(typeBuffers) != 0;
    
    //init additional buffers
    if (useDepthBuffer || useStencilBuffer)
    {
        //zbuffer and stencil
        if (useDepthBuffer && useStencilBuffer)
        {
            //create zbuffer/stencil
            glGenRenderbuffers(1, &depthRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, backingWidth, backingHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
            //no ptr to stencil
            stencilRenderbuffer = 0;
        }
        //only zbuffer
        else if (useDepthBuffer)
        {
            //create zbuffer
            glGenRenderbuffers(1, &depthRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, backingWidth, backingHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
            //no ptr to stencil
            stencilRenderbuffer = 0;
        }
        //only stencil
        else if (useStencilBuffer)
        {
            glGenRenderbuffers(1, &stencilRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, stencilRenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, backingWidth, backingHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderbuffer);
            //no ptr to zbuufer
            depthRenderbuffer = 0;
        }
        CHECK_GPU_ERRORS();
    }
    
    GLuint statusFramebuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(statusFramebuffer != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"failed to make complete framebuffer object %s",getErrorFrameBuffer(statusFramebuffer));
        return NO;
    }
    //set default state 
    RenderContext::setDefaultRenderTarget(
    {
      viewFramebuffer,
      viewRenderbuffer,
      depthRenderbuffer,
      stencilRenderbuffer,
      useDepthBuffer,
      useStencilBuffer
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
    if(viewFramebuffer)
    {
        glDeleteFramebuffers(1, &viewFramebuffer);
        viewFramebuffer = 0;
    }
    //delete render buffer
    if(viewRenderbuffer)
    {
        glDeleteRenderbuffers(1, &viewRenderbuffer);
        viewRenderbuffer = 0;
    }
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

-(void)setBounds:(CGRect)newBounds
{
    //resize event?
    BOOL const isResize = !CGSizeEqualToSize(newBounds.size, self.bounds.size);
    //execute the event
    if(isResize)
    {
        [super setBounds:newBounds];
        if (isResize) [self resizeEvent];
    }
}

-(void)resizeEvent
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    //fire the event
    inputios->__callOnResize(Vec2([self getWidth],[self getHeight]));
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


- (void) resetDefaultAnimationInterval
{
    //get default frames
    ScreeniOS* screen = (ScreeniOS*) Application::instance()->getScreen();
    float nframes = screen ? screen->getDefaultFrame() : 60.0f;
    //set animaion interval
    [self setAnimationInterval:(1.0f/nframes)];
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

- (unsigned int)getOrientation
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
