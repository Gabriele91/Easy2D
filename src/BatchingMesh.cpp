#include <stdafx.h>
#include <BatchingMesh.h>
#include <Debug.h>
#include <RenderContext.h>
///////////////////////
using namespace Easy2D;
///////////////////////////////////////////////////////////////
//Vertex Size
static const size_t sizeGVector=sizeof(float)*4;
///////////////////////////////////////////////////////////////
/** BatchingMesh */
BatchingMesh::BatchingMesh()
             :countVertexs(0)
{
    /* disable batching */
    Mesh::batching=false;
}
//distruttore
BatchingMesh::~BatchingMesh()
{
    /* void */
}

void BatchingMesh::createBuffer()
{
#ifdef ENABLE_STREAM_BUFFER
    //create the VBO
    //assert
    DEBUG_ASSERT(bVertex==0);
    //alloc
    bVertex=RenderContext::createBuffer();
    RenderContext::bufferData(bVertex, STREAM, 0, vertexs.size());
#endif
}
void BatchingMesh::createBufferByVertexs(size_t maxVertexs)
{
    //cpu alloc
    format(POSITION2D|UV,maxVertexs);
    //gpu alloc
    createBuffer();
    //init nvertex
    countVertexs=0;
}
void BatchingMesh::createBufferByTriangles(size_t maxTriangles)
{
    createBufferByVertexs(maxTriangles*3);
}
void BatchingMesh::restart(bool force)
{
    box=AABox2();
    countVertexs=0;
    //dealloc
    if(force)
    {
        cpuClear();
#ifdef ENABLE_STREAM_BUFFER
        //delete the VBO
        if(bVertex)
        {
            RenderContext::deleteBuffer(bVertex);
            bVertex=0;
        }
#endif
    }
}
bool BatchingMesh::canAdd(Mesh::ptr mesh)
{
    //is full?
    if(countVertexs > svertex())
    {
        return false;
    }
    //size
    const size_t nI=mesh->sizeIndexs();
    const size_t nV=mesh->sizeVertexs();
    //invalid mesh vertexs count
    const size_t nVfuture=(nI ? nI : nV);
    if(nVfuture <3) return false;
    //calc size
    size_t futuresize=countVertexs * sizeGVector;
    //calc new size
    if(mesh->getDrawMode()==DrawMode::TRIANGLE)
    {
        futuresize+=nVfuture *sizeGVector;
    }
    else if(mesh->getDrawMode()==DrawMode::TRIANGLE_STRIP)
    {
        futuresize+=((nVfuture-3) * 3 + 3)*sizeGVector;
    }
    //and now!?
    if(futuresize > vertexs.size())
    {
        return false;
    }
    //you can batching this mesh
    return true;
}

bool BatchingMesh::addMesh(const Mat4& modelView,Mesh::ptr mesh)
{
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINE_STRIP);
    
    if(!mesh->supportBatching())
    {
        return false;
    }
    
    
#define AddVertexML(i)\
    getVertex2(countVertexs,0)=modelView.mul2D(mesh->getVertex2(i,0));\
    getVertex2(countVertexs,sizeof(float)*2)=mesh->getVertex2(i,sizeof(float)*2);\
    box.addPoint(getVertex2(countVertexs,0));\
    ++countVertexs;
        
#define AddLastVertexML3(i)\
    getVertex2(countVertexs,0)=getVertex2(countVertexs+(i),0);\
    getVertex2(countVertexs,sizeof(float)*2)=getVertex2(countVertexs+(i),sizeof(float)*2);\
    ++countVertexs;

    //ibo
    const size_t nI=mesh->sizeIndexs();
    const size_t nV=mesh->sizeVertexs();
    //index ref
    const auto& refIBO=indexs;
    //invalid mesh
    if(!canAdd(mesh)) return false;
    //add mesh
    switch (mesh->getDrawMode())
    {
    case DrawMode::TRIANGLE:
        if(nI) //ibo?
            for(auto i:refIBO)
            {
                AddVertexML(i)
            }
        else
            for(size_t i=0;i!=nV;++i)
            {
                AddVertexML(i)
            };
        break;
    case DrawMode::TRIANGLE_STRIP:
        if(nI)  //ibo?
        {
            //first vetexts
            AddVertexML(refIBO[0])
            AddVertexML(refIBO[1])
            AddVertexML(refIBO[2])
            //
            for (size_t i=1; i != nI-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexML3(-1)
                    AddLastVertexML3(-3)
                    AddVertexML(refIBO[i+2])
                }
                else
                {
                    AddLastVertexML3(-2)
                    AddLastVertexML3(-2)
                    AddVertexML(refIBO[i+2])
                }
            }
        }
        else
        {
            //first vetexts
            AddVertexML(0)
            AddVertexML(1)
            AddVertexML(2)
            //cycle
            for (size_t i=1; i != nV-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexML3(-1)
                    AddLastVertexML3(-3)
                    AddVertexML(i+2)
                }
                else
                {
                    AddLastVertexML3(-2)
                    AddLastVertexML3(-2)
                    AddVertexML(i+2)
                }
            }
        }
        break;
    default: break;
    }

    return true;
}
void BatchingMesh::draw() const
{
    if(!countVertexs) return;
    //clients
    RenderContext::setClientState(true, false, true, false);
    //disable IBO
    RenderContext::unbindIndexBuffer();
#ifdef ENABLE_STREAM_BUFFER
    //enable VBO
    RenderContext::bindVertexBuffer(bVertex);
    //send vertexs
    RenderContext::bufferSubData(bVertex, (void*)getVertex(0,0), 0, sizeGVector*countVertexs);
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeGVector, 0 );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeGVector, (void*)(sizeof(float)*2) );
#else
    //disable VBO
    RenderContext::unbindVertexBuffer();
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeGVector, (void*)getVertex(0,0) );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeGVector, (void*)getVertex(0, sizeof(float)*2) );
#endif
    //draw
    RenderContext::drawPrimitive(TRIANGLE, 0, countVertexs);
}

