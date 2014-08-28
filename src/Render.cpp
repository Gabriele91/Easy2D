#include <stdafx.h>
#include <Debug.h>
#include <Screen.h>
#include <Application.h>
#include <Render.h>
#include <Object.h>
#include <RenderContext.h>
/////////////////////////
using namespace Easy2D;
/////////////////////////
#define MAX_BUFFER_TRIANGLES 2000 //2K

Render::Render()
{
    camera=nullptr;
}

void Render::append(Object* obj)
{
    //is randerable
    auto rable=obj->getComponent<Renderable>();
    if(rable && !rable->isVisible()) return;
    
    //draw randerable
    if(rable && rable->getMesh())
    {    
        ////////////////////////////////////////////////////////////////////////
        //CULLING (SLOW)
        //get box
        const AABox2& box=rable->getMesh()->getAABox();
        const AABox2& mbox=rable->canTransform() ? 
                           box.applay(obj->getGlobalMatrix()) : 
                           box;
        //get viewport box
        const AABox2& vpBox=camera->getBoxViewport();
        //applay camera matrix
        const AABox2& wbox=mbox.applay(camera->getGlobalMatrix());
        ////////////////////////////////////////////////////////////////////////
        //culling
        if(vpBox.isIntersection(wbox))
            queue.push(obj);
    }
    //childs
    for(auto child:*obj)
        append(child);
}

//add a element in queue
void Render::Queue::push(Object* obj)
{
    //obj values
    float obZ=obj->getZ(true);
    RenderState* objRS=obj->getComponent<Renderable>();
    //it values
    float itZ=0;
    RenderState* itRS=nullptr;
    //push object
    for(ItObjs it=objs.begin();
                it!=objs.end();
                ++it)
    {
        itZ=(*it)->getZ(true);
        //z sort
        if(itZ>obZ)
        {
            objs.insert(it,obj);
            return;
        }
        //like material sort
        else if(itZ==obZ)
        {
            itRS=(*it)->getComponent<Renderable>();
            if((*itRS)==(*objRS))
            {
                objs.insert(it,obj);
                return; 
            }
        }
    }
    //else
    objs.push_back(obj);
}

//called from scene
void Render::buildQueue(const std::list<Object*>& objs)
{
    //clear queue
    queue.clear();
    //add objcts
    for(auto obj:objs)
        append(obj);
}

void Render::draw()
{   
    if(!camera) return;
    //errors before draw
    CHECK_GPU_ERRORS();
	///////////////////////////////////////////////
    //create buffer
    if(!batchingMesh.getMaxSize())
        batchingMesh.createBufferByTriangles(MAX_BUFFER_TRIANGLES);
    //set view port
    RenderContext::setViewport(Vec4(0,0,camera->getViewport().x,camera->getViewport().y));
    //clear
    RenderContext::setColorClear(clearClr);
    if(enableClear)
        RenderContext::doClear();
    //safe state
    RenderContext::unbindVertexBuffer();
    RenderContext::unbindIndexBuffer();
    //ambient
    RenderContext::setAmbientColor(ambientClr);
    //set projection matrix
    RenderContext::setProjection(camera->getProjection());
    //return if queue is empty
    if(queue.begin()==queue.end()) return;
	///////////////////////////////////////////////
    //matrix camera
    RenderContext::setModelView(camera->getGlobalMatrix());
    //info data
    auto drawIt=queue.begin();
    auto drawNext=drawIt; ++drawNext;
    //draw queue
    while(drawIt!=queue.end())
    {
        //info temp
        Object*     oCurrent=*drawIt;
        Renderable* rCurrent=oCurrent->getComponent<Renderable>();

        Object*     oNext= drawNext!=queue.end() ? *drawNext : nullptr;
        Renderable* rNext= oNext ? oNext->getComponent<Renderable>() : nullptr;
        //Matrix
        const Mat4& otrasform=( rCurrent->canTransform() ? oCurrent->getGlobalMatrix() : Mat4::IDENTITY );
        //can add this mesh in the pool?
        if(rCurrent->doBatching())
        {
            //batching
            batchingMesh.addMesh(otrasform,
                                 rCurrent->getMesh());
            // draw?
            if(!rNext ||
               !rNext->doBatching() ||
               !rCurrent->canBatching(rNext)  || 
               !batchingMesh.canAdd(rNext->getMesh()))
            {
                //enable info draw
                rCurrent->enableStates();
                //draw
                batchingMesh.draw();
                //draw errors
                CHECK_GPU_ERRORS();
                //restart batching
                batchingMesh.relase();
            }
        }
        //direct draw
        else
        {
            //model view matrix
            RenderContext::setModelView(camera->getGlobalMatrix().mul2D(otrasform));
            //enable info draw
            rCurrent->draw();
            //draw errors
            CHECK_GPU_ERRORS();
            //matrix camera
            RenderContext::setModelView(camera->getGlobalMatrix());
        }
        //next
        drawIt=drawNext;
        if(drawNext!=queue.end())
            ++drawNext;
    }
}

//pikking
Object* Render::picking(const Vec2& point)
{
    //return if queue is empty
    if(queue.begin()==queue.end()) return nullptr;
    //seach
    for(Object* obj: Utility::reverse(queue) )
    {
        Renderable* renderable=nullptr;
        if((renderable=obj->getComponent<Renderable>()))
        {
            const AABox2& box=renderable->getMesh()->getAABox();
            const AABox2& wbox= renderable->canTransform() ? box.applay(obj->getGlobalMatrix()) : box;
            if(wbox.isIntersection(point))
            {
                return obj;
            }
        }
    }
    return nullptr;
}


void Render::aabox2Draw()
{   
    //return if queue is empty
    if(queue.begin()==queue.end()) return;
    //save states
    auto state=RenderContext::getRenderState();
    //set projection matrix
    RenderContext::setProjection(camera->getProjection());
    //set view port
    RenderContext::setViewport(Vec4(0,0,camera->getViewport().x,camera->getViewport().y));
    //set model view matrix
    RenderContext::setModelView(camera->getGlobalMatrix());
    //////////////////////////////////////////////////////////////////
    RenderContext::setCullFace(DISABLE);
    RenderContext::setBlend(true);
    RenderContext::setBlendFunction(BLEND::SRC::ALPHA, BLEND::ONE::MINUS::SRC::ALPHA);
    RenderContext::setTexture(false);
    RenderContext::setClientState(true, false, false, false);
    //////////////////////////////////////////////////////////////////
    //no vbo/ibo
    RenderContext::unbindVertexBuffer();
    RenderContext::unbindIndexBuffer();
    //////////////////////////////////////////////////////////////////
    //all aabbox
    for(Object* obj: Utility::reverse(queue) )
    {
        Renderable* renderable=nullptr;
        if((renderable=obj->getComponent<Renderable>()))
        {
            const AABox2& box=renderable->getMesh()->getAABox();
            const AABox2& wbox= renderable->canTransform() ? box.applay(obj->getGlobalMatrix()) : box;
            
            RenderContext::drawBox(wbox,
                                   Color((uchar(wbox.getSize().x)),
                                         (uchar(wbox.getSize().y)),
                                         (uchar(wbox.getSize().x+wbox.getSize().y)),
                                         128));
            
            RenderContext::drawFillBox(wbox,Color(25,128,(uchar(wbox.getSize().x+wbox.getSize().y)),40));
        }
    }
    //////////////////////////////////////////////////////////////////
    RenderContext::setRenderState(state);
    //////////////////////////////////////////////////////////////////
}
