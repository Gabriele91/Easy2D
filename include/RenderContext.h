#ifndef RENDERCONTEX_H
#define RENDERCONTEX_H

#include <Config.h>
#include <Color.h>
#include <Math3D.h>
#include <Blend.h>
#include <Shader.h>

namespace Easy2D
{

enum BufferType
{
	STATIC,
	DYNAMIC,
	STREAM
};
    
enum CullFace
{
    DISABLE,
    FRONT,
    BACK
};
    
enum DrawMode
{
	LINES,
	LINE_STRIP,
	LINE_LOOP,
	TRIANGLE,
	TRIANGLE_STRIP,
    TRIANGLE_FAN
};

enum VectorType
{
    INT,
    SHORT,
    UNSIGNED_INT,
    UNSIGNED_SHORT
};

enum StencilBuffer
{
    STENCIL_NONE,
    STENCIL_REPLACE,
    STENCIL_KEEP
};

class RenderContext
{
    
    struct Context
    {
        struct Bind
        {
            //vertex buffer oid
            uint vertexBuffer;
            //index buffer oid
            uint indexBuffer;
            //texture oid
            struct Texture
            {
                uint idtexture;
                bool enable;
                Texture()
                {
                    idtexture=0;
                    enable=false;
                }
			};
			static const uint maxTextures{ 20 };
			Texture textures[maxTextures];
            //render target oid
            uint idtarget;
            //shader program
            uint idprogram;
            //init
            Bind()
            {
                vertexBuffer=0;
                indexBuffer=0;
                idtarget=0;
            }
        }binds;
        struct Matrix
        {
            Mat4	 modelView;
            Mat4     projection;
            Mat4     display;
            Mat4     gProjection;
            //calc global projection
            void updateGProjection()
            {
                gProjection=projection.mul(display);
                //gProjection=display.mul(projection);
            }
        }matrixs;
    };
    
    
    public:
    
    struct RenderTarget
    {
        uint target;
        uint color;
        uint depth;
        bool hasDepth;
        
        RenderTarget()
        {
            target   = 0;
            color    = 0;
            depth    = 0;
            hasDepth = false;
        }
        RenderTarget(uint _target,
                     uint _color,
                     uint _depth,
                     bool _hasDepth)
        {
            target   = _target;
            color    = _color;
            depth    = _depth;
            hasDepth = _hasDepth;
        }
    };
    
	struct RenderState
	{
		//stencil
		StencilBuffer stencil;
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
		int      blendSRC,
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
    //buffers
    static uint createBuffer();
    static void deleteBuffer(uint buffer);
    static void bufferData(uint buffer,BufferType type,void* data,size_t size);
    static void bufferSubData(uint buffer,void* data,size_t offset,size_t size);
    
	//vertex buffer
    static void bindVertexBuffer(uint buffer);
    static void unbindVertexBuffer();

    //index buffer
    static void bindIndexBuffer(uint buffer);
    static void unbindIndexBuffer();
    
    //draw vb
    static void drawPrimitive(DrawMode mode, int first, int count);
    static void drawPrimitiveIndexed(DrawMode mode,
                                     int count,
                                     VectorType type,
                                     void* indexs=0);

    //textures
    static void setTexture(bool texture);
    static bool getTexture();
    static uint createTexture(uint format,
                              uint type,
                              uint width,
                              uint height,
                              const uchar* byte,
                              bool mipmap=false);
    static void deleteTexture(uint id);
    static void subTexture(uint type,
                           uint width,
                           uint height,
                           const uchar* byte);
    static void bindTexture(uint texture,ushort ntex=0);
    static void unbindTexture(ushort ntex=0);
    static void filterTexture(uint min,uint mag);
    static void wrapTexture(uint s,uint t);
    
    //render to texture
    static void setDefaultRenderTarget(RenderTarget target)
    {
        buffers=target;
    }
    static RenderTarget createRenderTarget(uint colorbuffer);
    static RenderTarget createRenderDepthTarget(uint colorbuffer,size_t width,size_t height);
    static void enableRenderTarget(const RenderTarget& target);
    static void disableRenderTarget();
    static void deleteRenderTarget(const RenderTarget& target);
    
    //shader program
    static void enableProgram(uint program);
    static void disableProgram();
    
