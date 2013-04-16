#include <stdafx.h>
#include <Mesh.h>
#include <Table.h>
#include <Application.h>
using namespace Easy2D;

//VAOs are slower (by nvidia and valve)
#undef ENABLE_VAOS
//

Mesh::Mesh(ResourcesGroup *rsmr,
		   const String& pathfile)
		  :Resource(rsmr,pathfile)
		  ,vertexBuffer(0)
		  ,indexBuffer(0)
		  ,vbaDraw(0)
		  ,dmode(TRIANGLE)
{}
//distruttore
Mesh::~Mesh(){
}
//metodo che aggiunge i vertici
void Mesh::addVertex(const gVertex& gv){
	//
	if(mVertexs.size()){
		//box value
		//found box                          m______
		min.x=Math::min(min.x,gv.vt.x);  //  |\    |
		min.y=Math::min(min.y,gv.vt.y);  //  | \.c |
		max.x=Math::max(max.x,gv.vt.x);  //  |   \ |
		max.y=Math::max(max.y,gv.vt.y);  //  |____\|M
		//
	}
	else{
		min=max=gv.vt;
	}
	//add vertex
	mVertexs.push_back(gv);
}
void Mesh::addIndex(ushort mIndex){		
	mIndexs.push_back(mIndex);
}
void Mesh::addTriangleIndexs(ushort v1,ushort v2,ushort v3){		
	mIndexs.push_back(v1);	
	mIndexs.push_back(v2);
	mIndexs.push_back(v3);
}
void Mesh::addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4){
	mIndexs.push_back(v1);	
	mIndexs.push_back(v2);
	mIndexs.push_back(v3);
		
	mIndexs.push_back(v2);
	mIndexs.push_back(v3);
	mIndexs.push_back(v4);
}
//
void Mesh::setDrawMode(DrawMode dmode){
	this->dmode=dmode;
}
//
void Mesh::build(){
	
	//create the VBO
    if( !vertexBuffer )
       glGenBuffers(1, &vertexBuffer );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertex) * mVertexs.size(), &mVertexs[0], GL_STATIC_DRAW);
	//create the IBO
	if(mIndexs.size()){
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

    //aabb culling
	extends.x=Math::max(fabs(max.x),fabs(min.y));
	extends.y=Math::max(fabs(max.y),fabs(min.x));
	center=(max+min)/2;
	extends-=center;

}
//resource
bool Mesh::load(){
	
	//load file
	void *data=NULL; uint len=0;
	Application::instance()->loadData(rpath,data,len);
	String filestring((char*)data);
	free(data);
	//deserialize mesh
	Table tbMesh;
	tbMesh.deserialize(filestring);
	//get data info
	//get mode
	if(tbMesh.exists("mode")){
		//get string
		String strmode=tbMesh.getString("mode");
		strmode.toUpper();
		//select mode
		if(strmode=="TRIANGLE") dmode=TRIANGLE;
		else if (strmode=="TRIANGLE_STRIP") dmode=TRIANGLE_STRIP;
		else if (strmode=="LINE_STRIP") dmode=LINE_STRIP;
		else if (strmode=="LINES") dmode=LINES;
		else{
			DEBUG_ASSERT_MSG(0,"mesh error:"
				               "to be set a correct kind of primitives to render "
							   "(parameter:mode)");
		}
	}

	//get vertices
	if(tbMesh.existsAsType("vertexes",Table::TABLE)){
		const Table& tbVertexes=tbMesh.getTable("vertexes");
		for(auto param:tbVertexes){
			if(param.second->asType(Table::VECTOR4D))
				addVertex(param.second->get<Vec4>());
			else{
				DEBUG_ASSERT_MSG(0,"mesh error:"
								   "vertexes must be only Vec4 (x x x x) "
								   "(parameter:vertexes)");
			}
		}
	}
	else{
		DEBUG_ASSERT_MSG(0,"mesh error:"
				           "must to be setted a vertexes table"
						   "(parameter:vertexes)");
		return false;
	}

	//get indexs	
	if(tbMesh.existsAsType("indexs",Table::TABLE)){
		const Table& tbIndexs=tbMesh.getTable("indexs");
		for(auto param:tbIndexs){
			if(param.second->asType(Table::FLOAT))
				addIndex(param.second->get<float>());
			else{
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
bool Mesh::unload(){
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
void Mesh::__bind(){
	
	//bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	//set vertex
	glVertexPointer(2, GL_FLOAT, sizeof(gVertex), 0 );
	glTexCoordPointer(2, GL_FLOAT, sizeof(gVertex), (void*)sizeof(Vec2) );
	//bind IBO
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexs.size() ? indexBuffer : 0 ); 

}
void Mesh::draw(){
	//bind mesh
#ifdef ENABLE_VAOS
	DEBUG_ASSERT(vbaDraw);
	glBindVertexArray( vbaDraw );
#else
	__bind();
#endif
	//get openGL draw mode
	uint glMode;
	switch( dmode ){
			case Mesh::TRIANGLE:       glMode = GL_TRIANGLES;        break;
			case Mesh::TRIANGLE_STRIP: glMode = GL_TRIANGLE_STRIP;   break;
			case Mesh::LINE_STRIP:     glMode = GL_LINE_STRIP;       break;
			case Mesh::LINES:          glMode = GL_LINES;            break;
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