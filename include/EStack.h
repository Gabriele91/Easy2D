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

};

#endif