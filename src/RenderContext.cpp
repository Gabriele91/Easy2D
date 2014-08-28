#include <stdafx.h>
#include <RenderContext.h>
#include <Debug.h>

///////////////////////////
using namespace Easy2D;
///////////////////////////
#define FAST_MODE(x) x
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//globals
RenderContext::Context RenderContext::context;
RenderContext::RenderState RenderContext::state=
{
    BACK,                     //cullface
    false,                    //zbuffer
    Vec4::ZERO,               //viewport
    true,                     //alpha
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
    FAST_MODE(if(context.binds.vertexBuffer!=buffer))
        glBindBuffer(GL_ARRAY_BUFFER, buffer);                      //(bind on vertex buffer)
    
    glBufferData(GL_ARRAY_BUFFER, size, data,
     ( type==STATIC ? GL_STATIC_DRAW :
                    ( type==DYNAMIC ? GL_DYNAMIC_DRAW :
                                      GL_STREAM_DRAW    ))
    );
    
    //(re)bind last buffer
    FAST_MODE(if(context.binds.vertexBuffer!=buffer))
        glBindBuffer(GL_ARRAY_BUFFER, context.binds.vertexBuffer);  //(reset vertex buffer)
}
void RenderContext::bufferSubData(uint buffer,void* data,size_t offset,size_t size)
{
    //bind buffer
    FAST_MODE(if(context.binds.vertexBuffer!=buffer))
        glBindBuffer(GL_ARRAY_BUFFER, buffer);                      //(bind on vertex buffer)
    
    //set data
    glBufferSubData(GL_ARRAY_BUFFER, offset,size, data);
    
    //(re)bind last buffer
    FAST_MODE(if(context.binds.vertexBuffer!=buffer))
        glBindBuffer(GL_ARRAY_BUFFER, context.binds.vertexBuffer);  //(reset vertex buffer)
}
//vertex buffer
void RenderContext::bindVertexBuffer(uint buffer)
{
    FAST_MODE(if(context.binds.vertexBuffer==buffer) return);
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
    FAST_MODE(if(context.binds.indexBuffer==buffer) return;)
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
static GLuint getGLPrimitiveType(DrawMode mode)
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
static GLuint getGLVectorType(VectorType type)
{
    switch (type)
    {
        case INT: return GL_INT;
        case SHORT: return GL_SHORT;
        case UNSIGNED_INT: return GL_UNSIGNED_INT;
        case UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
        default: return 0;
    }
}
void RenderContext::drawPrimitive(DrawMode mode, int first, int count)
{
    glDrawArrays( getGLPrimitiveType(mode), first, count );
}
void RenderContext::drawPrimitiveIndexed(DrawMode mode,
                                         int count,
                                         VectorType type,
                                         void* indexs)
{
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
        glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true );
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
    glActiveTexture( GL_TEXTURE0 + ntex );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, (GLuint)texture );
}
void RenderContext::unbindTexture(ushort ntex)
{
    glActiveTexture(GL_TEXTURE0 + ntex);
    glDisable(GL_TEXTURE_2D);
}
void RenderContext::filterTexture(uint min,uint mag)
{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min);
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
        //////////////////////////////////////////////////////////////////////
        //zbuffer
        if(newState.zbuffer)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        //////////////////////////////////////////////////////////////////////
        //viewport
        glViewport(newState.viewport.x,
                   newState.viewport.y,
                   newState.viewport.z,
                   newState.viewport.w);
        //////////////////////////////////////////////////////////////////////
        //alpha
        if(newState.alpha)
            glEnable(GL_ALPHA_TEST);
        else
            glDisable(GL_ALPHA_TEST);
        //////////////////////////////////////////////////////////////////////
        //blend
        if(newState.blend)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        //set
        glBlendFunc(newState.blendSRC, newState.blendDST);
        //////////////////////////////////////////////////////////////////////
        //texture
        if(newState.texture)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        //////////////////////////////////////////////////////////////////////
        //clear color = new clear color
        glClearColor(newState.clearColor.rNormalize(),
                     newState.clearColor.gNormalize(),
                     newState.clearColor.bNormalize(),
                     newState.clearColor.aNormalize());
        //colors
        glColor4ubv(newState.color*newState.ambientColor);
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
    setRenderState(
                   {
                       BACK,                     //cullface
                       false,                    //zbuffer
                       Vec4::ZERO,               //viewport
                       true,                     //alpha
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
                   },
                   true);
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
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
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
    glBlendFunc(source, dest);
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
    //set
    glColor4ubv(state.color*state.ambientColor);
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
    //set
    glColor4ubv(state.color*state.ambientColor);
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
void RenderContext::normalPointer(uint  	type,
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
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(pj);
}
const Mat4& RenderContext::getProjection()
{
    return context.matrixs.projection;
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

static void debugARenderState(RenderContext::RenderState state)
{
 /*
	struct RenderState
	{
		//state cullface
		CullFace cullface;
		//state zbuffer
		bool     zbuffer;
		//state viewport
		Vec4     viewport;
        //state alpha
        bool     alpha;
		//state blend
		bool     blend;
		uchar    blendSRC,
        blendDST;
		//state texture
		bool     texture;
		//colors
		Color    color,
        clearColor,
        ambientColor;
		//client states
		bool vertexsArray,
        normalsArray,
        texcoordsArray,
        colorsArray;
	};
*/
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
    Vec4 color;
    glGetFloatv(GL_CURRENT_COLOR, &color.x);
    glstate.color.fromNormalize(color);
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