    //render state
    static void setRenderState(const RenderState& state, bool force=false);
    static const RenderState& getRenderState();
    static void setDefaultRenderState();

    //context
    static const Context& getContext()
    {
        return context;
    }

    //cullface
    static void setCullFace(CullFace cullface);
    static CullFace getCullFace();
    
    //zbuffer
    static void setZBuffer(bool enable);
    static bool getZBuffer();
    
    //alpha
    static void setAlpha(bool enable);
    static bool getAlpha();
    
    //blend function
    static void setBlend(bool enable);
    static bool getBlend();
    static void setBlendFunction(uint source, uint dest);
    static uint getBlendSrc();
    static uint getBlendDst();

    //viewport
    static void setViewport(const Vec4& viewport);    
    static const Vec4& getViewport();
    
    //color
    static void setColor(const Color& color);
    static const Color& getColor();
    
    //clear
    static void setColorClear(const Color& color);
    static const Color& getColorClear();
    static void doClear();
    
    //ambient color
    static void setAmbientColor(const Color& color);
    static const Color& getAmbientColor();

    //stencil
	static void stencilClear();
	static void setStencil(StencilBuffer stencil);
    static StencilBuffer getStencil();
    
    //client states
    static void setClientState(bool vertex,bool normal,bool texcoord,bool color);
    static void setVertexClientState(bool enable);
    static void setNormalClientState(bool enable);
    static void setTexCoordClientState(bool enable);
    static void setColorClientState(bool enable);
    static bool getVertexClientState();
    static bool getNormalClientState();
    static bool getTexCoordClientState();
    static bool getColorClientState();
    
    //pointer
    static void vertexPointer(uint  	size,
                              uint  	type,
                              size_t  	stride,
                              const void *pointer);
    static void normalPointer(uint  	size,
							  uint  	type,
                              size_t  	stride,
                              const void *pointer);
    static void texCoordPointer(uint  	size,
                                uint  	type,
                                size_t  	stride,
                                const void *pointer);
    static void colorPointer(uint  	size,
                             uint  	type,
                             size_t  	stride,
                             const void *pointer);
    
    //matrix
    static void setModelView(const Mat4& mv);
    static const Mat4& getModelView();
    static void setProjection(const Mat4& pj);
    static const Mat4& getProjection();
    static const Mat4& getGlobalProjection();
    static void setDisplay(const Mat4& dy);
    static const Mat4& getDisplay();
    
    //corrent
    static const Context::Bind::Texture& currentTexture(uint ntex)
    {
        return context.binds.textures[ntex];
    }
    static uint currentTarget()
    {
        return context.binds.idtarget;
    }
    static uint currentVertexBuffer()
    {
        return context.binds.vertexBuffer;
    }
    static uint currentIndexBuffer()
    {
        return context.binds.indexBuffer;
	}
	static uint isTextureBind(int idtexture)
	{
		for (uint i = 0; i != Context::Bind::maxTextures; ++i)
		{
			if (context.binds.textures[i].enable && 
				context.binds.textures[i].idtexture == idtexture)
				return i;
		}
		return -1;
	}
    
    //utilities
    static void drawBox(const AABox2& box,const Color& color);
    static void drawFillBox(const AABox2& box,const Color& color);
    static void drawCircle(const Vec2& pos,float r,const Color& color);
    static void drawFillCircle(const Vec2& pos,float r,const Color& color);
    static void drawLine(const Vec2& v1,const Vec2& v2,const Color& color);

    //debug
    static void debugCurrentState();
    static void debugNativeState();
    static int stringToBlendContant(String fun,int vDefault=GL_ONE);
    static String blendConstantToString(int);
    
    //init/release render context
    static void initContext();
    static void releaseContext();
    
    //current shader standar
    static void selectStandardShader();
    static void bindStandardShader();
    static void updateStandardUniform();
    static uint getStandardShaderProgram();

    private:
    /////////////////////////////////////////
    //shaders
    enum StandardShader
    {
        SHADER_VERTEX,
        SHADER_VERTEX_TCOORDS,
        SHADER_VERTEX_COLOR,
        SHADER_VERTEX_TCOORDS_COLOR
    };
    static StandardShader standardShader;
    static std::vector<Shader::ptr>  shaders;
    /////////////////////////////////////////
	static Context context;
	static RenderState state;
    static RenderTarget buffers;

};

};

#endif