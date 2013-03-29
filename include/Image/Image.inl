#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include "Image.h"

#ifdef IMAGE_LOADER_OPENGL
	#define TYPE_RGB GL_RGB
	#define TYPE_RGBA GL_RGBA
#else
	#define TYPE_RGB 3
	#define TYPE_RGBA 4
#endif
/*
* HEADERS
*/
/* packaging */
#ifdef _MSC_VER
#define GCCALLINEAMENT 
#else
#define GCCALLINEAMENT __attribute__((__packed__))
#endif 
#ifdef _MSC_VER
 #pragma pack(1)
#endif 
/* TGA 2 HEADERS */
#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10
typedef struct TgaHeader
{
	Image::BYTE  identsize;          // size of ID field that follows 18 byte header (0 usually)
    Image::BYTE  colourmaptype;      // type of colour map 0=none, 1=has palette
    Image::BYTE  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourmapstart;     // first colour map entry in palette
    short colourmaplength;    // number of colours in palette
    Image::BYTE  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    Image::BYTE  bits;               // image bits per pixel 8,16,24,32
    Image::BYTE  descriptor;         // image descriptor bits (vh flip bits)
    
    // pixel data follows header
    
}GCCALLINEAMENT TgaHeader;
/* BITMAP HEADERS */
#define LOCAL_BI_RGB 0
typedef struct  BitmapFileHeader {
  unsigned short bfType;
  unsigned long bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long bfOffBits;
}GCCALLINEAMENT BitmapFileHeader;
typedef struct  BitmapInfoHeader {
  unsigned long biSize;
  long  biWidth;
  long  biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long  biXPelsPerMeter;
  long  biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
}GCCALLINEAMENT BitmapInfoHeader;
/* ICO HEADERS */
typedef struct  IcosHeader{
	unsigned short reserved; // to be 0
	unsigned short icoOrCur; // 1 ico, 2 cur
	unsigned short icoCount; //number of ico
}GCCALLINEAMENT IcosHeader;
typedef struct  IcoHeaderInfo{
	unsigned char width;     // 1 to 255, 0 is 256
	unsigned char height;    // 1 to 255, 0 is 256
	unsigned char npalette;  // number of palette colors (0=no palette color)
	unsigned char reserved;  // to be 0
	unsigned short zcolor;   // to be 1 (this value should be multiplied by the bits per pixel)
	unsigned short bitspixel;//bits per pixel
	unsigned int   size;	 //size of the image's data in bytes
	unsigned int   offset;   //Specifies the offset of BMP or PNG data from the beginning of the ICO/CUR file
}GCCALLINEAMENT IcoHeaderInfo;

#ifdef _MSC_VER
#pragma pack(0)
#endif 

//costruttore
Image::Image():name(""){
     bytes=NULL;
}
Image::Image(const std::string& path):name(""){
     loadImage(path);
}
Image::Image(int width,int height,int bits,bool set_default_color,const rgba& color):name(""){
     makeImage(width,height,bits,set_default_color,color);
}
//distruttore
Image::~Image(){
     clear();
}
//crea un'immagine
void Image::makeImage(int width,int height,int bits,bool set_default_color,const rgba& color){

	this->width=width;
	this->height=height;
	this->channels=bits;

	if(this->channels==4){
		this->type=TYPE_RGBA;
	}else
		this->type=TYPE_RGB;

	this->bytes=(BYTE*)malloc(width * height  * this->channels * sizeof(BYTE));

	if(set_default_color)
	for(int x=0;x<width;++x)
		for(int y=0;y<height;++y)
			this->setPixel(x,y,color);
}
//carica immagini
void Image::loadImage(const std::string& path){

    ////////////////////////////////////
    char tempstring[5] = {0};
	strncpy(tempstring, path.c_str() + path.size()-4, 4);
    char c;
    int i=0;
    while (tempstring[i])
    {
     c=tempstring[i];
     tempstring[i]=tolower(c);
     i++;
    }
    ////////////////////////////////////
	if(!strcmp(tempstring, ".bmp")){
            load_BitMap(this,path);
            this->name=path;
	    }else
	if(!strcmp(tempstring, ".tga")){
            load_TGA(this,path);
            this->name=path;
	}
	
#if defined( IMAGE_LOADER_OPENGL )
	this->flipY();
#endif
#if defined( __IPHONEOS__ )
	this->flipX();
#endif
}
//salvo in un file TGA
void Image::save(const std::string& path){

    ////////////////////////////////////
    char tempstring[5] = {0};
	strncpy(tempstring, path.c_str() + path.size()-4, 4);
    char c;
    int i=0;
    while (tempstring[i])
    {
     c=tempstring[i];
     tempstring[i]=tolower(c);
     i++;
    }
    ////////////////////////////////////
	if(!strcmp(tempstring, ".bmp")){
			save_BMP(this,path);
	}else
	 if(!strcmp(tempstring, ".tga")){
			save_TGA(this,path);
	}

}
//cancella
void Image::clear(){
     if(bytes!=NULL)
         free(bytes);
     bytes=NULL;
     width=0;
     height=0;
}
//restituisci un pixel
Image::rgba Image::getPixel(int x,int y){

      int alpha=255;

      if(channels!=3)
          alpha=(int)bytes[(y*width+x)*4+3]*1;

      return rgba((int)bytes[(y*width+x)*channels]*1,
                  (int)bytes[(y*width+x)*channels+1]*1,
                  (int)bytes[(y*width+x)*channels+2]*1,
                  alpha);

    }
