#ifndef BATCHINGCOLORMESH_H
#define BATCHINGCOLORMESH_H

#include <Config.h>
#include <Mesh.h>


namespace Easy2D 
{

    
class BatchingColorMesh : protected Mesh
{

public:

    //costruttore
    BatchingColorMesh();
    //distruttore
    virtual ~BatchingColorMesh();

    void createBufferByVertexs(size_t maxVertexs);
    void createBufferByTriangles(size_t maxTriangles);
    size_t getMaxSize()
    {
        return maxSize;
    }    
    size_t getCurrentSize()
    {
        return countVertexs;
    }
    size_t bitesSize()
    {
        return countVertexs*sizeof(gCVertex);
    }
    size_t realBitesSize()
    {
        return cpuIndexs().capacity()*sizeof(gCVertex);
    }

    void relase();
    bool canAdd(Mesh::ptr mesh);
    bool addMesh(const Mat4& modelView, Mesh::ptr mesh, const Vec4& color);
    //basic mesh
    virtual void draw();
    virtual DrawMode getDrawMode() const
    {
        return TRIANGLE;
    }
    virtual const ListGCVertexs* getCpuCVertexs() const
    {
        return &(cpuBuf<BMCCBuffers>()->mVertexs);
    }
    virtual const ListGVertexs* getCpuVertexs() const
    {
        return nullptr;
    }
    virtual const ListIndexs* getCpuIndexs() const
    {
        return nullptr;
    }
    virtual const bool supportBatching() const
    {
        return false;
    }

private:
    //cpu buffer
    class BMCCBuffers : public CpuBuffers
    {
        public:
        ListGCVertexs mVertexs;
    };
    ListGCVertexs& cpuVertexs()
    {
        return cpuBuf<BMCCBuffers>()->mVertexs;
    }
    //buffer CPU
    size_t countVertexs;
    //size buffer
    size_t maxSize;
    //alloc memory
    void createBuffer(size_t maxSize);

};


};

#endif