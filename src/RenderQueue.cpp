#include <stdafx.h>
#include <RenderQueue.h>
#include <Render.h>
#include <Application.h>
#include <Screen.h>
/////////////////////////
using namespace Easy2D;
/////////////////////////

RenderQueue::RenderQueue(Render* render)
:render(render)
{
	DEBUG_ASSERT(render);
	//get screen size
	Vec2 screenSize = Application::instance()->getScreen()->getSize();
	//create render target buffer
	target.color = RenderContext::createTexture(GL_RGBA, GL_RGBA, screenSize.x, screenSize.y, 0, false);
	RenderContext::filterTexture(GL_LINEAR, GL_LINEAR);
	RenderContext::wrapTexture(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	//create render target
	target = RenderContext::createRenderTarget(target.color);
}
RenderQueue::~RenderQueue()
{
	RenderContext::deleteRenderTarget(target);
	RenderContext::deleteTexture(target.color);
}
//append objects to queue
void RenderQueue::append(Object* obj)
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
        const AABox2& mbox=rable->canTransform() ? box.applay(obj->getGlobalMatrix()) : box;
        //applay camera matrix to obj box
        const AABox2& wbox=mbox.applay(render->getCamera()->getGlobalMatrix());
        ////////////////////////////////////////////////////////////////////////
        //culling
		if (render->getCamera()->getBoxViewport().isIntersection(wbox))
			push(obj);
    }
    //childs
    for(auto child:*obj) append(child);
}
//add a element in queue
void RenderQueue::push(Object* obj)
{
	//obj values
	float obZ = obj->getZ(true);
	RenderState* objRS = obj->getComponent<Renderable>();
	//it values
	float itZ = 0;
	RenderState* itRS = nullptr;
	//push object
	for (ItObjs it = objs.begin();
		it != objs.end();
		++it)
	{
		itZ = (*it)->getZ(true);
		//z sort
		if (itZ>obZ)
		{
			objs.insert(it, obj);
			return;
		}
		//like material sort
		else if (itZ == obZ)
		{
			itRS = (*it)->getComponent<Renderable>();
			if ((*itRS) == (*objRS))
			{
				objs.insert(it, obj);
				return;
			}
		}
	}
	//else
	objs.push_back(obj);
}
//draw queue
void RenderQueue::draw(Render* render)
{
	//return if queue is empty
	if (begin() == end()) return;
	//////////////////////////////////////////////////////
	//refs
	BatchingMesh& batchingMesh = render->getBatchingMesh();
    Camera*       camera = render->getCamera();
    //restart batching
    batchingMesh.restart();
	//////////////////////////////////////////////////////
	//init matrix
	//////////////////////////////////////////////////////
	//set projection matrix
	RenderContext::setProjection(camera->getProjection());
	//////////////////////////////////////////////////////
	//matrix camera
	RenderContext::setModelView(camera->getGlobalMatrix());
	//////////////////////////////////////////////////////
	//info data
	auto drawIt = begin();
	auto drawNext = drawIt;
	++drawNext;
	//draw queue
	while (drawIt != end())
	{
		//info temp
		Object*     oCurrent = *drawIt;
		Renderable* rCurrent = oCurrent->getComponent<Renderable>();

		Object*     oNext = drawNext != end() ? *drawNext : nullptr;
		Renderable* rNext = oNext ? oNext->getComponent<Renderable>() : nullptr;
		//Matrix
		const Mat4& otrasform = (rCurrent->canTransform() ? oCurrent->getGlobalMatrix() : Mat4::IDENTITY);
		//can add this mesh in the pool?
		if ( false && render->getBatchingIsEnable() && rCurrent->doBatching() )
		{
			//batching
			batchingMesh.addMesh(otrasform,
				rCurrent->getMesh());
			// draw?
			if (!rNext ||
				!rNext->doBatching() ||
				!rCurrent->canBatching(rNext) ||
				!batchingMesh.canAdd(rNext->getMesh()))
			{
				//enable info draw
				rCurrent->enableStates();
				//draw
				batchingMesh.draw();
				//draw errors
				CHECK_GPU_ERRORS();
				//restart batching
				batchingMesh.restart();
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
		drawIt = drawNext;
		if (drawNext != end())
			++drawNext;
	}
}