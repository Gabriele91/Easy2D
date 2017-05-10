//
//  UIComponents.h
//  ComponentGui
//
//  Created by Gabriele Di Bari on 18/05/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Font.h>
#include <Renderable.h>
#include <UIManager.h>

namespace Easy2D
{
namespace Ui
{

enum HLayout
{
    HL_TOP,
    HL_CENTER,
    HL_BOTTOM
};
enum VLayout
{
    VL_LEFT,
    VL_CENTER,
    VL_RIGHT
};
    
class Text;
class Label;
class Button;
    
class Anchor 
{
protected:

    Vec2 anchorPoint;
    bool anchor{ false };
    Mat4 calcAnchorGMatrix(Object* obj);

public:

    void enableAnchor();
    void disableAnchor();
    bool isEnableAnchor() const;

    void setAnchorPoint(const Vec2& value);
    Vec2 getAnchorPoint() const;
    //serialize/deserialize
    void serialize(Table& table);
    void deserialize(const Table& table);

};

class Text : public Renderable, public Anchor
{
protected:
    
    //text
    AABox2    box;
    String    text;
    Font::ptr font;
    std::vector< Font::NodeText > nodes;
    //protected set mesh
    void setMesh(Mesh::ptr mesh);
    void setTexture(Texture::ptr tex);
    //box of string
    virtual AABox2 getBaseBox();
    //get model matrix
    virtual Mat4 getModel();
    //render only text
    void textDraw();
    void textDrawClip(const AABox2& box);
    void textDrawClip(const Mat4& local,const AABox2& box);
    
public:
    //constructor
    Text(Font::ptr font=nullptr);
    Text(Font::ptr font,const String& text);
    //distructor
    virtual ~Text();
    //
    virtual void setFont(Font::ptr font);
    virtual void setText(const String& text);
    virtual void set(Font::ptr font, const String& text);
    //get text
    String getText() const
    {
        return text;
    }
    //draw
    virtual void draw();
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
    //is a component
    DERIVATE_COMPONENT(Text)
};
REGISTERED_COMPONENT(Text, "Text")


class Label : public Text
{
protected:

     //text color
     Color textColor;
     //protected set mesh
     void setMesh(Mesh::ptr mesh);
     //draw
     virtual void draw();
     //box of texture
     virtual AABox2 getBaseBox();
     //text scale
     Vec2 scale { Vec2::ONE };
     HLayout hlayout { HL_CENTER };
     VLayout vlayout { VL_CENTER };
     Mat4 model;
     void calcMat4Text();
     void calcTranslationText();
     void calcHTranslationText();
     void calcVTranslationText();
     //text scale
     bool scaleIndependent{ true };
     //scale9
     struct Uniform
     {
         int sizeTex    { 0 };
         int scaleSprite{ 0 };
         int inBox      { 0 };
     };
     Uniform uniform;
     Vec4    boxScale9;
     bool    scale9 { false };

     void serializeFields(Table& table) const;
     void deserializeFields(const Table& table);
     Color getDisplayedTextColor() const;
    
public:
    