//imposta un pixel
void Image::setPixel(int x,int y,const rgba &pixel){

	   bytes[(y*width+x)*channels]=pixel.r;
       bytes[(y*width+x)*channels+1]=pixel.g;
       bytes[(y*width+x)*channels+2]=pixel.b;
	   if(channels!=3)
			bytes[(y*width+x)*4+3]=pixel.a;

    }
//imposta un immagine/masca alpha
void Image::setAlphaMask(Image* img){

    if(width!=img->width)
    return;
    if(height!=img->height)
    return;

    //alloco la memoria
    BYTE *sorce=bytes;
    BYTE *dest=(BYTE*)malloc(width*height*4 * sizeof(BYTE));

    unsigned long  x,y;
    //ricopio i dati
    for(y=0;y<height;y++)
     for(x=0;x<width;x++){
      dest[(y*width+x)*channels]=sorce[(y*width+x)*channels];
      dest[(y*width+x)*channels+1]=sorce[(y*width+x)*channels+1];
      dest[(y*width+x)*channels+2]=sorce[(y*width+x)*channels+2];
      dest[(y*width+x)*channels+3]=img->bytes[(y*width+x)*channels];
     };
     //cancello la vecchia memoria
     free(sorce);
     //salvo la nuova immagine
     bytes=dest;
     //indico che ha il canale alpha
     channels=4;
     type=TYPE_RGBA;
    }
//imposta colore maschera
void Image::setColorMask(rgba RGBA){

    if(RGBA.a==255&&channels==3)
    return;

    //aloco la memoria
    BYTE *sorce=bytes;
    BYTE *dest=(BYTE*)malloc(width*height*4 * sizeof(BYTE));

    unsigned long  x,y;
    //ricopio i dati
    for(y=0;y<height;y++)
     for(x=0;x<width;x++){
      dest[(y*width+x)*channels]=sorce[(y*width+x)*channels];
      dest[(y*width+x)*channels+1]=sorce[(y*width+x)*channels+1];
      dest[(y*width+x)*channels+2]=sorce[(y*width+x)*channels+2];
      if((dest[(y*width+x)*channels]==RGBA.r)&&
         (dest[(y*width+x)*channels+1]==RGBA.g)&&
         (dest[(y*width+x)*channels+2]==RGBA.b))
      dest[(y*width+x)*channels+3]=RGBA.a;
      else
      dest[(y*width+x)*channels+3]=255;
     };
     //cancello la vecchia memoria
     free(sorce);
     //salvo la nuova immagine
     bytes=dest;
     //indico che ha il canale alpha
     channels=4;
     type=TYPE_RGBA;

    }
