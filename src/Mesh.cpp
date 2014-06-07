#include <stdafx.h>
#include <Mesh.h>
#include <Table.h>
#include <Debug.h>
#include <Application.h>

using namespace Easy2D;

//VAOs are slower (by nvidia and valve)
//#undef ENABLE_VAOS
//

Mesh::Mesh(ResourcesManager<Mesh> *rsmr,
           const String& pathfile)
    :Resource(rsmr,pathfile)
    ,vertexBuffer(0)
    ,indexBuffer(0)
    ,dmode(TRIANGLE)
{
    if(!rsmr||pathfile=="") reloadable=false;
#ifdef ENABLE_VAOS
    vbaDraw=(0);
#endif
}
//distruttore
Mesh::~Mesh()
{
    //release resource
    release();
}
//metodo che aggiunge i vertici
void Mesh::addVertex(const gVertex& gv)
{
    //update box
    box.addPoint(gv.vt);
    //add vertex
    mVertexs.push_back(gv);
}
void Mesh::addIndex(ushort mIndex)
{
    mIndexs.push_back(mIndex);
}
void Mesh::addTriangleIndexs(ushort v1,ushort v2,ushort v3)
{
    mIndexs.push_back(v1);
    mIndexs.push_back(v2);
    mIndexs.push_back(v3);
}
void Mesh::addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4)
{
    mIndexs.push_back(v1);
    mIndexs.push_back(v2);
    mIndexs.push_back(v3);

    mIndexs.push_back(v2);
    mIndexs.push_back(v3);
    mIndexs.push_back(v4);
}
//
void Mesh::setDrawMode(DrawMode dmode)
{
    this->dmode=dmode;
}
//
void Mesh::build()
{

    //create the VBO
    if( !vertexBuffer )
        glGenBuffers(1, &vertexBuffer );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertex) * mVertexs.size(), &mVertexs[0], GL_STATIC_DRAW);
    //create the IBO
    if(mIndexs.size())
    {
        if( !indexBuffer )
            glGenBuffers(1, &indexBuffer );
        glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ushort) * mIndexs.size(), &mIndexs[0], GL_STATIC_DRAW);
    }

#ifdef ENABLE_VAOS
    //create the VAO
    if( !vbaDraw )
        glGenVertexArrays( 1, &vbaDraw );
    glBindVertexArray( vbaDraw );
    //bind mesh
    __bind();
    //disable vao
    glBindVertexArray( 0 );

#endif

    //get vao/ibo/vbo errors
    CHECK_GPU_ERRORS();
    /*
    //aabb culling
    extends.x=Math::max(fabs(max.x),fabs(min.y));
    extends.y=Math::max(fabs(max.y),fabs(min.x));
    center=(max+min)/2;
    extends-=center;
    */
}
//resource
bool Mesh::load()
{
    //can't load this resource
    DEBUG_ASSERT(isReloadable());
    //
    //load file
    void *data=NULL;
    size_t len=0;
    Application::instance()->loadData(rpath,data,len);
    String filestring((char*)data);
    free(data);
    //deserialize mesh
    Table tbMesh;
    tbMesh.deserialize(filestring);
    //get data info
    //get mode
    if(tbMesh.exists("mode"))
    {
        //get string
        String strmode=tbMesh.getString("mode");
        strmode.toUpper();
        //select mode
        if(strmode=="TRIANGLE") dmode=TRIANGLE;
        else if (strmode=="TRIANGLE_STRIP") dmode=TRIANGLE_STRIP;
        else if (strmode=="LINE_STRIP") dmode=LINE_STRIP;
        else if (strmode=="LINES") dmode=LINES;
        else
        {
            DEBUG_ASSERT_MSG(0,"mesh error:"
                             "to be set a correct kind of primitives to render "
                             "(parameter:mode)");
        }
    }

    //get vertices
    if(tbMesh.existsAsType("vertexes",Table::TABLE))
    {
        const Table& tbVertexes=tbMesh.getTable("vertexes");
        for(auto param:tbVertexes)
        {
            if(param.second->asType(Table::VECTOR4D))
                addVertex(param.second->get<Vec4>());
            else
            {
                DEBUG_ASSERT_MSG(0,"mesh error:"
                                 "vertexes must be only Vec4 (x x x x) "
                                 "(parameter:vertexes)");
            }
        }
    }
    else
    {
        DEBUG_ASSERT_MSG(0,"mesh error:"
                         "must to be setted a vertexes table"
                         "(parameter:vertexes)");
        return false;
    }

    //get indexs
    if(tbMesh.existsAsType("indexs",Table::TABLE))
    {
        const Table& tbIndexs=tbMesh.getTable("indexs");
        for(auto param:tbIndexs)
        {
            if(param.second->asType(Table::FLOAT))
                addIndex(param.second->get<float>());
            else
            {
                DEBUG_ASSERT_MSG(0,"mesh error:"
                                 "indexs must be only number "
                                 "(parameter:indexs)");
            }
        }
    }
    //build (gpu) mesh
    build();
    //is loaded
    loaded=true;
    //
    return true;
}
bool Mesh::unload()
{
    //unload mesh
    if( vertexBuffer )
        glDeleteBuffers(1, &vertexBuffer );
    if( indexBuffer )
        glDeleteBuffers(1, &indexBuffer );
#ifdef ENABLE_VAOS
    if( vbaDraw )
        glDeleteVertexArrays( 1, &vbaDraw );
#endif
    //is loaded?
    loaded=false;
    //
    return true;
}
//draw
void Mesh::__bind()
{

    //bind VBO
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    //set vertex
    glVertexPointer(2, GL_FLOAT, sizeof(gVertex), 0 );
    glTexCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec2) );
    //bind IBO
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexs.size() ? indexBuffer : 0 );

}
void Mesh::draw()
{
    //bind mesh
#ifdef ENABLE_VAOS
    DEBUG_ASSERT(vbaDraw);
    glBindVertexArray( vbaDraw );
#else
    __bind();
#endif
    //get openGL draw mode
    uint glMode;
    switch( dmode )
    {
    case Mesh::TRIANGLE:
        glMode = GL_TRIANGLES;
        break;
    case Mesh::TRIANGLE_STRIP:
        glMode = GL_TRIANGLE_STRIP;
        break;
    case Mesh::LINE_STRIP:
        glMode = GL_LINE_STRIP;
        break;
    case Mesh::LINES:
        glMode = GL_LINES;
        break;
    }
    //draw mesh
    if( !mIndexs.size() )
        glDrawArrays( glMode, 0, mVertexs.size() );
    else
        glDrawElements( glMode, mIndexs.size(), GL_UNSIGNED_SHORT, 0 );

#ifdef ENABLE_VAOS
    glBindVertexArray( 0 );
#endif
}


