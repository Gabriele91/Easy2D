#ifndef MESH_H
#define MESH_H

#include <Config.h>
#include <Math3D.h>
#include <Color.h>
#include <EString.h>
#include <Resource.h>
#include <RenderContext.h>

namespace Easy2D
{
    
class GenericMesh
{
public:
    
    enum VertexField
    {
        NONE=0,
        POSITION2D=1,
        POSITION3D=2,
        COLOR     =4,
        NORMAL    =8,
        UV        =16
    };

    //begin create mash
    void format(uchar type, size_t vsize=0, size_t isize=0);
    
    //like opengl 1.4
    void vertex(const Vec2& vt);
    void vertex(const Vec3& vt);
    void normal(const Vec3& normal);
    void color(const Vec4& color);
    void color(const Color& color);
    void uv(const Vec2& uv);
    
    //bind
    virtual bool build(bool deletecpuinfo=true);
    
    //like OpenGL 2.X, 3.X, 4.x
    void vbuffer(const byte* b);
    
    //like OpenGL 2.X, 3.X, 4.x
    void ibuffer(const uint* b);
    
    //index
    void index(uint i);
    
    //set draw mode
    void mode(DrawMode m);
    
    //get draw mode
    virtual DrawMode getDrawMode() const
    {
        return dMode;
    }
    
    //draw
    virtual void draw() const;
    
    //clear mesh
    void clear();
    void cpuClear();
    
    //at mesh delete
    virtual ~GenericMesh();
    
    //get cpu info
    size_t svertex() const { return vertexs.size()/vSize; }
    size_t sindex() const { return indexs.size(); }
    size_t sizeVertexs() const;
    size_t sizeIndexs() const;
    uint  getIndex(size_t i) const;
    byte* getVertex(size_t i, size_t offset);
    const byte* const getVertex(size_t i, size_t offset) const;
    Vec2& getVertex2(size_t i, size_t offset);
    Vec3& getVertex3(size_t i, size_t offset);
    Vec4& getVertex4(size_t i, size_t offset);
    
protected:
    
    //buffer GPU
    //vertex
    uint sBVertex{ 0 };
    uint bVertex{ 0 };
    //index
    uint sBIndex{ 0 };
    uint bIndex{ 0 };
    //draw mode
    DrawMode dMode{ TRIANGLE };
    //size of vertex
    size_t vSize{ 0 };
    //cpu buffers
    size_t currentIndex{ 0 };
    std::vector<uint> indexs;
    size_t currentVertex{ 0 };
    std::vector<byte> vertexs;
    //save type field
    VertexField type{ NONE };
    //set buffers
    void set() const;
    //calc size
    static size_t attSize(uchar type);
    void calcVertexSize(uchar type);
    void addFild(const float* b, size_t size);
    void addVertexCPage(size_t next);
    void addIndexCPage();
    
    
};
    
class Mesh : public Resource<Mesh>, protected GenericMesh
{

public:
    
    //Surface
    Mesh(ResourcesManager<Mesh> *rsmr=NULL, const String& pathfile="");
    //clear cpu data
    void cpuClear()
    {
        GenericMesh::cpuClear();
    }
    //distruttore
    virtual ~Mesh();
    //metodo che aggiunge i vertici
    void addVertex(const Vec2& pos,const Vec2& uvmap);
    void addVertex(float x,float y, float u,float v);
    void addVertex(const Vec4& posuv);
    void addIndex(ushort mIndex);
    void addTriangleIndexs(ushort v1,ushort v2,ushort v3);
    void addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4);
    void setDrawMode(DrawMode dmode);
    //show build
    virtual bool build(bool clearcpu = false);
    //recompute the AABBox of the mesh
    void recomputeAABox();
    //show get draw mode
    virtual DrawMode getDrawMode() const
    {
        return GenericMesh::getDrawMode();
    }
    //query
    bool supportBatching()
    {
        return batching;
    }
    virtual void setBatching(bool enable)
    {
        batching=enable;
    }
    const AABox2& getAABox() const
    {
        return box;
    }
    void setAABox(const AABox2& value)
    {
        box = value;
    }
    size_t svertex() const
    {
        return GenericMesh::svertex();
    }
    size_t sindex() const
    {
        return GenericMesh::sindex();
    }
    size_t sizeVertexs() const
    {
        return GenericMesh::sizeVertexs();
    }
    size_t sizeIndexs() const
    {
        return GenericMesh::sizeIndexs();
    }
    //show
    uint  getIndex(size_t i)
    {
        return GenericMesh::getIndex(i);
    }
    byte* getVertex(size_t i, size_t offset)
    {
        return GenericMesh::getVertex(i,offset);
    }
    const byte * const getVertex(size_t i, size_t offset) const
    {
        return GenericMesh::getVertex(i,offset);
    }
    Vec2& getVertex2(size_t i, size_t offset)
    {
        return GenericMesh::getVertex2(i,offset);
    }
    Vec3& getVertex3(size_t i, size_t offset)
    {
        return GenericMesh::getVertex3(i,offset);
    }
    Vec4& getVertex4(size_t i, size_t offset)
    {
        return GenericMesh::getVertex4(i,offset);
    }
    //resource
    virtual bool load();
    virtual bool unload();
    
    //draw
    virtual void draw() const;

protected:
    //benchable
    bool batching;
    //draw bind mesh
    void __bind();
    //AABB
    AABox2 box;
};

};

#endif