//restituisce una normal map
Image::float3 GetNormal( float down, float up, float right, float left ){
	Image::float3 n;
	n.x = (float)(left - right);
	n.y = (float)(down - up);
	//n.z = 2.0f;
	n.z = 1.0f;
	n.normalise();
	return n;
}
Image* Image::getNormalFromHeight(Image* in,float scale){
	int i;
	int width=in->width;
	int height=in->height;
	Image *out=new Image(in->width,in->height,4,false);
	BYTE *datain=in->bytes;
	BYTE *dataout=out->bytes;

	float  *h = new float[height * width];

	if(in->channels==3)
		//#pragma omp parallel for
		for( i = 0; i < height * width; i++ )
			h[i] = ((int)datain[i * 3] + (int)datain[i * 3 + 1] + (int)datain[i * 3 + 2]) / 765.0f * scale;
	
	if(in->channels==4)
		//#pragma omp parallel for
		for( i = 0; i < height * width; i++ )
			h[i] = ((int)datain[i * 4] + (int)datain[i * 4 + 1] + (int)datain[i * 4 + 2]) / 765.0f * scale;

	//#pragma omp parallel for
	for( i = 0; i < height * width; i++ )
	{
		float3  normal;
		int col, row, pos;
		int left, right, up, down;

		row = i / width;
		col = i % width;

		left = col - 1;
		right = col + 1;
		up = row - 1;
		down = row + 1;

		if( left < 0 )
			left = width - 1;
		else if( right >= width )
			right = 0;

		if( up < 0 )
			up = height - 1;
		else if( down >= height )
			down = 0;

		normal = GetNormal( h[down * width + col], h[up * width + col], h[row * width + right], h[row * width + left] );

		pos = i * 4;
		dataout[pos]     = (unsigned char)((normal.x + 1.0f) * 127.5f);
		dataout[pos + 1] = (unsigned char)((normal.y + 1.0f) * 127.5f);
		dataout[pos + 2] = (unsigned char)((normal.z + 1.0f) * 127.5f);
		dataout[pos + 3] = 255;
	}

	delete [] h;
	return out;
}
//restituisci subImage
Image* Image::getImage(Image* surce,int x,int y, int width, int height){


    int _x,_y;
    rgba RGBA;
    //creo la memoria
    BYTE* bytes=(BYTE*)malloc(width*height*surce->channels*sizeof(BYTE));

    //creo una nuova immagine
    Image *out_img=new Image();

    for(_y=y;_y<(height+y);_y++){
        for(_x=x;_x<(width+x);_x++){
            RGBA=surce->getPixel(_x,_y);
            bytes[((_y-y)*width+_x-x)*surce->channels]=(BYTE)RGBA.r;
            bytes[((_y-y)*width+_x-x)*surce->channels+1]=(BYTE)RGBA.g;
            bytes[((_y-y)*width+_x-x)*surce->channels+2]=(BYTE)RGBA.b;
            if(surce->channels==4)
            bytes[((_y-y)*width+_x-x)*surce->channels+3]=(BYTE)RGBA.a;
        }
    }
    //salvo
    out_img->bytes=bytes;
    out_img->width=width;
    out_img->height=height;
    out_img->type=surce->type;
    out_img->channels=surce->channels;

    return out_img;
    }
