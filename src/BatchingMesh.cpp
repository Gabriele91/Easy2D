#include <stdafx.h>
#include <BatchingMesh.h>
#include <Debug.h>
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
        glGenBuffers(1, &vertexBuffer );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxSize,0, GL_STREAM_DRAW);
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
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINE_STRIP);
    
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
    case Mesh::DrawMode::TRIANGLE:
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
    case Mesh::DrawMode::TRIANGLE_STRIP:
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
    //disable IBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

#ifdef ENABLE_STREAM_BUFFER
    //enable VBO
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    //send vertexs
    glBufferSubData(GL_ARRAY_BUFFER, //target
                    0,               //offset
                    sizeof(gVertex)*countVertexs, //size
                    &cpuVertexs()[0] //data
                   );
    //set vertex
    glVertexPointer(2, GL_FLOAT, sizeof(gVertex), 0 );
    glTexCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec2) );
#else
    //disable VBO
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //set vertex
    static const size_t sizeofGV=sizeof(gVertex);
    glVertexPointer(2, GL_FLOAT, sizeofGV, &cpuVertexs()[0].vt.x );
    glTexCoordPointer(2, GL_FLOAT, sizeofGV, &cpuVertexs()[0].uv.x );
#endif
    //draw
    glDrawArrays( GL_TRIANGLES, 0, countVertexs );
}

