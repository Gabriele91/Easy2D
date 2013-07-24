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
	loadAResource(path,"fonts",fonts);
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
	fonts.__forceReload();
}

// GCC SUCK //
#ifdef COMPILER_GCC
template<> 
ResourcesManager<Texture>& ResourcesGroup::getManager<Texture>(){ return textures; }
template<> 
ResourcesManager<Table>& ResourcesGroup::getManager<Table>(){ return tables; }
template<> 
ResourcesManager<Mesh>& ResourcesGroup::getManager<Mesh>(){ return meshes; }
template<> 
ResourcesManager<FrameSet>& ResourcesGroup::getManager<FrameSet>(){ return frameSets; }
template<> 
ResourcesManager<Font>& ResourcesGroup::getManager<Font>(){ return fonts; }
#endif