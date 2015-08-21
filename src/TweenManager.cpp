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

/**
* TweenManager
*/
//constructors
TweenManager::TweenManager()
{
}
TweenManager::TweenManager(const std::initializer_list< TweenManager::TweensPtrsPair >& pairs, bool vloop)
{
	//intert
	for (auto pair : pairs)
	{
		for (auto value : pair.second)
		{
			mTweens[pair.first].push_back(Tween::ptr(value));
		}
	}
	//loop
	mLoop = vloop;
}
TweenManager::TweenManager(const TweenManager::TweensNameMap& values, bool vloop)
{
	mTweens = values;
	mLoop = vloop;
}
//actions
void TweenManager::start(const String& name)
{
	//null? Return
	if (!getObject()) return;
	//get tween
	auto tween = getTweens(name);
	DEBUG_ASSERT(tween);
	//if new tween
	if (tween != mTCurrent) { mSCurrent = name; mTCurrent = tween; mCurrent = -1; }
	//current
	if (mCurrent == -1) { (*mTCurrent)[++mCurrent]->init(getObject(), this); }
	//start
	mStart = true;
}
void TweenManager::stop()
{
	mStart = false;
}
void TweenManager::reset()
{
	mTCurrent = nullptr;
	mCurrent = -1;
	mTime = 0;
}
void TweenManager::loop()
{
	mLoop = true;
}
void TweenManager::noLoop()
{
	mLoop = false;
}
//events
void TweenManager::onSetObject(Object* obj)
{
	reset();
}
void TweenManager::onEraseObject()
{
	stop();
}
//push
void TweenManager::add(const String& name, Tween::ptr value)
{
	mTweens[name].push_back(value);
}
//query
bool TweenManager::isStart() const
{
	return mStart;
}
bool TweenManager::isLoop() const
{
	return mLoop;
}
//component loop
void TweenManager::onRun(float dt)
{
	if (mStart && mTCurrent && mCurrent < (*mTCurrent).size())
	{
		if (!(*mTCurrent)[mCurrent]->update(mTime, dt))
		{
			//end event
			(*mTCurrent)[mCurrent]->end();
			//reset time
			mTime = 0;
			//next
			++mCurrent;
			//is at end
			bool isEnd = mCurrent >= (*mTCurrent).size();
			//end?
			if (isEnd)
			{
				if (mLoop) { mCurrent %= (*mTCurrent).size(); }
				else { mStart = false;       return; }
			}
			//init next tween
			(*mTCurrent)[mCurrent]->init(getObject(), this);

		}
		//updata time
		mTime += dt;
	}
}
//get tweens
TweenManager::Tweens* TweenManager::getTweens(const String& name)
{
	auto it = mTweens.find(name);
	return it != mTweens.end() ? &it->second : nullptr;
}
//get tweens const
const TweenManager::Tweens* TweenManager::getTweens(const String& name) const
{
	auto it = mTweens.find(name);
	return it != mTweens.end() ? &it->second : nullptr;
}
//serialize
void TweenManager::serialize(Table& table)
{
	//put all maps
	for (auto& pair : mTweens)
	{
		auto& tpair = table.createTable(pair.first);
		//put all tween
		for (auto& tween : pair.second)
		{
			auto& newt = tpair.createTable();
			newt.set("tween", tween->getName());
			tween->serialize(newt);
		}
	}
	//loop?
	table.set("loop", (float)isLoop());
	//start?
	if (mTCurrent) table.set("start", mSCurrent);
}
//deserialize
void TweenManager::deserialize(const Table& table)
{
	mTweens.clear();
	reset();
	for (auto& pair : table)
	{
		if (pair.first.isString() && pair.second->asType(Table::TABLE))
		{
			//tweens table
			auto& tweenTable = pair.second->get<Table>();
			//add all tween
			for (int i = 0; i != tweenTable.size(); ++i)
			{
				const Table& readt = tweenTable.getConstTable(i);
				add(pair.first.string(), TweensMap::create(readt.getString("tween"), readt));
			}
		}

	}
	if (table.getFloat("loop", (float)isLoop()) != 0.0f) loop();
	if (table.existsAsType("start", Table::STRING)) start(table.getString("start"));
}