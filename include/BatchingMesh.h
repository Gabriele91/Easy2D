#ifndef BATCHINGMESH_H
#define BATCHINGMESH_H

#include <Config.h>
#include <Mesh.h>

namespace Easy2D 
{

    
class BatchingMesh : private Mesh
{
 
public:
    //costruttore
    BatchingMesh();
    //distruttore
    virtual ~BatchingMesh();

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
        return countVertexs*sizeof(gVertex);
    }
    size_t realBitesSize()
    {
        return cpuVertexs().capacity()*sizeof(gVertex);
    }

    void relase();
    bool canAdd(Mesh::ptr mesh);
    bool addMesh(const Mat4& modelView,Mesh::ptr mesh);

    //basic mesh
    virtual void draw();
    virtual DrawMode getDrawMode() const
    {
        return TRIANGLE;
    }

private:
    //buffer CPU
    size_t countVertexs;
    //size buffer
    size_t maxSize;
    //alloc memory
    void createBuffer(size_t maxSize);

};


};

#endif