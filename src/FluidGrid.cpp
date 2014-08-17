#include <stdafx.h>
#include <FluidGrid.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>


///////////////////////
namespace Easy2D
{
///////////////////////
namespace FluidGridUtils
{
///////////////////////
PointMass::PointMass()
:   acceleration(0,0,0),
    damping(0.98f),
    position(0,0,0),
    velocity(0,0,0),
    inverseMass(0)
{
}
PointMass::PointMass(const Vec3& position, float invMass)
:   acceleration(0,0,0),
    damping(0.98f),
    position(position),
    velocity(0,0,0),
    inverseMass(invMass)
{
}
void PointMass::applyForce(const Vec3& force)
{
    acceleration += force * inverseMass;
}
void PointMass::increaseDamping(float factor)
{
    damping *= factor;
}
void PointMass::update()
{
    velocity += acceleration;
    position += velocity;
    acceleration = Vec3(0,0,0);
    if (velocity.squaredLength() < 0.001f * 0.001f)
    {
        velocity = Vec3(0,0,0);
    }
 
    velocity *= damping;
    damping = 0.98f;
}
///////////////////////
Spring::Spring(PointMass* end1, PointMass* end2, float stiffness, float damping)
:   end1(end1),
    end2(end2),
    targetLength(end1->position.distance(end2->position) * 0.95f),
    stiffness(stiffness),
    damping(damping)
{
} 
void Spring::update()
{
    Vec3 x = end1->position - end2->position;
 
    float length = x.length();
 
    if (length > targetLength)
    {
        x = (x / length) * (length - targetLength);
        Vec3 dv = end2->velocity - end1->velocity;
        Vec3 force = stiffness * x - dv * damping;
 
        end1->applyForce(-force);
        end2->applyForce(force);
    }
}
///////////////////////
void Grid::createGrid(const Vec2& asize,const Vec2& aspacing)
{
    //save
    size=asize;
    spacing=aspacing;
    //calc size
    int numColumns = (int)((float)size.x / spacing.x) + 1;
    int numRows = (int)((float)size.y / spacing.y) + 1;
    //init points
    points.resize(numColumns*numRows);
    cols = numColumns;
    rows = numRows;
    //init
    fixedPoints.resize(numColumns * numRows);
 
    int column = 0, row = 0;
    for (float y = 0; y <= size.y; y += spacing.y)
    {
        for(float x = 0; x <= size.x; x += spacing.x)
        {
            setPoint(column, row, PointMass(Vec3(x, y, 0), 1));
            setFixPoint(column, row, PointMass(Vec3(x, y, 0), 0));
            column++;
        }
        row++;
        column = 0;
    }
 
    // link the point masses with springs
    for (int y = 0; y < numRows; y++)
    {
        for (int x = 0; x < numColumns; x++)
        {
            if (x == 0 || y == 0 || x == numColumns - 1 || y == numRows - 1)
            {
                springs.push_back(Spring(&getFixPoint(x, y), &getPoint(x, y), 0.1f, 0.1f));
            }
            else if (x % 3 == 0 && y % 3 == 0)
            {
                springs.push_back( Spring(&getFixPoint(x, y), &getPoint(x, y), 0.002f, 0.02f));
            }
 
            if (x > 0)
            {
                springs.push_back(Spring(&getPoint(x - 1, y), &getPoint(x, y), 0.28f, 0.06f));
            }
 
            if (y > 0)
            {
                springs.push_back(Spring(&getPoint(x, y - 1), &getPoint(x, y), 0.28f, 0.06f));
            }
        }
    }
}
void Grid::update()
{
    for(size_t i = 0; i < springs.size(); i++)
    {
        springs[i].update();
    }
 
    for(int i = 0; i < cols * rows; i++)
    {
        points[i].update();
    }
}
void Grid::applyDirectedForce(const Vec3& force, const Vec3& position, float radius)
{
    for(int i = 0; i < cols * rows; i++)
    {
        if (position.distancePow2(points[i].position) < radius * radius)
        {
            points[i].applyForce(10.0f * force / (10 + position.distance(points[i].position)));
        }
    }
}
void Grid::applyImplosiveForce(float force, const Vec3& position, float radius)
{
    for (int i = 0; i < cols * rows; i++)
    {
        float dist2 = position.distancePow2(points[i].position);
        if (dist2 < radius * radius)
        {
            points[i].applyForce(10.0f * force * (position - points[i].position) / (100 + dist2));
            points[i].increaseDamping(0.6f);
        }
    }
}
void Grid::applyExplosiveForce(float force, const Vec3& position, float radius)
{
    for (int i = 0; i < cols * rows; i++)
    {
        float dist2 = position.distancePow2(points[i].position);
        if (dist2 < radius * radius)
        {
            points[i].applyForce(100 * force * (points[i].position - position) / (10000 + dist2));
            points[i].increaseDamping(0.6f);
        }
    }
}
///////////////////////
};
///////////////////////
FluidGrid::FluidGrid(Texture::ptr texture)
    :Renderable(nullptr,texture,true)
    ,lsize(1.0)
{
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,
             BLEND::ONE::MINUS::SRC::ALPHA);
    //create mesh
    bcmesh=new BatchingMesh();
    //add mesh
    setMesh(Mesh::ptr((Mesh*)bcmesh));
}
//ops
void FluidGrid::applyDirectedForce(const Vec3& force,
                                   const Vec3& position,
                                   float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyDirectedForce(force,realpos,radius);
}
void FluidGrid::applyImplosiveForce(float force, 
                                    const Vec3& position,
                                    float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyImplosiveForce(force,realpos,radius);
}
void FluidGrid::applyExplosiveForce(float force, 
                                    const Vec3& position,
                                    float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyExplosiveForce(force,realpos,radius);
}