//restituisce una immagine  G in made alla media di RGB
Image* Image::getAVGImage(Image* surce){

    unsigned int _x,_y;
    rgba RGBA;
    //creo la memoria
    BYTE* bytes=(BYTE*)malloc(surce->width*surce->height*surce->channels*sizeof(BYTE));

    //creo una nuova immagine
    Image *out_img=new Image();

    for(_y=0;_y<(surce->height);_y++){
        for(_x=0;_x<(surce->width);_x++){
            RGBA=surce->getPixel(_x,_y);
            BYTE b_AVG=(RGBA.r+RGBA.g+RGBA.b)/3;
            bytes[(_y*surce->width+_x)*surce->channels]=(BYTE)b_AVG;
            bytes[(_y*surce->width+_x)*surce->channels+1]=(BYTE)b_AVG;
            bytes[(_y*surce->width+_x)*surce->channels+2]=(BYTE)b_AVG;
            if(surce->channels==4)
            bytes[(_y*surce->width+_x)*surce->channels+3]=(BYTE)RGBA.a;
        }
    }
    //salvo
    out_img->bytes=bytes;
    out_img->width=surce->width;
    out_img->height=surce->height;
    out_img->type=surce->type;
    out_img->channels=surce->channels;

    return out_img;
}
//cancella immagine
void Image::freeImage(){
    if(bytes!=NULL)
        free(bytes);
}
//capovolgi immagine
void Image::flipX(){

	BYTE *tmp=new BYTE[this->channels];

	for (unsigned int y=0; y<height; y++) {
		for (unsigned int x=0; x<width/2; x++) {
			for(unsigned int c=0;c<this->channels;c++){
				tmp[c]=bytes[(y*width+x)*this->channels+c];
				bytes[(y*width+x)*this->channels+c]=bytes[(y*width+width-x-1)*this->channels+c];
				bytes[(y*width+width-x-1)*this->channels+c]=tmp[c];
			}
		}
	}

	delete [] tmp;

}
void Image::flipY(){

  unsigned char bTemp;
  unsigned char *pLine1, *pLine2;
  int iLineLen,iIndex;

  iLineLen=width*channels;
  pLine1=bytes;
  pLine2=&bytes[iLineLen * (height - 1)];

   for( ;pLine1<pLine2;pLine2-=(iLineLen*2))
    {
     for(iIndex=0;iIndex!=iLineLen;pLine1++,pLine2++,iIndex++)
      {
       bTemp=*pLine1;
       *pLine1=*pLine2;
       *pLine2=bTemp;
      }
    }


}
void Image::convert16to24bit(bool freebuffer){

	if(channels!=3) return;
	char *bytes16=(char*)bytes;
	unsigned short pixels=0;
	bytes=(BYTE*)malloc(width * height * channels * sizeof(BYTE));
	
	for(int i = 0; i < width * height ; ++i){

		pixels=(bytes16[i*2]<<8);
		pixels|=bytes16[i*2+1];

		bytes[i * 3 + 0] = ((pixels >> 10) & 0x1f) << 3;
		bytes[i * 3 + 1] = ((pixels >> 5) & 0x1f) << 3;
		bytes[i * 3 + 2] = (pixels & 0x1f) << 3;
	}
   //
   if(freebuffer) free(bytes16);
}
void Image::swapRandBbits(){
	for(int i = 0; i < width * height ; ++i){
		bytes[i * channels + 0] = bytes[i * channels + 2];
		bytes[i * channels + 1] = bytes[i * channels + 1];
		bytes[i * channels + 2] = bytes[i * channels + 0];
	}
}
void Image::decoderRLE(bool freebuffer){ 
  //bytes
  BYTE *oldBuffer=(BYTE*)bytes;
  BYTE *pCur=(BYTE*)bytes;
  bytes=(BYTE*)malloc(width * height * channels * sizeof(BYTE));
  //loop data
  unsigned long Index=0;
  unsigned char bLength=0,bLoop=0;  
  size_t imgSize=(this->width*this->height*this->channels);
  // Decode
  while(Index<imgSize){
     if(*pCur & 0x80){
	   // Run length chunk (High bit = 1)
       bLength=*pCur-127; // Get run length
       pCur++;            // Move to pixel data
       // Repeat the next pixel bLength times
       for(bLoop=0;bLoop!=bLength;++bLoop,Index+=channels)
			memcpy(&bytes[Index],pCur,channels);
 
       pCur+=channels; // Move to the next descriptor chunk
     }
	 else{
	   // Raw chunk
       bLength=*pCur+1; // Get run length
       pCur++;          // Move to pixel data
	   // Write the next bLength pixels directly
       for(bLoop=0;bLoop!=bLength;++bLoop,Index+=channels,pCur+=channels)
         memcpy(&bytes[Index],pCur,channels);
      }
   }
   //
   if(freebuffer) free(oldBuffer);
   //
}
void Image::convert32to24bit(bool freebuffer){
	//if 32 bit?
	if(channels!=4) return;	
	//save old buffer
	char *bytes32=(char*)bytes;
	//new buffer
	channels=3;
	bytes=(BYTE*)malloc(width * height * channels * sizeof(BYTE));
	//copy values...
	for(int i=0;i<width * height;++i){
		bytes[i*3+0]=bytes32[i*4+0];
		bytes[i*3+1]=bytes32[i*4+1];
		bytes[i*3+2]=bytes32[i*4+2];
	}
	//now is a RBG
	type = TYPE_RGB ;
	//free alloc
	if(freebuffer) free(bytes32);
}
// openGL bite format
Image::BYTE& Image::pixel(Image::BYTE* bytes,int width,int x,int y,int c){
    return bytes[(y*width+x)*3+c];
    }
