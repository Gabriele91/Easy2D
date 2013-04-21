#include <stdafx.h>
#include <FrameSet.h>
#include <Application.h>
#include <ResourcesGroup.h>
#include <Table.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//create a frame set
FrameSet::FrameSet(ResourcesGroup *rsgr,const String& path)
				  :Resource(rsgr,path){}

void FrameSet::addFrame(const Vec4& frame){
		//create a mesh
		Mesh::ptr sprite(new Mesh());

		//upmapping
		Vec2 start(frame.x/texture->getRealWidth(),
				   frame.y/texture->getRealHeight());
		Vec2 end(frame.z/texture->getRealWidth(),
				 frame.w/texture->getRealHeight());

		//add vertexs
		sprite->addVertex(  0.5,
						   -0.5,
							end.x,
							end.y);
		sprite->addVertex(  0.5,
							0.5,
							end.x,
							start.y);
		sprite->addVertex( -0.5,
						   -0.5,
						    start.x,
							end.y);
		sprite->addVertex( -0.5,
							0.5,
							start.x,
							start.y);
		//end add vertexs
		//set draw mode
		sprite->setDrawMode(Mesh::TRIANGLE_STRIP);
		//build mesh
		sprite->build();
		//add frame
		frames.push_back(sprite);
}
//resource
bool FrameSet::load(){

	//load file
	void *data=NULL; uint len=0;
	Application::instance()->loadData(rpath,data,len);
	String filestring((char*)data);
	free(data);
	//deserialize frameset
	Table tbFrameSet;
	tbFrameSet.deserialize(filestring);

	//get texture
	if(tbFrameSet.existsAsType("texture",Table::STRING)){
		texture=getResourcesGroup()->get<Texture>(tbFrameSet.getString("texture"));
		if(texture==NULL){			
			DEBUG_ASSERT_MSG(0,"frameset error:"
							   "must to be setted a valid texture name"
							   "(parameter:texture)");
		}
	}
	else{
		DEBUG_ASSERT_MSG(0,"frameset error:"
				           "must to be setted a texture name"
						   "(parameter:texture)");
		return false;
	}
	//load texture
	if(!texture->isLoad()) texture->load();
	//get frames
	if(tbFrameSet.existsAsType("frames",Table::TABLE)){
		const Table& tbVertexes=tbFrameSet.getTable("frames");
		for(auto param:tbVertexes){
			if(param.second->asType(Table::VECTOR4D)){
				addFrame(param.second->get<Vec4>());
			}
			else{
				DEBUG_ASSERT_MSG(0,"frameset error:"
								   "frame must be only Vec4 (x x x x) "
								   "(parameter:frames)");
			}
		}
	}
	else{
		DEBUG_ASSERT_MSG(0,"mesh error:"
				           "must to be setted a frameset table"
						   "(parameter:frames)");
		return false;
	}
	loaded=true;
	return true;
}
bool FrameSet::unload(){
	//realse frame:
	frames.clear();
	//delete texture
	texture=NULL;
	//
	loaded=false;
	return true;
}
Mesh::ptr FrameSet::getFrame(uint i){
	DEBUG_ASSERT_MSG(i<frames.size(),"error : frame["<<i<<"] not exist : "<< rpath);
	return frames[i];
}