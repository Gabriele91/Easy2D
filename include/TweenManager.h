//
//  Tween.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 12/07/15.
//
//
#ifndef TWEENMANAGER_H
#define TWEENMANAGER_H

#include <Config.h>
#include <Component.h>
#include <EStack.h>
#include <Table.h>
#include <Renderable.h>

namespace Easy2D
{
    
    class Tween;
    class TweenManager;
    
    class Tween : public Pointers< Tween >
    {
    public:
        
        Tween(float time  = 0.0f,
              float delay = 0.0f)
        {
            mTime  = time;
            mDelay = delay;
        }
        
        Tween(const Table& table)
        {
            deserialize(table);
        }
        
        virtual ~Tween() { }
        
        //Events
        virtual void init(Object* obj,TweenManager* tmanager)  = 0;
        
        virtual bool update(float count,float dt) = 0;
        
        virtual void end()
        {
            if(callback) callback();
        }
        
        //Twenn times
        virtual float getGlobalTime()
        {
            return mTime + mDelay;
        }
        virtual float getDelay()
        {
            return mDelay;
        }
        virtual float getTime()
        {
            return mTime;
        }
        //interpolation
        float factor(float count)
        {
            return Math::clamp(((count-mDelay) / mTime) , 1.0, 0.0);
        }
        template <class T>
        T interpolation(const T& from,const T& to, float count)
        {
            return  Math::lerp(from, to, factor(count));
        }
        //Tween name
        virtual String getName() const = 0;
        
        //callback
        virtual void setCallback(DFUNCTION< void(void) > ptr)
        {
            callback = ptr;
        }
        
        //serialize
        virtual void serialize(Table& table)
        {
            table.set("time",mTime);
            table.set("delay",mDelay);
        }
        
        //deserialize
        virtual void deserialize(const Table& table)
        {
            mTime =table.getFloat("time",mTime);
            mDelay=table.getFloat("delay",mDelay);
        }
        
    protected:
        
        DFUNCTION< void(void) > callback { nullptr } ;
        float mTime;
        float mDelay;
        
    };
    
    //class factory of tweens
    class TweensMap
    {
        
    public:
        
        //types
        typedef Tween::ptr (*CreateTween)(const Table& value);
        typedef std::map< std::string,CreateTween > MapCreateTween;
        //static methos
        static Tween::ptr create(const std::string& name,const Table& value);
        static void append(const std::string& name,CreateTween fun);
        
    protected:
        
        //map of tweens
        static  MapCreateTween* mTMap;
        
    };
    
    //class used for static registration of a tween class
    template<class T>
    class TweenItem
    {
        
        static Tween::ptr create(const Table& value)
        {
            return std::make_shared<T>(value);
        }
        
        TweenItem(const std::string& name)
        {
            TweensMap::append(name, TweenItem<T>::create);
        }
        
    public:
        
        
        static TweenItem<T>& instance(const std::string& name)
        {
            static TweenItem<T> tweenItem(name);
            return tweenItem;
        }
        
    };
    #define REGISTERED_TWEEN(classname)\
    namespace\
    {\
        static const TweenItem<classname>& _Tween_ ## classname ## _Item = TweenItem<classname>::instance( #classname );\
    }
    
    //manager component
    class TweenManager : public Component
    {
        
    public:
        //pairs
        typedef std::vector< Tween* > TweensPtrs;
        typedef std::vector< Tween::ptr > Tweens;
        typedef std::pair< String, Tweens > TweensPair;
        typedef std::pair< String, TweensPtrs > TweensPtrsPair;
        //map
        typedef std::map< String, Tweens > TweensNameMap;
        
