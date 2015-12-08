#ifndef ESTACK_H
#define ESTACK_H

#include <Config.h>
#include <list>
#include <algorithm>

namespace Easy2D
{

template <class T>
class EStack : protected std::list<T>
{


public:

    //cosntructor
    EStack():std::list<T>() {}

    //push
    void push(T& t)
    {
        this->push_front(t);
    }
    void push(const T& t)
    {
        this->push_front(t);
    }
    //overload operator
    void operator=(T& t)
    {
        this->push_front(t);
    }
    void operator=(const T& t)
    {
        this->push_front(t);
    }
    //top
    T& top()
    {
        return this->front();
    }
    const T& top() const
    {
        return this->front();
    }
    //pop
    void pop()
    {
        this->pop_front();
    }
    //query
    bool empty() const
    {
        return this->std::list<T>::empty();
    }

    unsigned long size() const
    {
        return this->std::list<T>::size();
    }

    bool contains(const T& t) const
    {
        return std::find(begin(), end(), t)!=end();
    }
    //delete a element
    void erase(const T& t)
    {
        this->std::list<T>::erase(std::find(begin(),end(),t));
    }
    //
    typename std::list<T>::iterator begin()
    {
        return this->std::list<T>::begin();
    }
    typename std::list<T>::iterator end()
    {
        return this->std::list<T>::end();
    }
    typename std::list<T>::const_iterator begin() const
    {
        return this->std::list<T>::begin();
    }
    typename std::list<T>::const_iterator end() const
    {
        return this->std::list<T>::end();
    }

};
    
template < typename T >
class CachedStack
{
    long  mMaxStack;
    long  mTop;
    T*    mItems;
    
public:
    
    CachedStack(long size = 256)
    {
        mMaxStack   = size;
        mTop        = -1;
        mItems      = new T[mMaxStack];
    }
    
    ~CachedStack()
    {
        delete[] mItems;
    }
    
    void push(int c)
    {
        if(full()) resize(mMaxStack*2);
        mItems[++mTop] = c;
    }
    
    T& top() const
    {
        return mItems[mTop];
    }
    
    T& pop()
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
    
    long capacity() const
    {
        return mMaxStack;
    }
    
    void resize(size_t size)
    {
        if(size == mMaxStack) return;
        //save old pointer
        int* oldItems = mItems;
        //new alloc
        mItems = new T[size];
        std::memcpy(mItems, oldItems, sizeof(T)*size);
        //new size
        mMaxStack=size;
        //delete old alloc
        delete [] oldItems;
    }
    
};

};

#endif