// openGL screen save
Image* Image::getImageFromScreen(int width,int height){
	Image *out_img=new Image();
	out_img->width=width;
	out_img->height=height;
    out_img->channels=4;
    out_img->type=TYPE_RGBA;
    out_img->bytes=(BYTE*)malloc(width * height * 4);  ;
	#ifdef IMAGE_LOADER_OPENGL
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, out_img->bytes);
	#endif
	return out_img;
}
// Carica un'immagine TGA
void Image::loadBuffer_TGA(Image* img,BYTE *buffer,size_t bfsize){	
	TgaHeader *tgaHeader=(TgaHeader *)buffer;
	BYTE *dataImage=(BYTE*)(buffer+sizeof(TgaHeader));
	
	img->width=tgaHeader->width;
	img->height=tgaHeader->height;
	img->channels=tgaHeader->bits/8;

	if(tgaHeader->imagetype!=TGA_RLE){
		img->bytes=(BYTE*)malloc(img->width * img->height * img->channels * sizeof(BYTE));
		memcpy(img->bytes,dataImage,bfsize-sizeof(TgaHeader));
	}
	else{ //RLE LOOK LIKE N[RGB] N-> numbers repeat value rgb 
		img->bytes=dataImage;
		img->decoderRLE(false);
	}
	if(tgaHeader->bits==16){
		img->channels=3;
		img->convert16to24bit();
	}
	img->type  =img->channels==4? TYPE_RGBA : TYPE_RGB ;
	img->swapRandBbits();
}
void Image::load_TGA(Image* img,const std::string& path){
	
	FILE* pfile=fopen(path.c_str(),"rb");
	if(pfile){
		fseek(pfile,0,SEEK_END);
		size_t lengfile=ftell(pfile);
		fseek(pfile,0,SEEK_SET);
		BYTE *buffer=(BYTE*)malloc(lengfile);
		fread(buffer,lengfile,1,pfile);
		loadBuffer_TGA(img,buffer,lengfile);
		free(buffer);
	}

}
// salva un immagine in formato TGA (32-24bit)
void Image::saveBuffer_TGA(Image* img,BYTE*& buffer,size_t& bfsize){
	//SIZE/ALLOC
	bfsize=sizeof(TgaHeader)+img->width*img->height*img->channels;
	buffer=(BYTE*)malloc(bfsize);
	memset(buffer,0,bfsize);
	//HEADER
	TgaHeader *tgaHeader=(TgaHeader *)buffer;
	tgaHeader->width=img->width;
	tgaHeader->height=img->height;
	tgaHeader->bits=img->channels*8;
	tgaHeader->imagetype=TGA_RGB;
	//IMAGE
	img->swapRandBbits();
	memcpy(&buffer[sizeof(TgaHeader)],img->bytes,bfsize-sizeof(TgaHeader));
	img->swapRandBbits();
}
void Image::save_TGA(Image* img,const std::string& path){

	////////////////////////////////
	FILE *pfile;
	pfile = fopen(path.c_str(), "wb");
	if(pfile){
		BYTE* buffer;
		size_t bfsize;
		saveBuffer_TGA(img,buffer,bfsize);
		fwrite(buffer,bfsize,1,pfile);
		free(buffer);
		fclose(pfile);
	}
	////////////////////////////////
}
//loadImage BitMap
void Image::loadBuffer_BitMap(Image* img,BYTE *buffer,size_t bfsize){
	/* file header */
	BitmapFileHeader* bFH=(BitmapFileHeader*)buffer;
	if((bFH->bfType>>8)!='M' || (bFH->bfType&0x00FF)!='B') return ;
	/* bitmap header */
	BitmapInfoHeader* bIH=(BitmapInfoHeader*)(buffer+sizeof(BitmapFileHeader));
	img->width=bIH->biWidth;
	img->height=bIH->biHeight;
	img->channels=bIH->biBitCount / 8;
	BYTE* dataImage=(BYTE*)(buffer+bFH->bfOffBits);

	if(bIH->biCompression  == LOCAL_BI_RGB){
		if(bIH->biBitCount == 16){
			img->channels= 3;
			img->bytes=dataImage;
			img->convert16to24bit(false);
			img->swapRandBbits();
		}
		else if(bIH->biBitCount >= 24){		
			img->bytes=(BYTE*)malloc(img->width * img->height * img->channels * sizeof(BYTE));
			memcpy(img->bytes,dataImage,img->width * img->height * img->channels * sizeof(BYTE));
			img->swapRandBbits();
		}else return ;
	}else return ;
	
	img->type  =img->channels==4? TYPE_RGBA : TYPE_RGB ;
}
void Image::load_BitMap(Image* img,const std::string& path){
	FILE* pfile=fopen(path.c_str(),"rb");
	if(pfile){
		fseek(pfile,0,SEEK_END);
		size_t lengfile=ftell(pfile);
		fseek(pfile,0,SEEK_SET);
		BYTE *buffer=(BYTE*)malloc(lengfile);
		fread(buffer,lengfile,1,pfile);
		loadBuffer_BitMap(img,buffer,lengfile);
		free(buffer);
	}
 }