/*BatchingMesh*/

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
    mVertexs.resize(maxVertexs);
}
void BatchingMesh::createBufferByTriangles(size_t maxTriangles)
{
    createBufferByVertexs(maxTriangles*3);
}


void BatchingMesh::relase()
{
    countVertexs=0;
}
bool BatchingMesh::canAdd(Mesh::ptr mesh)
{
    //ibo
    size_t nI=mesh->getCpuIndexs().size();
    size_t nV=mesh->getCpuVertexs().size();
    //invalid mesh vertexs count
    int nVfuture=(nI ? nI : nV);
    if(nVfuture <3) return false;
    //future size < max size?
    size_t futuresize=bitesSize();
    //olready superated!?
    if(futuresize>maxSize)
        return false;
    else if(mesh->getDrawMode()==Mesh::DrawMode::TRIANGLE)
        futuresize+=nVfuture *sizeof(gVertex);
    else if(mesh->getDrawMode()==Mesh::DrawMode::TRIANGLE_STRIP)
        futuresize+=((nVfuture-3) * 3 + 3)*sizeof(gVertex);
    //and now!?
    if(futuresize>maxSize)
        return false;
    //you can batching this mesh
    return true;
}

bool BatchingMesh::addMesh(const Mat4& modelView,Mesh::ptr mesh,int z)
{
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINES);
    DEBUG_ASSERT(mesh->getDrawMode()!=Mesh::DrawMode::LINE_STRIP);

#define AddVertexML(gvt)\
				mVertexs[countVertexs].vtz=Vec3(modelView.mul2D(gvt.vt),z);\
				mVertexs[countVertexs].uv=gvt.uv;\
				++countVertexs;

#define AddLastVertexML3(i)\
				mVertexs[countVertexs]=mVertexs[countVertexs+i];\
				++countVertexs;
    //ibo
    size_t nI=mesh->getCpuIndexs().size();
    size_t nV=mesh->getCpuVertexs().size();
    const auto& refIBO=mesh->getCpuIndexs();
    const auto& refVBO=mesh->getCpuVertexs();
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
            for (int i=1; i < nI-2; ++i)
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
            for (int i=1; i < nV-2; ++i)
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
                    sizeof(BatchingMesh::gVertex)*countVertexs, //size
                    &mVertexs[0] //data
                   );
    //set vertex
    glVertexPointer(3, GL_FLOAT, sizeof(gVertex), 0 );
    glTexCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec3) );
#else
    //disable VBO
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //set vertex
    static const size_t sizeofGV=sizeof(BatchingMesh::gVertex);
    glVertexPointer(3, GL_FLOAT, sizeofGV, &mVertexs[0].vtz.x );
    glTexCoordPointer(2, GL_FLOAT, sizeofGV, &mVertexs[0].uv.x );
#endif
    //draw
    glDrawArrays( GL_TRIANGLES, 0, countVertexs );
}