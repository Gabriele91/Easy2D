
#include <stdafx.h>
#include <FrameSet.h>
#include <Application.h>
#include <ResourcesGroup.h>
#include <Table.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//create a frame set
FrameSet::FrameSet(ResourcesManager<FrameSet> *rsmr,const String& path)
    :Resource(rsmr,path)
{
    defaultTime=1.0;
    defaultLoop=true;
}
FrameSet::~FrameSet()
{
    //release resource
    release();
}
void FrameSet::addFrame(const Vec4& frame)
{
    //create a mesh
    Mesh::ptr sprite(new Mesh());
    //get frame size
    Vec2 sizeFrame=Vec2(frame.z,frame.w);
    //calc relative frame size
    Vec2 scaleFactor=texture->getSpriteSize()/texture->getSize();
    Vec4 rltFrame(frame.x/scaleFactor.x,
                  frame.y/scaleFactor.y,
                  frame.z/scaleFactor.x,
                  frame.w/scaleFactor.y);
    //save size
    sizeFrames.push_back(sizeFrame);
    //upmapping
    Vec2 start(rltFrame.x/texture->getRealWidth(),
               rltFrame.y/texture->getRealHeight());
    Vec2 end(  rltFrame.z/texture->getRealWidth()+start.x,
               rltFrame.w/texture->getRealHeight()+start.y);
    //set size mesh
    Vec2 hlSize(sizeFrame*0.5);
    //add vertexs
    sprite->addVertex(  hlSize.x,
                        -hlSize.y,
                        end.x,
                        end.y);

    sprite->addVertex(  hlSize.x,
                        hlSize.y,
                        end.x,
                        start.y);

    sprite->addVertex( -hlSize.x,
                       -hlSize.y,
                       start.x,
                       end.y);

    sprite->addVertex( -hlSize.x,
                       hlSize.y,
                       start.x,
                       start.y);
    //end add vertexs
    //set draw mode
    sprite->setDrawMode(TRIANGLE_STRIP);
    //build mesh
    sprite->build();
    //add frame
    frames.push_back(sprite);
}
//resource
bool FrameSet::load()
{

    //load file
    void *data=NULL;
    size_t len=0;
    Application::instance()->load_data(rpath,data,len);
    String filestring((char*)data);
    free(data);
    //deserialize frameset
    Table tbFrameSet;
    tbFrameSet.deserialize(filestring);

    //get texture
    if(tbFrameSet.existsAsType("texture",Table::STRING))
    {
        texture=getResourcesManager()->getResourcesGroup()->get<Texture>(tbFrameSet.getString("texture"));
        if(texture==NULL)
        {
            DEBUG_ASSERT_MSG(0,"frameset error:"
                             "must to be setted a valid texture name"
                             "(parameter:texture)");
        }
    }
    else
    {
        DEBUG_ASSERT_MSG(0,"frameset error:"
                         "must to be setted a texture name"
                         "(parameter:texture)");
        return false;
    }
    //load texture
    if(!texture->isLoad()) texture->load();
    //get frames
    if(tbFrameSet.existsAsType("frames",Table::TABLE))
    {
        const Table& tbVertexes=tbFrameSet.getTable("frames");
        for(auto param:tbVertexes)
        {
            if(param.second->asType(Table::VECTOR4D))
            {
                addFrame(param.second->get<Vec4>());
            }
            else
            {
                DEBUG_ASSERT_MSG(0,"frameset error:"
                                 "frame must be only Vec4 (x x x x) "
                                 "(parameter:frames)");
            }
        }
    }
    else if(tbFrameSet.existsAsType("frameSplit",Table::TABLE))
    {
        const Table& frameSplit=tbFrameSet.getTable("frameSplit");

        //get rect size
        Vec4 imgrec(0, 0, texture->getSpriteWidth(), texture->getSpriteHeight());//default
        imgrec=frameSplit.getVector4D("subtexture",imgrec);//load
        //load frame size
        Vec2 sizeframe(frameSplit.getVector2D("frame",Vec2::ZERO));

        int imagewidth=int(imgrec.z-imgrec.x);
        int imageheight=int(imgrec.w-imgrec.y);
        int framewidth=int(sizeframe.x);
        int frameheight=int(sizeframe.y);
        int countXtiles=0;
        int countYtiles=0;
        //calc count
        if(framewidth>0)//no division by 0
            countXtiles=imagewidth/framewidth;//width
        if(frameheight>0)//no division by 0
            countYtiles=imageheight/frameheight;//height
        //calc sub frame
        Vec4 subframe(frameSplit.getVector4D("subframe",Vec4(.0f,.0f,(float)framewidth,(float)frameheight)));
        //set first and count
        int first=Math::max(frameSplit.getFloat("first",1.0)-1.0,0.0);
        int count=frameSplit.getFloat("count",countXtiles*countYtiles);
        int step=Math::max((int)frameSplit.getFloat("step",1),1);

        if(framewidth*frameheight>0 &&  //freme no size 0
                count>0 &&					//frame > 0
                countXtiles>0 && 			//frames line x no size 0
                imagewidth>=framewidth &&	//size(frames)<=size(recimage)
                imageheight>=frameheight		//size(frames)<=size(recimage)
          )
        {
            for(int frm=first; frm<(count+first); frm+=step)
            {
                int posX=(frm%countXtiles)*framewidth;
                int posY=(frm/countXtiles)*frameheight;
                addFrame(Vec4(subframe.x+posX+imgrec.x,
                              subframe.x+posY+imgrec.y,
                              subframe.z,
                              subframe.w));
            }
        }
        else
        {
            DEBUG_ASSERT_MSG(0,"frameset error:"
                               "frameSplit invalid parameters"
                               "(parameters: subtexture, frame, subframe, first, count)");
        }
    }
    else
    {
        DEBUG_ASSERT_MSG(0,"frameset error:"
                           "must to be setted a frames/frameSplit table"
                           "(parameter:frames/frameSplit)");
        return false;
    }

    //set time
    defaultTime=tbFrameSet.getFloat("time",defaultTime);
    defaultLoop=((int)tbFrameSet.getFloat("loop",(float)defaultLoop))!=0;
    //revers frames
    if(((int)tbFrameSet.getFloat("reverse",(float)defaultLoop))!=0)
    {
        std::reverse(frames.begin(),frames.end());
    }
    //is loaded
    loaded=true;
    return true;
}
bool FrameSet::unload()
{
    //realse frame:
    frames.clear();
    //delete texture
    texture=NULL;
    //
    loaded=false;
    return true;
}
Mesh::ptr FrameSet::getFrame(uint i)
{
    DEBUG_ASSERT_MSG(i<frames.size(),"error : frame["<<i<<"] not exist : "<< rpath);
    return frames[i];
}
