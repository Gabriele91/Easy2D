//
//  DynamicVector.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 28/11/15.
//
//

#ifndef DynamicVector_h
#define DynamicVector_h

#include <Config.h>
#include <Math3D.h>
#include <SpaceManager.h>


namespace Easy2D
{
    
class DynamicVector : public SpaceManager
{
    
public:
    
    struct Node
    {
        //null type
        static const int Null { -1 };
        //box
        AABox2 mAABox;
        //data info
        void* mUserData{ nullptr };
        //left field
        int mNext  { Null };
        //right field
        int mParent { Null };
        //get userdata
        template < class T > T* data(int index) const
        {
            return (T*)mUserData;
        }
    };
    ///dynamic tree
    DynamicVector(size_t size = 256);
    //insert
    virtual int insert(const AABox2& box,void* userdata = nullptr);
    //remove
    virtual void remove(int index);
    //node update
    virtual void update(int index,const AABox2& box);
    //query
    virtual void query(const AABox2& box, std::vector< int >& output) const;
    //query
    virtual void renderQuery(const AABox2& viewBox,
                             const Mat4&   viewMat,
                             std::vector< int >& output) const;
    //et user data
    virtual void* getUserData(int index);
    //node..
          Node& operator[](size_t);
    const Node& operator[](size_t) const;
    
protected:
    //node alloc
    int allocNode();
    void freeNode(int node);
    //linking
    void linking(int start);
    //list
    std::vector<Node> mNodes;
    //list
    int mRoot       { Node::Null };
    int mFreeList   { Node::Null };
    int mLastAlloc  { Node::Null };
    int mNodeCount  { Node::Null };
    int mInsertCount{ Node::Null };
};
    
};

#endif /* DynamicVector_h */
