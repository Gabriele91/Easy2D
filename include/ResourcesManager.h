#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <Config.h>
#include <Debug.h>
#include <EString.h>
#include <Table.h>
#include <Resource.h>

namespace Easy2D{

	class ResourcesGroup;

	template <class T>
	class ResourcesManager {
		//private costructor
		friend class ResourcesGroup;
		ResourcesManager():version("default"){}
		//
		public:			
			//
			ResourcesManager(const Utility::Path& path,
							 const String& version="default")
				:mapResources(NULL,path)
				,version(version){
				//load map resources
				mapResources.load();
			}
			//
			~ResourcesManager(){}
			//manager mathos
			DS_PTR<T> find(const String &objectname){
				//search in this pool
				auto it=rsMap.find(objectname);
				if(it!=rsMap.end()) 
					return it->second.lock();

				return NULL;
			}
			DS_PTR<T> get(const String &objectname){
				//search
				auto resource=find(objectname);
				if(resource) return resource;
				//else make resource
				//get path:
				String path(pathFromName(objectname));
				if(path!=String() /* void string */){
					//
					DS_PTR<T> resource(new T(this,mapResources.getPath().getDirectory()+"/"+path));
					//set into map
					resource->name=objectname;
					rsMap[objectname]=resource;
					//
					return resource;
				}
				return DS_PTR<T>();
			}
			DS_PTR<T> load(const String &objectname){
				//get resource
				DS_PTR<T> resource=get(objectname);
				if(resource && !resource->isLoad())
					resource->load();
				return resource;
			}
			//methos
			String pathFromName(const String& objname){

				if(mapResources.existsAsType(objname,Table::TABLE)){
					const auto& table=mapResources.getTable(objname);
					//get version
					auto path=table.getString(version);
					if(path==""){
						path=table.getString("default");
						//DEBUG_ASSERT_MGS(path.size(),"set a default path: "<<objname);
					}
					return path;
				}else
					return mapResources.getString(objname);
			}
			//
			void load(){
				//load resources
				for(auto& itr :rsMap)
					if(!itr.second.lock()->isLoad())
						itr.second.lock()->load();	

			}
			void unload(bool destroy=false){
				//unload resources
				for(auto& it:rsMap) 
					if(it.second.lock()->isLoad())
						it.second.lock()->unload();
				//destroy resources references
				if(destroy) rsMap.clear();
			}
			//
			void erase(const String &objectname){
				//erase a resources reference
				rsMap.erase(objectname);
			}

		private:
			//name table
			Table mapResources;
			//key version
			Table::KeyTable version;
			//resources map
			DUNORDERED_MAP< String, DW_PTR<T> > rsMap;
	};

};

#endif