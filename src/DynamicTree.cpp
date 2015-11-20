//
//  DynamicTree.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 20/11/15.
//
//
#include <stdafx.h>
#include <DynamicTree.h>
#include <EStack.h>
#include <Debug.h>
using namespace    Easy2D;
#define ENABLE_TOLLERANCE
static const float E2DABBoxExtension = 1;

DynamicTree::DynamicTree(size_t size)
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
    //root
    mRoot = Node::Null;
}

DynamicTree::Node& DynamicTree::operator[](size_t index)
{
    return mNodes[index];
}

const  DynamicTree::Node&  DynamicTree::operator[](size_t index) const
{
    return mNodes[index];
}

void DynamicTree::query(const AABox2& box, std::vector< int >& output) const
{
    //alloc stack
    EStack< int > stack;
    //push root into stack
    stack.push(mRoot);
    
    while (stack.size())
    {
        //get id
        int id = stack.top();
        //pop from stack
        stack.pop();
        //null id?
        if ( id == Node::Null ) continue;
        //get node
        const Node* node = &mNodes[id];
        //overlap
        if (node->mAABox.isIntersection(box))
        {
            if(node->isLeaf())
                output.push_back(id);
            else
            {
                stack.push(node->mChilds[0]);
                stack.push(node->mChilds[1]);
            }
        }
        
    }
}
//insert
int DynamicTree::insert(const AABox2& box,void* userdata)
{
    int proxyId = allocNode();
    
    // Fatten the aabb.
#ifdef ENABLE_TOLLERANCE
    Vec2 r(E2DABBoxExtension, E2DABBoxExtension);
    mNodes[proxyId].mAABox.setMin( box.getMin() - r );
    mNodes[proxyId].mAABox.setMax( box.getMax() + r );
#else
    mNodes[proxyId].mAABox = box;
#endif
    mNodes[proxyId].mUserData = userdata;
    mNodes[proxyId].mHeight = 0;
    
    insertLeaf(proxyId);
    return proxyId;
}
//remove
void DynamicTree::remove(int proxyId)
{
    DEBUG_ASSERT(0 <= proxyId && proxyId < mNodes.size());
    DEBUG_ASSERT(mNodes[proxyId].isLeaf());
    removeLeaf(proxyId);
    freeNode(proxyId);
}
//node update
void DynamicTree::update(int proxyId,const AABox2& box)
{
    if(mNodes[proxyId].mAABox.isInside(box)) return;
    //else remove and re-insert
    removeLeaf(proxyId);
#ifdef ENABLE_TOLLERANCE
    Vec2 r(E2DABBoxExtension, E2DABBoxExtension);
    mNodes[proxyId].mAABox.setMin( box.getMin() - r );
    mNodes[proxyId].mAABox.setMax( box.getMax() + r );
#else
    mNodes[proxyId].mAABox = box;
#endif
    insertLeaf(proxyId);
}


//get root
int DynamicTree::getRoot() const
{
    return mRoot;
}

//get height
int DynamicTree::getHeight() const
{
    if(mRoot==Node::Null) return 0;
    return mNodes[mRoot].mHeight;
}

