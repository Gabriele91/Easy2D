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
//post effects
PostEffects::PostEffects()
{
    //set size mesh
    const Vec2  size(1.0f,1.0f);
    //const Vec4  offsetUV(0,0,1.0,1.0);
    //add vertexs
    plane.addVertex(-size.x,
                    -size.y,
                     0.0,
                     0.0);
    plane.addVertex( size.x,
                    -size.y,
                     1.0,
                     0.0);
    plane.addVertex(-size.x,
                     size.y,
                     0.0,
                     1.0);
    plane.addVertex( size.x,
                     size.y,
                     1.0,
                     1.0);
    //end add vertexs
    //set draw mode
    plane.setDrawMode(TRIANGLE_STRIP);
    //build mesh
    plane.build();
}
PostEffects::~PostEffects()
{
	/* void */
}
//draw effects
void PostEffects::draw(const RenderContext::RenderTarget& target)
{
    //set buffer to 0
    RenderContext::setColorClear(Color(0,0,0,255));
    RenderContext::doClear();
    //draw texture
    RenderContext::setCullFace(BACK);
    RenderContext::setColor(Color::WHITE);
    //identity
    RenderContext::setProjection(Mat4::IDENTITY);
    RenderContext::setView(Mat4::IDENTITY);
    RenderContext::setModel(Mat4::IDENTITY);
    //get screen size
    Vec2 screenSize=Application::instance()->getScreen()->getSize();
    RenderContext::setViewport(Vec4(0,0,screenSize.x,screenSize.y));
    //enable texture
    RenderContext::setTexture(true);
    RenderContext::bindTexture(target.color);
    //no rotation display
    Mat4 oretationMatrix=RenderContext::getDisplay();
    RenderContext::setDisplay(Mat4::IDENTITY);
    //draw
    for(auto effect : effects)
    {
        RenderContext::setBlend(effect.blend);
        RenderContext::setBlendFunction(effect.bsrc, effect.bdst);
        effect.shader->bind();
        plane.draw();
    }
    //reset oretation matrix
    RenderContext::setDisplay(oretationMatrix);
    //disable shader
    RenderContext::disableProgram();
    //unbind texture
    RenderContext::unbindTexture();
}
//render
Render::Render()
{
    camera=nullptr;
    enableBatching=true;
    enableClear=false;
    effects=nullptr;
}
//return batching mesh
Mesh::ptr Render::getBatchingMesh()
{
	return batchingMesh;
}
//init render graphics elements
void Render::init()
{
	/////////////////////////////////////////////////////////////////////
	//QUEUE 
	queue = RenderQueue::snew();
	/////////////////////////////////////////////////////////////////////
	//BATCH BUFFER
	auto mesh=new BatchingMesh();
	mesh->createBufferByTriangles(MAX_BUFFER_TRIANGLES);
	batchingMesh = Mesh::ptr((Mesh*)mesh);
	/////////////////////////////////////////////////////////////////////
	//POST EFFECT
	if (!effects)  effects = PostEffects::snew();

}
//called from scene
void Render::buildQueue(const std::list<Object*>& objs)
{
    if(!camera) return;
    //display/view camera
    const Mat4& disViewM4 = RenderContext::getDisplay().mul(camera->getGlobalMatrix());
    const AABox2& viewBox = camera->getBoxViewport();
    //clear queue
	queue->clear();
    //add objcts
    for(auto obj:objs)
        queue->append([&](const AABox2& mbox) -> bool
        {
            const AABox2& wbox = mbox.applay(disViewM4);
            return viewBox.isIntersection(wbox);
        }, obj);
}
//draw scene
void Render::draw()
{   
    if(!camera) return;
    //errors before draw
    CHECK_GPU_ERRORS();
	/////////////////////////////////////////////////////////////////////
    //set view port
    RenderContext::setViewport(Vec4(0, 0, camera->getViewport()));
    //set projection matrix
    RenderContext::setProjection(camera->getProjection());
    //matrix camera
    RenderContext::setView(camera->getGlobalMatrix());
    //safe state
    RenderContext::unbindVertexBuffer();
    RenderContext::unbindIndexBuffer();
    //ambient
    RenderContext::setAmbientColor(ambientClr);    
    /////////////////////////////////////////////////////////////////////
    //has some shaders?
	if (effects->hasPostEffects())
	{
		RenderContext::enableRenderTarget(queue->getTarget());
		//clear
		RenderContext::setColorClear(clearClr);
		if (enableClear) RenderContext::doClear();
		//draw queue
        if(getBatchingIsEnable())
            queue->draw(batchingMesh);
        else
            queue->draw();
		//end draw
		RenderContext::disableRenderTarget();
		//draw effects
        effects->draw(queue->getTarget());
    }
	else
	{		
		//clear
		RenderContext::setColorClear(clearClr);
		if (enableClear) RenderContext::doClear();
		//draw queue
        if (getBatchingIsEnable())
            queue->draw(batchingMesh);
        else
            queue->draw();
	}
    //////////////////////////////////////////////////////
}

//pikking
Object* Render::queuePicking(const Vec2& point) const
{
    //return if queue is empty
	if (!queue->size()) return nullptr;
    //seach
    for(Object* obj: Utility::reverse(*queue) )
    {
        Renderable* renderable=nullptr;
        if((renderable=obj->getComponent<Renderable>()))
        {
            const AABox2& box = renderable->getBaseBox();
            const AABox2& wbox= renderable->canTransform() ? box.applay(obj->getGlobalMatrix()) : box;
            if(wbox.isIntersection(point))
            {
                return obj;
            }
        }
    }
    return nullptr;
}
//debug draw
void Render::aabox2Draw() const
{   
    //return if queue is empty
	if (!queue->size())  return;
    //save states
    auto state=RenderContext::getRenderState();
    //set projection matrix
    RenderContext::setProjection(camera->getProjection());
    //set view port
    RenderContext::setViewport(Vec4(0,0,camera->getViewport().x,camera->getViewport().y));
    //set model view matrix
    RenderContext::setView(camera->getGlobalMatrix());
    RenderContext::setModel(Mat4::IDENTITY);
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
    for(Object* obj: Utility::reverse(*queue) )
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
