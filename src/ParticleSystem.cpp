#include <stdafx.h>
#include <Object.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
#include <Renderable.h>
#include <ParticleSystem.h>
///////////////////////
using namespace Easy2D;
///////////////////////


Emitter::Emitter(size_t size):
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
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,
             BLEND::ONE::MINUS::SRC::ALPHA);
    //default parametes
    setStartScale(Vec2::ONE);
    setStartColor(Color::WHITE);
    setLife(1.0);
    //mesh
    mesh=new BatchingColorMesh();
    setMesh(Mesh::ptr((Mesh*)mesh));
    //size
    if(size)  setMaxParticles(size);
}

void Emitter::Particle::doRelative(Object* obj)
{
    relative.pos=obj->getPosition(true);
    //start pos
    pos+=relative.pos;
    //dir
    Angle rrot=obj->getRotation(true);
    float c= Math::cos(rrot);
    float s= Math::sin(rrot);
    float x= c*dir.x-s*dir.y;
    float y= s*dir.x+c*dir.y;
    dir.x=x;
    dir.y=y;
    //save
    relative.c=c;
    relative.s=s;
    //scale
    relative.scale=obj->getScale(true);
    scale*=relative.scale;
    deltaScale*=relative.scale;
    dir*=relative.scale;
}
void Emitter::Particle::unRelative()
{
    //inverse scale
    scale/=relative.scale;
    deltaScale/=relative.scale;
    dir/=relative.scale;
    //inverse dir
    float c=-relative.c;
    float s=-relative.s;
    float x= c*dir.x-s*dir.y;
    float y= s*dir.x+c*dir.y;
    dir.x=x;
    dir.y=y;
    //inverse pos
    pos-=relative.pos;
}
Emitter::Particle* Emitter::emitParticle()
{
    auto p=newParticle();
    if(p)
    {
        //set pos
        p->pos.x=center.position.x+var.position.x*Math::randomRange(1,-1);
        p->pos.y=center.position.y+var.position.y*Math::randomRange(1,-1);
        //set dir
        p->dir.x=center.dir.x+var.dir.x*Math::randomRange(1,-1);
        p->dir.y=center.dir.y+var.dir.y*Math::randomRange(1,-1);    
        //set scale
        p->scale.x=center.startScale.x+var.startScale.x*Math::randomRange(1,-1);
        p->scale.y=center.startScale.y+var.startScale.y*Math::randomRange(1,-1);
        //set color
        #define setColorRange(to,c,from)\
        to.c=center.from.c+var.from.c*Math::randomRange(1,-1);
        setColorRange(p->color,r,startColor)
        setColorRange(p->color,g,startColor)
        setColorRange(p->color,b,startColor)
        setColorRange(p->color,a,startColor)
        //set spin
        p->spin = center.startSpin+(var.startSpin * Math::randomRange(1,-1));
        //set life time
        p->time=center.life+var.life*Math::randomRange(1,-1);
            
        //calc delta of attributes
        //scale delta
        Vec2 endScale;
        endScale.x=center.endScale.x+var.endScale.x*Math::randomRange(1,-1);
        endScale.y=center.endScale.y+var.endScale.y*Math::randomRange(1,-1);
        p->deltaScale=(endScale-p->scale)/p->time;
        //color delta
        Vec4 endColor;
        setColorRange(endColor,r,endColor)
        setColorRange(endColor,g,endColor)
        setColorRange(endColor,b,endColor)
        setColorRange(endColor,a,endColor)
        p->deltaColor=(endColor-p->color)*(1.0f/p->time);
        //spin
        Angle endSpin=center.endSpin + (var.endSpin * Math::randomRange(1,-1));
        p->deltaSpin=(endSpin-p->spin)/p->time;

        //if is relative
        if(relative) 
            p->doRelative(getObject());
        else
            p->relative=Particle::Relative();
    }
    return p;
}
void  Emitter::onRun(float dt)
{
    if(!pause)
    {
        //update logic
        update(dt);
        //gen mesh only if visible
        if(isVisible())
        {
            build();
        }
    }
}
void Emitter::update(float dt)
{
    if(duration>=0)
    {
        durationCount+=dt;
        if(durationCount>=duration)
            setStop(true);
    }

    if(active.size()!=size && !stop)
    {
        float rate=1.0/emissionRate;
        emissionCount+=dt;
        //emit
        while(active.size()!=size && emissionCount>rate)
        {
            Particle* np=emitParticle();
            emissionCount-=rate;
        }
    }

    for(Particle* p=active.getFirst();p;)
    {
        //next dir
        p->dir+=gravity*dt;
        //next pos
        p->pos+=p->dir*dt;
        //next scale
        p->scale+=p->deltaScale*dt;
        //next color
        p->color=p->color+p->deltaColor*dt;
        //next spin
        p->spin+=p->deltaSpin*dt;
        //updata time
        p->time-=dt;
        //if time < 0
        if(p->time < 0.0f)
        {
            Particle *toDel=p;
            p=p->next;
            removeParticle(toDel);
        }
        else
        {
            p=p->next;
        }
    }
}
void  Emitter::build()
{
    //restart
    mesh->restart();
    //texture?
    if(!getTexture()) return;
    //add particles
    for(Particle* p=active.getLast();p;p=p->prev)
    {
        Mat4 model;
        model.setRotZ(p->spin);
        model[12]=p->pos.x;
        model[13]=p->pos.y;
        model.addScale(p->scale);
        //
        mesh->addMesh(model,getTexture()->getPO2Sprite(),p->color);
    }
}
void Emitter::setRelative(bool argrelative)
{
    if(relative && !argrelative)
    {
        for(Particle* p=active.getFirst();p;p=p->next)
        {
            p->unRelative();
            p->relative=Particle::Relative();
        }
    }
    relative=argrelative;
}

