#ifndef  RESOURCE_H
#define  RESOURCE_H

#include <Config.h>
#include <EString.h>
#include <Utility.h>

namespace Easy2D
{
//
class ResourcesGroup;
template<class T> class ResourcesManager;
template <class T> class Resource;
//
template <class T>
class Resource : public Pointers<T>
{
public:
    //create a resource
    Resource(ResourcesManager<T> *rsmr=NULL,
             const String& sfile="")
        :ptrResources(rsmr)
        ,loaded(false)
        ,reloadable(true)
        ,rpath(sfile)
        ,cbRelease(nullptr) {};
    /*manager show hader disable this method*/
    void release(bool dtach=true)
    {
        if(((T*)this)->T::isLoad())
            ((T*)this)->T::unload();
        if(dtach && cbRelease)
            cbRelease(this);
    }
    //getter
    virtual bool isLoad()
    {
        return loaded;
    }
    virtual bool isReloadable()
    {
        return reloadable;
    }
    //load methods
    virtual bool load()=0;
    virtual bool unload()=0;
    //no virtual methods
    DFORCEINLINE const String& getName() const
    {
        return name;
    }
    DFORCEINLINE ResourcesManager<T>* getResourcesManager() const
    {
        return ptrResources;
    }
    DFORCEINLINE const Utility::Path& getPath() const
    {
        return rpath;
    }

protected:

    friend class ResourcesGroup;
    friend class ResourcesManager<T>;

    DFORCEINLINE void setName(const String& name)
    {
        this->name=name;
    }
    DFORCEINLINE void setReleaseCallBack(std::function<void(Resource<T>*)> cbRelease)
    {
        this->cbRelease=cbRelease;
    }

    std::function<void(Resource<T>*)> cbRelease;
    ResourcesManager<T> *ptrResources;
    String name;

    bool loaded;
    bool reloadable;
    Utility::Path rpath;

};


};


#endif

