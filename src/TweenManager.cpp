//
//  Tween.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/07/15.
//
//
#include <stdafx.h>
#include <TweenManager.h>
//using Easy2D namespace
using namespace Easy2D;
/**
 * TweensMap
 */
//map
TweensMap::MapCreateTween* TweensMap::mTMap = nullptr ;
//create function
Tween::ptr TweensMap::create(const std::string& name,const Table& value)
{
    //assert...
    assert(mTMap);
    //find
    auto it=mTMap->find(name);
    //exist?
    if(it!=mTMap->end())
    {
        return it->second(value);
    }
    //else null
    return nullptr;
}
//append a tween
void TweensMap::append(const std::string& name,TweensMap::CreateTween fun)
{
    //map exist?
    if(!mTMap) mTMap=new MapCreateTween();
    //assert...
    assert(mTMap->find(name)==mTMap->end());
    //append component
    (*mTMap)[name]=fun;
}
