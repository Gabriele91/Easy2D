#include <stdafx.h>
#include <ResourcesGroup.h>
#include <Application.h>
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
	//recall constructors
	loadAResource(path,"tables",tables);
	loadAResource(path,"textures",textures);
	loadAResource(path,"meshes",meshes);
	loadAResource(path,"frameSets",frameSets);
	//regist this resource group
	Application::instance()->addResourcesGroup(this);
}

ResourcesGroup::~ResourcesGroup(){	
	//unregist this resource group
	Application::instance()->eraseResourcesGroup(this);
}

/** reload only gpu resource */
void ResourcesGroup::reloadGpuResouce(){	
	textures.__forceReload();
	meshes.__forceReload();
	frameSets.__forceReload();
}

// GCC SUCK //
#ifdef COMPILER_GCC

/*
* Texture manager:
*/
template <>
/** load texture */
DFORCEINLINE Texture::ptr ResourcesGroup::load<Texture>(const String& path){
    return textures.load(path);
}
template <>
/** get texture (but not load in memory) */
DFORCEINLINE Texture::ptr ResourcesGroup::get<Texture>(const String& path){
    return textures.get(path);
}
template<>
/** find a texture already returned */
DFORCEINLINE Texture::ptr ResourcesGroup::find<Texture>(const String& path){
    return textures.find(path);
}
/** directory texture resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Texture>(){
    return textures.mapResources->getPath().getDirectory();
}
/*
* Table manager:
*/
template<>
/** load table */
DFORCEINLINE Table::ptr ResourcesGroup::load<Table>(const String& path){
    return tables.load(path);
}
template<>
/** get table  (but not load in memory) */
DFORCEINLINE Table::ptr ResourcesGroup::get<Table>(const String& path){
    return tables.get(path);
}
template<>
/** find a table already returned */
DFORCEINLINE Table::ptr ResourcesGroup::find<Table>(const String& path){
    return tables.find(path);
}
/** directory table resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Table>(){
    return tables.mapResources->getPath().getDirectory();
}

/*
* Mesh manager:
*/
template<>
/** load mesh */
DFORCEINLINE Mesh::ptr ResourcesGroup::load<Mesh>(const String& path){
    return meshes.load(path);
}
template<>
/** get mesh  (but not load in memory) */
DFORCEINLINE Mesh::ptr ResourcesGroup::get<Mesh>(const String& path){
    return meshes.get(path);
}
template<>
/** find a mesh already returned */
DFORCEINLINE Mesh::ptr ResourcesGroup::find<Mesh>(const String& path){
    return meshes.find(path);
}
/** directory mesh resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Mesh>(){
    return meshes.mapResources->getPath().getDirectory();
}

/*
* FrameSet manager:
*/
template<>
/** load FrameSet */
DFORCEINLINE FrameSet::ptr ResourcesGroup::load<FrameSet>(const String& path){
	return frameSets.load(path);
}
template<>
/** get FrameSet  (but not load in memory) */
DFORCEINLINE FrameSet::ptr ResourcesGroup::get<FrameSet>(const String& path){
	return frameSets.get(path);
}
template<>
/** find a FrameSet already returned */
DFORCEINLINE FrameSet::ptr ResourcesGroup::find<FrameSet>(const String& path){
	return frameSets.find(path);
}
/** directory FrameSet resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<FrameSet>(){
	return meshes.mapResources->getPath().getDirectory();
}
#endif