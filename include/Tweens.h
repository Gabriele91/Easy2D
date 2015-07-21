//
//  Tweens.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/07/15.
//
//

#ifndef TWEENS_H
#define TWEENS_H

#include <Config.h>
#include <TweenManager.h>

namespace Easy2D
{
    template< class Value, class Element = Object >
    class TweenFromTo : public Tween
    {
    protected:
        
        Element* mObj;
        Value    mFrom;
        Value    mTo;
        
    public:
        
        virtual Value get() = 0;
        virtual void set(const Value& value) = 0;
        virtual Element* getElement(Object* obj) = 0;
        
        TweenFromTo()
        {
        }
        
        TweenFromTo(const Value& to,
                    float time  = 0.0,
                    float delay = 0.0)
        :Tween(time,delay)
        {
            mTo=to;
        }
        
        TweenFromTo(const Table& table)
        {
            deserialize(table);
        }
        
        virtual void init(Object* obj, TweenManager* tmanager)
        {
            DEBUG_ASSERT(obj);
            mObj  = getElement(obj);
            mFrom = get();
        }
        
        virtual bool update(float count,float dt)
        {
            DEBUG_ASSERT(mObj);
            set(interpolation(mFrom, mTo, factor(count)));
            return count < getGlobalTime();
        }
    
        //serialize
        virtual void serialize(Table& table)
        {
            Tween::serialize(table);
            //set to field
            table.set("to",mTo);
        }
        
        //deserialize
        virtual void deserialize(const Table& table)
        {
            Tween::deserialize(table);
            //get to field
            mTo=table.get<Value>("to",mTo);
        }
    };
    ////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////
    class TPosition : public TweenFromTo< Vec2 >
    {
       
    public:
        
        TPosition(const Vec2& to = Vec2::ZERO,
                  float time     = 0.0f,
                  float delay    = 0.0f)
        :TweenFromTo< Vec2 >(to,time,delay)
        {
        }
        
        TPosition(const Table& table)
        {
            deserialize(table);
        }
        
        virtual Object* getElement(Object* obj)
        {
            return obj;
        }
        
        Vec2 get()
        {
            return mObj->getPosition();
        }
        
        void set(const Vec2& value)
        {
            mObj->setPosition(value);
        }
        
        virtual String getName() const
        {
            return "TPosition";
        }
        
    };
    REGISTERED_TWEEN(TPosition);
    ////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////
    class TScale : public TweenFromTo< Vec2 >
    {
        
    public:
        
        TScale(const Vec2& to = Vec2::ZERO,
               float time     = 0.0f,
               float delay    = 0.0f)
        :TweenFromTo< Vec2 >(to,time,delay)
        {
        }
        
        TScale(const Table& table)
        {
            deserialize(table);
        }
        
        virtual Object* getElement(Object* obj)
        {
            return obj;
        }
        
        Vec2 get()
        {
            return mObj->getScale();
        }
        
        void set(const Vec2& value)
        {
            mObj->setScale(value);
        }
        
        virtual String getName() const
        {
            return "TScale";
        }
        
    };
    REGISTERED_TWEEN(TScale);
    ////////////////////////////////////////////////////

    
    ////////////////////////////////////////////////////
    class TRotation : public TweenFromTo< Degree >
    {
        
    public:
        
        TRotation(const Angle& to = Angle::radian(0.0),
                  float time      = 0.0f,
                  float delay     = 0.0f)
        :TweenFromTo< Degree >(Degree(to.valueDegrees()),
                               time,
                               delay)
        {
        }
        
        TRotation(const Table& table)
        {
            deserialize(table);
        }
        
        virtual Object* getElement(Object* obj)
        {
            return obj;
        }
        
        Degree get()
        {
            return Degree(mObj->getRotation().valueDegrees());
        }
        
        void set(const Degree& value)
        {
            mObj->setRotation(Angle(value));
        }
        
        virtual String getName() const
        {
            return "TRotation";
        }
        
    };
    REGISTERED_TWEEN(TRotation);
    ////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////
    class TTranslate : public TweenFromTo< Vec2 >
    {
        
    public:
        
        TTranslate(const Vec2& to = Vec2::ZERO,
                   float time     = 0.0f,
                   float delay    = 0.0f)
        :TweenFromTo< Vec2 >(to,time,delay)
        {
        }
        
        TTranslate(const Table& table)
        {
            deserialize(table);
        }
        
        virtual Object* getElement(Object* obj)
        {
            return obj;
        }
        
        Vec2 get()
        {
            return mObj->getPosition();
        }
        
        void set(const Vec2& value)
        {
             mObj->setPosition(value);
        }
        
        virtual bool update(float count,float dt)
        {
            DEBUG_ASSERT(mObj);
            set(mFrom+interpolation(Vec2::ZERO, mTo, factor(count)));
            return count < getGlobalTime();
        }
        