     //costructor
     Label(Texture::ptr texture=nullptr,
           Font::ptr    font=nullptr);
     Label(Texture::ptr  texture,
           Font::ptr     font,
           const String& text);
     //set texture
     virtual void setTexture(Texture::ptr tex);
     virtual void setBoxScale9(const Vec4& box);
     virtual void enableScale9();
     virtual void disableScale9();
     virtual void setTextScaleIndependent(bool value);
     virtual void setFont(Font::ptr font);
     virtual void setText(const String& text);
     virtual void setTextColor(const Color& color);
     virtual void setTextScale(const Vec2& scale);
     virtual void set(Font::ptr font,const String& text);
     virtual void setLayout(HLayout h, VLayout v);
     //get
     virtual HLayout getHLayout() const;
     virtual VLayout getVLayout() const;
     virtual Color getTextColor() const;
     virtual Vec2 getTextScale() const;
     virtual bool isEnableScale9() const;
     virtual bool isTextScaleIndependent() const;
     //serialize/deserialize
     virtual void serialize(Table& table);
     virtual void deserialize(const Table& table);
     //component event
     virtual void onChangedMatrix();
     //is a component
     DERIVATE_COMPONENT(Label)
};
REGISTERED_COMPONENT(Label, "Label")


class Button : public WidgetHandler,
               public Label
    
{
protected:

    enum State
    {
        BT_NORMAL = 0,
        BT_OVER   = 1,
        BT_PRESS  = 2
    };
    State state{ BT_NORMAL };
    //textures
    Texture::ptr textures[3];
    //event callback
    Function<void(void)> callback{ nullptr };
    //currente texture
    void setTexture(Texture::ptr ptr=nullptr)
    {
        if(!textures[state])
            Label::setTexture(textures[BT_NORMAL]);
        else
            Label::setTexture(textures[state]);
    }
    //events
    void onLostFocus();
    void onOver(const Vec2& over);
    void onNotOver(const Vec2& over);
    void onClick(const Vec2& press);
    void onRelease(const Vec2& over);
    //event box
    OBBox2 getEventBox();
    uint getZ();
    //attach listener
    void onSetScene(Easy2D::Scene *scene);
    void onEraseScene();
    
public:
    
    Button(Texture::ptr  normal=nullptr,
           Texture::ptr  over=nullptr,
           Texture::ptr  press=nullptr,
           Font::ptr     font=nullptr)
    :Label(normal,font)
    {
        setTextures(normal,over,press);
    }
    
    Button(Texture::ptr  normal,
           Texture::ptr  over,
           Texture::ptr  press,
           Font::ptr     font,
           const String& text)
    :Label(normal,font,text)
    {
        setTextures(normal,over,press);
    }
    //set callback
    void onClick(Function<void(void)> value)
    {
        callback = value;
    }
    //set textures
    virtual void setTextures(Texture::ptr  normal=nullptr,
                             Texture::ptr  over=nullptr,
                             Texture::ptr  press=nullptr)
    {
    
        textures[0]=normal;
        textures[1]=over;
        textures[2]=press;
        setTexture();
    }
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
    //is a component
    DERIVATE_COMPONENT(Button)
    
};
REGISTERED_COMPONENT(Button, "Button")


class TextField : public WidgetHandler,
                  public Label
{
protected:
    //colors
    Color pointerColor;
    Color selectorColor;
    //attach listener
    void onSetScene(Easy2D::Scene *scene);
    void onEraseScene();
    //events
    void onFocus();
    void onLostFocus();
    void onOver(const Vec2& over);
    void onNotOver(const Vec2& over);
    void onClick(const Vec2& press);
    void onDoubleClick(const Vec2& press);
    void onDown(const Vec2& value);
    void onRelease(const Vec2& value);
    void onTextInput(const String& input);
    void onKeyPress(Key::Keyboard key);
    void onKeyDown(Key::Keyboard key){}
    void onKeyRelease(Key::Keyboard key){}
    //event box
    OBBox2 getEventBox();
    uint getZ();
    //state info
    enum State
    {
        TF_NORMAL=0,
        TF_SELECT_CHAR=1,
        TF_SELECT_AREA=2
    };
    //state of text field
    State state{ TF_NORMAL };
    //cursor pos
    float  offset{ 0.0f };
	int    cursor{ 0 };
	int    cursorSelect{ 0 };
	AABox2 charAt(int pos);
    //overload draw
    virtual void draw();
    //box of texture
    virtual AABox2 getBaseBox();
    //compute offset
    void computeOffset(int index);
    //delete text
    void deleteSelectChars(bool left=true);
    //input verify
    typedef Function<bool(const String& input)> Verify;
    Verify verify{ nullptr };

public:
    
    //costructor
    TextField(Texture::ptr texture=nullptr,
              Font::ptr    font = nullptr);
    TextField(Texture::ptr  texture,
              Font::ptr     font,
              const String& text);
    //set text/font
    virtual void setFont(Font::ptr font);
    virtual void setText(const String& text);
    virtual void setTextPointerColor(const Color& color);
    virtual void setTextSelectorColor(const Color& color);
    virtual void set(Font::ptr argfont, const String& text);
    virtual void setVerify(Verify funVerify);
    virtual Color getTextPointerColor() const;
    virtual Color getTextSelectorColor() const;
    virtual Verify getVerify() const;
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
    //is a component
    DERIVATE_COMPONENT(TextField)
};
REGISTERED_COMPONENT(TextField, "TextField")


};
};

#endif
