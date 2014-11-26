#include <stdafx.h>
#include <RenderContext.h>
#include <Debug.h>
///////////////////////////
using namespace Easy2D;
///////////////////////////

//////////////////////////////////////////////////////////////////////
//globals
RenderContext::StandardShader RenderContext::standardShader=RenderContext::SHADER_VERTEX;
std::vector<Shader::ptr> RenderContext::shaders;
RenderContext::Context RenderContext::context;
RenderContext::RenderState RenderContext::state=
{
    BACK,                     //cullface
    false,                    //zbuffer
    Vec4::ZERO,               //viewport
    false,                    //alpha
    true,                     //blend
    GL_ONE,                   //blend source
    GL_ZERO,                  //blend dst
    true,                     //texture
    Color::WHITE,             //color
    Color::BLUE,              //clear color
    Color(128,128,128,128),   //ambient light
    true,                     //vertexs client state
    false,                    //normals client state
    true,                     //texture coords client state
    false                     //colors client statst
};
RenderContext::RenderTarget RenderContext::buffers=
{
    0,        //frame buffer
    0,        //color buffer
    0,        //df buffer
    false
};
//////////////////////////////////////////////////////////////////////
inline static void e2dBlendFunc(GLenum x,GLenum y) 
{
    glBlendFuncSeparate(x,y,x,y);
}
//////////////////////////////////////////////////////////////////////
//buffer
uint RenderContext::createBuffer()
{
    GLuint buffer;
    glGenBuffers(1,&buffer);
    return buffer;
}
void RenderContext::deleteBuffer(uint buffer)
{
    glDeleteBuffers(1,&buffer);
}
void RenderContext::bufferData(uint buffer,BufferType type,void* data,size_t size)
{
    //bind buffer
    if(context.binds.vertexBuffer!=buffer)
        glBindBuffer(GL_ARRAY_BUFFER, buffer);                      //(bind on vertex buffer)
    
    glBufferData(GL_ARRAY_BUFFER, size, data,
     ( type==STATIC ? GL_STATIC_DRAW :
                    ( type==DYNAMIC ? GL_DYNAMIC_DRAW :
                                      GL_STREAM_DRAW    ))
    );
    
    //(re)bind last buffer
    if(context.binds.vertexBuffer!=buffer)
        glBindBuffer(GL_ARRAY_BUFFER, context.binds.vertexBuffer);  //(reset vertex buffer)
}
void RenderContext::bufferSubData(uint buffer,void* data,size_t offset,size_t size)
{
    //bind buffer
    if(context.binds.vertexBuffer!=buffer)
        glBindBuffer(GL_ARRAY_BUFFER, buffer);                      //(bind on vertex buffer)
    
    //set data
    glBufferSubData(GL_ARRAY_BUFFER, offset,size, data);
    
    //(re)bind last buffer
    if(context.binds.vertexBuffer!=buffer)
        glBindBuffer(GL_ARRAY_BUFFER, context.binds.vertexBuffer);  //(reset vertex buffer)
}
//vertex buffer
void RenderContext::bindVertexBuffer(uint buffer)
{
    if(context.binds.vertexBuffer==buffer) return;
    //save
    context.binds.vertexBuffer=buffer;
    //set
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
}
void RenderContext::unbindVertexBuffer()
{
    context.binds.vertexBuffer=0;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//index buffer
void RenderContext::bindIndexBuffer(uint buffer)
{
    if(context.binds.indexBuffer==buffer) return;
    //save
    context.binds.indexBuffer=buffer;
    //set
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}
void RenderContext::unbindIndexBuffer()
{
    context.binds.indexBuffer=0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//////////////////////////////////////////////////////////////////////
static GLenum getGLPrimitiveType(DrawMode mode)
{
    switch( mode )
    {
        case TRIANGLE: return  GL_TRIANGLES;
        case TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case LINE_STRIP: return GL_LINE_STRIP;
        case LINES: return GL_LINES;
        case LINE_LOOP: return GL_LINE_LOOP;
        case TRIANGLE_FAN: return  GL_TRIANGLE_FAN;
        default: return 0;
    }
}
static GLenum getGLVectorType(VectorType type)
{
    switch (type)
    {
	case VectorType::INT: return GL_INT;
	case VectorType::SHORT: return GL_SHORT;
	case VectorType::UNSIGNED_INT: return GL_UNSIGNED_INT;
	case VectorType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    default: return 0;
    }
}
void RenderContext::drawPrimitive(DrawMode mode, int first, int count)
{

    //is a standard shader
    if(context.binds.idprogram==getStandardShaderProgram())
    {
        //select shader
        selectStandardShader();
        //is change
        if(context.binds.idprogram!=getStandardShaderProgram())
        {
            //bind shader
            bindStandardShader();
        }
        else
            //update
            updateStandardUniform();
    }
    //uniform errors
    CHECK_GPU_ERRORS();
    //draw
    glDrawArrays( getGLPrimitiveType(mode), first, count );
}
void RenderContext::drawPrimitiveIndexed(DrawMode mode,
                                         int count,
                                         VectorType type,
                                         void* indexs)
{

    //is a standard shader
    if(context.binds.idprogram==getStandardShaderProgram())
    {
        //select shader
        selectStandardShader();
        //is change
        if(context.binds.idprogram!=getStandardShaderProgram())
            //bind shader
            bindStandardShader();
        else
            //update
            updateStandardUniform();
    }
    //uniform errors
    CHECK_GPU_ERRORS();

    //draw
    glDrawElements( getGLPrimitiveType(mode), count, getGLVectorType(type),indexs);
}
//////////////////////////////////////////////////////////////////////
//texture
void RenderContext::setTexture(bool enable)
{
    if(state.texture==enable) return;
    //save
    state.texture=enable;
    //set

    if(enable)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

}
bool RenderContext::getTexture()
{
    return state.texture;
}
uint RenderContext::createTexture(uint format,
                                  uint type,
                                  uint width,
                                  uint height,
                                  const uchar* byte,
                                  bool mipmap)
{
    uint texture=0;
    //opengl texture
    glGenTextures(1,&texture);
    //enable texture
    bindTexture(texture);
    //gpu buffer
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 width,
                 height,
                 0,
                 type,
                 GL_UNSIGNED_BYTE,
                 byte );
    //mip mapping
	
    if(mipmap)
    {
        glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
    }
	





    //return
    return texture;
}
void RenderContext::deleteTexture(uint texture)
{
    glDeleteTextures(1, &texture );
}
void RenderContext::subTexture(uint type,
                               uint width,
                               uint height,
                               const uchar* byte)
{
    //send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
                    width,
                    height,
                    type,
                    GL_UNSIGNED_BYTE,
                    byte );
}
void RenderContext::bindTexture(uint texture,ushort ntex)
{
    DEBUG_ASSERT(ntex<20);
    glActiveTexture( GL_TEXTURE0 + ntex );

    glEnable( GL_TEXTURE_2D );

    glBindTexture( GL_TEXTURE_2D, (GLuint)texture );
    //state texture
    context.binds.textures[ntex].enable=true;
    context.binds.textures[ntex].idtexture=texture;
}
void RenderContext::unbindTexture(ushort ntex)
{
    glActiveTexture(GL_TEXTURE0 + ntex);

    glDisable(GL_TEXTURE_2D);



    //state texture
    context.binds.textures[ntex].enable=false;
    context.binds.textures[ntex].idtexture=0;
}
void RenderContext::filterTexture(uint min,uint mag)
{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min);
}
void RenderContext::wrapTexture(uint s,uint t)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}


