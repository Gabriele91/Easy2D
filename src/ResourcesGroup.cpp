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
    ,isSubscribed(false)
{
    loadResourceFile();
    subscription(rsname);
}

ResourcesGroup::ResourcesGroup():version("default"),isSubscribed(false)

{
}
void ResourcesGroup::subscription(const String& rgname)
{
    //debug
    DEBUG_ASSERT(isSubscribed==false);
    DEBUG_ASSERT(rgname.size());
    DEBUG_ASSERT(name.size()==0);
    //save name
    name=rgname;
    //regist this resource group
    Application::instance()->subscription_resources_group (name,this);
    //is subscribed
    isSubscribed=true;
}
void ResourcesGroup::unsubscription()
{
    //debug
    DEBUG_ASSERT(name.size());
    //unregist this resource group
    Application::instance()->unsubscription_resources_group(name);
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
    loadAResource(resources.getPath(),"scripts",scripts);
    loadAResource(resources.getPath(),"shaders",shaders);

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
    if(isSubscribed)
        unsubscription();
}

/** reload only gpu resource */
void ResourcesGroup::reloadGpuResouce()
{
    textures.__forceReload();
    meshes.__forceReload();
    frameSets.__forceReload();
    fonts.__forceReload();
    shaders.__forceReload();
}
