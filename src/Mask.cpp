#include <stdafx.h>
#include <Mask.h>
#include <Scene.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
/*
    MaskBox
*/
MaskBox::MaskBox()
{
    //no batch
    setCanBatch(false);
    //queue
    queue = RenderQueue::snew();
}
MaskBox::MaskBox(const OBBox2& box):box(box)
{
    //no batch
    setCanBatch(false);
    //queue
    queue = RenderQueue::snew();
}
MaskBox::~MaskBox()
{
}

AABox2 MaskBox::getBaseBox()
{
    return box.toAABox2();
}
void MaskBox::onSetObject(Object* obj)
{
    //save state
    objCanDrawChilds = obj->getCanDrawChilds();
    //set new state
    obj->setCanDrawChilds(false);
}
void MaskBox::onEraseObject()
{
    //set old state
    getObject()->setCanDrawChilds(objCanDrawChilds);
}
void MaskBox::draw()
{    
    //draw childs
    Scene*  scene  = getScene();
    Object* object = getObject();
    //
    if (object && scene && isVisible())
    {
        Render* render = dynamic_cast<Render*>(scene);
        Camera* camera = render->getCamera();
        //display/view camera
        const Mat4& disViewM4 = RenderContext::getDisplay().mul(camera->getGlobalMatrix());
        const AABox2& viewBox = camera->getBoxViewport();
        /////////////////////////////////////////////////////////////////////////////////////
        //new box pos
        AABox2 modelBox = Renderable::getBox().applay(disViewM4);
        //culling
        if (!modelBox.isIntersection(viewBox)) return;
        /////////////////////////////////////////////////////////////////////////////////////
        //build queue
        queue->clear();
        //add all childs
        for (auto child : *object)
            queue->append([&](const AABox2& mbox) -> bool
            {
                const AABox2& wbox = mbox.applay(disViewM4);
                return modelBox.isIntersection(wbox);
            },child);
        //draw filter
        RenderContext::setStencil(STENCIL_REPLACE);
        RenderContext::stencilClear();
        RenderContext::setColorWritable(false, false, false, false);
        RenderContext::setZBufferWritable(false);
        RenderContext::drawFillBox(box, Color::BLACK);
        //set filter
        RenderContext::setStencil(STENCIL_KEEP);
        RenderContext::setColorWritable(true, true, true, true);
        RenderContext::setZBufferWritable(true);
        //draw queue
        if (render->getBatchingIsEnable())
            queue->draw(render->getBatchingMesh());
        else
            queue->draw();
        //disable filter
        RenderContext::setStencil(STENCIL_NONE);
    }
}

void MaskBox::serialize(Table& table)
{
    table.set("center", box.getCenter());
    table.set("size", box.getSize());
    table.set("angle", box.getAngle().valueDegrees());
}
void MaskBox::deserialize(const Table& table)
{
    box.set(table.getVector2D("center", box.getCenter()),
            table.getVector2D("size", box.getSize()),
            Angle::degree(table.getFloat("angle", box.getAngle().valueDegrees())));
}
///////////////////////
/*
    MaskSprite
*/
MaskSprite::MaskSprite(Texture::ptr texture, float alpha) 
:Sprite(texture)
,alphaMask(alpha)
{
    //no batch
    setCanBatch(false);
    //queue
    queue = RenderQueue::snew();
    //get shader
    Shader::ptr shader=RenderContext::getShader("MaskShader");
    if (!shader)
    {
        shader=Shader::snew();
        shader->load("vec4 vertex(){ return ftransform(); }",
                     "uniform float e2dAlphaMask;                          \n"
                     "vec4 fragment(Texture tex,vec4 coord)                \n"
                     "{                                                    \n"
                     "   if (texture2D(tex, coord.xy).w <= e2dAlphaMask)   \n"
                     "   { discard; }                                      \n"
                     "   return vec4(1.0);                                 \n"
                     "}                                                    \n" );
        RenderContext::subscriptionShader("MaskShader", shader);
    }
    setShader(shader);
    uAlphaMask=shader->uniformID("e2dAlphaMask");
}
MaskSprite::~MaskSprite()
{
}

void MaskSprite::setAlphaMask(float val)
{
    alphaMask = val;
}
float MaskSprite::getAlphaMask()
{
    return alphaMask;
}
void MaskSprite::onSetObject(Object* obj)
{
    //save state
    objCanDrawChilds = obj->getCanDrawChilds();
    //set new state
    obj->setCanDrawChilds(false);
}
void MaskSprite::onEraseObject()
{
    //set old state
    getObject()->setCanDrawChilds(objCanDrawChilds);
}
void MaskSprite::draw()
{
    //draw childs
    Scene*  scene = getScene();
    Object* object = getObject();
    //
    if (object && scene && isVisible())
    {
        Render* render = dynamic_cast<Render*>(scene);
        Camera* camera = render->getCamera();
        //display/view camera
        const Mat4& disViewM4 = RenderContext::getDisplay().mul(camera->getGlobalMatrix());
        const AABox2& viewBox = camera->getBoxViewport();
        /////////////////////////////////////////////////////////////////////////////////////
        //new box pos
        AABox2 modelBox = getBox().applay(disViewM4);
        //culling
        if (!modelBox.isIntersection(viewBox)) return;
        /////////////////////////////////////////////////////////////////////////////////////
        //build queue
        queue->clear();
        //add all childs
        for (auto child : *object)
        queue->append([&](const AABox2& mbox) -> bool
        {
            const AABox2& wbox = mbox.applay(disViewM4);
            return modelBox.isIntersection(wbox);
        }, child);
        //draw filter
        RenderContext::setStencil(STENCIL_REPLACE);
        RenderContext::stencilClear();
        RenderContext::setColorWritable(false, false, false, false);
        RenderContext::setZBufferWritable(false);
        ///////////////////////////////////
        //enable state settings
        enableStates();
        //set uniform
        getShader()->uniform(uAlphaMask,alphaMask);
        //draw mesh
        if (getMesh()) getMesh()->draw();
        ///////////////////////////////////
        //set filter
        RenderContext::setStencil(STENCIL_KEEP);
        RenderContext::setColorWritable(true, true, true, true);
        RenderContext::setZBufferWritable(true);
        //draw queue
        if (render->getBatchingIsEnable())
            queue->draw(render->getBatchingMesh());
        else
            queue->draw();
        //disable filter
        RenderContext::setStencil(STENCIL_NONE);
    }
}

void MaskSprite::serialize(Table& table)
{
    Sprite::serialize(table);
    table.set("alphaMask", getAlphaMask());
}
void MaskSprite::deserialize(const Table& table)
{
    Sprite::deserialize(table);
    setAlphaMask( table.getFloat("alphaMask", getAlphaMask()) );
}