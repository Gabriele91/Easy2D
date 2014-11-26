#include <stdafx.h>
#include <Mesh.h>
#include <Table.h>
#include <Debug.h>
#include <Application.h>

using namespace Easy2D;

#define VBO_N_PAGE  sizeof(float)*4
#define IBO_N_PAGE  4
//#define A_SMALL_MESH  8000
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//mesh size attribute
size_t GenericMesh::attSize(uchar type)
{
    switch (type) {
        case VertexField::UV:
        case VertexField::POSITION2D:
            return sizeof(float)*2;
            break;
        case VertexField::NORMAL:
        case VertexField::POSITION3D:
            return sizeof(float)*3;
            break;
        case VertexField::COLOR:
            return sizeof(float)*4;
            break;
        default:
            return 0;
            break;
    }
}
//mesh calc size
void GenericMesh::calcVertexSize(uchar type){
    vSize=0;
    vSize+=attSize(type & VertexField::POSITION2D);
    vSize+=attSize(type & VertexField::POSITION3D);
    vSize+=attSize(type & VertexField::NORMAL);
    vSize+=attSize(type & VertexField::UV);
    vSize+=attSize(type & VertexField::COLOR);
}
//mesh field
void GenericMesh::addFild(const float* b, size_t size)
{
    addVertexCPage(sizeof(float)*size);
    Math::memcpy(&vertexs[currentVertex], (const byte*)b, sizeof(float)*size);
    currentVertex += sizeof(float)*size;
}
//size vertexs
size_t GenericMesh::sizeVertexs() const
{
    return vSize ? (Math::multipleOfX(currentVertex, vSize) / vSize) : 0;
}
size_t GenericMesh::sizeIndexs() const
{
    return currentIndex;
}
void GenericMesh::addVertexCPage(size_t next)
{
    while ( (currentVertex+next) >=  vertexs.size() )
    {
        vertexs.resize(vertexs.size() + VBO_N_PAGE*vSize);
    }
}
void GenericMesh::addIndexCPage()
{
    if ( currentIndex == indexs.size() )
    {
        indexs.resize(indexs.size() + IBO_N_PAGE);
    }
}
//begin create mash
void GenericMesh::format(uchar argtype, size_t vsize, size_t isize)
{
    clear();
    calcVertexSize(argtype);
    type = (VertexField)argtype;
    vertexs = std::vector<byte> (vSize*(vsize != 0 ? vsize : VBO_N_PAGE));
    indexs = std::vector<uint> (isize != 0 ? isize : IBO_N_PAGE);
}

