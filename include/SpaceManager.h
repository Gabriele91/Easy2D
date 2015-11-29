//
//  SpaceManager.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 28/11/15.
//
//

#ifndef SpaceManager_h
#define SpaceManager_h

#include <Config.h>
#include <MemoryAllocator.h>

namespace Easy2D
{
    class SpaceManager : public Pointers<SpaceManager>
    {
    public:
        //insert
        virtual int insert(const AABox2& box,void* userdata = nullptr) = 0;
        //remove
        virtual void remove(int index) = 0;
        //node update
        virtual void update(int index,const AABox2& box) = 0;
        //query
        virtual void query(const AABox2& box, std::vector< int >& output) const = 0;
        //query
        virtual void renderQuery(const AABox2& viewBox,
                                 const Mat4&   viewMat,
                                 std::vector< int >& output) const = 0;
        //raw data
        virtual void* getUserData(int index) = 0;
        //get data
        template< typename T >
        T* data(int index)
        {
            return (T*)getUserData(index);
        }

    };
}

#endif /* SpaceManager_h */
