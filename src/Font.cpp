#include <stdafx.h>
#include <Config.h>
#include <Debug.h>
#include <Color.h>
#include <Font.h>
#include <Types.h>
#include <Screen.h>
#include <Application.h>
#include <BMFontLoader.h>
#include <FreeTypeFontLoader.h>
#include <RenderContext.h>
//#include "Thread/Thread.h"
//#include "Image/Image.h"

///////////////////////
using namespace Easy2D;
///////////////////////
//costructor
Font::Font(ResourcesManager<Font> *rsmr,const String& path)
    :Resource(rsmr,path)
    ,fontSize(0)
    ,isBMFont(false)
{}
//load methods
bool Font::load()
{
    //can't load this resource
    DEBUG_ASSERT(isReloadable());
    //is a BMFont format?
    if(rpath.getExtension()=="fnt")
    {
        BMFontLoader::load(*this,rpath);
    }
    else
    {
        //is a Table
        //load font info
        void *data=NULL;
        size_t len=0;
        Application::instance()->loadData(rpath,data,len);
        String filestring((char*)data);
        free(data);
        //deserialize font info
        Table fontInfo;
        fontInfo.deserialize(filestring);
        DEBUG_ASSERT_MSG(fontInfo.exists("font"),"font error:"
                                                 "must to be setted font path"
                                                 "(parameter:font), "+rpath.getPath());

        Utility::Path pathFont(rpath.getDirectory()+"/"+fontInfo.getString("font"));
        
        if((isBMFont=(pathFont.getExtension()=="fnt")))
            BMFontLoader::load(*this,pathFont);
        else
            FreeTypeFontLoader::load(*this,fontInfo,pathFont);
    }
    //errors
    CHECK_GPU_ERRORS();
    //is loaded
    loaded=true;
    //
    return true;
}
bool Font::unload()
{
    return true;
}
//getters
int Font::size()
{
    return fontSize;
}
const String& Font::getFontName()
{
    return fontName;
}

//define lambda function
typedef void(*lambdaChar)(int fontSize,Vec2 pos,Vec2& cursor);
//list of special char :D
struct SpecialChars
{
    int c;
    lambdaChar lambda;
}
specialChars[]=
{
    {
        '\n',[](int fontSize,Vec2 pos,Vec2& cursor)->void{
            cursor.y-=fontSize;
            cursor.x=pos.x;
        }
    },
    {
        '\v',[](int fontSize,Vec2 pos,Vec2& cursor)->void{
            cursor.y-=fontSize;
            cursor.x=pos.x;
        }
    },
    {
        ' ',[](int fontSize,Vec2 pos,Vec2& cursor)->void{
            cursor.x+=fontSize*0.5f;
        }
    },
    {
        '\t',[](int fontSize,Vec2 pos,Vec2& cursor)->void{
            cursor.x+=fontSize*2.0f;
        }
    }
};
lambdaChar isASpecialChar(int c)
{
    for(auto& sc:specialChars)
        if(sc.c==c)
            return sc.lambda;
    return NULL;
}


