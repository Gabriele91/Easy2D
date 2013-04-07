#include <stdafx.h>
#include <ResourcesGroup.h>
///////////////////////
using namespace Easy2D;
///////////////////////

ResourcesGroup::ResourcesGroup(const Utility::Path& path,
							  const String& version)
	:resources(NULL,path)
	,version(version)
{
	//load table
	resources.load();
	//temp var
	String globalPath;
	//recall constructors
	//tables
	if(resources.existsAsType("tables",Table::STRING)){
		globalPath=path.getDirectory()+"/"+resources.getString("tables");
		new (&tables) ResourcesManager<Table>(globalPath,version);
	}
	else {
		DEBUG_MESSAGE("error: "<<path <<" not setted tables");
	}
	//textures
	if(resources.existsAsType("textures",Table::STRING)){
		globalPath=path.getDirectory()+"/"+resources.getString("textures");
		new (&textures) ResourcesManager<Texture>(globalPath,version);
	}
	else {
		DEBUG_MESSAGE("error: "<<path <<" not setted textures");
	}
	//meshes
	if(resources.existsAsType("meshes",Table::STRING)){
		globalPath=path.getDirectory()+"/"+resources.getString("meshes");
		new (&meshes) ResourcesManager<Mesh>(globalPath,version);
	}
	else {
		DEBUG_MESSAGE("error: "<<path <<" not setted meshes");
	}
}