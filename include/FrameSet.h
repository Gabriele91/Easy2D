#ifndef FRAMESET_H
#define FRAMESET_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Resource.h>
#include <Texture.h>
#include <Mesh.h>

namespace Easy2D
{

class FrameSet : public Resource<FrameSet>
{

    Texture::ptr texture;
    std::vector<Mesh::ptr> frames;
    void addFrame(const Vec4& frame);
    std::vector<Vec2> sizeFrames;
    float defaultTime;
    bool  defaultLoop;

public:
    //create a frame set
    FrameSet(ResourcesManager<FrameSet> *rsmr,const String& path);
    virtual ~FrameSet();
    //resource
    virtual bool load();
    virtual bool unload();
    //return numbers of frames
    DFORCEINLINE uint size()
    {
        return (uint)frames.size();
    }
    DFORCEINLINE Vec2 sizeFrame(uint i)
    {
        return sizeFrames[i];
    }
    //set default time animation
    DFORCEINLINE void setDefaultTime(float time)
    {
        defaultTime=time;
    }
    DFORCEINLINE float getDefaultTime()
    {
        return defaultTime;
    }
    DFORCEINLINE bool getDefaultLoop()
    {
        return defaultLoop;
    }
    //return a frame
    Mesh::ptr getFrame(uint i);
    //return texture
    DFORCEINLINE Texture::ptr getTexture()
    {
        return texture;
    }
    //

};

};

#endif