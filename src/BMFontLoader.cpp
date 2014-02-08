#include <stdafx.h>
#include <BMFontLoader.h>
#include <Application.h>
#include <Debug.h>
#include <Texture.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
///////////////////////
using namespace Easy2D;
///////////////////////

bool BMFontLoader::load(Font& font,const Utility::Path& fontPath){
    //load font
	void *voidFntBuffer=NULL;
	size_t fntLen=0;
	Application::instance()->loadData(fontPath,voidFntBuffer,fntLen);
	//save info ptr
	char *startFntBuffer,*fntBuffer;
	startFntBuffer=fntBuffer=(char*)voidFntBuffer;


	ASPACKED(struct BlockSize{
				  uchar type;
				  int size;
			  });

	ASPACKED(struct BlockInfo{
			  bit16 fontSize;
			  bit8  bitField;
			  bit8  charSet;
			  bit16 stretchH;
			  bit8  aa;
			  bit8  paddingUp;
			  bit8  paddingRight;
			  bit8  paddingDown;
			  bit8  paddingLeft;
			  bit8  spacingHoriz;
			  bit8  spacingVert;
			  bit8  outline;
			  char fontName[1];//first char
			});

	ASPACKED(struct BlockCommon{
			  bit16 lineHeight;
			  bit16 base;
			  bit16 scaleW;
			  bit16 scaleH;
			  bit16 pages;
			  bit8 bitField;
			  bit8 alphaChnl;
			  bit8 redChnl;
			  bit8 greenChnl;
			  bit8 blueChnl;
			});

	ASPACKED(struct BlockPage{
				char fontName[1];//first char
			});
	ASPACKED(struct BlockChars{
				struct charInfo{
					bit32  id;
					bit16  x;
					bit16  y;
					bit16  width;
					bit16  height;
					short xoffset;
					short yoffset;
					short xadvance;
					bit8  page;
					bit8  chnl;
				} chars[1];
			  });

	ASPACKED(struct BlocKerningPairs{
		struct {
			bit32 first;
			bit32 second;
			short amount;
		}kerningPairs[1];
	});

	#define readBlockSize(bufferPtr,blockPtr)\
		blockPtr=(BlockSize*)bufferPtr; bufferPtr+=sizeof(BlockSize);

	#define readBlockInfo(bufferPtr,blockPtr,size)\
		blockPtr=(BlockInfo*)bufferPtr; bufferPtr+=size;
	
	#define readBlockCommon(bufferPtr,blockPtr,size)\
		blockPtr=(BlockCommon*)bufferPtr; bufferPtr+=size;

	#define readBlocKerningPairs(bufferPtr,blockPtr,size)\
		blocKerningPairs=(BlocKerningPairs*)bufferPtr; bufferPtr+=size;

	BlockSize* blockSize;
	BlockInfo* blockInfo;
	BlockCommon* blockCommon;
	BlockChars *blockChar;
	BlocKerningPairs *blocKerningPairs;

	//versione file
	DEBUG_ASSERT(
	fntBuffer[0]=='B'&&
	fntBuffer[1]=='M'&&
	fntBuffer[2]=='F'&&
	fntBuffer[3]==3);
	fntBuffer+=4;

	while(size_t(fntBuffer-startFntBuffer)<fntLen){
		readBlockSize(fntBuffer,blockSize)
		switch (blockSize->type)
		{
			case 1: //info
				readBlockInfo(fntBuffer,blockInfo,blockSize->size)
				font.setSize(blockInfo->fontSize);
				font.setName(blockInfo->fontName);
			break;
			case 2: //common
				readBlockCommon(fntBuffer,blockCommon,blockSize->size)
			break;
			case 3: { //image filename
				char *localFntBuffer=fntBuffer;
				while(size_t(localFntBuffer)<(size_t)(fntBuffer+blockSize->size)){
					//load image
					String filename(localFntBuffer);
					Texture* pageTex=new Texture(&font.getResourcesManager()->getResourcesGroup()->getManager<Texture>(),
                                                  fontPath.getDirectory()+"/"+filename);
					pageTex->bilinear(false);
					pageTex->mipmaps(false);
					pageTex->flipVertical(true);
					pageTex->load();
					font.addPage(Texture::ptr(pageTex));
					//next string
					localFntBuffer+=strlen(localFntBuffer)+1;
				}
				fntBuffer+=blockSize->size;
			}
			break;
			case 4: {  // chars info
				int n=0;
				blockChar=(BlockChars*)fntBuffer;
				while((n*sizeof(BlockChars))<blockSize->size){
					Font::Character* chr=new Font::Character();

					chr->srcX=blockChar->chars[n].x;
					chr->srcY=blockChar->chars[n].y;
					chr->srcW=blockChar->chars[n].width;
					chr->srcH=blockChar->chars[n].height;
					chr->page=blockChar->chars[n].page;

					chr->xAdv=blockChar->chars[n].xadvance;
					chr->xOff=blockChar->chars[n].xoffset;
					chr->yOff=blockChar->chars[n].yoffset;

					chr->chnl=blockChar->chars[n].chnl;

					font.addCharacter(blockChar->chars[n].id,chr);
					++n;
				}
				fntBuffer+=blockSize->size;
			}
			break;
			case 5:{ // kerning pairs	
				int n=0;
				blocKerningPairs=(BlocKerningPairs*)fntBuffer;
				while((n*sizeof(BlocKerningPairs))<blockSize->size){
					font.pushAKerningPairs(blocKerningPairs->kerningPairs[n].first,
										   blocKerningPairs->kerningPairs[n].second,
										   blocKerningPairs->kerningPairs[n].amount);
					++n;
				}
				fntBuffer+=blockSize->size;
			}
			break;
			default:
				fntBuffer+=blockSize->size;
			break;
		}
	}

	return true;
}
