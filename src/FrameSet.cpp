
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
FrameSet::~FrameSet(){
	//release resource
	release();
}
void FrameSet::addFrame(const Vec4& frame){
		//create a mesh
		Mesh::ptr sprite(new Mesh());
		//save size
		sizeFrames.push_back(Vec2(frame.z,frame.w));
		//upmapping
		Vec2 start(frame.x/texture->getRealWidth(),
				   frame.y/texture->getRealHeight());
		Vec2 end(frame.z/texture->getRealWidth()+start.x,
				 frame.w/texture->getRealHeight()+start.y);
		//set size mesh
		float hlSizeX=frame.z*0.5;
		float hlSizeY=frame.w*0.5;
		//add vertexs
		sprite->addVertex(  hlSizeX,
						   -hlSizeY,
							end.x,
							end.y);

		sprite->addVertex(  hlSizeX,
							hlSizeY,
							end.x,
							start.y);

		sprite->addVertex( -hlSizeX,
						   -hlSizeY,
						    start.x,
							end.y);

		sprite->addVertex( -hlSizeX,
							hlSizeY,
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
	void *data=NULL; size_t len=0;
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
	
	if(tbFrameSet.existsAsType("time",Table::FLOAT)){
		defaultTime=tbFrameSet.getFloat("time",0.0);
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
