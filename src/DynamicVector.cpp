//
//  DynamicVector.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 28/11/15.
//
//
#include <stdafx.h>
#include <DynamicVector.h>
#include <Debug.h>
#include <EStack.h>
#define ENABLE_TOLLERANCE
static const float E2DABBoxExtension = 2;
//namespace Easy2D
using namespace Easy2D;
//vector
DynamicVector::DynamicVector(size_t size)
{
    DEBUG_ASSERT(size);
    //alloc
    mNodes.resize( size );
    //liking
    linking(0);
    //start free list
    mFreeList = 0;
    //allocs
    mNodeCount = 0;
    //inserts
    mInsertCount = 0;
}
//linking
void DynamicVector::linking(int start)
{
    for (int i = start; i < mNodes.size() - 1; ++i)
    {
        mNodes[i].mNext   = i + 1;
    }
    mNodes[mNodes.size()-1].mNext   = Node::Null;
    mFreeList = start;
}
//insert
int DynamicVector::insert(const AABox2& box,void* userdata)
{
    int proxyId = allocNode();
#ifdef ENABLE_TOLLERANCE
    // Fatten the aabb.
    Vec2 r(E2DABBoxExtension, E2DABBoxExtension);
    mNodes[proxyId].mAABox.setMin( box.getMin() - r );
    mNodes[proxyId].mAABox.setMax( box.getMax() + r );
#else
    mNodes[proxyId].mAABox = box;
#endif
    mNodes[proxyId].mUserData = userdata;
    //return proxy id
    return proxyId;
}
//remove
void DynamicVector::remove(int proxyId)
{
    DEBUG_ASSERT(0 <= proxyId && proxyId < mNodes.size());
    freeNode(proxyId);
}
//query
void DynamicVector::query(const AABox2& box, std::vector< int >& output) const
{
    for(int id = mRoot; id!=Node::Null ;id=mNodes[id].mNext)
    {
        //overlap
        if (mNodes[id].mAABox.isIntersection(box)) output.push_back(id);
    }
}

//query
void DynamicVector::renderQuery(const AABox2& viewBox,
                                const Mat4&   viewMat,
                                std::vector< int >& output) const
{
    for(int id = mRoot; id!=Node::Null ;id=mNodes[id].mNext)
    {
        //overlap
        if (viewBox.isIntersection(mNodes[id].mAABox.applay(viewMat))) output.push_back(id);
    }
}
//get user data
void* DynamicVector::getUserData(int index)
{
    return mNodes[index].mUserData;
}
//node update
void DynamicVector::update(int proxyId,const AABox2& box)
{
    if(mNodes[proxyId].mAABox.isInside(box)) return;
#ifdef ENABLE_TOLLERANCE
    Vec2 r(E2DABBoxExtension, E2DABBoxExtension);
    mNodes[proxyId].mAABox.setMin( box.getMin() - r );
    mNodes[proxyId].mAABox.setMax( box.getMax() + r );
#else
    mNodes[proxyId].mAABox = box;
#endif
}
//operators
DynamicVector::Node& DynamicVector::operator[](size_t index)
{
    return mNodes[index];
}

const  DynamicVector::Node&  DynamicVector::operator[](size_t index) const
{
    return mNodes[index];
}
//node alloc
int DynamicVector::allocNode()
{
    if(mFreeList == Node::Null)
    {
        DEBUG_ASSERT(mNodeCount == mNodes.size());
        mNodes.resize(mNodeCount * 2);
        //liking
        linking(mNodeCount);
        //link last alloc
        mNodes[mLastAlloc].mNext = mFreeList;
    }
    //alloc new node
    int nodeId                = mFreeList;
    mFreeList                 = mNodes[nodeId].mNext;
    mNodes[nodeId].mParent    = Node::Null;
    mNodes[nodeId].mUserData  = nullptr;
    ++mNodeCount;
    //save last alloc
    mLastAlloc = nodeId;
    //void root? This node is root
    if (mRoot == Node::Null) mRoot = nodeId;
    //return new node
    return nodeId;
}
//free a node
void DynamicVector::freeNode(int nodeId)
{
    DEBUG_ASSERT(0 <= nodeId && nodeId < mNodes.size());
    DEBUG_ASSERT(0 < mNodeCount);
    mNodes[nodeId].mNext = mFreeList;
    mFreeList = nodeId;
    --mNodeCount;
    //void vector? Void root...
    if(!mNodeCount) mRoot = Node::Null;
}