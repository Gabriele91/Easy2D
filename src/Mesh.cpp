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
           ,batching(true)
{
    //is reloadable?
    if(!rsmr||pathfile=="") reloadable=false;
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
           ,batching(true)
{
    //is reloadable?
    if(!rsmr||pathfile=="") reloadable=false;
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
        vertexBuffer=RenderContext::createBuffer();
    RenderContext::bufferData(vertexBuffer, STATIC, &cpuVertexs()[0], sizeof(gVertex) * cpuVertexs().size());
    //create the IBO
    if(cpuIndexs().size())
    {
        if( !indexBuffer )
            indexBuffer=RenderContext::createBuffer();
        RenderContext::bufferData(indexBuffer, STATIC, &cpuIndexs()[0], sizeof(ushort) * cpuIndexs().size());
    }
    //get vao/ibo/vbo errors
    CHECK_GPU_ERRORS();
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
        RenderContext::deleteBuffer(vertexBuffer);
    if( indexBuffer )
        RenderContext::deleteBuffer(indexBuffer);
    //reset box
    box=AABox2();
    //is loaded?
    loaded=false;
    //
    return true;
}
//draw
void Mesh::__bind()
{
    //clients
    RenderContext::setClientState(true, false, true, false);
    
    //vbo
    if(vertexBuffer)
    {
        //bind VBO
        RenderContext::bindVertexBuffer(vertexBuffer);
        //set vertex
        RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gVertex), 0 );
        RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec2) );
    }
    //vba
    else
    {
        //unbind VBO
        RenderContext::unbindVertexBuffer();
        //set vertex
        RenderContext::vertexPointer(2, GL_FLOAT, sizeof(gVertex), &cpuVertexs()[0] );
        RenderContext::texCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)(&cpuVertexs()[0].uv) );
    }
    
    //IBO
    if(indexBuffer)
        RenderContext::bindIndexBuffer(indexBuffer);
    //DRAW ARRAY
    else
        RenderContext::unbindIndexBuffer();
}
void Mesh::draw()
{
    //bind mesh
    __bind();
    //draw mesh
    if( !cpuIndexs().size() )
    {
        RenderContext::drawPrimitive(dmode, 0, cpuVertexs().size());
    }
    //IBO
    else if(indexBuffer)
    {
        RenderContext::drawPrimitiveIndexed(dmode, cpuIndexs().size(), UNSIGNED_SHORT, 0 );
    }
    //IBA
    else
    {
        RenderContext::drawPrimitiveIndexed(dmode, cpuIndexs().size(), UNSIGNED_SHORT, &cpuIndexs()[0] );
    }
}
