#include <stdafx.h>
#include <Texture.h>
#include <Debug.h>
#include <Table.h>
#include <Application.h>
#include <RenderContext.h>
#define IMAGE_LOADER_OPENGL
#include "Image/Image.h"

using namespace Easy2D;


Texture::Texture(ResourcesManager<Texture> *rsmr,
                 const String& pathfile)
    :Resource(rsmr,pathfile)
    ,bBilinear(true)
    ,chBlr(true)
    ,bMipmaps(true)
    ,chMps(true)
    ,bFlipVertical(false)
    ,width(0)
    ,height(0)
    ,realWidth(0)
    ,realHeight(0)
    ,spriteWidth(0)
    ,spriteHeight(0)
    ,gpuid(0)
    ,offsetUV(0,0,1,1)
    ,po2Srpite(nullptr)
{
    //is not loaded
    loaded=false;
}

Texture::~Texture()
{
    //release resource
    release();
}
//flip vertical
bool Texture::flipVertical()
{
    return bFlipVertical;
}
bool Texture::flipVertical(bool value)
{
    return bFlipVertical=value;
}
//texture enable
void Texture::bind(uint ntexture)
{
    //
    DEBUG_ASSERT(gpuid);
    //
    RenderContext::bindTexture(gpuid,ntexture);
    //filters
#ifndef DISABLE_MIDMAP
    if(chBlr || chMps)
    {
        //change filter
        auto filterMag=bBilinear?GL_LINEAR:GL_NEAREST;
        auto filter= bBilinear ?  ( bMipmaps ? GL_LINEAR_MIPMAP_LINEAR   : GL_LINEAR  ):
                                  ( bMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST );
        RenderContext::filterTexture(filter,filterMag);
        CHECK_GPU_ERRORS();

        //update
        chMps=chBlr=false;
    }
#else
    if(chBlr)
    {
        //change filter
        auto filter=bBilinear?GL_LINEAR:GL_NEAREST;
        RenderContext::filterTexture(filter,filter);
        //update
        chBlr=false;
    }
#endif
}
//setting
bool Texture::bilinear()
{
    return bBilinear;
}
bool Texture::bilinear(bool value)
{
    chBlr=bBilinear!=value;
    return bBilinear=value;
}
bool Texture::mipmaps()
{
    return bMipmaps;
}
bool Texture::mipmaps(bool value)
{
    chMps=bMipmaps!=value;
    return bMipmaps=value;
}
//operators
bool Texture::operator ==(const Texture& t) const
{
    return gpuid==t.gpuid;
}
bool Texture::operator !=(const Texture& t) const
{
    return gpuid!=t.gpuid;
}

//return mesh
Mesh::ptr Texture::getPO2Sprite()
{
    //create texture
    if(!po2Srpite)
    {
        po2Srpite=Mesh::ptr (new Mesh());
        //if loaded build mesh
        __build();
    }
    //
    return po2Srpite;
}
//texture
void Texture::__build()
{
    //set loaded values
    if(isLoad()&&po2Srpite)
    {
        //clear cpu mesh
        po2Srpite->cpuClear();
        //unload gpu mesh
        if(po2Srpite->isLoad())
            po2Srpite->unload();
        //set size mesh
        float hlSizeX=spriteWidth*0.5f;
        float hlSizeY=spriteHeight*0.5f;
        //add vertexs
        po2Srpite->addVertex(  hlSizeX,
                               -hlSizeY,
                               offsetUV.z,
                               offsetUV.w);
        po2Srpite->addVertex(  hlSizeX,
                               hlSizeY,
                               offsetUV.z,
                               offsetUV.y);
        po2Srpite->addVertex( -hlSizeX,
                              -hlSizeY,
                              offsetUV.x,
                              offsetUV.w);
        po2Srpite->addVertex( -hlSizeX,
                              hlSizeY,
                              offsetUV.x,
                              offsetUV.y);
        //end add vertexs
        //set draw mode
        po2Srpite->setDrawMode(TRIANGLE_STRIP);
        //build mesh
        po2Srpite->build();
    }
}

