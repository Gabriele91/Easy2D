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
    //gen buffer
    void createBufferByVertexs(size_t maxVertexs);
    void createBufferByTriangles(size_t maxTriangles);
    //relase
    void restart(bool force=false);
    //info
    bool canAdd(Mesh::ptr mesh);
    //add mesh
    bool addMesh(const Mat4& modelView,Mesh::ptr mesh,const Vec4& color);
    
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