//render state
void RenderContext::setRenderState(const RenderState& newState, bool force)
{
    if(force)
    {
        //////////////////////////////////////////////////////////////////////
        //CullFace
        if(newState.cullface==CullFace::DISABLE)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(newState.cullface==CullFace::FRONT?GL_FRONT:GL_BACK);
        }
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //zbuffer
        if(newState.zbuffer)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //viewport
        glViewport(newState.viewport.x,
                   newState.viewport.y,
                   newState.viewport.z,
                   newState.viewport.w);
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //alpha
		
        if(newState.alpha)
            glEnable(GL_ALPHA_TEST);
        else
            glDisable(GL_ALPHA_TEST);
        
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //blend
        if(newState.blend)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        //set
        e2dBlendFunc(newState.blendSRC, newState.blendDST);
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //texture
        
        if(newState.texture)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //clear color = new clear color
        glClearColor(newState.clearColor.rNormalize(),
                     newState.clearColor.gNormalize(),
                     newState.clearColor.bNormalize(),
                     newState.clearColor.aNormalize());
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //clients
		
			if(newState.vertexsArray)    glEnableClientState( GL_VERTEX_ARRAY );
			else                         glDisableClientState( GL_VERTEX_ARRAY );
			if(newState.normalsArray)    glEnableClientState( GL_NORMAL_ARRAY );
			else                         glDisableClientState( GL_NORMAL_ARRAY );
			if(newState.texcoordsArray)  glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			else                         glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			if(newState.colorsArray)     glEnableClientState( GL_COLOR_ARRAY );
			else                         glDisableClientState( GL_COLOR_ARRAY );
        
        //find errors:
        CHECK_GPU_ERRORS();
        //////////////////////////////////////////////////////////////////////
        //overwrite context state
        state=newState;
    }
    else
    {
        setCullFace(newState.cullface);
        setZBuffer(newState.zbuffer);
        setViewport(newState.viewport);
        setAlpha(newState.alpha);
        setBlend(newState.blend);
        setBlendFunction(newState.blendSRC, newState.blendDST);
        setTexture(newState.texture);
        setColor(newState.color);
        setColorClear(newState.clearColor);
        setAmbientColor(newState.ambientColor);
        setClientState(newState.vertexsArray,
                       newState.normalsArray,
                       newState.texcoordsArray,
                       newState.colorsArray);
    }
}
const RenderContext::RenderState& RenderContext::getRenderState()
{
    return state;
}
void RenderContext::setDefaultRenderState()
{
	RenderContext::RenderState defaultState=
    {
        BACK,                     //cullface
        false,                    //zbuffer
        Vec4::ZERO,               //viewport
        false,                    //alpha
        true,                     //blend
        GL_ONE,                   //blend source
        GL_ZERO,                  //blend dst
        true,                     //texture
        Color::WHITE,             //color
        Color::BLUE,              //clear color
        Color(128,128,128,255),   //ambient light
        true,                     //vertexs client state
        false,                    //normals client state
        true,                     //texture coords client state
        false                     //colors client statst
    };
    setRenderState(defaultState, true);
}


