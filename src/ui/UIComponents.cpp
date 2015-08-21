//
//  UIComponents.cpp
//  ComponentGui
//
//  Created by Gabriele Di Bari on 18/05/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include <stdafx.h>
#include <RenderContext.h>
#include <ResourcesGroup.h>
#include <Object.h>
#include <Scene.h>
#include <UIComponents.h>

using namespace Easy2D;
using namespace Easy2D::Ui;

/**
* ANCHOR
*/
Mat4 Anchor::calcAnchorGMatrix(Object* obj)
{
    if (obj && anchor && obj->getParent() &&
        //parent renderable
        obj->getParent()->getComponent<Renderable>())
    {
        Object*     parent = obj->getParent();
        Renderable* renderable = parent->getComponent<Renderable>();
        //obb parent
        Mat4   pmat  = renderable->getModel();
        OBBox2 pbox  = renderable->getBaseBox();
        Vec2   psize = pbox.getSize();
        //anchor pos
        Vec2 pos     = pmat.mul(Vec4(psize*anchorPoint, 0, 0)).xy();
        //pivot
        Mat4 pmodel(pmat);
        pmodel.addScale(1.0f / parent->getScale(true));
        Vec2 pivot = pmodel.mul2D(obj->getPosition());
        //set tranform
        Mat4 omat;
        omat.setTransform2D({
            pivot + pos, 
            obj->getRotation() + pmodel.getRotZ(),
            obj->getScale()
        });
        return omat;
    }
    else if (obj && anchor && !obj->getParent() &&
             //scene camera
             obj->getScene()  && obj->getScene()->getCamera())
    {
        //camera
        Camera* camera=obj->getScene()->getCamera();
        //obb parent
        OBBox2 pbox  = camera->getBoxViewport();
        Vec2   psize = pbox.getSize();
        //anchor pos
        Vec2   pos = psize*anchorPoint;
        //pivot
        Vec2 pivot = obj->getPosition();
        //set tranform
        Mat4 omat;
        omat.setTransform2D({
            pivot + pos,
            obj->getRotation(),
            obj->getScale()
        });
        return omat;
    }
    return obj ? obj->getGlobalMatrix() : Mat4::IDENTITY;
}
void Anchor::enableAnchor()
{
    anchor = true;
}
void Anchor::disableAnchor()
{
    anchor = false;
}
bool Anchor::isEnableAnchor() const
{
    return anchor;
}
void Anchor::setAnchorPoint(const Vec2& value)
{
    anchorPoint = value;
}
Vec2 Anchor::getAnchorPoint() const
{
    return anchorPoint;
}
void Anchor::serialize(Table& table)
{
    //anchor
    table.set("anchor", isEnableAnchor() ? "yes" : "no");
    //anchor point
    if (getAnchorPoint()!=Vec2::ZERO)
        table.set("anchorPoint", getAnchorPoint());
}
void Anchor::deserialize(const Table& table)
{
    //anchor
    if (table.existsAsType("anchor", Table::STRING))
    {
        if (table.getString("anchor", isEnableAnchor() ? "yes" : "no") != "no") 
            enableAnchor();
        else
            disableAnchor();
    }
    //anchor point
    setAnchorPoint(table.getVector2D("anchorPoint", getAnchorPoint()));
}
/**
* TEXT
*/
//constructor
Text::Text(Font::ptr font):font(font)
{
    //no batch
    setCanBatch(false);
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,  BLEND::ONE::MINUS::SRC::ALPHA);
}
Text::Text(Font::ptr font,const String& text):Text(font)
{
    setText(text);
}
//distructor
Text::~Text()
{
}
//box of text
AABox2 Text::getBaseBox()
{
    return box;
}
//get model matrix
Mat4 Text::getModel()
{
    return calcAnchorGMatrix(getObject());
}
//set font
void Text::setFont(Font::ptr argfont)
{
    font=argfont;
    setText(text);
}
void Text::setText(const String& argtext)
{
    DEBUG_ASSERT(font);
    text     =argtext;
    nodes    =font->genStaticText(text);
    //cmpute box
    box=AABox2();
    for(auto& t : nodes)  box.addBox(t.mesh->getAABox());
}
void Text::set(Font::ptr argfont,const String& argtext)
{
    font=argfont;
    text=argtext;
    nodes=font->genStaticText(text);
    //cmpute box
    box=AABox2();
    for(auto& t : nodes)  box.addBox(t.mesh->getAABox());
}
//protected set mesh
void Text::setMesh(Mesh::ptr mesh)
{
    Renderable::setMesh(mesh);
}
void Text::setTexture(Texture::ptr tex)
{
    Renderable::setTexture(tex);
}
//draw
void Text::textDraw()
{
    //draw
    for(auto& t :nodes)
    {
        t.texture->bind();
        t.mesh->draw();
    }
}
void Text::textDrawClip(const AABox2& box)
{
    //draw
    for(auto& t :nodes)
    {
        if(t.mesh->getAABox().isInside(box))
        {
            t.texture->bind();
            t.mesh->draw();
        }
    }
}
void Text::textDrawClip(const Mat4& local,const AABox2& box)
{
    //draw
    for(auto& t :nodes)
    {
        if(t.mesh->getAABox().applay(local).isInside(box))
        {
            t.texture->bind();
            t.mesh->draw();
        }
    }
}
void Text::draw()
{
    //enable state
    RenderState::enableStates();
    //draw text
    textDraw();
}
//serialize/deserialize
void Text::serialize(Table& table)
{    
    //serialize rander state
    rsSerialize(table);
    //serialize anchor
    Anchor::serialize(table);
    //font
    if (font)
    {
        table.set("font", font->getName());
        table.set("text", text);
    }
}
void Text::deserialize(const Table& table)
{    
    //deserialize rander state
    rsDeserialize(table);
    //deserialize anchor
    Anchor::deserialize(table);
    //get font
    if (table.existsAsType("font", Table::STRING))
    {
        auto rsmanager = table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup = rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setFont(rsgroup->load<Font>(table.getString("font")));
    }
    //get text
    setText(table.getString("text",text));
}

