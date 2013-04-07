#ifndef  RESOURCE_H
#define  RESOURCE_H

#include <Config.h>
#include <EString.h>
#include <Utility.h>

namespace Easy2D{
	//
	template <class T> class ResourcesManager;
	//
	template <class T>
	class Resource{
	public:			
		//define ptr
		typedef DS_PTR<T> ptr;
		//create a resource
		Resource(ResourcesManager<T> *rsmr=NULL,
				 const String& sfile="")
				 :ptrResources(rsmr)
				 ,loaded(false)
				 ,reloadable(false)
				 ,rpath(sfile){};
		//destroy resource
		virtual ~Resource(){
			if(((T*)this)->T::isLoad()) 
					((T*)this)->T::unload();
		};
		//getter
		virtual bool isLoad(){
			return loaded;
		}
		virtual bool isReloadable(){
			return loaded;
		}
		//load methods
		virtual bool load()=0;
		virtual bool unload()=0;
		//no virtual methods
		DFORCEINLINE const String& getName(){
			return name;
		}
		DFORCEINLINE ResourcesManager<T>* getResourcesManager(){
			return ptrResources;
		}
		DFORCEINLINE const Utility::Path& getPath(){
			return rpath;
		}

	private:

		friend class ResourcesManager<T>;
		ResourcesManager<T> *ptrResources;
		String name;
		
	protected:

		bool loaded;
		bool reloadable;
		Utility::Path rpath;

	};


};
#endif