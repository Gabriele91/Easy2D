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
//#include <Font.h>
//#include <Sound.h>

namespace Easy2D {

	class ResourcesGroup{
	
		ResourcesManager<Texture> textures;
		ResourcesManager<Table> tables;
		ResourcesManager<Mesh> meshes;
		//teble resources
		Table resources;
		//key version
		Table::KeyTable version;

	public:
		/**
		* Set the managers resources 
		* @param path table resources
		*/
		ResourcesGroup(const Utility::Path& path,
					   const String& version="default");
		
		/*
		* void template
		*/
		template<typename T>  DS_PTR<T> load(const String& path){ return DS_PTR<T>(); }
		template<typename T>  DS_PTR<T> get(const String& path){ return DS_PTR<T>(); }
		template<typename T>  DS_PTR<T> find(const String& path){ return DS_PTR<T>(); }
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
		

		/** load all resources returned  */
		DFORCEINLINE void load(){
			textures.load();
			tables.load();
			meshes.load();
		}
		/** unload all resources returned  */
		DFORCEINLINE void unload(bool destroy){
			textures.unload(destroy);
			tables.unload(destroy);
			meshes.unload(destroy);
		}

	};


};

#endif