//other methods
void Font::text(const Vec2& _pos,
                const String& textDraw,
                const Color& color,
                bool kerning)
{

    if(textDraw.size()==0) return;
    Vec2 pos(_pos.x,-_pos.y+Application::instance()->getScreen()->getHeight());
    //matrixs
    Matrix4x4 oldProjection=RenderContext::getProjection(),
              oldModelview=RenderContext::getModelView();
    //state
    auto state=RenderContext::getRenderState();
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    //change param
    RenderContext::setCullFace(DISABLE);
    //blend
    RenderContext::setBlend(true);
    RenderContext::setBlendFunction(BLEND::SRC::ALPHA, BLEND::ONE::MINUS::SRC::ALPHA);
    //reset projection matrix
    Matrix4x4 projection;
    //set viewport
    Vec2 viewport(Application::instance()->getScreen()->getWidth(),
                  Application::instance()->getScreen()->getHeight());
    //update projection is always the same
    projection.setOrtho(0,viewport.x, 0,viewport.y, 0,1);
    RenderContext::setViewport(Vec4( 0, 0, viewport.x, viewport.y ));
    RenderContext::setProjection(projection);
    //reset model matrix
    RenderContext::setModelView(Mat4::IDENTITY);
    //color
    RenderContext::setColor(color);
    //////////////////////////////////////////////////////////////////
    //disable vbo
    RenderContext::unbindIndexBuffer();
    RenderContext::unbindVertexBuffer();
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    Vec2 cursor(pos);
    //vector sprites
    std::vector<float> xyUV(textDraw.size()*24,0);
    //temp vars
    //int oldPage=0;
    int countCharPage=0;
    Character* chr=NULL;
    Character* nextChr=getCharacter(textDraw[0]);
    int pageLast=0;

    for(int i=0; i<textDraw.length(); ++i)
    {
        //string's char
        char c=textDraw[i];
        char nextC=textDraw[i+1];
        //image's char
        chr=nextChr;
        //next char
        nextChr=getCharacter(nextC);
        //is special?
        lambdaChar charFunction=isASpecialChar(c);
        if(charFunction)
            charFunction(fontSize,pos,cursor);
        else if(chr)
        {
            //page
            pageLast=chr->page;
            //
            Vec2 sizePage(pages[chr->page]->getRealWidth(),
                          pages[chr->page]->getRealHeight());
            //uv
            Vec2 nSXY(chr->srcX,chr->srcY);
            nSXY/=sizePage;
            Vec2 nEXY(chr->srcX+chr->srcW,chr->srcY+chr->srcH);
            nEXY/=sizePage;
            Math::swap(nSXY.v,nEXY.v);

            //opengl uv flipped error on y axis
            float yoffset=isBMFont ? -chr->srcH-chr->yOff : -fontSize-chr->srcH+chr->yOff;
            float xoffset=chr->xOff - (kerning? getKerningPairs(c,nextC) : 0);
            Vec2 posChr(cursor+Vec2(xoffset,yoffset));

#define XYUV(x) xyUV[countCharPage*24+x]
            XYUV(0)=posChr.x;
            XYUV(1)=posChr.y;
            XYUV(2)=nSXY.u;
            XYUV(3)=nSXY.v;
            XYUV(4)=posChr.x;
            XYUV(5)=posChr.y+chr->srcH;
            XYUV(6)=nSXY.u;
            XYUV(7)=nEXY.v;
            XYUV(8)=posChr.x+chr->srcW;
            XYUV(9)=posChr.y;
            XYUV(10)=nEXY.u;
            XYUV(11)=nSXY.v;

            XYUV(12)=posChr.x;
            XYUV(13)=posChr.y+chr->srcH;
            XYUV(14)=nSXY.u;
            XYUV(15)=nEXY.v,
            XYUV(16)=posChr.x+chr->srcW;
            XYUV(17)=posChr.y;
            XYUV(18)=nEXY.u;
            XYUV(19)=nSXY.v;
            XYUV(20)=posChr.x+chr->srcW;
            XYUV(21)=posChr.y+chr->srcH;
            XYUV(22)=nEXY.u;
            XYUV(23)=nEXY.v;
#undef XYUV
            //count this char
            ++countCharPage;
            //next pos
            cursor.x+=chr->xAdv;
        }
        //draw?
        if(nextC=='\0' ||(nextChr!=NULL && pageLast!=nextChr->page))
        {
            //draw chars
            drawListCharArray(pageLast,&xyUV[0],&xyUV[2],(countCharPage)*6);
            //reset count
            countCharPage=0;
        }

    }
    //////////////////////////////////////////////////////////////////
    RenderContext::setRenderState(state);
    RenderContext::setProjection(oldProjection);
    RenderContext::setModelView(oldModelview);
    //////////////////////////////////////////////////////////////////
    
}