/**
*  LABEL
*/
//protected set mesh
void Label::setMesh(Mesh::ptr mesh)
{
    Renderable::setMesh(mesh);
}
//costructor
Label::Label(Texture::ptr texture,
             Font::ptr    font)
:textColor(Color::WHITE)
{
    if (texture) setTexture(texture);
    if (font)    setFont(font);
}
Label::Label(Texture::ptr  texture,
             Font::ptr     font,
             const String& text)
            :Label(texture, font)
        {
            setText(text);
        }
//text color to draw
Color Label::getDisplayedTextColor() const
{
    //compute color
    Color endColor(textColor);
    endColor.a = (uchar)(textColor.aNormalize() * getColor().aNormalize() * 255.0f);
    return endColor;
}
//draw
void Label::draw()
{
    //uniform
    if (scale9) getShader()->setUniformCallback([this](Shader& shader)
    {
        Vec2 scale(Vec2::ONE);
        if (getObject()) scale = getObject()->getScale(true);

        shader.uniform(uniform.inBox, boxScale9);
        shader.uniform(uniform.sizeTex, getTexture()->getSize());
        shader.uniform(uniform.scaleSprite, scale);
    });
    //draw backgound
    Renderable::draw();
    //disable shader
    RenderContext::disableProgram();    
    //if text exist
    if(text.length())
    {
        //if scale indipedent
        AABox2 box = getBaseBox();
        //set text model
        RenderContext::setModel(RenderContext::getModel().mul2D(model));
        //text color
        RenderContext::setColor(getDisplayedTextColor());
        //draw text
        textDrawClip(model, box);
    }
}
void Label::setBoxScale9(const Vec4& box)
{
    boxScale9 = box;
}
void Label::enableScale9()
{
    if (scale9) return;
    scale9=true;
    //shader
    Shader::ptr shader = RenderContext::getShader("Scale9Shader");
    if (!shader)
    {
#define TOSTRING(x) #x
        shader = Shader::snew();
        shader->load("vec4 vertex(){ return ftransform(); }",
            TOSTRING
            (
            uniform vec2 sizeTex;
            uniform vec2 scaleSprite;
            uniform vec4 inBox;

            float xScale9(float posx,float sizex)
            {
                float leftSize   = inBox.x;
                float left       = inBox.x;
                float leftBase   = 0.0;
                float leftFactor = leftSize / sizeTex.x;

                float rightSize   = (sizeTex.x - (inBox.y + inBox.x));
                float right       = sizex - rightSize;
                float rightBase   = (sizeTex.x - rightSize) / sizeTex.x;
                float rightFactor = rightSize / sizeTex.x;

                float centerSize   = right - leftSize;
                float centerBase   = leftBase + leftFactor;
                float centerFactor = inBox.y / sizeTex.x;

                if (posx <= left)
                    return leftBase + leftFactor   * (posx / leftSize);
                else if (posx >= right)
                    return rightBase + rightFactor  * ((posx - right) / rightSize);
                else
                    return centerBase + centerFactor * ((posx - left) / centerSize);
            }

            float yScale9(float posy, float sizey)
            {
                float topSize   = inBox.z;
                float top       = inBox.z;
                float topBase   = 0.0;
                float topFactor = topSize / sizeTex.y;

                float bottomSize   = (sizeTex.y - (inBox.z + inBox.w));
                float bottom       = sizey - bottomSize;
                float bottomBase   = (sizeTex.x - bottomSize) / sizeTex.y;
                float bottomFactor = bottomSize / sizeTex.y;

                float centerSize   = bottom - topSize;
                float centerBase   = topBase + topFactor;
                float centerFactor = inBox.w / sizeTex.x;

                if (posy <= top)
                    return topBase + topFactor   * (posy / topSize);
                else if (posy >= bottom)
                    return bottomBase + bottomFactor  * ((posy - bottom) / bottomSize);
                else
                    return centerBase + centerFactor * ((posy - top) / centerSize);
            }

            vec4 fragment(Texture tex, vec4 coord)
            {
                vec2 sizeSprite  = scaleSprite * sizeTex;
                vec2 pixel       = coord.xy * sizeSprite;
                vec2 uv = vec2(xScale9(pixel.x, sizeSprite.x), 
                               yScale9(pixel.y, sizeSprite.y));
                return texture2D(tex, uv) * e2dColor ;
            }
        ));
#undef TOSTRING
        //box : left, right, top, bottom
    }
    setShader(shader);
    uniform.inBox = shader->uniformID("inBox");
    uniform.sizeTex = shader->uniformID("sizeTex");
    uniform.scaleSprite = shader->uniformID("scaleSprite");
}
void Label::disableScale9()
{
    if (!scale9) return;
    scale9 = false;
    setShader(nullptr);
}
void Label::setTextScaleIndependent(bool value)
{
    if (scaleIndependent == value) return;
    scaleIndependent=value;
    calcMat4Text();
}
void Label::calcMat4Text()
{
    //set scale
    if (getObject() && scaleIndependent)
        model.setScale(scale / getObject()->getScale(true));
    else
        model.setScale(scale);
    //translation
    calcTranslationText();
}
void Label::calcTranslationText()
{
    model[12] = 0;
    model[13] = 0;
    //compute translation
    Vec2 pos;
    const AABox2& bbox = getBaseBox();
    const AABox2& tbox = Text::box.applay(model);
    switch (hlayout)
    {
        case HL_TOP:    pos.y = -bbox.getMin().y;                  break;
        case HL_CENTER: pos.y = -tbox.getCenter().y;               break;
        case HL_BOTTOM: pos.y = -bbox.getMax().y - tbox.getMin().y; break;
        default: break;
    };
    switch (vlayout)
    {
        case VL_LEFT:    pos.x = bbox.getMin().x;                  break;
        case VL_CENTER:  pos.x = -tbox.getCenter().x;               break;
        case VL_RIGHT:   pos.x = bbox.getMax().x - tbox.getMax().x;  break;
        default: break;
    };
    model[12] = pos.x;
    model[13] = pos.y;
}
void Label::calcHTranslationText()
{
    Vec2 pos(model[12], model[13]);
    model[12] = 0;
    model[13] = 0;
    //compute translation
    const AABox2& bbox = getBaseBox();
    const AABox2& tbox = Text::box.applay(model);
    switch (hlayout)
    {
        case HL_TOP:    pos.y = -bbox.getMin().y;                  break;
        case HL_CENTER: pos.y = -tbox.getCenter().y;               break;
        case HL_BOTTOM: pos.y = -bbox.getMax().y - tbox.getMin().y; break;
        default: break;
    };
    model[12] = pos.x;
    model[13] = pos.y;
}
void Label::calcVTranslationText()
{
    Vec2 pos(model[12], model[13]);
    model[12] = 0;
    model[13] = 0;
    //compute translation
    const AABox2& bbox = getBaseBox();
    const AABox2& tbox = Text::box.applay(model);
    switch (vlayout)
    {
        case VL_LEFT:    pos.x = bbox.getMin().x;                  break;
        case VL_CENTER:  pos.x = -tbox.getCenter().x;               break;
        case VL_RIGHT:   pos.x = bbox.getMax().x - tbox.getMax().x;  break;
        default: break;
    };
    model[12] = pos.x;
    model[13] = pos.y;
}
//set
void Label::setFont(Font::ptr font)
{
    Text::setFont(font);
    calcMat4Text();
}
void Label::setText(const String& text)
{
    Text::setText(text);
    calcMat4Text();
}
void Label::setTextColor(const Color& color)
{
    textColor = color;
}
void Label::set(Font::ptr argfont,const String& argtext)
{
    Text::set(argfont,argtext);
    calcMat4Text();
}
void Label::setTextScale(const Vec2& argscale)
{
    scale=argscale;
    calcMat4Text();
}
void Label::setLayout(HLayout h,VLayout v)
{
    hlayout=h;
    vlayout=v;
    calcMat4Text();
}
//get
HLayout Label::getHLayout() const
{
    return hlayout;
}
VLayout Label::getVLayout() const
{
    return vlayout;
}
Color Label::getTextColor() const
{
    return textColor;
}
Vec2 Label::getTextScale() const
{
    return scale;
}
bool Label::isEnableScale9() const
{
    return scale9;
}
bool Label::isTextScaleIndependent() const
{
    return scaleIndependent;
}
//event
void Label::onChangedMatrix()
{
    if (scaleIndependent) calcMat4Text();
}
//box of texture
AABox2  Label::getBaseBox()
{
    return Renderable::getBaseBox();
}
//set texture
void Label::setTexture(Texture::ptr tex)
{
    Renderable::setTexture(tex);
    if(tex)
    {
        Renderable::setMesh(tex->getPO2Sprite());
        calcMat4Text();
    }
    else
        Renderable::setMesh(nullptr);
}
//serialize / deserialize
static const char * const STRHLayout[] =
{
    "HL_TOP",
    "HL_CENTER",
    "HL_BOTTOM"
};
static const char * const STRHLayoutSugar[] =
{
    "TOP",
    "CENTER",
    "BOTTOM"
};
static const char * const STRVLayout[] =
{
    "VL_LEFT",
    "VL_CENTER",
    "VL_RIGHT"
};
static const char * const STRVLayoutSugar[] =
{
    "LEFT",
    "CENTER",
    "RIGHT"
};
void Label::serializeFields(Table& table) const
{
    //set
    table.set("enableScale9", isEnableScale9() ? "yes" : "no");
    table.set("boxScale9", boxScale9);
    table.set("scaleIndependent", isTextScaleIndependent() ? "yes" : "no");
    //color/scale text
    table.set("textColor", textColor.toVec4());
    table.set("textScale", scale);
    //layout
    table.set("horizontal", STRHLayout[hlayout]);
    table.set("vertical", STRVLayout[vlayout]);
}
void Label::deserializeFields(const Table& table)
{
    //enable/disable scale 9
    if (table.getString("enableScale9", isEnableScale9() ? "yes" : "no") != "no")
        enableScale9();
    else
        disableScale9();
    //box scale 9
    setBoxScale9(table.getVector4D("boxScale9", boxScale9));
    //enable/disable scale independent
    setTextScaleIndependent(table.getString("scaleIndependent",
        isTextScaleIndependent() ? "yes" : "no") != "no"
        );
    //horizontal layout 
    String horizontal = table.getString("horizontal", STRHLayout[hlayout]);
    for (int i = 0; i != 3; ++i)
        if (horizontal == STRHLayout[i] || horizontal == STRHLayoutSugar[i] )
        {
            hlayout = (HLayout)(i);
            break;
        };
    //vertical layout 
    String vertical = table.getString("vertical", STRVLayout[vlayout]);
    for (int i = 0; i != 3; ++i)
        if (vertical == STRVLayout[i] || vertical == STRVLayoutSugar[i] )
        {
            vlayout = (VLayout)(i);
            break;
        };
    //scale text
    setTextScale(table.getVector2D("textScale", scale));
    //text color
    textColor = table.getColor("textColor", textColor);
}
void Label::serialize(Table& table)
{
    Text::serialize(table);
    Label::serializeFields(table);
    //serialize texture
    if (getTexture())
        table.set("texture", getTexture()->getName());
}
void Label::deserialize(const Table& table)
{
    Text::deserialize(table);
    //get texture
    if (table.existsAsType("texture", Table::STRING))
    {
        auto rsmanager = table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup = rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }
    //deserialize after the texture
    Label::deserializeFields(table);
}

