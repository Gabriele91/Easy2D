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
        //constructors
		TweenManager();
		TweenManager(const std::initializer_list< TweensPtrsPair >& pairs, bool vloop = false);
		TweenManager(const TweensNameMap& values, bool vloop = false);
        //events
		virtual void onSetObject(Object* obj);
		virtual void onEraseObject();
        //push
		void add(const String& name, Tween::ptr value);
        //query
        bool isStart() const;
		bool isLoop() const;
        //actions
		void start(const String& name);
		void stop();
		void reset();
		void loop();
		void noLoop();
        //event: run
		virtual void onRun(float dt);
        //get tweens
		Tweens* getTweens(const String& name);
        //get tweens const
		const Tweens* getTweens(const String& name) const;
        //component
		ADD_COMPONENT_METHOS(TweenManager)
		//serialize
		virtual void serialize(Table& table);
		//deserialize
		virtual void deserialize(const Table& table);
        
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