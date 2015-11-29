//
//  DynamicTree.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 20/11/15.
//
//

#ifndef DYNAMICTREE_H
#define DYNAMICTREE_H

#include <Config.h>
#include <Math3D.h>
#include <SpaceManager.h>

namespace Easy2D
{
    
    class DynamicTree : public SpaceManager
    {
        
        class QueryStack
        {
            size_t  mMaxStack;
            long    mTop;
            int*    mItems;
            
        public:
            
            QueryStack(int size = 256)
            {
                mMaxStack   = size;
                mTop        = -1;
                mItems      = new int[mMaxStack];
            }
            
            ~QueryStack()
            {
                delete[] mItems;
            }
            
            void push(int c)
            {
                if(full()) resize(mMaxStack*2);
                mItems[++mTop] = c;
            }
            
            int pop()
            {
                return mItems[mTop--];
            }
            
            void free()
            {
                mTop = 0;
            }
            
            size_t size() const
            {
                return mTop + 1;
            }
            
            int full() const
            {
                return size() == mMaxStack;
            }
            
            int empty() const
            {
                return mTop == -1;
            }
            
            void resize(size_t size)
            {
                if(size == mMaxStack) return;
                //save old pointer
                int* oldItems = mItems;
                //new alloc
                mItems = new int[size];
                std::memcpy(mItems, oldItems, sizeof(int)*size);
                //new size
                mMaxStack=size;
                //delete old alloc
                delete [] oldItems;
            }

        };
        
    public:
        
        struct Node
        {
            //null type
            static const int Null { -1 };
            //height, leaf = 0, free nodes = -1
            int height { Null };
            //box
            AABox2 mAABox;
            //data info
            void* mUserData{ nullptr };
            //node height leaf = 0, not valid -1
            int mHeight { -1 };
            //left field
            int mNext  { Null };
            //right field
            int mParent { Null };
            //childs
            int mChilds[2]{ Null, Null };
            //leaf
            bool isLeaf() const
            {
                return mChilds[0] == Null;
            }
            //get userdata
            template < class T > T* data(int index) const
            {
                return (T*)mUserData;
            }
        };
        ///dynamic tree
        DynamicTree(size_t size = 256);
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
        //get root
        int getRoot() const;
        //get height
        int getHeight() const;
        
    protected:
        //node alloc
        int allocNode();
        void freeNode(int node);
        //insert / remove
        void insertLeaf(int node);
        void removeLeaf(int node);
        //linking
        void linking(int start);
        //balance a root node
        int balance(int iA);
        //list
        mutable QueryStack mStackQuery;
        std::vector<Node>  mNodes;
        //vector info
        int mRoot       { Node::Null };
        int mFreeList   { Node::Null };
        int mLastAlloc  { Node::Null };
        int mNodeCount  { Node::Null };
        int mInsertCount{ Node::Null };
    };
    
};

#endif /* DYNAMICTREE_H */