//load methods
bool Texture::load()
{
    /////////////////////////////////////////////////////////////////////
    //cpu load
    //load image
    Image image;
    Utility::Path imagePath=rpath;
    //size sprite
    spriteWidth=0;
    spriteHeight=0;
    //offset?
    bool textureUVOffset=false;
    //type of image
    if(rpath.getExtension()=="e2d")
    {
        //load table
        Table texInfo(NULL,rpath);
        texInfo.load();
        //get image path
        DEBUG_ASSERT_MSG(texInfo.exists("image"),"texture error:"
                         "must to be setted image path"
                         "(parameter:image), "+rpath.getPath());
        imagePath=rpath.getDirectory()+'/'+texInfo.getString("image");
        //get relative coordinate
        spriteWidth=(uint)texInfo.getFloat("width");
        spriteHeight=(uint)texInfo.getFloat("height");
        //operations
        flipVertical(((uint)texInfo.getFloat("flipVertical",(float)flipVertical()))!=0);
#ifndef DISABLE_MIDMAP
		bMipmaps=(((uint)texInfo.getFloat("mipmaps",(float)mipmaps()))!=0);
#endif
        bBilinear=(((uint)texInfo.getFloat("bilinear",(float)bilinear()))!=0);
        //offset
        if(texInfo.existsAsType("uv",Table::VECTOR2D))
        {
            textureUVOffset=true;
            auto uv2=texInfo.getVector2D("uv",Vec2::ONE);
            offsetUV=Vec4(0,0,uv2.x,uv2.y);
        }
        else if(texInfo.existsAsType("uv",Table::VECTOR4D))
        {
            textureUVOffset=true;
            offsetUV=texInfo.getVector4D("uv",Vec4::ONE);
        }
    }
    //load image
    {
        //get raw file
        void *data=NULL;
        size_t len=0;
        Application::instance()->loadData(imagePath,data,len);
        image.loadFromData(data,
                           len,
                           Image::getTypeFromExtetion(imagePath.getExtension()));
        if(bFlipVertical)
            image.flipY();
        //save width end height
        width=realWidth=image.width;
        height=realHeight=image.height;
        //set width end height
        spriteWidth =spriteWidth ? spriteWidth  : realWidth;
        spriteHeight=spriteHeight? spriteHeight : realHeight;
        //free raw file
        free(data);
    }
    /////////////////////////////////////////////////////////////////////
    //gen gpu
    //support only pow of 2?
    if(Application::instance()->onlyPO2())
    {
        bool isNotPO2Width =!Math::isPowerOfTwo(realWidth);
        bool isNotPO2Height=!Math::isPowerOfTwo(realHeight);
        bool isNotPO2=(isNotPO2Width||isNotPO2Height);
        
        if(isNotPO2Width)
            realWidth=Math::nextPowerOfTwo(realWidth);
        if(isNotPO2Width)
            realHeight=Math::nextPowerOfTwo(realHeight);
        
        DEBUG_MESSAGE_IF(textureUVOffset && isNotPO2 ,"WARRNING Texture: "
                         "the device isn't support non-power-of-two texture, "
                         "uv mapping is probably failed");
        
        if(!textureUVOffset && isNotPO2)
        {
            //calc offset uv
            offsetUV.z=(float)width/realWidth;
            offsetUV.w=(float)height/realHeight;
        }
    }
//#ifdef OPENGL_ES AND ENABLE_SHADERS
    //DEBUG_MESSAGE_IF(image.type==TYPE_ALPHA8,"WARRNING Texture: OpenGL ES not support alpha texture");
    if(image.type==TYPE_ALPHA8)
        image.convertAlphaTo32bit();
    uint type=image.type;
//#else
 //   uint type= image.type== GL_ALPHA8 ? GL_ALPHA : image.type;
//#endif
    //create texture
    loadFromBinaryData(image.bytes,
                       width,height,
                       realWidth,realHeight,
                       image.type,type);
    //is loaded
    loaded=true;
    //if olready getted, build mesh
    __build();
    //return
    return loaded;
}
bool Texture::unload()
{
    //unload
    DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
    //reset values
    width = height = 0;
    realWidth = realHeight = 0;
    gpuid = 0;
    //unbuild mesh
    if(po2Srpite)
        if(po2Srpite->isLoad())
            po2Srpite->unload();
    //is not loaded
    loaded=false;
    //return
    return !loaded;
}


bool Texture::loadFromBinaryData(std::vector<uchar>& bytes,
                                 uint width,
                                 uint height,
                                 uint format,
                                 uint type)
{
    return loadFromBinaryData(&bytes[0],width,height,width,height,format,type);
}
bool Texture::loadFromBinaryData(std::vector<uchar>& bytes,
                                 uint width,
                                 uint height,
                                 uint gpuwidth,
                                 uint gpuheight,
                                 uint format,
                                 uint type)
{
    return loadFromBinaryData(&bytes[0],width,height,gpuwidth,gpuwidth,format,type);
}

bool Texture::loadFromBinaryData(const uchar* bytes,
                                 uint width,
                                 uint height,
                                 uint format,
                                 uint type)
{
    return loadFromBinaryData(bytes,width,height,width,height,format,type);
}
bool Texture::loadFromBinaryData(const uchar* bytes,
                                 uint argWidth,
                                 uint argHeight,
                                 uint argGpuWidth,
                                 uint argGpuHeight,
                                 uint format,
                                 uint type)
{
    //error if olready loaded
    DEBUG_ASSERT(!loaded);
    /////////////////////////////////////////////////////////////////////
    //save width end height
    width=argWidth;
    height=argHeight;
    realWidth=argGpuWidth;
    realHeight=argGpuHeight;
    //resize
    //create a gpu texture
#ifndef DISABLE_MIDMAP
    gpuid=RenderContext::createTexture(format, type,  realWidth, realHeight, 0, bMipmaps);
#else   
    gpuid=RenderContext::createTexture(format, type,  realWidth, realHeight, 0);
#endif
    //bind
    bind();
    CHECK_GPU_ERRORS();
    //add image
    if(bytes)
    {
        RenderContext::subTexture(type,width, height, bytes);
    }
    CHECK_GPU_ERRORS();
    //is loaded
    loaded=true;
    //is not relodable
    reloadable=false;
    //if olready getted, build mesh
    __build();
    //return
    return loaded;

}