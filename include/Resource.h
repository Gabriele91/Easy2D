#ifndef  RESOURCE_H
#define  RESOURCE_H

#include <EString.h>

namespace Easy2D{

	class ResourcesManager;

	class Resource{
	public:	

		Resource(ResourcesManager *rsmr=NULL,
				 const String& sfile="")
				 :ptrResources(rsmr)
				 ,loaded(false)
				 ,reloadable(false)
				 ,sfile(sfile){};

		virtual ~Resource(){}
		virtual bool isLoad(){
			return loaded;
		}
		virtual bool isReloadable(){
			return loaded;
		}
		//load methods
		virtual bool load()=0;
		virtual bool unload()=0;

	protected:

		friend class ResourcesManager;
		ResourcesManager *ptrResources;
		bool loaded;
		bool reloadable;
		String sfile;

	};


};
#endif