//node alloc
int DynamicTree::allocNode()
{
    if(mFreeList == Node::Null)
    {
        DEBUG_ASSERT(mNodeCount != mNodes.size());
        mNodes.resize(mNodeCount * 2);
        linking(mNodeCount);
    }
    //alloc new node
    int nodeId                = mFreeList;
    mFreeList                 = mNodes[nodeId].mNext;
    mNodes[nodeId].mParent    = Node::Null;
    mNodes[nodeId].mChilds[0] = Node::Null;
    mNodes[nodeId].mChilds[1] = Node::Null;
    mNodes[nodeId].mHeight    = 0;
    mNodes[nodeId].mUserData = nullptr;
    ++mNodeCount;
    //return new node
    return nodeId;
}
void DynamicTree::freeNode(int nodeId)
{
    DEBUG_ASSERT(0 <= nodeId && nodeId < mNodes.size());
    DEBUG_ASSERT(0 < mNodeCount);
    mNodes[nodeId].mNext = mFreeList;
    mNodes[nodeId].mHeight = -1;
    mFreeList = nodeId;
    --mNodeCount;
}
//insert / remove
void DynamicTree::insertLeaf(int leaf)
{
    ++mInsertCount;
    //void root?
    if (mRoot == Node::Null)
    {
        mRoot = leaf;
        mNodes[mRoot].mParent = Node::Null;
        return;
    }
    //else..
    AABox2 leafAABB = mNodes[leaf].mAABox;
    //index
    int index = mRoot;
    //start to search best position, compute the sibling
    while (mNodes[index].isLeaf() == false)
    {
        int childs[] = { mNodes[index].mChilds[0],  mNodes[index].mChilds[1] };
        //get area
        float area = mNodes[index].mAABox.getArea();
        //combinate area
        AABox2 combinedAABB;
        combinedAABB.addBox(leafAABB);
        combinedAABB.addBox(mNodes[index].mAABox);
        float combinedArea = combinedAABB.getArea();
        //cost of creating a new parent for this node and the new leaf
        float cost = 2.0f * combinedArea;
        // minimum cost of pushing the leaf further down the tree
        float inheritanceCost = 2.0f * (combinedArea - area);
        // cost of descending into child0/1
        float costs[] = { 0, 0 };
        //compute..
        for(int i=0; i!=2 ;++i)
        {
            //compute costs
            if (mNodes[childs[i]].isLeaf())
            {
                AABox2 aabb;
                aabb.addBox(leafAABB);
                aabb.addBox(mNodes[childs[i]].mAABox);
                costs[i] = aabb.getArea() + inheritanceCost;
            }
            else
            {
                AABox2 aabb;
                aabb.addBox(leafAABB);
                aabb.addBox(mNodes[childs[i]].mAABox);
                float oldArea = mNodes[childs[i]].mAABox.getArea();
                float newArea = aabb.getArea();
                costs[i] = (newArea - oldArea) + inheritanceCost;
            }
        }
        // Descend according to the minimum cost.
        if (cost < costs[0] && cost < costs[1]) break;
        // Descend
        if (costs[0] < costs[1]) index = childs[0];
        else                     index = childs[1];
    }
    //save sibling
    int sibling = index;
    // Create a new parent.
    int oldParent = mNodes[sibling].mParent;
    int newParent = allocNode();
    mNodes[newParent].mParent = oldParent;
    mNodes[newParent].mUserData = NULL;
    mNodes[newParent].mAABox = AABox2();
    mNodes[newParent].mAABox.addBox(leafAABB);
    mNodes[newParent].mAABox.addBox(mNodes[sibling].mAABox);
    mNodes[newParent].height = mNodes[sibling].mHeight + 1;
    //position of old parent
    if (oldParent != Node::Null)
    {
        // The sibling was not the root.
        if (mNodes[oldParent].mChilds[0] == sibling) mNodes[oldParent].mChilds[0] = newParent;
        else                                         mNodes[oldParent].mChilds[1] = newParent;
        
        mNodes[newParent].mChilds[0] = sibling;
        mNodes[newParent].mChilds[1] = leaf;
        mNodes[sibling].mParent = newParent;
        mNodes[leaf].mParent = newParent;
    }
    else
    {
        // The sibling was the root.
        mNodes[newParent].mChilds[0] = sibling;
        mNodes[newParent].mChilds[1] = leaf;
        mNodes[sibling].mParent = newParent;
        mNodes[leaf].mParent = newParent;
        mRoot = newParent;
    }
    
    // Walk back up the tree fixing heights and AABBs
    index = mNodes[leaf].mParent;
    while (index != Node::Null)
    {
        index = balance(index);
        
        int child0 = mNodes[index].mChilds[0];
        int child1 = mNodes[index].mChilds[1];
        
        DEBUG_ASSERT(child0 != Node::Null);
        DEBUG_ASSERT(child1 != Node::Null);
        
        mNodes[index].mHeight = 1 + std::max(mNodes[child0].mHeight, mNodes[child1].mHeight);
        mNodes[index].mAABox = AABox2();
        mNodes[index].mAABox.addBox(mNodes[child0].mAABox);
        mNodes[index].mAABox.addBox(mNodes[child1].mAABox);
        
        index = mNodes[index].mParent;
    }

}
void DynamicTree::removeLeaf(int leaf)
{
    if (leaf == mRoot)
    {
        mRoot = Node::Null;
        return;
    }
    
    int parent = mNodes[leaf].mParent;
    int grandParent = mNodes[parent].mParent;
    int sibling;
    if (mNodes[parent].mChilds[0] == leaf)  sibling = mNodes[parent].mChilds[1];
    else                                    sibling = mNodes[parent].mChilds[0];
    
    if (grandParent != Node::Null)
    {
        // Destroy parent and connect sibling to grandParent.
        if (mNodes[grandParent].mChilds[0] == parent)
            mNodes[grandParent].mChilds[0] = sibling;
        else
            mNodes[grandParent].mChilds[1] = sibling;
        
        mNodes[sibling].mParent = grandParent;
        freeNode(parent);
        
        // Adjust ancestor bounds.
        int index = grandParent;
        while (index != Node::Null)
        {
            index = balance(index);
            
            int child0 = mNodes[index].mChilds[0];
            int child1 = mNodes[index].mChilds[1];
            
            mNodes[index].mAABox = AABox2();
            mNodes[index].mAABox.addBox(mNodes[child0].mAABox);
            mNodes[index].mAABox.addBox(mNodes[child1].mAABox);
            mNodes[index].mHeight = 1 + std::max(mNodes[child0].mHeight, mNodes[child1].mHeight);
            
            index = mNodes[index].mParent;
        }
    }
    else
    {
        mRoot = sibling;
        mNodes[sibling].mParent = Node::Null;
        freeNode(parent);
    }
}
//linking
void DynamicTree::linking(int start)
{
    for (int i = start; i < mNodes.size() - 1; ++i)
    {
        mNodes[i].mNext   = i + 1;
        mNodes[i].mHeight = -1;
    }
    mNodes[mNodes.size()-1].mNext   = Node::Null;
    mNodes[mNodes.size()-1].mHeight = -1;
    mFreeList = start;
}
//balance
int DynamicTree::balance(int iA)
{
    
    DEBUG_ASSERT(iA != Node::Null);
    
    Node* A = &mNodes[ iA ];
    //if a leaf in h2 heap?
    if (A->isLeaf() || A->mHeight < 2)  return iA;
    //else search balance child
    int iB = A->mChilds[0];
    int iC = A->mChilds[1];
    DEBUG_ASSERT(0 <= iB && iB < mNodes.size());
    DEBUG_ASSERT(0 <= iC && iC < mNodes.size());
    //point to
    Node* B = &mNodes[iB];
    Node* C = &mNodes[iC];
    //balance
    int balance = C->mHeight - B->mHeight;
    
    // Rotate C up
    if (balance > 1)
    {
        int iF = C->mChilds[0];
        int iG = C->mChilds[1];
        Node* F = &mNodes[iF];
        Node* G = &mNodes[iG];
        DEBUG_ASSERT(0 <= iF && iF <  mNodes.size());
        DEBUG_ASSERT(0 <= iG && iG <  mNodes.size());
        
        // Swap A and C
        C->mChilds[0] = iA;
        C->mParent    = A->mParent;
        A->mParent    = iC;
        
        // A's old parent should point to C
        if (C->mParent != Node::Null)
        {
            if (mNodes[C->mParent].mChilds[0] == iA)
            {
                mNodes[C->mParent].mChilds[0] = iC;
            }
            else
            {
                DEBUG_ASSERT(mNodes[C->mParent].mChilds[1] == iA);
                mNodes[C->mParent].mChilds[1] = iC;
            }
        }
        else
        {
            mRoot = iC;
        }
        
        // Rotate
        if (F->height > G->height)
        {
            C->mChilds[1] = iF;
            A->mChilds[1] = iG;
            G->mParent    = iA;
            
            A->mAABox = AABox2();
            A->mAABox.addBox(B->mAABox);
            A->mAABox.addBox(G->mAABox);
            
            C->mAABox = AABox2();
            C->mAABox.addBox(A->mAABox);
            C->mAABox.addBox(F->mAABox);
            
            A->mHeight = 1 + std::max(B->mHeight, G->mHeight);
            C->mHeight = 1 + std::max(A->mHeight, F->mHeight);
        }
        else
        {
            C->mChilds[1] = iG;
            A->mChilds[1] = iF;
            F->mParent    = iA;
            
            A->mAABox = AABox2();
            A->mAABox.addBox(B->mAABox);
            A->mAABox.addBox(F->mAABox);
            
            C->mAABox = AABox2();
            C->mAABox.addBox(A->mAABox);
            C->mAABox.addBox(G->mAABox);
            
            A->height = 1 + std::max(B->mHeight, F->mHeight);
            C->height = 1 + std::max(A->mHeight, G->mHeight);
        }
        
        return iC;
    }
    
    // Rotate B up
    if (balance < -1)
    {
        int iD  = B->mChilds[0];
        int iE  = B->mChilds[1];
        Node* D = &mNodes[iD];
        Node* E = &mNodes[iE];
        DEBUG_ASSERT(0 <= iD && iD < mNodes.size());
        DEBUG_ASSERT(0 <= iE && iE < mNodes.size());
        
        // Swap A and B
        B->mChilds[0] = iA;
        B->mParent    = A->mParent;
        A->mParent    = iB;
        
        // A's old parent should point to B
        if (B->mParent != Node::Null)
        {
            if (mNodes[B->mParent].mChilds[0] == iA)
            {
                mNodes[B->mParent].mChilds[0] = iB;
            }
            else
            {
                DEBUG_ASSERT(mNodes[B->mParent].mChilds[1] == iA);
                mNodes[B->mParent].mChilds[1] = iB;
            }
        }
        else
        {
            mRoot = iB;
        }
        
        // Rotate
        if (D->height > E->height)
        {
            B->mChilds[1] = iD;
            A->mChilds[0] = iE;
            E->mParent = iA;
            
            A->mAABox = AABox2();
            A->mAABox.addBox(E->mAABox);
            A->mAABox.addBox(C->mAABox);
            
            B->mAABox = AABox2();
            B->mAABox.addBox(A->mAABox);
            B->mAABox.addBox(D->mAABox);
            
            A->height = 1 + std::max(C->mHeight, E->mHeight);
            B->height = 1 + std::max(A->mHeight, D->mHeight);
        }
        else
        {
            B->mChilds[1] = iE;
            A->mChilds[0] = iD;
            D->mParent = iA;
            
            A->mAABox = AABox2();
            A->mAABox.addBox(C->mAABox);
            A->mAABox.addBox(D->mAABox);
            
            B->mAABox = AABox2();
            B->mAABox.addBox(A->mAABox);
            B->mAABox.addBox(E->mAABox);
            
            A->height = 1 + std::max(C->mHeight, D->mHeight);
            B->height = 1 + std::max(A->mHeight, E->mHeight);
        }
        
        return iB;
    }
    
    return iA;
}