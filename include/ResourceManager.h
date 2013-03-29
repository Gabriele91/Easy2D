#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <Config.h>
#include <Resource.h>

namespace Easy2D{
	class ResourcesManager {
		public:

			ResourcesManager(){}
			~ResourcesManager(){
				//unload resources
				unload();
				//destroy all resources
				rsMap.clear();
			}			
			//templete mathos
			template <class T>
			T *find(const std::string &filename){
				//search in this pool
				auto it=rsMap.find(filename);
				if(it!=rsMap.end()) 
					return it->second;
				//search in other pool
				for(auto& maps :  subRM){
					T* value=maps.find<T>(filename);
					if(value)
						return value;
				}

				return NULL;
			}
			template <class T>
			T *get(const std::string &filename){
				//search
				T *resource=find<T>(filename)
				//else make resource
				if(!resource){
					resource = new T(this,filename);
					rsMap[filename] = resource;
				}
				return resource;
			}
			//methos
			void load(){
				//load resources
				for(auto& itr :rsMap)
					if(!itr.second->isLoad())
						itr.second->load();	
				//load subs resources 
				for(auto& itr:subRM) itr->load();
			}
			void unload(bool destroy=false){
				//unload resources 
				if(destroy) 
					for(auto& it:rsMap) 
						delete it.second;
				else 
					for(auto& it:rsMap) 
						if(it.second->isLoad())
							it.second->unload();
				//unload subs resources 
				for(auto& itr:subRM) itr->unload(destroy);
				//destroy resources references
				if(destroy) rsMap.clear();
			}

		private:
			
			typedef std::vector<ResourcesManager*> SubResources;
			typedef DUNORDERED_MAP< String, Resource* > ResourcesMap;
			SubResources subRM;
			ResourcesMap rsMap;
	};
};

#endif