//cullface
void RenderContext::setCullFace(CullFace cullface)
{
    if(state.cullface==cullface) return;
    //save
    state.cullface=cullface;
    //set
    if(cullface==CullFace::DISABLE)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(cullface==CullFace::FRONT?GL_FRONT:GL_BACK);
    }
}
CullFace RenderContext::getCullFace()
{
    return state.cullface;
}
//zbuffer
void RenderContext::setZBuffer(bool enable)
{
    if(state.zbuffer==enable) return;
    //save
    state.zbuffer=enable;
    //set
    if(enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}
bool RenderContext::getZBuffer()
{
    return state.zbuffer;
}
//alpha
void RenderContext::setAlpha(bool enable)
{
    if(state.alpha==enable) return;
    //save
    state.alpha=enable;
    //set

    if(enable)
        glEnable(GL_ALPHA_TEST);
    else
        glDisable(GL_ALPHA_TEST);

}
bool RenderContext::getAlpha()
{
    return state.alpha;
}
//blend function
void RenderContext::setBlend(bool enable)
{
    if(state.blend==enable) return;
    //save
    state.blend=enable;
    //set
    if(enable)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}
bool RenderContext::getBlend()
{
    return state.blend;
}
void RenderContext::setBlendFunction(uint source, uint dest)
{
    if(state.blendSRC==source && state.blendDST==dest) return;
    //save
    state.blendSRC=source;
    state.blendDST=dest;
    //set
    e2dBlendFunc(source, dest);
}
uint RenderContext::getBlendSrc()
{
    return state.blendSRC;
}
uint RenderContext::getBlendDst()
{
    return state.blendDST;
}

//viewport
void RenderContext::setViewport(const Vec4& viewport)
{
    if(state.viewport==viewport) return;
    //save
    state.viewport=viewport;
    //set
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}
const Vec4& RenderContext::getViewport()
{
    return state.viewport;
}


//color
void RenderContext::setColor(const Color& color)
{
    if(state.color==color) return;
    //save
    state.color=color;
}
const Color& RenderContext::getColor()
{
    return state.color;
}

//clear
void RenderContext::setColorClear(const Color& color)
{
    if(state.clearColor==color) return;
    //save
    state.clearColor=color;
    //set
    glClearColor(color.rNormalize(),
                 color.gNormalize(),
                 color.bNormalize(),
                 color.aNormalize());
}
const Color& RenderContext::getColorClear()
{
    return state.clearColor;
}
void RenderContext::doClear()
{
    glClear(GL_COLOR_BUFFER_BIT|(state.zbuffer?GL_DEPTH_BUFFER_BIT:0));
}

//ambient color
void RenderContext::setAmbientColor(const Color& color)
{
    if(state.ambientColor==color) return;
    //save
    state.ambientColor=color;
}
const Color& RenderContext::getAmbientColor()
{
    return state.ambientColor;
}




//client states
void RenderContext::setClientState(bool vertex,bool normal,bool texcoord,bool color)
{

    if(state.vertexsArray!=vertex)
    {
        if(vertex) glEnableClientState( GL_VERTEX_ARRAY );
        else       glDisableClientState( GL_VERTEX_ARRAY );
        state.vertexsArray=vertex;
    }
    if(state.normalsArray!=normal)
    {
        if(normal) glEnableClientState( GL_NORMAL_ARRAY );
        else       glDisableClientState( GL_NORMAL_ARRAY );
        state.normalsArray=normal;
    }
    if(state.texcoordsArray!=texcoord)
    {
        if(texcoord) glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        else       glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        state.texcoordsArray=texcoord;
    }
    if(state.colorsArray!=color)
    {
        if(color)  glEnableClientState( GL_COLOR_ARRAY );
        else       glDisableClientState( GL_COLOR_ARRAY );
        state.colorsArray=color;
    }
}
void RenderContext::setVertexClientState(bool enable)
{
    if(state.vertexsArray==enable) return;
    //save
    state.vertexsArray=enable;
    //set
    if(enable) glEnableClientState( GL_VERTEX_ARRAY );
    else       glDisableClientState( GL_VERTEX_ARRAY );
}
void RenderContext::setNormalClientState(bool enable)
{
    if(state.normalsArray==enable) return;
    //save
    state.normalsArray=enable;
    //set
    if(enable) glEnableClientState( GL_NORMAL_ARRAY );
    else       glDisableClientState( GL_NORMAL_ARRAY );
}
void RenderContext::setTexCoordClientState(bool enable)
{
    if(state.texcoordsArray==enable) return;
    //save
    state.texcoordsArray=enable;
    //set

    if(enable) glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	else       glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}
void RenderContext::setColorClientState(bool enable)
{
    if(state.colorsArray==enable) return;
    //save
    state.colorsArray=enable;
    //set
    if(enable) glEnableClientState( GL_COLOR_ARRAY );
    else       glDisableClientState( GL_COLOR_ARRAY );

}
bool RenderContext::getVertexClientState()
{
    return state.vertexsArray;
}
bool RenderContext::getNormalClientState()
{
    return state.normalsArray;
}
bool RenderContext::getTexCoordClientState()
{
    return state.texcoordsArray;
}
bool RenderContext::getColorClientState()
{
    return state.colorsArray;
}

//pointer

void RenderContext::vertexPointer(uint  	size,
                                  uint  	type,
                                  size_t  	stride,
                                  const void *pointer)
{
    glVertexPointer(size,type,stride,pointer);
}
void RenderContext::normalPointer(uint  	size,
								  uint  	type,
                                  size_t  	stride,
                                  const void *pointer)
{
    glNormalPointer(type,stride,pointer);
}
void RenderContext::texCoordPointer(uint  	size,
                                    uint  	type,
                                    size_t  	stride,
                                    const void *pointer)
{
    glTexCoordPointer(size,type,stride,pointer);
}
void RenderContext::colorPointer(uint  	size,
                                 uint  	type,
                                 size_t  	stride,
                                 const void *pointer)
{
    glColorPointer(size,type,stride,pointer);
}

//matrix
void RenderContext::setModelView(const Mat4& mv)
{
    context.matrixs.modelView=mv;
    //update gpu matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mv);

}
const Mat4& RenderContext::getModelView()
{
    return context.matrixs.modelView;
}
void RenderContext::setProjection(const Mat4& pj)
{
    context.matrixs.projection=pj;
    //update gpu matrix
    context.matrixs.updateGProjection();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(context.matrixs.gProjection);

}
const Mat4& RenderContext::getProjection()
{
    return context.matrixs.projection;
}
const Mat4& RenderContext::getGlobalProjection()
{
    return context.matrixs.gProjection;
}
void RenderContext::setDisplay(const Mat4& dy)
{
    context.matrixs.display=dy;
    //update gpu matrix
    context.matrixs.updateGProjection();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(context.matrixs.gProjection);

}
const Mat4& RenderContext::getDisplay()
{
    return context.matrixs.display;
}

//render target
RenderContext::RenderTarget  RenderContext::createRenderTarget(uint colorbuffer)
{
    //save last target
    auto lastTarget=context.binds.idtarget;
    //----------------------------------------------------------------------------------------------------
    //new render target
    RenderTarget render;
    //Create FBO
    glGenFramebuffers(1, &render.target);
	glBindFramebuffer(GL_FRAMEBUFFER, render.target);
    //----------------------------------------------------------------------------------------------------
    //save color buffer
    render.color=colorbuffer;
    //Attach color buffer to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, colorbuffer, 0);
    //----------------------------------------------------------------------------------------------------
    render.hasDepth=false;
    //----------------------------------------------------------------------------------------------------
    //renable last target
    glBindFramebuffer(GL_FRAMEBUFFER,lastTarget);
    
    //return runder buffer
    return render;

}
RenderContext::RenderTarget  RenderContext::createRenderDepthTarget(uint colorbuffer,size_t width,size_t height)
{
    //save last target
    auto lastTarget=context.binds.idtarget;
    //----------------------------------------------------------------------------------------------
    //new render target
    RenderTarget render;
    //Create FBO
    glGenFramebuffers(1, &render.target);
	glBindFramebuffer(GL_FRAMEBUFFER, render.target);
    //----------------------------------------------------------------------------------------------
    //save color buffer
    render.color=colorbuffer;
    //Attach color buffer to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, colorbuffer, 0);
    //---------------------------------------------------------------------------------------------
    render.hasDepth=true;
    //df buffer
    glGenRenderbuffers(1, &render.depth);
    glBindRenderbuffer(GL_RENDERBUFFER, render.depth);
    glRenderbufferStorage(GL_RENDERBUFFER,  GL_DEPTH_COMPONENT32, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //Attach depth buffer to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, render.depth);
    //---------------------------------------------------------------------------------------------
    //renable last target
    glBindFramebuffer(GL_FRAMEBUFFER,lastTarget);
    
    //return runder buffer
    return render;
}

void RenderContext::enableRenderTarget(const RenderContext::RenderTarget& target)
{
    if(context.binds.idtarget==target.target) return;
    //enable target
    glBindFramebuffer(GL_FRAMEBUFFER,target.target);
    //save
    context.binds.idtarget=target.target;
}
void RenderContext::disableRenderTarget()
{
    if(context.binds.idtarget==buffers.target) return;
    //enable target
    glBindFramebuffer(GL_FRAMEBUFFER,buffers.target);
    //save
    context.binds.idtarget=buffers.target;
}
void RenderContext::deleteRenderTarget(const RenderTarget& target)
{
    //delete frame buffer
    glDeleteFramebuffers(1,&target.target);
    //delete zbuffer
    if(target.hasDepth)
    {
        glDeleteRenderbuffers(1,&target.depth);
    }
}

//shader program
void RenderContext::enableProgram(uint program)
{
    if(context.binds.idprogram==program) return;
    //enable shader
    glUseProgram(program);
    //save
    context.binds.idprogram=program;
}
void RenderContext::disableProgram()
{
    if(context.binds.idprogram==getStandardShaderProgram()) return;
    //disable shader
    glUseProgram(getStandardShaderProgram());
    //save
    context.binds.idprogram=getStandardShaderProgram();
}

//utilities
void RenderContext::drawBox(const AABox2& aabox2,const Color& color)
{
    //disable ibo/vbo
    setClientState(true, false, false, false);
    unbindIndexBuffer();
    unbindVertexBuffer();
    //draw
    setColor(color);
    const float vertices[]=
    {
        aabox2.getMin().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMax().y,
        aabox2.getMin().x,aabox2.getMax().y,
    };
    vertexPointer(2, GL_FLOAT, 0, vertices);
    drawPrimitive(LINE_LOOP, 0, 4);
}
void RenderContext::drawFillBox(const AABox2& aabox2,const Color& color)
{
    //disable ibo/vbo
    setClientState(true, false, false, false);
    unbindIndexBuffer();
    unbindVertexBuffer();
    //draw
    RenderContext::setColor(color);
    const float vertices[]=
    {
        aabox2.getMin().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMin().y,
        aabox2.getMin().x,aabox2.getMax().y,
        aabox2.getMax().x,aabox2.getMax().y,
    };
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(TRIANGLE_STRIP, 0, 4);
}
void RenderContext::drawCircle(const Vec2& center,float radius,const Color& e2color)
{
    const float segments = 32.0f;
    const uint  vertexCount=32;
    const float increment =  Math::PI2 / segments;
    float theta = 0.0f;
    Vec2 vertices[vertexCount];
    //disable ibo/vbo
    setClientState(true, false, false, false);
    unbindIndexBuffer();
    unbindVertexBuffer();
    //draw
    RenderContext::setColor(e2color);
    for (uint i = 0; i != segments; ++i)
    {
        vertices[i]= center + radius * Vec2(cosf(theta), sinf(theta));
        theta += increment;
    }
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(LINE_LOOP, 0, vertexCount);
}
void RenderContext::drawFillCircle(const Vec2& center,float radius,const Color& e2color)
{
    const float segments = 32.0f;
    const uint  vertexCount=32;
    const float increment =  Math::PI2 / segments;
    float theta = 0.0f;
    Vec2 vertices[vertexCount];
    //disable ibo/vbo
    setClientState(true, false, false, false);
    unbindIndexBuffer();
    unbindVertexBuffer();
    //draw
    RenderContext::setColor(e2color);
    for (uint i = 0; i != segments; ++i)
    {
        vertices[i]= center + radius * Vec2(cosf(theta), sinf(theta));
        theta += increment;
    }
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(TRIANGLE_FAN, 0, vertexCount);
}
void RenderContext::drawLine(const Vec2& v1,const Vec2& v2,const Color& color)
{
    //disable ibo/vbo
    setClientState(true, false, false, false);
    unbindIndexBuffer();
    unbindVertexBuffer();
    //draw
    setColor(color);
    const float vertices[]=
    {
        v1.x,v1.y,
        v2.x,v2.y
    };
    vertexPointer(2, GL_FLOAT, 0, vertices);
    drawPrimitive(LINES, 0, 2);
}

static void debugARenderState(const RenderContext::RenderState& state)
{
    Debug::message()<< "Render state:\n";
    Debug::message()<< " cullface: " << (state.cullface == DISABLE ? "DISABLE" : (state.cullface==BACK ? "BACK" : "FRONT")) << "\n";
    Debug::message()<< " zbuffer: " << (state.zbuffer  ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " viewport: " << state.viewport.toString() << "\n";
    Debug::message()<< " alpha: " << (state.alpha  ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " blend: " << (state.blend  ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " blend function: (" << RenderContext::blendConstantToString(state.blendSRC) << ", "
                                            << RenderContext::blendConstantToString(state.blendDST) << ")\n";
    Debug::message()<< " texture: " << (state.texture  ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " color: " << state.color.toVec4().toString();
    Debug::message()<< " ambientColor: " << state.ambientColor.toVec4().toString();
    Debug::message()<< " Client states:\n";
    Debug::message()<< " \t vertexsArray: " << (state.vertexsArray ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " \t normalsArray: " << (state.normalsArray ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " \t texcoordsArray: " << (state.texcoordsArray ? "TRUE" : "FALSE") << "\n";
    Debug::message()<< " \t colorsArray: " << (state.colorsArray ? "TRUE" : "FALSE") << "\n";
}
void RenderContext::debugCurrentState()
{
    debugARenderState(state);
}
void RenderContext::debugNativeState()
{
    
    RenderState glstate;
    //get cull face
    if(glIsEnabled(GL_CULL_FACE))
    {
        int cullmode;
        glGetIntegerv(GL_CULL_FACE_MODE,(GLint*)&cullmode);
        glstate.cullface= cullmode==GL_BACK ? BACK : FRONT;
    }
    else
        glstate.cullface=DISABLE;
    //get zbuffer
        glstate.zbuffer=glIsEnabled(GL_DEPTH_TEST);
    //viewport
		glGetFloatv(GL_VIEWPORT ,  &glstate.viewport.x);
    //get alpha
		glstate.alpha=glIsEnabled(GL_ALPHA_TEST);
    //get blend
        glstate.blend=glIsEnabled(GL_BLEND);
    //get
		glGetIntegerv(GL_BLEND_SRC, (GLint*)&(glstate.blendSRC));
		glGetIntegerv(GL_BLEND_DST, (GLint*)&(glstate.blendDST));
    //get texture
        glstate.texture=glIsEnabled(GL_TEXTURE_2D);
    //color
		glstate.color=state.color;
		glstate.ambientColor=state.ambientColor;
    //clients
		glstate.vertexsArray=glIsEnabled(GL_VERTEX_ARRAY);
		glstate.normalsArray=glIsEnabled(GL_NORMAL_ARRAY);
		glstate.texcoordsArray=glIsEnabled(GL_TEXTURE_COORD_ARRAY);
		glstate.colorsArray=glIsEnabled(GL_COLOR_ARRAY);
    //print...
    debugARenderState(glstate);
    
}

int RenderContext::stringToBlendContant(String fun, int vlDefault)
{
    //normalize
    fun.replaceAll(" ","");
    fun=fun.toUpper();
    //
    if(fun=="ONE") return GL_ONE;
    if(fun=="ZERO") return GL_ZERO;
    //
    if(fun=="ONE::MINUS::DST::COLOR") return GL_ONE_MINUS_DST_COLOR;
    if(fun=="ONE::MINUS::DST::ALPHA") return GL_ONE_MINUS_DST_ALPHA;
    if(fun=="ONE::MINUS::SRC::COLOR") return GL_ONE_MINUS_SRC_COLOR;
    if(fun=="ONE::MINUS::SRC::ALPHA") return GL_ONE_MINUS_SRC_ALPHA;
    //
    if(fun=="DST::COLOR") return GL_DST_COLOR;
    if(fun=="DST::ALPHA") return GL_DST_ALPHA;
    //
    if(fun=="SRC::COLOR") return GL_SRC_COLOR;
    if(fun=="SRC::ALPHA") return GL_SRC_ALPHA;
    if(fun=="SRC::ALPHA::SATURATE") return GL_SRC_ALPHA_SATURATE;
    
    return vlDefault;
}
String RenderContext::blendConstantToString(int fun)
{
    switch (fun)
    {
        case GL_ONE: return "ONE";
        case GL_ZERO: return "ZERO";
            
        case GL_ONE_MINUS_DST_COLOR: return "ONE::MINUS::DST::COLOR";
        case GL_ONE_MINUS_DST_ALPHA: return "ONE::MINUS::DST::ALPHA";
        case GL_ONE_MINUS_SRC_COLOR: return "ONE::MINUS::SRC::COLOR";
        case GL_ONE_MINUS_SRC_ALPHA: return "ONE::MINUS::SRC::ALPHA";
            
            
        case GL_DST_COLOR: return "DST::COLOR";
        case GL_DST_ALPHA: return "DST::ALPHA";
            
        case GL_SRC_COLOR: return "SRC::COLOR";
        case GL_SRC_ALPHA: return "SRC::ALPHA";
        case GL_SRC_ALPHA_SATURATE: return "SRC::ALPHA::SATURATE";
        default: return "?";
    }
}

//////////////////////////////////////////////////////////////////////
// Standard Shader
void RenderContext::selectStandardShader()
{

    //default
    standardShader=SHADER_VERTEX;
    //else
    if(getTexCoordClientState() && getColorClientState())
    {
        standardShader=SHADER_VERTEX_TCOORDS_COLOR;
    }
    else if(getTexCoordClientState())
    {
        standardShader=SHADER_VERTEX_TCOORDS;
    }
    else if(getColorClientState())
    {
        standardShader=SHADER_VERTEX_COLOR;
    }

}
void RenderContext::bindStandardShader()
{
    //bind shader
    shaders[standardShader]->bind();
}
void RenderContext::updateStandardUniform()
{
  //bind shader
  shaders[standardShader]->updateStandardUniform();
}
uint RenderContext::getStandardShaderProgram()
{
    return shaders[standardShader]->programID();
}
// Render Context

void RenderContext::initContext()
{
    //init state
    setDefaultRenderState();   
    //find errors:
    CHECK_GPU_ERRORS();
    //init shaders
    auto vertex=
    "#ifdef VERTEX_COLOR           \n"
    "   varying  vec4 e2dVColor;   \n"
    "#endif //VERTEX_COLOR         \n"
    "vec4 vertex()                 \n"
    "{                             \n"
    "#ifdef  VERTEX_COLOR          \n"
    "    e2dVColor=e2dVertexColor; \n"
    "#endif //VERTEX_COLOR         \n"
    "return ftransform();          \n"
    "}";
    auto fragment=
    "#ifdef   VERTEX_COLOR                                 \n"
    "   varying  vec4 e2dVColor;                           \n"
    "#endif //VERTEX_COLOR                                 \n"
    "vec4 fragment(Texture tex,vec4 coord)                 \n"
    "{                                                     \n"
    "     vec4 frag=e2dColor*e2dAmbientColor;              \n"
    "     #ifdef  VERTEX_TCOORDS                           \n"
    "     frag*=texture2D(tex,coord.xy);                   \n"
    "     #endif //VERTEX_TCOORDS                          \n"
    "     #ifdef  VERTEX_COLOR                             \n"
    "     frag*=e2dVColor;                                 \n"
    "     #endif //VERTEX_COLOR                            \n"
    "     return frag;                                     \n"
    "}";
    //alloc shaders
    Shader::ptr cVertex=Shader::ptr(new Shader());
    Shader::ptr cVtxTCoords=Shader::ptr(new Shader());
    Shader::ptr cVtxColor=Shader::ptr(new Shader());
    Shader::ptr cVtxTCoordsColor=Shader::ptr(new Shader());
    //vector defines
    std::vector<String> defines;
    
    //vertex only
    cVertex->load(vertex,fragment,defines);
    
    //resize
    defines.resize(1);
    //coords
    defines[0]="VERTEX_TCOORDS";
    cVtxTCoords->load(vertex,fragment,defines);
    //colors
    defines[0]="VERTEX_COLOR";
    cVtxColor->load(vertex,fragment,defines);
    //resize
    defines.resize(2);
    //coords & colors
    defines[0]="VERTEX_TCOORDS";
    defines[1]="VERTEX_COLOR";
    cVtxTCoordsColor->load(vertex,fragment,defines);
    //push shaders
    shaders.resize(4);
    shaders[SHADER_VERTEX]=cVertex;
    shaders[SHADER_VERTEX_TCOORDS]=cVtxTCoords;
    shaders[SHADER_VERTEX_COLOR]=cVtxColor;
    shaders[SHADER_VERTEX_TCOORDS_COLOR]=cVtxTCoordsColor;
    //force get default uniforms
    for(auto shader:shaders)
    {
        enableProgram(shader->programID());
        shader->getDefaultUniform();
    }
    //set default program
    disableProgram();
}
void RenderContext::releaseContext()
{
    //delete shaders
    shaders.resize(0);
}
//////////////////////////////////////////////////////////////////////




