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
//#include <Font.h>
//#include <Sound.h>

namespace Easy2D {

	class ResourcesGroup{

		ResourcesManager<Texture> textures;
		ResourcesManager<Table> tables;
		ResourcesManager<Mesh> meshes;
		ResourcesManager<FrameSet> frameSets;
		//teble resources
		Table resources;
		//key version
		Table::KeyTable version;
		//load a resource 
		template<class RM>
		void loadAResource(const Utility::Path& path, const String& name,RM& object){
			//tables
			if(resources.existsAsType(name,Table::STRING)){
				String globalPath=path.getDirectory()+"/"+resources.getString(name);
				new (&object) RM(this,globalPath,version);
			}
			else if(resources.existsAsType(name,Table::TABLE)){
				new (&object) RM(this,(Table*)(&resources.getTable(name)),false,version);
			}
			else {
				DEBUG_MESSAGE("error: "<<path <<" not setted tables");
			}
		}

	public:
		/**
		* Set the managers resources
		* @param path table resources
		*/
		ResourcesGroup(const Utility::Path& path,
					   const String& version="default");
		/**
		* delete resource goup
		*/
		virtual ~ResourcesGroup();

		/*
		* void template
		*/
		template<typename T>  DS_PTR<T> load(const String& path){ return DS_PTR<T>(); }
		template<typename T>  DS_PTR<T> get(const String& path){ return DS_PTR<T>(); }
		template<typename T>  DS_PTR<T> find(const String& path){ return DS_PTR<T>(); }
		template<typename T>  String getResourceDirectory(){
			return String();
		}

		#ifdef COMPILER_VISUAL_STUDIO
		/*
		* Texture manager:
		*/
		template <>
		/** load texture */
		DFORCEINLINE Texture::ptr load<Texture>(const String& path){
			return textures.load(path);
		}
		template <>
		/** get texture (but not load in memory) */
		DFORCEINLINE Texture::ptr get<Texture>(const String& path){
			return textures.get(path);
		}
		template<>
		/** find a texture already returned */
		DFORCEINLINE Texture::ptr find<Texture>(const String& path){
			return textures.find(path);
		}
		/** directory texture resources */
		template<>
		DFORCEINLINE String getResourceDirectory<Texture>(){
			return textures.mapResources->getPath().getDirectory();
		}
		/*
		* Table manager:
		*/
		template<>
		/** load table */
		DFORCEINLINE Table::ptr load<Table>(const String& path){
			return tables.load(path);
		}
		template<>
		/** get table  (but not load in memory) */
		DFORCEINLINE Table::ptr get<Table>(const String& path){
			return tables.get(path);
		}
		template<>
		/** find a table already returned */
		DFORCEINLINE Table::ptr find<Table>(const String& path){
			return tables.find(path);
		}
		/** directory table resources */
		template<>
		DFORCEINLINE String getResourceDirectory<Table>(){
			return tables.mapResources->getPath().getDirectory();
		}

		/*
		* Mesh manager:
		*/
		template<>
		/** load mesh */
		DFORCEINLINE Mesh::ptr load<Mesh>(const String& path){
			return meshes.load(path);
		}
		template<>
		/** get mesh  (but not load in memory) */
		DFORCEINLINE Mesh::ptr get<Mesh>(const String& path){
			return meshes.get(path);
		}
		template<>
		/** find a mesh already returned */
		DFORCEINLINE Mesh::ptr find<Mesh>(const String& path){
			return meshes.find(path);
		}
		/** directory mesh resources */
		template<>
		DFORCEINLINE String getResourceDirectory<Mesh>(){
			return meshes.mapResources->getPath().getDirectory();
		}

		/*
		* FrameSet manager:
		*/
		template<>
		/** load FrameSet */
		DFORCEINLINE FrameSet::ptr load<FrameSet>(const String& path){
			return frameSets.load(path);
		}
		template<>
		/** get FrameSet  (but not load in memory) */
		DFORCEINLINE FrameSet::ptr get<FrameSet>(const String& path){
			return frameSets.get(path);
		}
		template<>
		/** find a FrameSet already returned */
		DFORCEINLINE FrameSet::ptr find<FrameSet>(const String& path){
			return frameSets.find(path);
		}
		/** directory FrameSet resources */
		template<>
		DFORCEINLINE String getResourceDirectory<FrameSet>(){
			return meshes.mapResources->getPath().getDirectory();
		}
		#endif

		/** load all resources returned  */
		DFORCEINLINE void load(){
			textures.load();
			tables.load();
			meshes.load();
			frameSets.load();
		}
		/** unload all resources returned  */
		DFORCEINLINE void unload(bool destroy){
			textures.unload(destroy);
			tables.unload(destroy);
			meshes.unload(destroy);
		}
		/** reload only gpu resource */
		void reloadGpuResouce();

	};
	
    // GCC SUCK //
	#ifdef COMPILER_GCC
    /*
    * Texture manager:
    */
    template<> 
    /** load texture */
    DFORCEINLINE Texture::ptr ResourcesGroup::load<Texture>(const String& path);
    template<>
    /** get texture (but not load in memory) */
    DFORCEINLINE Texture::ptr ResourcesGroup::get<Texture>(const String& path);
    template<>
    /** find a texture already returned */
    DFORCEINLINE Texture::ptr ResourcesGroup::find<Texture>(const String& path);
    /** directory texture resources */
    template<>
    DFORCEINLINE String ResourcesGroup::getResourceDirectory<Texture>();

    /*
    * Table manager:
    */
    template<>
    /** load table */
    DFORCEINLINE Table::ptr ResourcesGroup::load<Table>(const String& path);
    template<>
    /** get table  (but not load in memory) */
    DFORCEINLINE Table::ptr ResourcesGroup::get<Table>(const String& path);
    template<>
    /** find a table already returned */
    DFORCEINLINE Table::ptr ResourcesGroup::find<Table>(const String& path);
    /** directory table resources */
    template<>
    DFORCEINLINE String ResourcesGroup::getResourceDirectory<Table>();
    /*
    * Mesh manager:
    */
    template<>
    /** load mesh */
    DFORCEINLINE Mesh::ptr ResourcesGroup::load<Mesh>(const String& path);
    template<>
    /** get mesh  (but not load in memory) */
    DFORCEINLINE Mesh::ptr ResourcesGroup::get<Mesh>(const String& path);
    template<>
    /** find a mesh already returned */
    DFORCEINLINE Mesh::ptr ResourcesGroup::find<Mesh>(const String& path);
    /** directory mesh resources */
    template<>
    DFORCEINLINE String ResourcesGroup::getResourceDirectory<Mesh>();
	
	/*
	* FrameSet manager:
	*/
	template<>
	/** load FrameSet */
	DFORCEINLINE FrameSet::ptr ResourcesGroup::load<FrameSet>(const String& path);
	template<>
	/** get FrameSet  (but not load in memory) */
	DFORCEINLINE FrameSet::ptr ResourcesGroup::get<FrameSet>(const String& path);
	template<>
	/** find a FrameSet already returned */
	DFORCEINLINE FrameSet::ptr ResourcesGroup::find<FrameSet>(const String& path);
	/** directory FrameSet resources */
	template<>
	DFORCEINLINE String ResourcesGroup::getResourceDirectory<FrameSet>();
	#endif
};

#endif