/**
 *  BUTTON
 */
void Button::onLostFocus()
{
    state=BT_NORMAL;
    setTexture();
}
void Button::onOver(const Vec2& over)
{
    if(state==BT_NORMAL)
    {
        state=BT_OVER;
        setTexture();
    }
}
void Button::onNotOver(const Vec2& over)
{
    if(state==BT_OVER)
    {
        state=BT_NORMAL;
        setTexture();
    }
}
void Button::onClick(const Vec2& press)
{
    state=BT_PRESS;
    setTexture();
    if(callback) callback();
}
void Button::onRelease(const Vec2& over)
{
    if(state==BT_PRESS)
    {
        if(getEventBox().isIntersection(over))
            state=BT_OVER;
        else
            state=BT_NORMAL;
        //change texture
        setTexture();
    }
}
//event utils
OBBox2 Button::getEventBox()
{
    OBBox2  box=getBaseBox();
    return  box.applay(getModel());
}
uint Button::getZ()
{
    if(getObject()) return getObject()->getZ(true);
    return 0;
}
void Button::onSetScene(Easy2D::Scene *scene)
{
    auto manager = dynamic_cast<Ui::WidgetManager*>(scene);
    if (manager) addTo(manager);
}
void Button::onEraseScene()
{
    auto manager = dynamic_cast<Ui::WidgetManager*>(getScene());
    if (manager) removeFrom(manager);
}
//serialize / deserialize
void Button::serialize(Table& table)
{
    Text::serialize(table);
    Label::serializeFields(table);
    //serialize texture
    if (textures[0]) table.set("normal", textures[0]->getName());
    if (textures[1]) table.set("over",   textures[1]->getName());
    if (textures[2]) table.set("press",  textures[2]->getName());
}
void Button::deserialize(const Table& table)
{
    Text::deserialize(table);
    //textures
    bool existsNormal = table.existsAsType("normal", Table::STRING);
    bool existsOver   = table.existsAsType("over",   Table::STRING);
    bool existsPress  = table.existsAsType("press",  Table::STRING);
    //get texture
    if (existsNormal || existsOver || existsPress)
    {
        auto rsmanager = table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup = rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        //load
        if (existsNormal) textures[0] = (rsgroup->load<Texture>(table.getString("normal")));
        if (existsOver)   textures[1] = (rsgroup->load<Texture>(table.getString("over")));
        if (existsPress)  textures[2] = (rsgroup->load<Texture>(table.getString("press")));
        //set default texture
        setTexture();
    }
    //deserialize after texture
    Label::deserializeFields(table);
}