        virtual String getName() const
        {
            return "TTranslate";
        }
        
    };
    REGISTERED_TWEEN(TTranslate);
    ////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////
    class TTurn : public TweenFromTo< Degree >
    {
        
    public:
        
        TTurn(const Angle& to = Angle::radian(0.0),
              float time      = 0.0f,
              float delay     = 0.0f)
        :TweenFromTo< Degree >(Degree(to.valueDegrees()),
                               time,
                               delay)
        {
        }
        
        TTurn(const Table& table)
        {
            deserialize(table);
        }
        
        virtual Object* getElement(Object* obj)
        {
            return obj;
        }
        
        Degree get()
        {
            return Degree(mObj->getRotation().valueDegrees());
        }
        
        void set(const Degree& value)
        {
             mObj->setRotation(Angle(value));
        }
        
        virtual bool update(float count,float dt)
        {
            DEBUG_ASSERT(mObj);
            set(mFrom+interpolation(Degree(0), mTo, factor(count)));
            return count < getGlobalTime();
        }
        
        virtual String getName() const
        {
            return "TTurn";
        }
        
    };
    REGISTERED_TWEEN(TTurn);
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    class TColor : public TweenFromTo< Color, Renderable >
    {
        
    public:
        
        TColor(const Color& to = Color::WHITE,
               float time      = 0.0f,
               float delay     = 0.0f)
        :TweenFromTo< Color, Renderable >(to,
                                          time,
                                          delay)
        {
        }
        
        TColor(const Table& table)
        {
            deserialize(table);
        }
        
        Renderable* getElement(Object* obj)
        {
            return obj->getComponent<Renderable>();
        }
        
        Color get()
        {
            return mObj->getColor();
        }
        
        void set(const Color& value)
        {
            mObj->setColor(value);
        }
        
        void set(const Vec4& value)
        {
            mObj->setColor(Color::from(value));
        }
        
        virtual bool update(float count,float dt)
        {
            DEBUG_ASSERT(mObj);
            Vec4 from  = mFrom.toVec4();
            Vec4 to    = mTo.toVec4();
            Vec4 value = interpolation(from, to, factor(count));

            set(value);
            return count < getGlobalTime();
        }
        
        virtual String getName() const
        {
            return "TColor";
        }
        
        //serialize
        virtual void serialize(Table& table)
        {
            Tween::serialize(table);
            //set to field
            table.set("to",mTo);
        }
        
        //deserialize
        virtual void deserialize(const Table& table)
        {
            Tween::deserialize(table);
            //get to field
            mTo = table.getColor("to",mTo);
        }
        
    };
    REGISTERED_TWEEN(TColor);
    ////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////
    class TCompose : public Tween
    {
        Object* mObj { nullptr };
        std::vector < Tween::ptr > mTweens;
        
        void computeTime()
        {
            mDelay = 0;
            mTime  = 0;
            for(auto& tween:mTweens)
                mTime =  Math::max(mTime,tween->getGlobalTime());
        }
        
    public:
        
        
        TCompose(const Table& table)
        {
            deserialize(table);
        }
        
        virtual void init(Object* obj, TweenManager* tmanager)
        {
            DEBUG_ASSERT(obj);
            //objects
            mObj  = obj;
            //all tweens
            for(auto& tween : mTweens) tween->init(obj, tmanager);
        }
        
        virtual bool update(float count,float dt)
        {
            DEBUG_ASSERT(mObj);
            //no tween
            if(!mTweens.size()) return false;
            //flag
            bool flag = false;
            //all tween
            for(auto& tween:mTweens) flag = flag | tween->update(count,dt);
            //end
            return flag;
        }
        
        virtual void end()
        {
            for(auto& tween : mTweens) tween->end();
            Tween::end();
        }
        
        //get name
        virtual String getName() const
        {
            return "TCompose";
        }
        
        //serialize
        virtual void serialize(Table& table)
        {
            //Tween::serialize(table);
            //set to field
            for(auto& tween:mTweens)
            {
                auto& newt=table.createTable();
                newt.set("tween",tween->getName());
                tween->serialize(newt);
            }
        }
        
        //deserialize
        virtual void deserialize(const Table& table)
        {
            //Tween::deserialize(table);
            //get to field
            mTweens.clear();
            for(int i=0;i!=table.size();++i)
            {
                if(table.existsAsType(i, Table::TABLE))
                {
                    const Table& readt = table.getConstTable(i);
                    mTweens.push_back(TweensMap::create(readt.getString("tween"),readt));
                }
            }
            computeTime();
        }
    };
    REGISTERED_TWEEN(TCompose);
    ////////////////////////////////////////////////////
}
#endif