//utils
float  FluidGrid::catmullRom(const float value1, const float value2, 
                             const float value3, const float value4, 
                             float amount)
{
    // Using formula from http://www.mvps.org/directx/articles/catmull/
    float amountSquared = amount * amount;
    float amountCubed = amountSquared * amount;
    return (float)(0.5f * (2.0f * value2 +
                            (value3 - value1) * amount +
                            (2.0f * value1 - 5.0f * value2 + 4.0f * value3 - value4) * amountSquared +
                            (3.0f * value2 - value1 - 3.0f * value3 + value4) * amountCubed));
}
Vec2 FluidGrid::catmullRom(const Vec2& value1, const Vec2& value2,
                           const Vec2& value3, const Vec2& value4, 
                           float amount)
{
    return Vec2(catmullRom(value1.x, value2.x, value3.x, value4.x, amount),
                catmullRom(value1.y, value2.y, value3.y, value4.y, amount));
}

//push a line
Vec2 FluidGrid::toVec2(const Vec3& v)
{
    float factor = (v.z + 2000.0f) * 0.0005f;
    return Vec2(v.x, v.y) * factor;

}
void FluidGrid::pushLine(const Vec2& start,const Vec2& end,float size)
{
    //vars
    Vec2 psize=getTexture()->getSpriteSize();//*getObject()->getScale();
    Vec2 center=(start+end)*0.5f;
    Vec2 delta=(end-start);
    float angle=(delta.direction());
    //model
    Mat4 model;
    Transform2D local;
    local.alpha=Math::todeg(angle);
    local.position=center;
    //scale
    /*
    float l=delta.length(),
          c=cos(angle),
          s=sin(angle);
    Vec2 scale(
        l*c-size*s,
        l*s+size*c
    );*/
    Vec2 scale(delta.length(),size);
    local.scale=(scale.getAbs() /psize);
    //
    model.setTransform2D(local);
    //push
    bcmesh->addMesh(model,getTexture()->getPO2Sprite());
}
//component
void FluidGrid::onRun(float dt)
{
    //release mesh
    bcmesh->relase();
    //update grid
    grid.update();
    //update
    int width = grid.getCols();
    int height = grid.getRows();
#if 0
    //temp var
    Vec2 left, up, p;
    //for all points
    for (int y = 1; y < height; y++)
    {
        for (int x = 1; x < width; x++)
        {
            p = toVec2(grid.getPoint(x, y).position);
            if (x > 1)
            {
                left = toVec2(grid.getPoint(x-1, y).position);
                pushLine(left, p, lsize);
            }
            if (y > 1)
            {
                up = toVec2(grid.getPoint(x, y-1).position);
                pushLine( up, p, lsize);
            }
        }
    }
#else
    //temp var
    Vec2 left, up, p;
    //for all points
	//#pragma omp parallel for num_threads(2)
    for (int y = 1; y < height; ++y)
    for (int x = 1; x < width;  ++x)
    {
        p = toVec2(grid.getPoint(x, y).position);
        
        if (x > 1)
        {
            left = toVec2(grid.getPoint(x - 1, y).position);
            int clampedX = (int)Math::min(x + 1, width - 1);
            Vec2 mid = catmullRom(toVec2(grid.getPoint( x - 2, y).position), 
                                  left, 
                                  p,
                                  toVec2(grid.getPoint(clampedX, y).position), 
                                  0.5f);
 
            if (mid.distancePow2((left + p) / 2) > 1)
            {
                pushLine(left, mid, lsize);
                pushLine(mid, p, lsize);
            }
            else
            {
                pushLine(left, p, lsize);
            }
        }
        if (y > 1)
        {
            up = toVec2(grid.getPoint(x, y-1).position);
            int clampedY = (int)Math::min(y + 1, height - 1);
            Vec2 mid = catmullRom(toVec2(grid.getPoint(x, y-2).position), 
                                  up, 
                                  p,
                                  toVec2(grid.getPoint(x, clampedY).position), 
                                  0.5f);
            if (mid.distancePow2((up + p) / 2) > 1)
            {
                pushLine(up, mid, lsize);
                pushLine(mid, p, lsize);
            }
            else
            {
                pushLine(up, p, lsize);
            }
        }
    };
#endif
}
//serialize/deserialize
void FluidGrid::serialize(Table& table)
{
    Table& rfgrid=table;
    //serialize render state
    rsSerialize(rfgrid);
    //serialize texture
    rfgrid.set("texture",getTexture()->getName());
    //save size and spacing
    rfgrid.set("size",grid.getSize());
    rfgrid.set("spacing",grid.getSpacing());
    rfgrid.set("lineSize",lsize);
}
void FluidGrid::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
    //get texture
    if(table.existsAsType("texture",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }
    //get grid size
    grid.createGrid(table.getVector2D("size"),
                    table.getVector2D("spacing"));
    lsize=table.getFloat("lineSize");
    //buffer by triangles
    int width = grid.getCols();
    int height = grid.getRows();
    bcmesh->createBufferByTriangles(width*height*6);   
}
///////////////////////
    
