#ifndef RESOURCESGROUP_H
#define RESOURCESGROUP_H

#include <Config.h>
#include <Utility.h>
#include <EString.h>
#include <ResourcesManager.h>
//resources object
#include <Table.h>
#include <Texture.h>
#include <Mesh.h>
#include <FrameSet.h>
#include <Font.h>
#include <Sound.h>
#include <Script.h>
#include <Shader.h>

namespace Easy2D
{

class ResourcesGroup
{

    ResourcesManager<Texture> textures;
    ResourcesManager<Table> tables;
    ResourcesManager<Mesh> meshes;
    ResourcesManager<FrameSet> frameSets;
    ResourcesManager<Font> fonts;
    ResourcesManager<Sound> sounds;
    ResourcesManager<Script> scripts;
    ResourcesManager<Shader> shaders;

public:

    template<typename T>
    ResourcesManager<T>& getManager()
    {
        return NULL;
    }
    
private:
    //is subscribe
    bool isSubscribed;
    //teble resources
    Table resources;
    //key version
    Table::KeyTable version;
    //load a resource
    template<class RM>
    void loadAResource(const Utility::Path& path, const String& name,RM& object)
    {
        //tables
        if(resources.existsAsType(name,Table::STRING))
        {
            String globalPath=path.getDirectory()+"/"+resources.getString(name);
            new (&object) RM(this,globalPath,version);
        }
        else if(resources.existsAsType(name,Table::TABLE))
        {
            new (&object) RM(this,(Table*)(&resources.getTable(name)),false,version);
        }
        else
        {
            DEBUG_MESSAGE("error: "<<path <<" not setted tables");
        }
    }
    //
    void loadResourceFile();
    //resource group in map
    String name;
    void subscription(const String& name);
    void unsubscription();

public:
    /**
    * Set the managers resources
    * @param path table resources
     */
	ResourcesGroup(const String& name,
                   const String& path,
                   const String& version = "default")
                   :ResourcesGroup(name, Utility::Path(path),version)
	{
	}

    ResourcesGroup(const String& name,
                   const Utility::Path& path,
                   const String& version="default");
    ResourcesGroup();
    /**
     * Set the managers resources
     * @param path table resources
     */
    void addResourceFiles(const String& name,
                          const Utility::Path& path,
                          const String& version="default");
    /**
    * delete resource goup
    */
    virtual ~ResourcesGroup();

    /*
    * void template
    */
    template<typename T>  SPtr<T> load(const String& path)
    {
        return  getManager<T>().load(path);
    }
    template<typename T>  SPtr<T> get(const String& path)
    {
        return getManager<T>().get(path);
    }
    template<typename T>  SPtr<T> find(const String& path)
    {
        return getManager<T>().find(path);
    }
    template<typename T>  String getResourceDirectory()
    {
        return getManager<T>().mapResources->getPath().getDirectory();
    }


    /** load all resources returned  */
    DFORCEINLINE void load()
    {
        textures.load();
        tables.load();
        meshes.load();
        frameSets.load();
        fonts.load();
        sounds.load();
        scripts.load();
        shaders.load();
    }
    /** unload all resources returned  */
    DFORCEINLINE void unload(bool destroy)
    {
        textures.unload(destroy);
        tables.unload(destroy);
        meshes.unload(destroy);
        frameSets.unload(destroy);
        fonts.unload(destroy);
        sounds.unload(destroy);
        scripts.unload(destroy);
        shaders.unload(destroy);
    }
    /** directory of resources */
    String getResourcesDirectory()
    {
        return resources.getPath().getDirectory();
    }
    /** reload only gpu resource */
    void reloadGpuResouce();

    /** return resources trable  */
    const Table& getTable() const
    {
        return  resources;       
    }
    Table& getTable()
    {
        return  resources;       
    }

};

template<>
inline ResourcesManager<Texture>& ResourcesGroup::getManager<Texture>()
{
    return textures;
}
template<>
inline ResourcesManager<Table>& ResourcesGroup::getManager<Table>()
{
    return tables;
}
template<>
inline ResourcesManager<Mesh>& ResourcesGroup::getManager<Mesh>()
{
    return meshes;
}
template<>
inline ResourcesManager<FrameSet>& ResourcesGroup::getManager<FrameSet>()
{
    return frameSets;
}
template<>
inline ResourcesManager<Font>& ResourcesGroup::getManager<Font>()
{
    return fonts;
}
template<>
inline ResourcesManager<Sound>& ResourcesGroup::getManager<Sound>()
{
    return sounds;
}
template<>
inline ResourcesManager<Script>& ResourcesGroup::getManager<Script>()
{
    return scripts;
}

template<>
inline ResourcesManager<Shader>& ResourcesGroup::getManager<Shader>()
{
    return shaders;
}

};

#endif
