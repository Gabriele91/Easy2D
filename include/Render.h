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
#include <DynamicTree.h>

namespace Easy2D
{
//class declaretion
class Render;
class RenderQueue;
class PostEffects;

//effects
class PostEffects  : public AlignedAlloc<16>,
                     public Pointers<PostEffects>
{

    //friend class
    friend class Render;
    //model
    Mesh plane;
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


public:

    //post effect costructor
    PostEffects();
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
    //tree
    DynamicTree mDTree;
    //subscribe and unsuscribe
    int  subscribe(Renderable* randerable);
    void update(int index,const AABox2& box);
    void unsubscribe(int index);
    friend class Renderable;
    //post effect
    PostEffects::ptr  effects;
    //info screen
    Camera* camera       { nullptr };
    //colors
    bool enableBatching  { true  };
    bool enableClear     { true  };
    bool enableDebugDraw { false };
    Color clearClr;
    Color ambientClr;
    //Batching
	Mesh::ptr batchingMesh;
    //render queue
    RenderQueue::ptr queue{ nullptr };
	friend class RenderQueue;
    //called from scene
    void buildQueue();
    //draw
    void draw();
	void drawDebug() const;
	//init render
	virtual void init();

public:
    //
    Render();
    virtual ~Render() {}
    //setting
    void setCamera(Camera *cam);
    Camera* getCamera();
    //help to components
    Mesh::ptr getBatchingMesh();
    //colors
    void setClear(const Color& color,bool enable=true);
    void setEnableBatching(bool enable);
    void setEnableClear(bool enable);
    const Color& getClear() const;
    const bool getClearIsEnable() const;
    const bool getBatchingIsEnable() const;
    void setAmbientLight(const Color& color);
    const Color& getAmbientLight() const;
    ///////////////////////////////////////////////////
    //POST EFFECT
    void addPostEffect(Shader::ptr shader,
                       bool blend=false,
                       uint bsrc=BLEND::ONE,
                       uint bdst=BLEND::ZERO)
    {
        if(!effects) effects=PostEffects::snew();
        effects->addEffect(shader,blend,bsrc,bdst);
    }
    void removePostEffect(Shader::ptr shader)
    {
        if(!effects) effects=PostEffects::snew();
        effects->removeEffect(shader);
    }
    ///////////////////////////////////////////////////
    //utility
    Object* queuePicking(const Vec2& point) const;
    void renderDebugDraw(bool);
    ///////////////////////////////////////////////////
    //queue size
    size_t queueSize();
    //rebuild queue render target (call it when screen will be resize)
    void rebuildPostEffectTarget()
    {
        queue->rebuildTarget();
    }
    ///////////////////////////////////////////////////
    //serialize
    void serialize(Table& table);
    //deserrialize
    void deserialize(const Table& table);

};

};

#endif
