#include <stdafx.h>
#include <BatchingMesh.h>
#include <Debug.h>
#include <RenderContext.h>
///////////////////////
using namespace Easy2D;
///////////////////////
/** BatchingMesh */
BatchingMesh::BatchingMesh():countVertexs(0),maxSize(0)
{
#ifdef ENABLE_STREAM_BUFFER
    vertexBuffer=(0);
#endif
}
//distruttore
BatchingMesh::~BatchingMesh()
{
#ifdef ENABLE_STREAM_BUFFER	//unload mesh
    if( vertexBuffer )
        glDeleteBuffers(1, &vertexBuffer );
#endif
}

void BatchingMesh::createBuffer(size_t maxSize)
{
    //save max size
    this->maxSize=maxSize;
    //create the VBO
#ifdef ENABLE_STREAM_BUFFER
    if( !vertexBuffer )
        vertexBuffer=RenderContext::createBuffer();
    RenderContext::bufferData(vertexBuffer, STREAM, 0, maxSize);
#endif
}
void BatchingMesh::createBufferByVertexs(size_t maxVertexs)
{
    //gpu alloc
    createBuffer(maxVertexs*sizeof(gVertex));
    //cpu alloc
    cpuVertexs().resize(maxVertexs);
}
void BatchingMesh::createBufferByTriangles(size_t maxTriangles)
{
    createBufferByVertexs(maxTriangles*3);
}

void BatchingMesh::relase()
{
    countVertexs=0;
    box=AABox2();
}
bool BatchingMesh::canAdd(Mesh::ptr mesh)
{
    //ibo
    size_t nI=mesh->getCpuIndexs()->size();
    size_t nV=mesh->getCpuVertexs()->size();
    //invalid mesh vertexs count
    int nVfuture=(nI ? nI : nV);
    if(nVfuture <3) return false;
    //future size < max size?
    size_t futuresize=bitesSize();
    //olready superated!?
    if(futuresize>maxSize)
        return false;
    else if(mesh->getDrawMode()==DrawMode::TRIANGLE)
        futuresize+=nVfuture *sizeof(gVertex);
    else if(mesh->getDrawMode()==DrawMode::TRIANGLE_STRIP)
        futuresize+=((nVfuture-3) * 3 + 3)*sizeof(gVertex);
    //and now!?
    if(futuresize>maxSize)
        return false;
    //you can batching this mesh
    return true;
}

bool BatchingMesh::addMesh(const Mat4& modelView,Mesh::ptr mesh)
{
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINE_STRIP);
    
    if(!mesh->supportBatching())
        return false;

#define AddVertexML(gvt)\
				cpuVertexs()[countVertexs].vt=modelView.mul2D(gvt.vt);\
				cpuVertexs()[countVertexs].uv=gvt.uv;\
                box.addPoint(cpuVertexs()[countVertexs].vt);\
				++countVertexs;

#define AddLastVertexML3(i)\
				cpuVertexs()[countVertexs]=cpuVertexs()[countVertexs+i];\
				++countVertexs;
    //ibo
    size_t nI=mesh->getCpuIndexs()->size();
    size_t nV=mesh->getCpuVertexs()->size();
    const auto& refIBO=*mesh->getCpuIndexs();
    const auto& refVBO=*mesh->getCpuVertexs();
    //invalid mesh
    if(!canAdd(mesh)) return false;
    //
    switch (mesh->getDrawMode())
    {
    case DrawMode::TRIANGLE:
        if(nI) //ibo?
            for(auto i:refIBO)
            {
                AddVertexML(refVBO[i])
            }
        else
            for(auto& gvt:refVBO)
            {
                AddVertexML(gvt)
            };
        break;
    case DrawMode::TRIANGLE_STRIP:
        if(nI)  //ibo?
        {
            //first vetexts
            AddVertexML(refVBO[refIBO[0]])
            AddVertexML(refVBO[refIBO[1]])
            AddVertexML(refVBO[refIBO[2]])
            //
            for (size_t i=1; i != nI-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexML3(-1)
                    AddLastVertexML3(-3)
                    AddVertexML(refVBO[refIBO[i+2]])
                }
                else
                {
                    AddLastVertexML3(-2)
                    AddLastVertexML3(-2)
                    AddVertexML(refVBO[refIBO[i+2]])
                }
            }
        }
        else
        {
            //first vetexts
            AddVertexML(refVBO[0])
            AddVertexML(refVBO[1])
            AddVertexML(refVBO[2])
            //cycle
            for (size_t i=1; i != nV-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexML3(-1)
                    AddLastVertexML3(-3)
                    AddVertexML(refVBO[i+2])
                }
                else
                {
                    AddLastVertexML3(-2)
                    AddLastVertexML3(-2)
                    AddVertexML(refVBO[i+2])
                }
            }
        }
        break;
    default:
        break;
    }

    return true;
}
void BatchingMesh::draw()
{
    if(!countVertexs) return;
    //clients
    RenderContext::setClientState(true, false, true, false);
    //disable IBO
    RenderContext::unbindIndexBuffer();
#ifdef ENABLE_STREAM_BUFFER
    //enable VBO
    RenderContext::bindVertexBuffer(vertexBuffer);
    //send vertexs
    RenderContext::bufferSubData(vertexBuffer, &cpuVertexs()[0], 0, sizeof(gVertex)*countVertexs);
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gVertex), 0 );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec2) );
#else
    //disable VBO
    RenderContext::unbindVertexBuffer();
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gVertex), &cpuVertexs()[0].vt.x );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gVertex), &cpuVertexs()[0].uv.x );
#endif
    //draw
    RenderContext::drawPrimitive(TRIANGLE, 0, countVertexs);
}