/**
 *  Text Field
 */   
  //costructor
TextField::TextField(Texture::ptr texture,
                     Font::ptr    font)
{
    textColor     = Color::WHITE;
    pointerColor  = Color::BLACK;
    selectorColor = Color(128, 128, 255, 128);
    if(texture) setTexture(texture);
    if(font)    setFont(font);
    Renderable::setCull(DISABLE);
}
TextField::TextField(Texture::ptr  texture,
                     Font::ptr     font,
                     const String& text)
{
    textColor     = Color::WHITE;
    pointerColor  = Color::BLACK;
    selectorColor = Color(128, 128, 255, 128);
    setTexture(texture);
    set(font, text);
    Renderable::setCull(DISABLE);
}
//events
void TextField::onFocus()
{
    // show Virtual Keyboard
    if(manager && manager->isAFinger())
        Application::instance()->virtualKeyboard(true);
}
void TextField::onLostFocus()
{
    state=TF_NORMAL;
    // hide Virtual Keyboard
    if(manager && manager->isAFinger())
        Application::instance()->virtualKeyboard(false);
}
void TextField::onOver(const Vec2& over)
{
}
void TextField::onNotOver(const Vec2& over)
{

}
void TextField::onKeyPress(Key::Keyboard key)
{
    switch (key)
    {
    case Key::RIGHT:
        if (cursor < (int)text.length()) ++cursor;
        cursorSelect = cursor;
        state = TF_SELECT_CHAR;
        computeOffset(cursor);
    break;
    case Key::LEFT:
        if (cursor>0) --cursor;
        cursorSelect = cursor;
        state = TF_SELECT_CHAR;
        computeOffset(cursor);
    break;
    case Key::BACKSPACE: deleteSelectChars(true);  break;
    case Key::KDELETE:   deleteSelectChars(false); break;
    case Key::SPACE:     onTextInput(" ");         break;
    case Key::TAB:       onTextInput("\t");         break;
    default:break;
    }
}
void TextField::onClick(const Vec2& press)
{
	Mat4 matrix = getModel().mul2D(model);
	Vec2 pModel = matrix.getInverse().mul2D(press);
	//is a text field
	pModel.y = font->size() *  0.5f;
	//
    Font::InfoSelection info = font->selectSpace(pModel + Vec2(offset, 0), Vec2::ZERO, text);
	if (info.index == -1)
	{
		if (pModel.x <= 0.0f) info.index = 0;
		if (pModel.x >= 0.0f) info.index = text.length();
	}
	cursor       = info.index;
	cursorSelect = cursor;
	state = TF_SELECT_CHAR;
    computeOffset(cursor);
}
void TextField::onDoubleClick(const Vec2& press)
{
    if (!text.length()) return;

    Mat4 matrix = getModel().mul2D(model);
    Vec2 pModel = matrix.getInverse().mul2D(press);
    //is a text field
    pModel.y = font->size() *  0.5f;
    Font::InfoSelection info = font->selectSpace(pModel + Vec2(offset, 0), Vec2::ZERO, text);
    //on text?
    if (info.index != -1)
    {
        cursor       = 0;
        cursorSelect = text.length();
        //set offset
        computeOffset(cursorSelect);
        //select mode
        state = TF_SELECT_AREA;
    }
}
void TextField::onDown(const Vec2& press)
{
    Mat4 matrix = getModel().mul2D(model);
    Vec2 pModel = matrix.getInverse().mul2D(press);
	//is a text field
    pModel.y  = font->size() *  0.5f;
	//
    Font::InfoSelection info = font->selectSpace(pModel + Vec2(offset, 0), Vec2::ZERO, text);
    if (info.index == -1)
	{
		if (pModel.x <= 0.0f) info.index = 0;
		if (pModel.x >= 0.0f) info.index = text.length();
    }

	cursorSelect = info.index;
	if (cursorSelect != cursor)
    {
        //set offset
        computeOffset(cursorSelect);
        //select mode
        state = TF_SELECT_AREA;
    }
    else if (state == TF_SELECT_AREA)
    {
        state = TF_SELECT_CHAR;
    }
}
void TextField::onRelease(const Vec2& value)
{
}
void TextField::onTextInput(const String& input)
{
    if (verify && !verify(input)) return;
    //add input
	switch (state)
	{
		case Easy2D::Ui::TextField::TF_NORMAL:break;
		case Easy2D::Ui::TextField::TF_SELECT_CHAR:
			if (cursor)
				setText(text.substr(0, cursor) + input + text.substr(cursor, text.size() - cursor));
			else
				setText(input + text);
			//next
            cursor += input.length();
            //set offset
            computeOffset(cursor);
		break;
		case Easy2D::Ui::TextField::TF_SELECT_AREA:
        {
            int min_c = Math::min(cursor, cursorSelect);
            int max_c = Math::max(cursor, cursorSelect);
            String left = text.substr(0, min_c);
            String right = text.substr(max_c, text.length() - max_c);
			text = left + input;
			//disable selection
			cursor       = text.length();
			cursorSelect = cursor;
			state = TF_SELECT_CHAR;
			//add right text
            setText(text + right);
            //set offset
            computeOffset(cursor);
		}
		break;
		default:break;
	}
}
//event utils
OBBox2 TextField::getEventBox()
{
    OBBox2  box=getBaseBox();
    return  box.applay(getModel());
}
uint TextField::getZ()
{
    if(getObject()) return getObject()->getZ(true);
    return 0;
}
void TextField::computeOffset(int index)
{   
    ////////////////////////////////////////////////
    AABox2 cbox;
    AABox2 bbox = getBaseBox();
    Vec2   size = bbox.getSize() * 2.0 * getObject()->getScale(true);
    Vec2   sizeText = font->textSpaceSize(text);
    ////////////////////////////////////////////////
    if (sizeText.x <= size.x)
    {
        offset = 0;
        calcVTranslationText();
        return;
    }
    else
    {
        //VL_LEFT
        model[12] = bbox.getMin().x;
    } 
    ////////////////////////////////////////////////
    if (index >= (int)text.length())
    {
        cbox = font->getSpaceCharPosition2D(text.length() - 1, text);
        cbox.setMin(cbox.getMax());
    }
    else
    {
        cbox = font->getSpaceCharPosition2D(index, text);
    }    
    ////////////////////////////////////////////////
    if (size.x < (cbox.getMax().x - offset))
        offset = (cbox.getMax().x - size.x);
    else if (0 > (cbox.getMin().x - offset))
        offset = cbox.getMin().x;

    //limits
    const float maxVal = Math::max(sizeText.x - size.x, 0.0f);
    const float minVal = 0.0f;
    offset = Math::min(Math::max(offset, minVal), maxVal);

}
void TextField::deleteSelectChars(bool left)
{
    switch (state)
    {
    case TextField::TF_SELECT_CHAR:
        if (cursor && left)
        {
            setText(text.substr(0, cursor - 1) + text.substr(cursor, text.size() - cursor));
            //next
            --cursor;
            //set offset
            computeOffset(cursor);
        }
        else if (cursor < text.length() && !left)
        {
            setText(text.substr(0, cursor) + text.substr(cursor + 1, text.size() - cursor - 1));
            //set offset
            computeOffset(cursor);
        }
        break;
    case TextField::TF_SELECT_AREA:
    {
        int min_c = Math::min(cursor, cursorSelect);
        int max_c = Math::max(cursor, cursorSelect);
        String left = text.substr(0, min_c);
        String right = text.substr(max_c, text.length() - max_c);
        //disable selection
        cursor = left.length();
        cursorSelect = cursor;
        state = TF_SELECT_CHAR;
        //add right text
        setText(left + right);
        //set offset
        computeOffset(cursor);
    }
    break;
    default:break;
    }
}
//attach listener
void TextField::onSetScene(Easy2D::Scene *scene)
{
    auto manager = dynamic_cast<Ui::WidgetManager*>(scene);
    if (manager) addTo(manager);
}
void TextField::onEraseScene()
{
    auto manager = dynamic_cast<Ui::WidgetManager*>(getScene());
    if (manager) removeFrom(manager);
}
//overload
void TextField::setFont(Font::ptr argfont)
{
    font=argfont;
    //compute box
    Vec2 pos = font->textSpaceSize(text);
    Text::box.setMin(Vec2(     0,-pos.y));
    Text::box.setMax(Vec2( pos.x,     0));
    calcMat4Text();
}
void TextField::setText(const String& argtext)
{
    text=argtext;
    //compute box
    Vec2 pos = font->textSpaceSize(text);
    Text::box.setMin(Vec2(     0,-pos.y));
    Text::box.setMax(Vec2( pos.x,     0));
    calcMat4Text();
}
void TextField::setVerify(TextField::Verify funVerify)
{
    verify=funVerify;
}
void TextField::setTextPointerColor(const Color& color)
{
    pointerColor = color;
}
void TextField::setTextSelectorColor(const Color& color)
{
    selectorColor = color;
}
void TextField::set(Font::ptr argfont,const String& argtext)
{
    font=argfont;
    text=argtext;
    //compute box
    Vec2 pos = font->textSpaceSize(text);
    Text::box.setMin(Vec2(     0,-pos.y));
    Text::box.setMax(Vec2( pos.x,     0));
    calcMat4Text();
}
Color TextField::getTextPointerColor() const
{
    return pointerColor;
}
Color TextField::getTextSelectorColor() const
{
    return selectorColor;
}
TextField::Verify TextField::getVerify() const
{
    return verify;
}
AABox2 TextField::charAt(int charID)
{
	AABox2 cpos;
	if (charID < (int)text.length())
	{
        cpos = font->getSpaceCharPosition2D(charID, text);
		Vec2 n_min = Vec2(cpos.getMin().x, -cpos.getMax().y);
		Vec2 n_max = Vec2(cpos.getMin().x, -cpos.getMin().y);
		cpos.setMin(n_min);
		cpos.setMax(n_max);
	}
	else if(text.length())
	{
        cpos = font->getSpaceCharPosition2D(charID - 1, text);
		Vec2 n_min = Vec2(cpos.getMax().x, -cpos.getMax().y);
		Vec2 n_max = Vec2(cpos.getMax().x, -cpos.getMin().y);
		cpos.setMin(n_min);
		cpos.setMax(n_max);
	}
    else
    {
        cpos.setMin(Vec2( 0.0f, (float)(-font->size()) ));
        cpos.setMax(Vec2::ZERO);
    }
	return cpos;
}
void TextField::draw()
{    
    //uniform
    if (scale9) getShader()->setUniformCallback([this](Shader& shader)
    {
        Vec2 scale(Vec2::ONE);
        if (getObject()) scale = getObject()->getScale(true);

        shader.uniform(uniform.inBox, boxScale9);
        shader.uniform(uniform.sizeTex, getTexture()->getSize());
        shader.uniform(uniform.scaleSprite, scale);
    });
    //draw backgound
    Renderable::draw();
    //disable shader
    RenderContext::disableProgram();
    //if text exist
    if(text.length())
    {
        //ENABLE STENCIL
        RenderContext::setStencil(STENCIL_REPLACE);
        RenderContext::stencilClear();
        RenderContext::setColorWritable(false, false, false, false);
        RenderContext::setZBufferWritable(false);
        RenderContext::drawFillBox(getBaseBox(), Color::BLACK);
        RenderContext::setStencil(STENCIL_KEEP);
        RenderContext::setColorWritable(true, true, true, true);
        RenderContext::setZBufferWritable(true);
        //offset
        Mat4 textModel;
        textModel.setTranslation(Vec2(-offset,0));
        textModel = model.mul2D(textModel);
        //set text model
        RenderContext::setModel(RenderContext::getModel().mul2D(textModel));
        //text color
        RenderContext::setColor(getDisplayedTextColor());
        //draw text
        auto cullmode=RenderContext::getRenderState().cullface;
        RenderContext::setCullFace(DISABLE);
        font->mesh(text);
        RenderContext::setCullFace(cullmode);
        //select
        switch (state)
        {
            case TF_NORMAL:
            break;
            case TF_SELECT_CHAR:
            {
                AABox2 cpos = charAt(cursor);
                //get position
                RenderContext::drawBox(cpos, pointerColor);
            }
            break;
            case TF_SELECT_AREA:
            {
                //get position
                AABox2 cpos = charAt(cursor);
                AABox2 cpos2 = charAt(cursorSelect);
                Vec2 n_min = Vec2(cpos.getMin().x,   cpos.getMax().y);
                Vec2 n_max = Vec2(cpos2.getMax().x,  cpos.getMin().y);
                cpos.setMin({Math::min(n_min.x,n_max.x), Math::min(n_min.y,n_max.y)});
                cpos.setMax({Math::max(n_min.x,n_max.x), Math::max(n_min.y,n_max.y)});
                RenderContext::drawFillBox(cpos, selectorColor);
            }
            break;
            default: break;
        }
        //DISABLE STENCIL
        RenderContext::setStencil(STENCIL_NONE);
    }
    else if(state != TF_NORMAL)
    {
        //ENABLE STENCIL
        RenderContext::setStencil(STENCIL_REPLACE);
        RenderContext::stencilClear();
        RenderContext::setColorWritable(false, false, false, false);
        RenderContext::setZBufferWritable(false);
        RenderContext::drawFillBox(getBaseBox(), Color::BLACK);
        RenderContext::setStencil(STENCIL_KEEP);
        RenderContext::setColorWritable(true, true, true, true);
        RenderContext::setZBufferWritable(true);
        //offset
        Mat4 textModel;
        textModel.setTranslation(Vec2(-offset,0));
        textModel = model.mul2D(textModel);
        //set text model
        RenderContext::setModel(RenderContext::getModel().mul2D(textModel));
        //draw pointer
        AABox2 cpos = charAt(cursor);
        //get position
        RenderContext::drawBox(cpos, pointerColor);
        //DISABLE STENCIL
        RenderContext::setStencil(STENCIL_NONE);
    }
}
//box of texture
AABox2 TextField::getBaseBox()
{
    return Renderable::getBaseBox();
}
//serialize / deserialize
void TextField::serialize(Table& table)
{
    Text::serialize(table);
    Label::serialize(table);
    //serialize color
    table.set("textPointerColor", pointerColor.toVec4());
    table.set("textSelectorColor", selectorColor.toVec4());
}
void TextField::deserialize(const Table& table)
{
    Text::deserialize(table);
    Label::deserialize(table);
    //deserialize color
    pointerColor = table.getColor("textPointerColor", pointerColor);
    selectorColor = table.getColor("textSelectorColor", selectorColor);
}