void  Emitter::serialize(Table& table)
{
	rsSerialize(table);
    //////////////////////////////////////////////////////////
    table.set("maxParticles",getMaxParticles());
    table.set("emissionRate",getEmissionRate());
    table.set("gravity",getGravity());
    if(getDuration()>=0.0) 
        table.set("duration",getDuration());
    table.set("relative",(float)getRelative());
    //////////////////////////////////////////////////////////
    table.set("position",getPosition());
    table.set("direction",getDirection());

    table.set("startScale",getStartScale());
    table.set("endScale",getEndScale());

    table.set("startSpin",getStartSpin().valueDegrees());
    table.set("endSpin",  getEndSpin().valueDegrees());
    
    table.set("startColor",getStartColor().toVec4());
    table.set("endColor",getEndColor().toVec4());

    table.set("life",getLife());
    //////////////////////////////////////////////////////////
    table.set("positionVar",getPositionVar());
    table.set("directionVar",getDirectionVar());

    table.set("startScaleVar",getStartScaleVar());
    table.set("endScaleVar",getEndScaleVar());

    table.set("startSpinVar",getStartSpinVar().valueDegrees());
    table.set("endSpinVar"  ,getEndSpinVar().valueDegrees());

    table.set("startColorVar",getStartColorVar().toVec4());
    table.set("endColorVar",getEndColorVar().toVec4());

    table.set("lifeVar",getLifeVar());
    //////////////////////////////////////////////////////////
    //serialize texture
    if(getTexture())
        table.set("texture",getTexture()->getName());
    //serialize shader
    if(getShader())
        table.set("shader",getShader()->getName());
    
}
void  Emitter::deserialize(const Table& table)
{
    //////////////////////////////////////////////////////////
	rsDeserialize(table);
    //allways false
    setCanBatch(false);
    //get shader
    if(table.existsAsType("shader",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setShader(rsgroup->load<Shader>(table.getString("shader")));
    }
    //get texture
    if(table.existsAsType("texture",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);   
        //set texture 
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }
    //////////////////////////////////////////////////////////
    DEBUG_ASSERT(table.existsAsType("maxParticles",Table::FLOAT));
    //////////////////////////////////////////////////////////
    setMaxParticles(table.get("maxParticles",getMaxParticles()));
    setEmissionRate(table.get("emissionRate",getEmissionRate()));
    setDuration(table.get("duration",getDuration()));
    setGravity(table.get("gravity",getGravity()));
    setRelative(table.get("relative",getRelative()));
    center.dir=table.get("direction",getDirection());

    //////////////////////////////////////////////////////////
    center.position=table.get("position",getPosition());
    center.startScale=table.get("startScale",getStartScale());
    center.endScale=table.get("endScale",getEndScale());
    
    center.startSpin=Angle( Degree( table.getFloat("startSpin",getStartSpin().valueDegrees()) ) );
    center.endSpin  =Angle( Degree( table.getFloat("endSpin"  ,getEndSpin().valueDegrees()  ) ) );

    center.startColor=table.get("startColor",getStartColor()).toNormalize();
    center.endColor=table.get("endColor",getEndColor()).toNormalize();

    center.life=table.getFloat("life",getLife());
    //////////////////////////////////////////////////////////
    var.position=table.getVector2D("positionVar",getPositionVar());
    var.dir=table.getVector2D("directionVar",getDirectionVar());

    var.startScale=table.getVector2D("startScaleVar",getStartScaleVar());
    var.endScale=table.getVector2D("endScaleVar",getEndScaleVar());

	var.startSpin=Angle( Degree( table.getFloat("startSpinVar",getStartSpinVar().valueDegrees()) ) );
    var.endSpin  =Angle( Degree( table.getFloat("endSpinVar",getEndSpinVar().valueDegrees()) ) ); 

    var.startColor=table.get("startColorVar",getStartColorVar()).toNormalize();
    var.endColor=table.get("endColorVar",getEndColorVar()).toNormalize();

    var.life=table.getFloat("lifeVar",getLifeVar());
    //////////////////////////////////////////////////////////
}

