#include <stdafx.h>
#include <Mesh.h>
#include <Table.h>
#include <Debug.h>
#include <Application.h>

using namespace Easy2D;

//VAOs are slower (by nvidia and valve)
//#undef ENABLE_VAOS
//

Mesh::Mesh(DFUNCTION<void(void)> fun,
           ResourcesManager<Mesh> *rsmr,
           const String& pathfile)
           :BasicMesh(fun)
           ,Resource(rsmr,pathfile)
           ,vertexBuffer(0)
           ,indexBuffer(0)
           ,dmode(TRIANGLE)
{
    //is reloadable?
    if(!rsmr||pathfile=="") reloadable=false;

#ifdef ENABLE_VAOS
    vbaDraw=(0);
#endif
}

Mesh::Mesh(ResourcesManager<Mesh> *rsmr,
           const String& pathfile)
           :BasicMesh([this](){
                //create cpu buffer
                cpuBuffers=CpuBuffers::ptr(new MCBuffers());
            })
           ,Resource(rsmr,pathfile)
           ,vertexBuffer(0)
           ,indexBuffer(0)
           ,dmode(TRIANGLE)
{
    //is reloadable?
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
    cpuVertexs().push_back(gv);
}
void Mesh::addIndex(ushort mIndex)
{
    cpuIndexs().push_back(mIndex);
}
void Mesh::addTriangleIndexs(ushort v1,ushort v2,ushort v3)
{
    cpuIndexs().push_back(v1);
    cpuIndexs().push_back(v2);
    cpuIndexs().push_back(v3);
}
void Mesh::addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4)
{
    cpuIndexs().push_back(v1);
    cpuIndexs().push_back(v2);
    cpuIndexs().push_back(v3);

    cpuIndexs().push_back(v2);
    cpuIndexs().push_back(v3);
    cpuIndexs().push_back(v4);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertex) * cpuVertexs().size(), &cpuVertexs()[0], GL_STATIC_DRAW);
    //create the IBO
    if(cpuIndexs().size())
    {
        if( !indexBuffer )
            glGenBuffers(1, &indexBuffer );
        glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ushort) * cpuIndexs().size(), &cpuIndexs()[0], GL_STATIC_DRAW);
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
                                 "vertexes must be only Vec4 (x y u v) "
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
                addIndex((ushort)param.second->get<float>());
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
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, cpuIndexs().size() ? indexBuffer : 0 );

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
    if( !cpuIndexs().size() )
        glDrawArrays( glMode, 0, cpuVertexs().size() );
    else
        glDrawElements( glMode, cpuIndexs().size(), GL_UNSIGNED_SHORT, 0 );

#ifdef ENABLE_VAOS
    glBindVertexArray( 0 );
#endif
}