Vec2 Font::textSize( const String& textDraw,bool kerning)
{
    //////////////////////////////////////////////////////////////////
    Vec2 cursor;
    //temp vars
    int countCharPage=0;
    Character* chr=NULL;
    Character* nextChr=getCharacter(textDraw[0]);
    //int pageLast=0;
    //min max
    Vec2 min= Vec2::MAX;
    Vec2 max=-Vec2::MAX;
    //
    for(int i=0; i<textDraw.length(); ++i)
    {
        //string's char
        char c=textDraw[i];
        char nextC=textDraw[i+1];
        //image's char
        chr=nextChr;
        //next char
        nextChr=getCharacter(nextC);
        //is special?
        lambdaChar charFunction=isASpecialChar(c);
        if(charFunction)
            charFunction(fontSize,Vec2::ZERO,cursor);
        else if(chr)
        {
            //page
            //pageLast=chr->page;
            //
            Vec2 sizePage(pages[chr->page]->getRealWidth(),
                          pages[chr->page]->getRealHeight());
            //opengl uv flipped error on y axis
            float yoffset=isBMFont ?  chr->srcH+chr->yOff : -fontSize-chr->srcH+chr->yOff;
            float xoffset=chr->xOff - (kerning? getKerningPairs(c,nextC) : 0);
            //get min
            min.x=Math::min(min.x, cursor.x);
            min.y=Math::min(min.y, cursor.y);
            //get max
            max.x=Math::max(max.x, cursor.x+xoffset+chr->srcW);
            max.y=Math::max(max.y, cursor.y+yoffset+chr->srcH*.5f);
            //count this char
            ++countCharPage;
            //next pos
            cursor.x+=chr->xAdv;
        }
    }

    return (max-min);
}
/*
OLD METHOD

void Font::text(const Vec2& pos,const String& textDraw,const Color& color){

	GLboolean cull,blend;
	GLint bs_src, bs_dst;
	Matrix4x4 old_projection,old_modelview;
	GLfloat old_color[4];
	glGetBooleanv(GL_CULL_FACE,&cull);
	glGetBooleanv(GL_BLEND , &blend);
	glGetIntegerv(GL_BLEND_SRC_RGB , &bs_src);
	glGetIntegerv(GL_BLEND_DST_RGB , &bs_dst);
	glGetFloatv(GL_PROJECTION_MATRIX ,  old_projection );
	glGetFloatv(GL_MODELVIEW_MATRIX , old_modelview );
    glGetFloatv(GL_CURRENT_COLOR, old_color);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//change param
	glDisable(GL_CULL_FACE);
	//blend
	if(!blend) glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//reset projection matrix
	Matrix4x4 projection;
	//set viewport
	Vec2 viewport(Application::instance()->getScreen()->getWidth(),
				  Application::instance()->getScreen()->getHeight());
	//update projection is always the same
	projection.setOrtho(0,viewport.x, 0,viewport.y, 0,1);
	glViewport( 0, 0, viewport.x, viewport.y );
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	//reset model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//color
	glColor4ub(color.r,color.g,color.b,color.a);
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	Vec2 cursor(pos);
	for(auto c:textDraw){

		if(c=='\n'){ cursor.y-=fontSize; cursor.x=pos.x; continue; } //next line
		if(c==' '){ cursor.x+=fontSize*0.5; continue; } //next char

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		Character* chr=getCharacter(c);

		if(chr){
			//
			Vec2 sizePage(pages[chr->page]->getRealWidth(),
						  pages[chr->page]->getRealHeight());
			//uv
			Vec2 nSXY(chr->srcX,chr->srcY); nSXY/=sizePage;
			Vec2 nEXY(chr->srcX+chr->srcW,chr->srcY+chr->srcH); nEXY/=sizePage;
			Math::swap(nSXY.v,nEXY.v);

			//opengl uv flipped error on y axis
			float yerror=isBMFont ? fontSize-chr->srcH : 0;

			Vec2 posChr(cursor+Vec2(chr->xOff,-chr->yOff));
			float xyUV[]={
				 posChr.x,             posChr.y+yerror,          nSXY.u,nSXY.v,
				 posChr.x,             posChr.y+chr->srcH+yerror,nSXY.u,nEXY.v,
				 posChr.x+chr->srcW,   posChr.y+yerror,          nEXY.u,nSXY.v,
				 posChr.x+chr->srcW,   posChr.y+chr->srcH+yerror,nEXY.u,nEXY.v
	        };

			//bind texture
			pages[chr->page]->bind();
			//set vertex
			glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
			glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
			//draw array
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//next pos
			cursor.x+=chr->xAdv;

		}
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	if(cull)
		glEnable( GL_CULL_FACE );
	//blend
	if(!blend)
		glDisable( GL_BLEND );
	else
		glBlendFunc(bs_src,bs_dst);
	//matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(old_projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(old_modelview);
	//color
	glColor4fv(old_color);
}*/
