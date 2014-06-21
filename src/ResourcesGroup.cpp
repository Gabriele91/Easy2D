#include <stdafx.h>
#include <ResourcesGroup.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;
///////////////////////

ResourcesGroup::ResourcesGroup(const String& rsname,
                               const Utility::Path& path,
                               const String& version)
    :resources(NULL,path)
    ,version(version)
{
    loadResourceFile();
    subscription(rsname);
}

ResourcesGroup::ResourcesGroup():version("default")
{
}
void ResourcesGroup::subscription(const String& rgname)
{
    //debug
    DEBUG_ASSERT(rgname.size());
    DEBUG_ASSERT(name.size()==0);
    //save name
    name=rgname;
    //regist this resource group
    Application::instance()->subscriptionResourcesGroup (name,this);
}
void ResourcesGroup::unsubscription()
{
    //debug
    DEBUG_ASSERT(name.size());
    //unregist this resource group
    Application::instance()->unsubscriptionResourcesGroup(name);
}
void ResourcesGroup::loadResourceFile()
{
    //load table
    resources.load();
    //recall constructors
    loadAResource(resources.getPath(),"tables",tables);
    loadAResource(resources.getPath(),"textures",textures);
    loadAResource(resources.getPath(),"meshes",meshes);
    loadAResource(resources.getPath(),"frameSets",frameSets);
    loadAResource(resources.getPath(),"fonts",fonts);
    loadAResource(resources.getPath(),"sounds",sounds);

}

void ResourcesGroup::addResourceFiles(const String& rsname,
                                      const Utility::Path& path,
                                      const String& argversion)
{

    DEBUG_ASSERT_MSG(!resources.isLoad(), "ResourcesGroup addResourceFiles: resource file olready added");
    version=argversion;
    resources=Table(NULL,path);
    loadResourceFile();
    subscription(rsname);
}

ResourcesGroup::~ResourcesGroup()
{
    //unregist this resource group
    unsubscription();
}

/** reload only gpu resource */
void ResourcesGroup::reloadGpuResouce()
{
    textures.__forceReload();
    meshes.__forceReload();
    frameSets.__forceReload();
    fonts.__forceReload();
}

// GCC SUCK //
#ifdef COMPILER_GCC
template<>
ResourcesManager<Texture>& ResourcesGroup::getManager<Texture>()
{
    return textures;
}
template<>
ResourcesManager<Table>& ResourcesGroup::getManager<Table>()
{
    return tables;
}
template<>
ResourcesManager<Mesh>& ResourcesGroup::getManager<Mesh>()
{
    return meshes;
}
template<>
ResourcesManager<FrameSet>& ResourcesGroup::getManager<FrameSet>()
{
    return frameSets;
}
template<>
ResourcesManager<Font>& ResourcesGroup::getManager<Font>()
{
    return fonts;
}
template<>
ResourcesManager<Sound>& ResourcesGroup::getManager<Sound>()
{
    return sounds;
}
#endif