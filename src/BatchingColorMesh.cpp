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
        glGenBuffers(1, &vertexBuffer );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxSize,0, GL_STREAM_DRAW);
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
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINE_STRIP);
    
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
    case Mesh::DrawMode::TRIANGLE:
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
    case Mesh::DrawMode::TRIANGLE_STRIP:
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
    //////////////////////////////////////////
    //disable IBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    //enable color
    glEnableClientState( GL_COLOR_ARRAY );
    //////////////////////////////////////////
#ifdef ENABLE_STREAM_BUFFER
    //enable VBO
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    //send vertexs
    glBufferSubData(GL_ARRAY_BUFFER, //target
                    0,               //offset
                    sizeof(gCVertex)*countVertexs, //size
                    &cpuVertexs()[0] //data
                   );
    //set vertex
    glVertexPointer(2, GL_FLOAT, sizeof(gCVertex), 0 );
    glTexCoordPointer(2, GL_FLOAT, sizeof(gCVertex), (void*)(sizeof(float)*2) );
    glColorPointer(4, GL_FLOAT, sizeof(gCVertex), (void*)(sizeof(float)*4) );
#else
    //disable VBO
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //set vertex
    static const size_t sizeofGV=sizeof(gCVertex);
    glVertexPointer(2, GL_FLOAT, sizeofGV, &cpuVertexs()[0].vt.x );
    glTexCoordPointer(2, GL_FLOAT, sizeofGV, &cpuVertexs()[0].uv.x );
    glColorPointer(4, GL_FLOAT, sizeofGV, &cpuVertexs()[0].color.x );
#endif
    //draw
    glDrawArrays( GL_TRIANGLES, 0, countVertexs );
    //////////////////////////////////////////
    //disable
    glDisableClientState( GL_COLOR_ARRAY );
    //////////////////////////////////////////
}