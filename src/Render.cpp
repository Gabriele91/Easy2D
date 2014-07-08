#include <stdafx.h>
#include <Debug.h>
#include <Screen.h>
#include <Application.h>
#include <Render.h>
#include <Object.h>
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
    if(rable)
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
	///////////////////////////////////////////////
    //create buffer
    if(!batchingMesh.getMaxSize())
        batchingMesh.createBufferByTriangles(MAX_BUFFER_TRIANGLES);
    //set view port
    glViewport(0, 
               0, 
               (GLsizei)camera->getViewport().x, 
               (GLsizei)camera->getViewport().y );
    //clear
    glClearColor(clearClr.rNormalize(),
                 clearClr.gNormalize(),
                 clearClr.bNormalize(),
                 clearClr.aNormalize());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera->getProjection());
    //set model view matrix
    glMatrixMode(GL_MODELVIEW);   
    //return if queue is empty
    if(queue.begin()==queue.end()) return;
	///////////////////////////////////////////////
    //matrix camera
    glLoadMatrixf(camera->getGlobalMatrix());
    //info data
    auto drawIt=queue.begin();
    auto drawNext=drawIt; ++drawNext;
    Object* lastDraw=nullptr;
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
               !rNext->doBatching() || //n.b. zbuffer...
               !rCurrent->canBatching(rNext)  || 
               !batchingMesh.canAdd(rNext->getMesh()))
            {
                //enable info draw
                if(lastDraw)
                    rCurrent->enableStates(lastDraw->getComponent<Renderable>());
                else
                    rCurrent->enableStates();
                //draw
                batchingMesh.draw();
                //save last draw
                lastDraw=oCurrent;
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
            glLoadMatrixf(camera->getGlobalMatrix().mul2D(otrasform));
            //enable info draw
            if(lastDraw)
                rCurrent->enableStates(lastDraw->getComponent<Renderable>());
            else
                rCurrent->enableStates();
            //draw
            rCurrent->draw();
            //save last draw
            lastDraw=oCurrent;
            //draw errors
            CHECK_GPU_ERRORS();
            //matrix camera
            glLoadMatrixf(camera->getGlobalMatrix());
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
        if(renderable=obj->getComponent<Renderable>())
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


inline void drawAABox2(const AABox2& aabox2, const Color& color) 
{

    glColor4ub(color.r, color.g, color.b, color.a);
    const float vertices[]=
    {
        aabox2.getMin().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMax().y,
        aabox2.getMin().x,aabox2.getMax().y,
    };
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

}
inline void drawFillAABox2(const AABox2& aabox2, const Color& color) 
{

    glColor4ub(color.r, color.g, color.b, color.a);
    const float vertices[]=
    {
        aabox2.getMin().x,aabox2.getMin().y,
        aabox2.getMax().x,aabox2.getMin().y,
        aabox2.getMin().x,aabox2.getMax().y,
        aabox2.getMax().x,aabox2.getMax().y,
    };
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Render::aabox2Draw()
{   
    //return if queue is empty
    if(queue.begin()==queue.end()) return;
    //set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera->getProjection());
    //set view port
    glViewport(0, 
               0, 
               (GLsizei)camera->getViewport().x, 
               (GLsizei)camera->getViewport().y );
    //set model view matrix
    glMatrixMode(GL_MODELVIEW);   
    glLoadMatrixf(camera->getGlobalMatrix());
    //////////////////////////////////////////////////////////////////
    GLboolean cull,blend;
    GLint bs_src, bs_dst;
    glGetBooleanv(GL_CULL_FACE,&cull);
    glGetBooleanv(GL_BLEND , &blend);
    glGetIntegerv(GL_BLEND_SRC , &bs_src);
    glGetIntegerv(GL_BLEND_DST , &bs_dst);    
    //change param
    glDisable(GL_CULL_FACE);
    //blend
    if(!blend) glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //color
    GLfloat saveGLColor4f[4];
    glGetFloatv(GL_CURRENT_COLOR, saveGLColor4f);
    //////////////////////////////////////////////////////////////////
    //no vbo/ibo
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  0 );
    //no texture
    glDisable( GL_TEXTURE_2D );
    //////////////////////////////////////////////////////////////////
    //all aabbox
    for(Object* obj: Utility::reverse(queue) )
    {
        Renderable* renderable=nullptr;
        if(renderable=obj->getComponent<Renderable>())
        {
            const AABox2& box=renderable->getMesh()->getAABox();
            const AABox2& wbox= renderable->canTransform() ? box.applay(obj->getGlobalMatrix()) : box;
            
            drawAABox2(wbox,Color((uchar(wbox.getSize().x)),
                                  (uchar(wbox.getSize().y)),
                                  (uchar(wbox.getSize().x+wbox.getSize().y)),
                                  128));
            drawFillAABox2(wbox,Color(25,128,(uchar(wbox.getSize().x+wbox.getSize().y)),40));
        }
    }
    //////////////////////////////////////////////////////////////////
    if(cull)
        glEnable( GL_CULL_FACE );
    //blend
    if(!blend)
        glDisable( GL_BLEND );
    else
        glBlendFunc(bs_src,bs_dst);
    //color
    glColor4fv(saveGLColor4f);
    //////////////////////////////////////////////////////////////////
}
