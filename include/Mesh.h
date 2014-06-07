#ifndef MESH_H
#define MESH_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Resource.h>

namespace Easy2D
{


class Mesh : public Resource<Mesh>
{

public:

    // graphics vertex
    struct gVertex
    {
        Vec2 vt,uv;
        //cast op
        gVertex(const Vec4& vtuv)
        {
            vt=vtuv.xy();
            uv=Vec2(vtuv.z,vtuv.w);
        }
        gVertex(const Vec2& v)
        {
            vt=v;
        }
        gVertex(float x,float y,float u,float v):vt(x,y),uv(u,v) {}
    };
    //draw mode
    enum DrawMode
    {
        TRIANGLE,
        TRIANGLE_STRIP,
        LINE_STRIP,
        LINES
    };

    typedef std::vector<gVertex> ListGVertexs;
    typedef std::vector<ushort> ListIndexs;

protected:
    //buffer GPU
    uint vertexBuffer;
    uint indexBuffer;
    //vba GPU
#ifdef ENABLE_VAOS
    uint vbaDraw;
#endif
    //Valori CPU
    ListGVertexs mVertexs;
    ListIndexs mIndexs;
    //tipo di disegno
    DrawMode dmode;
    //draw bind mesh
    void __bind();
    //AABB
    AABox2 box;

public:
    //Surface
    Mesh(ResourcesManager<Mesh> *rsmr=NULL,
         const String& pathfile="");
    //clear cpu data
    void cpuClear()
    {
        mVertexs.clear();
        mIndexs.clear();
    }
    //distruttore
    virtual ~Mesh();
    //aabb
    DFORCEINLINE const AABox2& getAABox() const
    {
        return box;
    }
    //metodo che aggiunge i vertici
    void addVertex(const gVertex& gv);
    void addVertex(float x,float y, float u,float v)
    {
        addVertex(gVertex(x,y,u,v));
    }
    void addIndex(ushort mIndex);
    void addTriangleIndexs(ushort v1,ushort v2,ushort v3);
    void addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4);
    void setDrawMode(DrawMode dmode);
    void build();
    //query on privates
    DrawMode getDrawMode() const
    {
        return dmode;
    }
    const ListGVertexs& getCpuVertexs() const
    {
        return mVertexs;
    }
    const ListIndexs& getCpuIndexs() const
    {
        return mIndexs;
    }
    //resource
    virtual bool load();
    virtual bool unload();
    //draw
    void draw();

};

class BatchingMesh
{
    // graphics 3D vertex
    struct gVertex
    {
        Vec3 vtz;
        Vec2 uv;
        //cast op
        gVertex() {};
        gVertex(const Vec2& argvt,
                const Vec2& arguv,
                float z)
        {
            vtz=Vec3( argvt,z );
            uv=arguv;
        }
    };
    //buffer CPU
    size_t countVertexs;
    std::vector<gVertex> mVertexs;
    //buffer GPU
#ifdef ENABLE_STREAM_BUFFER
    uint vertexBuffer;
#endif
    //size buffer
    size_t maxSize;
    //alloc memory
    void createBuffer(size_t maxSize);

public:
    //costruttore
    BatchingMesh();
    //distruttore
    virtual ~BatchingMesh();

    void createBufferByVertexs(size_t maxVertexs);
    void createBufferByTriangles(size_t maxTriangles);
    size_t getBufferSize()
    {
        return maxSize;
    }

    void relase();
    bool canAdd(Mesh::ptr mesh);
    bool addMesh(const Mat4& modelView,Mesh::ptr mesh,int z);
    void draw();
    size_t bitesSize()
    {
        return countVertexs*sizeof(gVertex);
    }
    size_t realBitesSize()
    {
        return mVertexs.capacity()*sizeof(gVertex);
    }
};

};

#endif