///////////////////////
FluidMesh::FluidMesh(Texture::ptr texture)
:Renderable(nullptr,texture,true)
,lsize(1.0)
{
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,
             BLEND::ONE::MINUS::SRC::ALPHA);
    //create mesh
    mesh=Mesh::ptr(new Mesh());
    //add mesh
    setMesh(mesh);
}
//ops
void FluidMesh::applyDirectedForce(const Vec3& force,
                                   const Vec3& position,
                                   float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyDirectedForce(force,realpos,radius);
}
void FluidMesh::applyImplosiveForce(float force,
                                    const Vec3& position,
                                    float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyImplosiveForce(force,realpos,radius);
}
void FluidMesh::applyExplosiveForce(float force,
                                    const Vec3& position,
                                    float radius)
{
    Mat4 model=(getObject()?
                getObject()->getGlobalMatrix().getInverse() :
                Mat4::IDENTITY);
    Vec3 realpos=model.mul(Vec4(position,1.0f)).xyz();
    grid.applyExplosiveForce(force,realpos,radius);
}

//utils
float  FluidMesh::catmullRom(const float value1, const float value2,
                             const float value3, const float value4,
                             float amount)
{
    // Using formula from http://www.mvps.org/directx/articles/catmull/
    float amountSquared = amount * amount;
    float amountCubed = amountSquared * amount;
    return (float)(0.5f * (2.0f * value2 +
                           (value3 - value1) * amount +
                           (2.0f * value1 - 5.0f * value2 + 4.0f * value3 - value4) * amountSquared +
                           (3.0f * value2 - value1 - 3.0f * value3 + value4) * amountCubed));
}
Vec2 FluidMesh::catmullRom(const Vec2& value1, const Vec2& value2,
                           const Vec2& value3, const Vec2& value4,
                           float amount)
{
    return Vec2(catmullRom(value1.x, value2.x, value3.x, value4.x, amount),
                catmullRom(value1.y, value2.y, value3.y, value4.y, amount));
}

