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
    //gen buffer
    void createBufferByVertexs(size_t maxVertexs);
    void createBufferByTriangles(size_t maxTriangles);
    //relase
    void restart(bool force=false);
    //info
    bool canAdd(Mesh::ptr mesh);
    //add mesh
    bool addMesh(const Mat4& modelView,Mesh::ptr mesh);
    
    //draw
    virtual void draw() const;
    //unbatchable
    virtual void setBatching(bool enable)
    {
        /* none */
    }

private:
    //vertex edded
    size_t countVertexs;
    //alloc GPU memory
    void createBuffer();

};


};

#endif