        TweenManager()
        {
        }
        TweenManager( const std::initializer_list< TweensPtrsPair >& pairs, bool vloop = false )
        {
            //intert
            for(auto pair:pairs)
            {
                for(auto value : pair.second )
                {
                    mTweens[ pair.first ].push_back(Tween::ptr(value));
                }
            }
            //loop
            mLoop   = vloop;
        }
        TweenManager( const TweensNameMap& values, bool vloop = false )
        {
            mTweens = values;
            mLoop   = vloop;
        }
        //events
        virtual void onSetObject(Object* obj)
        {
            reset();
        }
        virtual void onEraseObject()
        {
            stop();
        }
        //push
        void add(const String& name,Tween::ptr value)
        {
            mTweens[name].push_back(value);
        }
        //query
        bool isStart()
        {
            return mStart;
        }
        bool isLoop()
        {
            return mLoop;
        }
        //set
        void start(const String& name)
        {
            //null? Return
            if(!getObject()) return;
            //get tween
            auto tween=getTweens(name);
            DEBUG_ASSERT(tween);
            //if new tween
            if(tween != mTCurrent){ mSCurrent=name; mTCurrent=tween; mCurrent = -1; }
            //current
            if(mCurrent == -1){  (*mTCurrent)[++mCurrent]->init(getObject(),this); }
            //start
            mStart = true;
        }
        void stop()
        {
            mStart = false;
        }
        void reset()
        {
            mTCurrent = nullptr;
            mCurrent  = -1;
            mTime     =  0;
        }
        void loop()
        {
            mLoop = true;
        }
        void noLoop()
        {
            mLoop = false;
        }
        //event: run
        virtual void onRun(float dt)
        {
            if (mStart && mTCurrent && mCurrent < (*mTCurrent).size())
            {
                if( !(*mTCurrent)[mCurrent]->update(mTime,dt) )
                {
                    //end event
                    (*mTCurrent)[mCurrent]->end();
                    //reset time
                    mTime = 0;
                    //next
                    ++mCurrent;
                    //is at end
                    bool isEnd=mCurrent >= (*mTCurrent).size();
                    //end?
                    if(isEnd)
                    {
                        if(mLoop) {  mCurrent %= (*mTCurrent).size(); }
                        else      {  mStart=false;       return;      }
                    }
                    //init next tween
                    (*mTCurrent)[mCurrent]->init(getObject(),this);
                   
                }
                //updata time
                mTime+=dt;
            }
        }
        //get tweens
        Tweens* getTweens(const String& name)
        {
            auto it=mTweens.find(name);
            return it!=mTweens.end() ? &it->second : nullptr ;
        }
        //get tweens const
        const Tweens* getTweens(const String& name) const
        {
            auto it=mTweens.find(name);
            return it!=mTweens.end() ? &it->second : nullptr ;
        }
        //component
        ADD_COMPONENT_METHOS(TweenManager)
        //serialize/deserialize
        virtual void serialize(Table& table)
        {
            //put all maps
            for(auto& pair:mTweens)
            {
                auto& tpair=table.createTable(pair.first);
                //put all tween
                for(auto& tween:pair.second)
                {
                    auto& newt=tpair.createTable();
                    newt.set("tween",tween->getName());
                    tween->serialize(newt);
                }
            }
            //loop?
            table.set("loop",(float)isLoop());
            //start?
            if(mTCurrent) table.set("start",mSCurrent);
        }
        virtual void deserialize(const Table& table)
        {
            mTweens.clear();
            reset();
            for(auto& pair:table)
            {
                if(pair.first.isString() && pair.second->asType(Table::TABLE))
                {
                    //tweens table
                    auto& tweenTable=pair.second->get<Table>();
                    //add all tween
                    for(int i=0;i!=tweenTable.size();++i)
                    {
                        const Table& readt = tweenTable.getConstTable(i);
                        add(pair.first.string(),TweensMap::create(readt.getString("tween"),readt));
                    }
                }
                
            }
            if(table.getFloat("loop",(float)isLoop())!=0.0f) loop();
            if(table.existsAsType("start", Table::STRING)) start(table.getString("start"));
        }
        
    protected:
        
        //values
        TweensNameMap mTweens;
        String  mSCurrent;
        Tweens* mTCurrent  { nullptr };
        bool    mStart     { false };
        bool    mLoop      { false };
        int     mCurrent   { -1 };
        float   mTime      { 0 };
    };
    REGISTERED_COMPONENT(TweenManager,"TweenManager");
    };

#endif