//like opengl 1.4
void GenericMesh::vertex(const Vec2& vt)
{
    //mBox.addPoint(Vec3(vt, 0.0));
    //centroid += Vec3(vt, 0.0);
    addFild(vt,2);
}
void GenericMesh::vertex(const Vec3& vt)
{
    //mBox.addPoint(vt);
    //centroid += vt;
    addFild(vt,3);
}
void GenericMesh::normal(const Vec3& normal)
{
    addFild(normal,3);
}
void GenericMesh::color(const Vec4& color)
{
    addFild(color,4);
}
void GenericMesh::color(const Color& ucolor)
{
    color(ucolor.toNormalize());
}
void GenericMesh::uv(const Vec2& uv)
{
    addFild(uv,2);
}
//like OpenGL 2.X, 3.X, 4.x
void GenericMesh::vbuffer(const Easy2D::byte* b)
{
    Math::memcpy(&vertexs[0], b, vertexs.size());
    currentVertex = vertexs.size();
}
void GenericMesh::ibuffer(const Easy2D::uint* b)
{
    Math::memcpy((Easy2D::byte*)&indexs[0], (Easy2D::byte*)b, indexs.size()*sizeof(uint));
    currentIndex = indexs.size();
}
//clear mesh
void GenericMesh::clear()
{
    //cpu clear
    cpuClear();
    //clear gpu vars:
    sBVertex = 0;
    sBIndex = 0;
    //gpu clear
    if (bVertex)
    {
        RenderContext::deleteBuffer(bVertex);
        bVertex = 0;
    }
    if (bIndex)
    {
        RenderContext::deleteBuffer(bIndex);
        bIndex = 0;
    }
}
void GenericMesh::cpuClear()
{
    vertexs = std::vector<byte>(0);
    indexs = std::vector<uint>(0);
    currentVertex = 0;
    currentIndex = 0;
}
//at mesh delete
GenericMesh::~GenericMesh()
{
    clear();
}
//get cpu info
uint  GenericMesh::getIndex(size_t i) const
{
    return indexs[i];
}
Easy2D::byte* GenericMesh::getVertex(size_t i, size_t offset)
{
    return &vertexs[i*vSize + offset];
}
const Easy2D::byte* const GenericMesh::getVertex(size_t i, size_t offset) const
{
    return &vertexs[i*vSize + offset];
}
Vec2& GenericMesh::getVertex2(size_t i, size_t offset)
{
    return *((Vec2*)getVertex(i,offset));
}
Vec3& GenericMesh::getVertex3(size_t i, size_t offset)
{
    return *((Vec3*)getVertex(i, offset));
}
Vec4& GenericMesh::getVertex4(size_t i, size_t offset)
{
    return *((Vec4*)getVertex(i, offset));
}
//add a index
void GenericMesh::index(uint i)
{
    addIndexCPage();
    indexs[currentIndex] = i;
    ++currentIndex;
}
//bind
bool GenericMesh::build(bool force)
{
    DEBUG_ASSERT(bVertex==0);
    //sizes
    sBVertex = (uint)sizeVertexs();
    sBIndex = (uint)sizeIndexs();
    //calc center
    //centroid /= (float)sBVertex;
    //vertex buffer
    bVertex = RenderContext::createBuffer();
    RenderContext::bufferData(bVertex, STATIC, &vertexs[0], vSize*sBVertex);
    //index buffer
    if(sBIndex)
    {
        bIndex = RenderContext::createBuffer();
        RenderContext::bufferData(bIndex, STATIC, &indexs[0], sizeof(uint)*sBIndex);
    }
    //delete cpu info
    if (force)
    {
        vertexs = std::vector<byte>(0);
        indexs = std::vector<uint>(0);
    }
    //get vao/ibo/vbo errors
    CHECK_GPU_ERRORS();
    //return 
    return bVertex != 0;
}
void GenericMesh::mode(DrawMode m)
{
    dMode = m;
}
void GenericMesh::set() const
{
    if (bVertex)
        RenderContext::bindVertexBuffer(bVertex);
    else
        RenderContext::unbindVertexBuffer();
    
    if (bIndex)
        RenderContext::bindIndexBuffer(bIndex);
    else
        RenderContext::unbindIndexBuffer();
}
void GenericMesh::draw() const
{
    #define isIndexed (bIndex||sizeIndexs())
    //VBO NO INDEXED
    if( bVertex && !isIndexed )
    {
        RenderContext::drawPrimitive(dMode, 0, sBVertex);
    }
    //VBA NO INDEXED
    else if( !isIndexed )
    {
        RenderContext::drawPrimitive(dMode, 0, sizeVertexs());
    }
    //IBO
    else if(bIndex)
    {
        RenderContext::drawPrimitiveIndexed(dMode, sBIndex, UNSIGNED_INT, 0 );
    }
    //IBA
    else
    {
        RenderContext::drawPrimitiveIndexed(dMode, sizeIndexs(), UNSIGNED_INT, (void*)&indexs[0]);
    }
    #undef isIndexed
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

//VAOs are slower (by nvidia and valve)
//#undef ENABLE_VAOS
///////////////////////////////////////////////////////////////
//Vertex Size
static const size_t sizeGVector=sizeof(float)*4;
///////////////////////////////////////////////////////////////

Mesh::Mesh(ResourcesManager<Mesh> *rsmr,
           const String& pathfile)
           :Resource(rsmr,pathfile)
           ,batching(true)
{
    //is reloadable?
    if(!rsmr||pathfile=="") reloadable=false;
    //set draw mode
    setDrawMode(TRIANGLE);
    //set format
    format(POSITION2D|UV);
}
//distruttore
Mesh::~Mesh()
{
    //release resource
    release();
}
//metodo che aggiunge i vertici
void Mesh::addVertex(const Vec2& pos,const Vec2& uvmap)
{
    //update box
    box.addPoint(pos);
    //add into mesh
    vertex(pos);
    uv(uvmap);
}
void Mesh::addVertex(float x,float y, float u,float v)
{
    addVertex(Vec2(x,y),Vec2(u,v));
}
void Mesh::addVertex(const Vec4& posuv)
{
    addVertex(posuv.xy(),Vec2(posuv.z,posuv.w));
}
void Mesh::addIndex(ushort mIndex)
{
    index(mIndex);
}
void Mesh::addTriangleIndexs(ushort v1,ushort v2,ushort v3)
{
    index(v1);
    index(v2);
    index(v3);
}
void Mesh::addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4)
{
    index(v1);
    index(v2);
    index(v3);

    index(v2);
    index(v3);
    index(v4);
}
void Mesh::setDrawMode(DrawMode dmode)
{
    dMode=dmode;
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
        if(strmode=="TRIANGLE") dMode=TRIANGLE;
        else if (strmode=="TRIANGLE_STRIP") dMode=TRIANGLE_STRIP;
        else if (strmode=="LINE_STRIP") dMode=LINE_STRIP;
        else if (strmode=="LINES") dMode=LINES;
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
    clear();
    //reset box
    box=AABox2();
    //is loaded?
    loaded=false;
    //
    return true;
}
//
void Mesh::draw() const
{
    //clients
    RenderContext::setClientState(true, false, true, false);
    //vbo
    if(bVertex)
    {
        //bind VBO
        RenderContext::bindVertexBuffer(bVertex);
        //set vertex
        RenderContext::vertexPointer(2, GL_FLOAT, sizeGVector , 0 );
        RenderContext::texCoordPointer(2, GL_FLOAT, sizeGVector, (void*)(sizeof(float)*2) );
    }
    //vba
    else
    {
        //unbind VBO
        RenderContext::unbindVertexBuffer();
        //set vertex
        RenderContext::vertexPointer(2, GL_FLOAT, sizeGVector,  (void*)getVertex(0,0) );
        RenderContext::texCoordPointer(2, GL_FLOAT, sizeGVector, (void*)getVertex(0,sizeof(float)*2) );
    }
    //IBO
    if(bIndex)
        RenderContext::bindIndexBuffer(bIndex);
    else
        RenderContext::unbindIndexBuffer();
    //static draw
    GenericMesh::draw();
}
