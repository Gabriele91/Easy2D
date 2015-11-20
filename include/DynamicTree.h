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

namespace Easy2D
{
    
    class DynamicTree
    {
        
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
        int insert(const AABox2& box,void* userdata = nullptr);
        //remove
        void remove(int index);
        //node update
        void update(int index,const AABox2& box);
        //query
        void query(const AABox2& box, std::vector< int >& ouput) const;
        //node..
              Node& operator[](size_t);
        const Node& operator[](size_t) const;
        //get userdata
        template < class T > T* data(int index) const
        {
            return (T*)mNodes[index].mUserData;
        }
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
        std::vector<Node> mNodes;
        //vector info
        int mRoot       { Node::Null };
        int mFreeList   { Node::Null };
        int mNodeCount  { Node::Null };
        int mInsertCount{ Node::Null };
    };
    
};

#endif /* DYNAMICTREE_H */
