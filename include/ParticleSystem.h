#ifndef PARTICLE_SYSTEM_H
#define PARTICL_ESYSTEM_H

#include <Config.h>
#include <Color.h>
#include <Math3D.h>
#include <Renderable.h>
#include <BatchingColorMesh.h>

namespace Easy2D 
{

class Emitter : public Renderable
{
    struct Particle
    {

        struct Relative
        { 
            Vec2 pos,scale;
            float c,s;
        }
        relative;

        Vec2 pos,dir;
        Vec2 scale,deltaScale;
        Vec4 color,deltaColor;
        float spin,deltaSpin;
        float time;
        Particle *prev,*next;

        Particle():prev(nullptr)
                  ,next(nullptr)
        {
        }
        //relative
        void doRelative(Object* obj);
        void unRelative();
    };
    ////////////////////////
    //info particle
    struct ParticleInfo
    {
        Vec2 position,dir;
        Vec2 startScale,endScale;
        Vec4 startColor,endColor; 
        float startSpin,endSpin;
        float life;

        ParticleInfo()
            :startSpin(0)
            ,endSpin(0)
            ,life(0)
        {
        }
    };
    ParticleInfo center;
    ParticleInfo var;
    ////////////////////////
    //commond values
    float emissionRate;
    float emissionCount;
    float duration;
    float durationCount;
    bool  relative;
    Vec2  gravity;
    ////////////////////////
    class Link
    {
        size_t count;
        Particle *first, *last;

        public:

        Link()
            :count(0)
            ,first(nullptr)
            ,last(nullptr)
        {
        }
        Particle* append(Particle* np)
        {
            if(!first && !last) 
            {
                first=np;
                last=np;
                np->prev=nullptr;
                np->next=nullptr;
            }
            else if(first && last)
            {
                np->prev=last;
                np->next=nullptr;

                last->next=np;
                last=np;
            }
            //else{ wrong }
            ++count;
            //return 
            return np;
        }
        Particle* erase(Particle* np)
        {
            //if first
            if(first==np)
                first=np->next;
            //if last
            if(last==np)
                last=np->prev;
            //dt node
            if(np->next)
                np->next->prev=np->prev;
            if(np->prev)
                np->prev->next=np->next;
            //uncount
            --count;
            //unlink
            np->next=nullptr;
            np->prev=nullptr;

            return np;
        }
        size_t size()
        {
            return count;
        }
        Particle* getFirst()
        {
            return first;
        }
        Particle* getLast()
        {
            return last;
        }
        void reset()
        {
            first=nullptr;
            last=nullptr;
        }
    };
    ////////////////////////
    Link active;
    Link inactive;
    ////////////////////////
    size_t size;
    Particle* particles;
    BatchingColorMesh* mesh;
    bool pause, stop;
    ////////////////////////
    void initBuffer(size_t argsize)
    {
        //init variables
        durationCount=0;
        emissionCount=0;
        active.reset();
        inactive.reset();
        mesh->relase();
        //save size
        size=argsize;
        //new buffer
        particles=new Particle[size];
        //mesh buffer
        mesh->createBufferByTriangles(size*2);
        //linking
        for(size_t i=0;i!=size;++i)
        {
            inactive.append(&particles[i]);
        }
    }
    void destroyBuffer()
    {
        if(particles)
        {
            delete[] particles;
            size=0;
            emissionCount=0;
            active.reset();
            inactive.reset();
        }
    }
    Particle* newParticle()
    {
        if(!inactive.getFirst()) return nullptr;
        auto newnode=inactive.erase(inactive.getFirst());
        return active.append(newnode);
    }
    void removeParticle(Particle* np)
    {
        active.erase(np);
        inactive.append(np);
    }
    void memoryRestart()
    {   
        durationCount=0;
        emissionCount=0;
        active.reset();
        inactive.reset();
        mesh->relase();
        for(size_t i=0;i!=size;++i)
        {
            inactive.append(&particles[i]);
        }
    }
    ////////////////////////
    Particle* emitParticle();

    public:

    Emitter(size_t size=0):
           emissionRate(0)
          ,emissionCount(0)
          ,duration(-1)
          ,durationCount(0)
          ,relative(false)
          ,size(0)
          ,particles(nullptr)
          ,pause(false)
          ,stop(false)
    {
        mesh=new BatchingColorMesh();
        setMesh(Mesh::ptr((Mesh*)mesh));
        if(size)  setMaxParticles(size);
    }
    virtual ~Emitter()
    {
        destroyBuffer();
    }
    //I can batching!?
    virtual bool doBatching()
    {
        return false;
    }
    virtual bool canTransform()
    {
        return !relative;
    }
    //update
    void update(float dt);
    void build();   
    //set duration
    void setDuration(float argduration)
    {
        duration=argduration;
    }
    float getDuration() const
    {
        return duration;
    }
    //relative
    bool getRelative()
    {
        return relative;
    }
    void setRelative(bool argrelative);
    //gravity
    void setGravity(const Vec2& arggravity)
    {
        gravity=arggravity;
    }
    const Vec2& getGravity() const
    {
        return gravity;
    }
    //info 
    bool getPause() const
    {
        return pause;
    }
    void setPause(bool argpause)
    {
        pause=argpause;
    }
    bool getStop() const
    {
        return stop;
    }
    void setStop(bool argstop)
    {
        stop=argstop;
    }
    void restart()
    {
        pause=false;
        stop=false;
        memoryRestart();
    }
    //emission
    void setMaxParticles(size_t n)
    {
        destroyBuffer();
        initBuffer(n);
    }
    size_t getMaxParticles() const
    {
        return size;
    }
    void setEmissionRate(float emrate)
    {
        emissionRate=emrate;
    }
    float getEmissionRate() const
    {
        return emissionRate;
    }
    //life
    void setLife(float time)
    {
        center.life=time;
    }
    void setLifeVar(float time)
    {
        var.life=time;
    }
    float getLife() const
    {
        return center.life;
    }
    float getLifeVar() const
    {
        return var.life;
    }
    //position
    void setPosition(const Vec2& p)
    {
        center.position=p;
    }    
    void setPositionVar(const Vec2& p)
    {
        var.position=p;
    }
    const Vec2& getPosition() const
    {
        return center.position;
    }    
    const Vec2& getPositionVar() const
    {
        return var.position;
    }
    //direction
    void setDirection(const Vec2& p)
    {
        center.dir=p;
    }
    void setDirectionVar(const Vec2& p)
    {
        var.dir=p;
    }    
    const Vec2& getDirection() const
    {
        return center.dir;
    }    
    const Vec2& getDirectionVar() const
    {
        return var.dir;
    } 
    //scale
    void setStartScale(const Vec2& p)
    {
        center.startScale=p;
    }
    void setStartScaleVar(const Vec2& p)
    {
        var.startScale=p;
    }
    const Vec2& getStartScale() const
    {
        return center.startScale;
    }    
    const Vec2& getStartScaleVar() const
    {
        return var.startScale;
    } 
    void setEndScale(const Vec2& p)
    {
        center.endScale=p;
    }
    void setEndScaleVar(const Vec2& p)
    {
        var.endScale=p;
    }
    const Vec2& getEndScale() const
    {
        return center.endScale;
    }    
    const Vec2& getEndScaleVar() const
    {
        return var.endScale;
    } 
    //color
    void setStartColor(const Color& c)
    {
        center.startColor=c.toNormalize();
    }
    void setStartColorVar(const Color& c)
    {
        var.startColor=c.toNormalize();
    }
    Color getStartColor() const
    {
        Color out; 
        return out.fromNormalize(center.startColor);
    }
    Color getStartColorVar() const
    {
        Color out; 
        return out.fromNormalize(var.startColor);
    }
    void setEndColor(const Color& c)
    {
        center.endColor=c.toNormalize();
    }
    void setEndColorVar(const Color& c)
    {
        var.endColor=c.toNormalize();
    }
    Color getEndColor() const
    {
        Color out; 
        return out.fromNormalize(center.endColor);
    }
    Color getEndColorVar() const
    {
        Color out; 
        return out.fromNormalize(var.endColor);
    }
    //spin
    void setStartSpin(float s)
    {
        center.startSpin=s;
    }
    void setStartSpinVar(float s)
    {
        var.startSpin=s;
    }
    float getStartSpin() const
    {
        return center.startSpin;
    }
    float getStartSpinVar() const
    {
        return var.startSpin;
    }
    void setEndSpin(float s)
    {
        center.endSpin=s;
    }
    void setEndSpinVar(float s)
    {
        var.endSpin=s;
    }
    float getEndSpin() const
    {
        return center.endSpin;
    }
    float getEndSpinVar() const
    {
        return var.endSpin;
    }
    
    //event: run
    virtual void onRun(float dt);
    //is a component
    DERIVATE_COMPONENT(Emitter)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(Emitter,"Emitter")


};

#endif