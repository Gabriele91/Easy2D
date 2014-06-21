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

public:

    template<typename T>
    ResourcesManager<T>& getManager()
    {
        return NULL;
    }


#ifdef COMPILER_VISUAL_STUDIO
    template<>
    ResourcesManager<Texture>& getManager<Texture>()
    {
        return textures;
    }
    template<>
    ResourcesManager<Table>& getManager<Table>()
    {
        return tables;
    }
    template<>
    ResourcesManager<Mesh>& getManager<Mesh>()
    {
        return meshes;
    }
    template<>
    ResourcesManager<FrameSet>& getManager<FrameSet>()
    {
        return frameSets;
    }
    template<>
    ResourcesManager<Font>& getManager<Font>()
    {
        return fonts;
    }
    template<>
    ResourcesManager<Sound>& getManager<Sound>()
    {
        return sounds;
    }
#endif

private:

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
    template<typename T>  DS_PTR<T> load(const String& path)
    {
        return  getManager<T>().load(path);
    }
    template<typename T>  DS_PTR<T> get(const String& path)
    {
        return getManager<T>().get(path);
    }
    template<typename T>  DS_PTR<T> find(const String& path)
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
    }
    /** reload only gpu resource */
    void reloadGpuResouce();

};

// GCC SUCK //
#ifdef COMPILER_GCC
template<>
ResourcesManager<Texture>& ResourcesGroup::getManager<Texture>();
template<>
ResourcesManager<Table>& ResourcesGroup::getManager<Table>();
template<>
ResourcesManager<Mesh>& ResourcesGroup::getManager<Mesh>();
template<>
ResourcesManager<FrameSet>& ResourcesGroup::getManager<FrameSet>();
template<>
ResourcesManager<Font>& ResourcesGroup::getManager<Font>();
template<>
ResourcesManager<Sound>& ResourcesGroup::getManager<Sound>();
#endif
};

#endif