// salva un immagine in formato BMP (32-24bit)
void Image::saveBuffer_BMP(Image* img,BYTE*& buffer,size_t& bfsize){
	//ALLOC
    bfsize=sizeof(BitmapFileHeader)+
		   sizeof(BitmapInfoHeader)+
		   img->width*
		   img->height*
		   img->channels;
	buffer=(BYTE*)malloc(bfsize);
	memset(buffer,0,bfsize);
	//headers
	BitmapFileHeader *bmfh=(BitmapFileHeader*)buffer;
	BitmapInfoHeader *bmih=(BitmapInfoHeader*)(buffer+sizeof(BitmapFileHeader));
	//memory data
	size_t imgSize=img->width*img->height*img->channels;
	BYTE* imageData=(BYTE*)(buffer+sizeof(BitmapFileHeader)+sizeof(BitmapInfoHeader));
	//write file header
	bmfh->bfType      = 'M'<<8;
	bmfh->bfType     |= 'B'; //MB
	bmfh->bfReserved1 = 0;
	bmfh->bfReserved2 = 0;
	bmfh->bfOffBits   = sizeof(BitmapFileHeader)+sizeof(BitmapInfoHeader); //size hader
	bmfh->bfSize      = bfsize;
	//write bitmap header
	bmih->biSize          = sizeof(BitmapInfoHeader); //size header info
	bmih->biHeight        = img->height;
	bmih->biWidth         = img->width;
	bmih->biPlanes        = 1;
	bmih->biCompression   = LOCAL_BI_RGB; //IS RGB (No Compression)
	bmih->biBitCount	  = img->channels * 8 ; //pixel format (24, or 32)
	bmih->biSizeImage     = imgSize;
	bmih->biXPelsPerMeter = 3780;
	bmih->biYPelsPerMeter = 3780;
	bmih->biClrUsed       = 0;
	bmih->biClrImportant  = 0;
	//save image
	img->swapRandBbits();
	memcpy(imageData,img->bytes,imgSize);
	img->swapRandBbits();
}
void Image::save_BMP(Image* img,const std::string& path){

	////////////////////////////////
	FILE *pfile;
	pfile = fopen(path.c_str(), "wb");
	if(pfile){
		BYTE* buffer;
		size_t bfsize;
		saveBuffer_BMP(img,buffer,bfsize);
		fwrite(buffer,bfsize,1,pfile);
		free(buffer);
		fclose(pfile);
	}
	////////////////////////////////
}

//UNDEF
#ifdef GCCALLINEAMENT
#undef GCCALLINEAMENT
#endif

#ifdef TYPE_RGB
#undef TYPE_RGB
#endif
#ifdef TYPE_RGBA
#undef TYPE_RGBA
#endif

#ifdef TGA_RGB
#undef TGA_RGB
#endif
#ifdef TGA_A
#undef TGA_A
#endif
#ifdef TGA_RLE
#undef TGA_RLE
#endif

#ifdef LOCAL_BI_RGB
#undef LOCAL_BI_RGB
#endif