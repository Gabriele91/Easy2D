#ifndef MESH_H
#define MESH_H

#include <Config.h>
#include <Math3D.h>
#include <Color.h>
#include <EString.h>
#include <Resource.h>

namespace Easy2D
{

class BasicMesh 
{
    public:

    // graphics vertex
    struct gVertex
    {
        Vec2 vt,uv;
        //cast op
        gVertex(){}
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
    struct gCVertex
    {
        Vec2 vt,uv;
        Vec4 color;
        //cast op
        gCVertex(){}
    };
    //draw mode
    enum DrawMode
    {
        TRIANGLE,
        TRIANGLE_STRIP,
        LINE_STRIP,
        LINES
    };
    
    //typedef
    //list type
    typedef std::vector<gVertex> ListGVertexs;
    typedef std::vector<gCVertex> ListGCVertexs;
    typedef std::vector<ushort> ListIndexs;
    //values CPU
    class CpuBuffers : public Pointers<CpuBuffers>{ };
    CpuBuffers::ptr cpuBuffers;
    virtual void initCpuBuffer(){ cpuBuffers=nullptr; }
    template<class T> const T* cpuBuf() const{ return (T*)(&*cpuBuffers); }
    template<class T> T* cpuBuf(){ return (T*)(&*cpuBuffers); }
    //methos    
    virtual void draw()=0;
    virtual DrawMode getDrawMode() const=0;
    virtual const ListGCVertexs* getCpuCVertexs() const=0;
    virtual const ListGVertexs* getCpuVertexs() const=0;
    virtual const ListIndexs* getCpuIndexs() const=0;
    virtual const String& getName() const=0;
    virtual const bool supportBatching() const
    {
        return true;
    }
    //init
    BasicMesh(DFUNCTION<void()> initCpuBuffers)
    {
        initCpuBuffers();
    }
    //aabb
    virtual const AABox2& getAABox() const=0;
};

class Mesh : public Resource<Mesh>,
             public BasicMesh
{
protected:
    //private init mesh
    Mesh(DFUNCTION<void(void)>,
         ResourcesManager<Mesh> *rsmr=NULL,
         const String& pathfile="");

public:
    
    //Surface
    Mesh(ResourcesManager<Mesh> *rsmr=NULL,
         const String& pathfile="");
    
    //clear cpu data
    void cpuClear()
    {
        cpuVertexs().clear();
        cpuIndexs().clear();
    }
    //distruttore
    virtual ~Mesh();
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
    //resource
    virtual bool load();
    virtual bool unload();
    //basic mesh
    virtual void draw();
    virtual DrawMode getDrawMode() const
    {
        return dmode;
    }
    virtual const ListGCVertexs* getCpuCVertexs() const
    {
        return nullptr;
    }
    virtual const ListGVertexs* getCpuVertexs() const
    {
        return &(cpuBuf<MCBuffers>()->mVertexs);
    }
    virtual const ListIndexs* getCpuIndexs() const
    {
        return &(cpuBuf<MCBuffers>()->mIndexs);
    }
    virtual const String& getName() const
    {
        return Resource<Mesh>::getName();
    }
    virtual const AABox2& getAABox() const
    {
        return box;
    }

protected:
    //buffer GPU
    uint vertexBuffer;
    uint indexBuffer;
    //vba GPU
#ifdef ENABLE_VAOS
    uint vbaDraw;
#endif
    //cpu buffer
    class MCBuffers : public CpuBuffers
    {
        public:
        ListGVertexs mVertexs;
        ListIndexs mIndexs;
    };
    ListGVertexs& cpuVertexs()
    {
        return cpuBuf<MCBuffers>()->mVertexs;
    }
    ListIndexs& cpuIndexs() 
    {
        return cpuBuf<MCBuffers>()->mIndexs;
    }
    //tipo di disegno
    DrawMode dmode;
    //draw bind mesh
    void __bind();
    //AABB
    AABox2 box;
};

};

#endif