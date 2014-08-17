#include <stdafx.h>
#include <BatchingColorMesh.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////


/** BatchingColorMesh */
BatchingColorMesh::BatchingColorMesh():Mesh([this](){
                                            //create cpu buffer
                                            cpuBuffers=CpuBuffers::ptr(new BMCCBuffers());
                                       })
                                      ,countVertexs(0)
                                      ,maxSize(0)
{
#ifdef ENABLE_STREAM_BUFFER
    vertexBuffer=(0);
#endif
}

BatchingColorMesh::~BatchingColorMesh()
{
#ifdef ENABLE_STREAM_BUFFER	//unload mesh
    if( vertexBuffer )
        glDeleteBuffers(1, &vertexBuffer );
#endif
}

void BatchingColorMesh::createBuffer(size_t maxSize)
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
void BatchingColorMesh::createBufferByVertexs(size_t maxVertexs)
{
    //gpu alloc
    createBuffer(maxVertexs*sizeof(gCVertex));
    //cpu alloc
    cpuVertexs().resize(maxVertexs);
}
void BatchingColorMesh::createBufferByTriangles(size_t maxTriangles)
{
    createBufferByVertexs(maxTriangles*3);
}

void BatchingColorMesh::relase()
{
    countVertexs=0;
    box=AABox2();
}
bool BatchingColorMesh::canAdd(Mesh::ptr mesh)
{
    if(!mesh->supportBatching()) return false;
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
        futuresize+=nVfuture *sizeof(gCVertex);
    else if(mesh->getDrawMode()==DrawMode::TRIANGLE_STRIP)
        futuresize+=((nVfuture-3) * 3 + 3)*sizeof(gCVertex);
    //and now!?
    if(futuresize>maxSize)
        return false;
    //you can batching this mesh
    return true;
}

bool BatchingColorMesh::addMesh(const Mat4& modelView,Mesh::ptr mesh,const Vec4& color)
{
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=DrawMode::LINE_STRIP);
    
    if(!mesh->supportBatching())
        return false;

#define AddVertexCML(gvt)\
				cpuVertexs()[countVertexs].vt=modelView.mul2D(gvt.vt);\
				cpuVertexs()[countVertexs].uv=gvt.uv;\
				cpuVertexs()[countVertexs].color=color;\
                box.addPoint(cpuVertexs()[countVertexs].vt);\
				++countVertexs;

#define AddLastVertexCML3(i)\
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
                AddVertexCML(refVBO[i])
            }
        else
            for(auto& gvt:refVBO)
            {
                AddVertexCML(gvt)
            };
        break;
    case DrawMode::TRIANGLE_STRIP:
        if(nI)  //ibo?
        {
            //first vetexts
            AddVertexCML(refVBO[refIBO[0]])
            AddVertexCML(refVBO[refIBO[1]])
            AddVertexCML(refVBO[refIBO[2]])
            //
            for (size_t i=1; i != nI-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexCML3(-1)
                    AddLastVertexCML3(-3)
                    AddVertexCML(refVBO[refIBO[i+2]])
                }
                else
                {
                    AddLastVertexCML3(-2)
                    AddLastVertexCML3(-2)
                    AddVertexCML(refVBO[refIBO[i+2]])
                }
            }
        }
        else
        {
            //first vetexts
            AddVertexCML(refVBO[0])
            AddVertexCML(refVBO[1])
            AddVertexCML(refVBO[2])
            //cycle
            for (size_t i=1; i != nV-2; ++i)
            {
                //TRIANGLE_STRIP -> TRIANGLE
                if(i&1)
                {
                    AddLastVertexCML3(-1)
                    AddLastVertexCML3(-3)
                    AddVertexCML(refVBO[i+2])
                }
                else
                {
                    AddLastVertexCML3(-2)
                    AddLastVertexCML3(-2)
                    AddVertexCML(refVBO[i+2])
                }
            }
        }
        break;
    default:
        break;
    }

    return true;
}
void BatchingColorMesh::draw()
{
    if(!countVertexs) return;
    //////////////////////////////////////////
    RenderContext::setClientState(true, false, true, true);
    //////////////////////////////////////////
    //disable IBO
    RenderContext::unbindIndexBuffer();
#ifdef ENABLE_STREAM_BUFFER
    //enable VBO
    RenderContext::bindVertexBuffer(vertexBuffer);
    //send vertexs
    RenderContext::bufferSubData(vertexBuffer, &cpuVertexs()[0], 0, sizeof(gCVertex)*countVertexs);
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gCVertex), 0 );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gCVertex), (void*)(offsetof(gCVertex,uv)) );
    RenderContext::colorPointer(4, GL_FLOAT, sizeof(gCVertex), (void*)(offsetof(gCVertex,color)) );
#else
    //disable VBO
    RenderContext::unbindVertexBuffer();
    //set vertex
    RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gCVertex), &cpuVertexs()[0].vt.x );
    RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gCVertex), &cpuVertexs()[0].uv.x );
    RenderContext::colorPointer(4, GL_FLOAT, sizeof(gCVertex), &cpuVertexs()[0].color.x );
#endif
    //draw
    RenderContext::drawPrimitive(TRIANGLE, 0, countVertexs);
}