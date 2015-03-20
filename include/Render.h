#ifndef RENDER_H
#define RENDER_H

#include <queue>
#include <Config.h>
#include <Screen.h>
#include <Camera.h>
#include <Color.h>
#include <Sortable.h>
#include <Renderable.h>
#include <RenderState.h>
#include <RenderQueue.h>
#include <BatchingMesh.h>

namespace Easy2D
{
//class declaretion
class Render;
class RenderQueue;
class PostEffects;

//effects
class PostEffects : public Pointers<PostEffects>
{
    
    //friend class
    friend class Render;
    //model
    Mesh                        plane;
    //render mode
    class Effect
    {
    public:
    
        bool blend;
        uint bsrc,bdst;
        Shader::ptr shader;
        
        //init effect
        Effect(Shader::ptr shader=nullptr)
        :blend(true)
        ,bsrc(BLEND::ONE)
        ,bdst(BLEND::ZERO)
        ,shader(shader)
        {
        }
        //shader and blend
        Effect(Shader::ptr shader,
               bool blend,
               uint bsrc,
               uint bdst)
        :blend(blend)
        ,bsrc(bsrc)
        ,bdst(bdst)
        ,shader(shader)
        {
        }
        //cast operator
        operator Shader::ptr ()
        {
            return shader;
        }
        //equal operator
        bool  operator == (Shader::ptr shader)
        {
            return this->shader==shader;
        }
    
    };
    //list of effects
    std::vector<Effect>   effects;
    //post effect costructor
    PostEffects();
    
public:
    
    //post effect destructor
    ~PostEffects();
    //add shaders effect
    void addEffect(Shader::ptr shader)
    {
        effects.push_back(shader);
    }
    void addEffect(Shader::ptr shader,uint bsrc, uint bdst)
    {
        effects.push_back(Effect(shader,true,bsrc,bdst));
    }
    void addEffect(Shader::ptr shader,bool blend,uint bsrc, uint bdst)
    {
        effects.push_back(Effect(shader,blend,bsrc,bdst));
    }
    //remove shader
    void removeEffect(Shader::ptr shader)
    {
        effects.erase(findEffect(shader));
    }
    std::vector<Effect>::iterator findEffect(Shader::ptr shader)
    {
        std::vector<Effect>::iterator first=effects.begin();
        while (first!=effects.end())
        {
            if (*first==shader)
            {
                return first;
            }
            ++first;
        }
        return effects.end();
    }
    //draw effects
    void draw(const RenderContext::RenderTarget& target);
    //has a post effect
    bool hasPostEffects() const
    {
        return effects.size()!=0;
    }
    
};

//Render
class Render
{
protected:
    //post effect
    PostEffects::ptr  effects;
    //info screen
    Camera *camera;
    //colors
    bool enableBatching;
    bool  enableClear;
    Color clearClr;
    Color ambientClr;
    //Batching
	Mesh::ptr batchingMesh;
	BatchingMesh& getBatchingMesh();
    //render queue
    RenderQueue::ptr queue;
	friend class RenderQueue;
    //called from scene
    void buildQueue(const std::list<Object*>& objs);
    //draw
    void draw();
	//init render
	virtual void init(); 

public:
    //
    Render();
    virtual ~Render() {}
    //setting
    DFORCEINLINE void setCamera(Camera *cam)
    {
        camera=cam;
    }
    DFORCEINLINE Camera* getCamera()
    {
        return camera;
    }
    //
    DFORCEINLINE void setClear(const Color& color,bool enable=true)
    {
        clearClr=color;
        enableClear=enable;
    }
    DFORCEINLINE void setEnableBatching(bool enable)
    {
        enableBatching=enable;
    }
    DFORCEINLINE void setEnableClear(bool enable)
    {
        enableClear=enable;
    }
    DFORCEINLINE const Color& getClear() const
    {
        return clearClr;
    }
    DFORCEINLINE const bool getClearIsEnable() const
    {
        return enableClear;
    }
    DFORCEINLINE const bool getBatchingIsEnable() const
    {
        return enableBatching;
    }
    DFORCEINLINE void setAmbientLight(const Color& color)
    {
        ambientClr=color;
    }
    DFORCEINLINE const Color& getAmbientLight() const
    {
        return ambientClr;
    }
    ///////////////////////////////////////////////////
    //POST EFFECT
    DFORCEINLINE void addPostEffect(Shader::ptr shader, bool blend=false,uint bsrc=BLEND::ONE,uint bdst=BLEND::ZERO)
    {
        if(!effects) effects=PostEffects::ptr(new PostEffects);
        effects->addEffect(shader,blend,bsrc,bdst);
    }
    DFORCEINLINE void removePostEffect(Shader::ptr shader)
    {
        if(!effects) effects=PostEffects::ptr(new PostEffects);
        effects->removeEffect(shader);
    }
    ///////////////////////////////////////////////////
    //utility
    Object* picking(const Vec2& point);
    void aabox2Draw();
    ///////////////////////////////////////////////////
    size_t queueSize()
    {
        return queue->size();
    }
    void serialize(Table& table)
    {
        table.set("clearColor",getClear().toVec4());
        table.set("enableClear",(float)getClearIsEnable());
        table.set("enableBatching",(float)getBatchingIsEnable());
        table.set("ambientLight",getAmbientLight().toVec4());
    }
    //deserrialize
    void deserialize(const Table& table)
    {
        Color clear,ambient;
        //set clear color
        clear.fromVec4(table.getVector4D("clearColor",Vec4(255,255,255,255)));
        setClear(clear);
        //if enable?
        setEnableClear(table.getFloat("enableClear",(float)enableClear)!=0.0);
        setEnableBatching(table.getFloat("enableBatching",(float)enableBatching)!=0.0);
        //get ambient color
        ambient.fromVec4(table.getVector4D("ambientLight",Vec4(255,255,255,255)));
        setAmbientLight(ambient);
    }

};

};

#endif
