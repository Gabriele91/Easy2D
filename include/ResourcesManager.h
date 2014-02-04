#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <Config.h>
#include <Debug.h>
#include <EString.h>
#include <Resource.h>
#include <Table.h>

namespace Easy2D{

	class ResourcesGroup;

	template <class T>
	class ResourcesManager {
		//private costructor
		friend class ResourcesGroup;
		ResourcesManager():rsgr(NULL),version("default"){}
		
		//
		public:			
			//
			ResourcesManager(ResourcesGroup *rsgr,
							 const Utility::Path& path,
							 const Table::KeyTable& version="default")
				:rsgr(rsgr)
				,mapResources( new Table (NULL,path) )
				,deleteTable(true)
				,version(version){
				//load map resources
				mapResources->load();
			}
			ResourcesManager(ResourcesGroup *rsgr,
							 Table *mapRS,
							 bool  deleteRS,
							 const Table::KeyTable& version="default")
				:rsgr(rsgr)
				,mapResources(mapRS)
				,deleteTable(deleteRS)
				,version(version){}
			//
			~ResourcesManager(){
				if(deleteTable) delete mapResources;
			}
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
					DS_PTR<T> resource(new T(this,mapResources->getPath().getDirectory()+"/"+path));
					//set into map
					rsMap[objectname]=resource;
					resource->Resource<T>::setName(objectname);
					resource->Resource<T>::setReleaseCallBack([this](Resource<T>* rs){
						this->erase(rs->getName());
					});
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

				if(mapResources->existsAsType(objname,Table::TABLE)){
					const auto& table=mapResources->getTable(objname);
					//get version
					auto path=table.getString(version);
					if(path==""){
						path=table.getString("default");
						//DEBUG_ASSERT_MSG(path.size(),"set a default path: "<<objname);
					}
					return path;
				}else
					return mapResources->getString(objname);
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
			//
			ResourcesGroup* getResourcesGroup(){
				return rsgr;
			}

		private:
			//ptr resource group			
			ResourcesGroup *rsgr;
			//name table
			Table *mapResources;
			bool deleteTable;
			//key version
			Table::KeyTable version;
			//resources map
			DUNORDERED_MAP< String, DW_PTR<T> > rsMap;
			//for only ResourcesGroup
			void __forceReload(){			
				//load resources
				for(auto& itr :rsMap)
						itr.second.lock()->load();	
			}
	};
};

#endif