//push a line
Vec2 FluidMesh::toVec2(const Vec3& v)
{
    float factor = (v.z + 2000.0f) * 0.0005f;
    return Vec2(v.x, v.y) * factor;
    
}
void FluidMesh::pushLine(const Vec2& cr1,
                         const Vec2& cr2,
                         const Vec2& start,
                         const Vec2& end,
                         float size)
{
    /*
    //calc BOX
    Vec2 gsize((float)grid.getCols()-2,(float)grid.getRows()-2);
    Vec2 isize=1.0f/gsize;
    Vec2 nuv(isize*(cr1-1.0));
    Vec2 uv(isize*(cr2-1.0));
    //mesh
    Mesh::ptr mesh(new Mesh());
    mesh->addVertex(start.x, end.y,     uv.u, nuv.v);
    mesh->addVertex(start.x, start.y,   uv.u,  uv.v);
    mesh->addVertex(end.x,   end.y,    nuv.u, nuv.v);
    mesh->addVertex(end.x,   start.y,  nuv.u,  uv.v);
    mesh->setDrawMode(Mesh::TRIANGLE_STRIP);
    //push
    bcmesh->addMesh(Mat4::IDENTITY,mesh);
    */
}
//component
void FluidMesh::onRun(float dt)
{
    //release mesh
    mesh->cpuClear();
    mesh->release();
    mesh->setDrawMode(TRIANGLE_STRIP);
    //update grid
    grid.update();
    //update
    int width = grid.getCols();
    int height = grid.getRows();
    //temp var
    Vec2 p;
    Vec2 gsize((float)grid.getCols()-1,(float)grid.getRows()-1);
    Vec2 isize=1.0f/gsize;
    //for all points
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            p = toVec2(grid.getPoint(x, y).position);
            //calc uv
            Vec2 uv(isize*Vec2(x,y));
            //mesh
            mesh->addVertex(p.x,p.y,uv.u,uv.v);
        }
    }
    //indexs
    for ( int row=0; row<height-1; ++row ) {
        if ( (row&1)==0 )
        { // even rows
            for ( int col=0; col<width; ++col )
            {
                mesh->addIndex(col + row * width);
                mesh->addIndex(col + (row+1) * width);
            }
        }
        else
        { // odd rows
            for ( int col=width-1; col>0; --col )
            {
                mesh->addIndex(col + (row+1) * width);
                mesh->addIndex(col - 1 + + row * width);
            }
        }
    }
    if ( (height&1) && height>2 )
    {
        mesh->addIndex((height-1) * width);
    }
}
//serialize/deserialize
void FluidMesh::serialize(Table& table)
{
    Table& rfgrid=table;
    //serialize render state
    rsSerialize(rfgrid);
    //serialize texture
    rfgrid.set("texture",getTexture()->getName());
    //save size and spacing
    rfgrid.set("size",grid.getSize());
    rfgrid.set("spacing",grid.getSpacing());
    rfgrid.set("lineSize",lsize);
}
void FluidMesh::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
    //get texture
    if(table.existsAsType("texture",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }
    //get grid size
    grid.createGrid(table.getVector2D("size"),
                    table.getVector2D("spacing"));
    lsize=table.getFloat("lineSize");
    //buffer by triangles
    //int width = grid.getCols();
    //int height = grid.getRows();
    //bcmesh->createBufferByTriangles(width*height*6);
}
///////////////////////
};
