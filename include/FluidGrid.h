#ifndef FLUIDGRID_H
#define FLUIDGRID_H

#include <Config.h>
#include <Math3D.h>
#include <Renderable.h>
#include <BatchingMesh.h>
#include <Object.h>
//from http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-for-ios-the-warping-grid--gamedev-14637

namespace Easy2D
{

namespace FluidGridUtils
{

class PointMass
{
protected:
    Vec3   acceleration;
    float  damping;
 
public:
    Vec3   position;
    Vec3   velocity;
    float  inverseMass;
 
public:
    PointMass();
    PointMass(const Vec3& position, float invMass);
    void applyForce(const Vec3& force);
    void increaseDamping(float factor);
    void update();
};
class Spring
{
public:
    PointMass*  end1;
    PointMass*  end2;
    float       targetLength;
    float       stiffness;
    float       damping;
 
public:
    Spring(PointMass* end1, PointMass* end2, float stiffness, float damping);
    void update();
};
class Grid
{
    std::vector<Spring> springs;
    std::vector<PointMass> points;
    std::vector<PointMass> fixedPoints;
    int cols,rows;
    Vec2 size,spacing;
    PointMass& getFixPoint(int x,int y)
    {
        return fixedPoints[x+(y*cols)];
    }    
    void setFixPoint(int x,int y,const PointMass& point)
    {
        fixedPoints[x+(y*cols)]=point;
    }

    public:

    Grid()
    :cols(0)
    ,rows(0)
    {
    }

    void createGrid(const Vec2& size,const Vec2& spacing);

    PointMass& getPoint(int x,int y)
    {
        return points[x+(y*cols)];
    }    
    void setPoint(int x,int y,const PointMass& point)
    {
        points[x+(y*cols)]=point;
    }
    
    int getRows()
    {
        return rows;
    }
    int getCols()
    {
        return cols;
    }
    Vec2 getSize()
    {
        return size;
    }
    Vec2 getSpacing()
    {
        return spacing;
    }
    
    void update();
    void applyDirectedForce(const Vec3& force, const Vec3& position, float radius);
    void applyImplosiveForce(float force, const Vec3& position, float radius);
    void applyExplosiveForce(float force, const Vec3& position, float radius);
};

};

class FluidGrid : public Renderable
{

protected:
    //Fluid grid
    BatchingMesh* bcmesh;
    FluidGridUtils::Grid grid;
    float lsize;
    //protected set mesh
    DFORCEINLINE void setMesh(Mesh::ptr mesh)
    {
        Renderable::setMesh(mesh);
    }
    //utils
    float  catmullRom(const float value1, const float value2, 
                      const float value3, const float value4, 
                      float amount);
    Vec2 catmullRom(const Vec2& value1, const Vec2& value2,
                    const Vec2& value3, const Vec2& value4, 
                    float amount);
 
    //push a line
    Vec2 toVec2(const Vec3& v);
    void pushLine(const Vec2& start,const Vec2& end,float size);

public:

    //costructor
    FluidGrid(Texture::ptr image=nullptr);
    //no batching
    virtual bool doBatching()
    {
        return false;
    }
    //set info
    void createGrid(const Vec2& size,const Vec2& spacing);
    void setSizeLine(float size);
    //ops
    void applyDirectedForce(const Vec3& force, const Vec3& position, float radius);
    void applyImplosiveForce(float force, const Vec3& position, float radius);
    void applyExplosiveForce(float force, const Vec3& position, float radius);
    //is a component
    DERIVATE_COMPONENT(FluidGrid)
    //component
    virtual void onRun(float dt);
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(FluidGrid, "FluidGrid")

class FluidMesh : public Renderable
{
    
    protected:
    //Fluid grid
    Mesh::ptr mesh;
    FluidGridUtils::Grid grid;
    float lsize;
    //protected set mesh
    DFORCEINLINE void setMesh(Mesh::ptr mesh)
    {
        Renderable::setMesh(mesh);
    }
    //utils
    float  catmullRom(const float value1, const float value2,
                      const float value3, const float value4,
                      float amount);
    Vec2 catmullRom(const Vec2& value1, const Vec2& value2,
                    const Vec2& value3, const Vec2& value4,
                    float amount);
    
    //push a line
    Vec2 toVec2(const Vec3& v);
    void pushLine(const Vec2& cr1,
                  const Vec2& cr2,
                  const Vec2& start,
                  const Vec2& end,
                  float size);
    
    public:
    
    //costructor
    FluidMesh(Texture::ptr image=nullptr);
    //no batching
    virtual bool doBatching()
    {
        return false;
    }
    //build values
    void createGrid(const Vec2& size,const Vec2& spacing);
    void setSizeLine(float size);
    //ops
    void applyDirectedForce(const Vec3& force, const Vec3& position, float radius);
    void applyImplosiveForce(float force, const Vec3& position, float radius);
    void applyExplosiveForce(float force, const Vec3& position, float radius);
    //is a component
    DERIVATE_COMPONENT(FluidMesh)
    //component
    virtual void onRun(float dt);
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
    
};
REGISTERED_COMPONENT(FluidMesh, "FluidMesh")

};

#endif