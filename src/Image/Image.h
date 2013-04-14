#ifndef IMAGE_H
#define IMAGE_H
/**********************************************************************************
* @autor: Gabriele Di Bari
* @version 0.5
*
* MIT LICENSE
*
* Copyright (c) 2012 Gabriele Di Bari
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
**********************************************************************************/
#include <string>


/********************
* class Image
********************/
class Image{

	public:		
		/* image type */
		enum ImageType{
			NONE,PNG,JPEG,TGA,BMP
		};
		/* pixel structure */
		struct rgba{

			unsigned char r,g,b,a;
			rgba(){r=g=b=a=255;}
			rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a):r(r),g(g),b(b),a(a){}
			void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
				this->r=r; this->g=g; this->b=b; this->a=a;
			}
			rgba operator + (const rgba& v){ return (rgba(r+v.r,g+v.g,b+v.b,a+v.a)); }
			rgba operator - (const rgba& v){ return (rgba(r-v.r,g-v.g,b-v.b,a-v.a)); }
			operator const unsigned char* () const {return (const unsigned char*) this;}
			operator unsigned char* () const {return (unsigned char*) this;}
		};
		/* data structure */
		typedef unsigned char BYTE;
		/* vector */
		struct float3{ 
			float x,y,z; 
			float3():x(0),y(0),z(0){} 
			float3(float x,float y,float z):x(x),y(y),z(z){} 
			void normalise(){
				float d=sqrtf(x*x+y*y+z*z);
				x/=d; y/=d; z/=d;
			}
		};

		unsigned long width;            ///< image width in pixels  
		unsigned long height;           ///< image height in pixels 
		unsigned int type;				///< openGL image type...   
		unsigned int channels;			///< count bytes per pixel  
		BYTE *bytes;					///< image data             
		std::string name;               ///< image name             

		//constructors
		Image();
		Image(const std::string& path);
		Image(int width,
			  int height,
			  int bits,
			  bool set_default_color=false,
			  const rgba& color=rgba(255,255,255,255));
		//destructor
		~Image();
		//create a image				
		void makeImage(int width,
					   int height,
					   int bits,
					   bool set_default_color=false,
					   const rgba& color=rgba(255,255,255,255));
		//return image type from a string estetion
		static ImageType getTypeFromExtetion(const std::string& ext);
		//load a image from file
		void loadImage(const std::string& path);
		void loadFromData(void *data,unsigned int size,ImageType type=BMP);
		//save in a file	
		void save(const std::string& path);
		//clear
		void clear();
		//get a pixel
		rgba getPixel(int x,int y);
		//imposta un pixel
		void setPixel(int x,int y,const rgba &pixel);
		//set alpha channel from a B/W immage
		void setAlphaMask(Image* img);
		//set mask color (Alpha channel)
		void setColorMask(rgba RGBA);
		//return a normal map from height map
		static Image* getNormalFromHeight(Image* surce,float scale=1.0f);
		//returna a sub immage
		static Image* getImage(Image* surce,int x,int y, int width, int height);
		//return a avb image (pixel=(pixel.r+pixel.g+pixel.b)/3)
		static Image* getAVGImage(Image* surce);
		//free image
		void freeImage();
		//change image byte data
		void flipX();
		void flipY();
		void convert16to24bit(bool freebuffer=true);
		void swapRandBbits();
		void decoderRLE(bool freebuffer=true);
		void convert32to24bit(bool freebuffer=true);		
		//scale image:
		void scale(unsigned int width,unsigned int height);
		static void scaleLine(BYTE *source,
							  int srcWidth,
							  short srcChannel,
							  BYTE *target,
							  int tgtWidth,
							  short tgtChannel);
		// return openGL bite format
		static BYTE& pixel(BYTE* bytes,int width,int x,int y,int c);
		// save a openGL screen 
		static Image* getImageFromScreen(int width,int height);
		// load tga image
		static void loadBuffer_TGA(Image* img,BYTE *buffer,size_t bfsize);
		static void load_TGA(Image* img,const std::string& path);
		// save image in tga format
		static void saveBuffer_TGA(Image* img,BYTE*& buffer,size_t& bfsize);
		static void save_TGA(Image* img,const std::string& path);
		// load BitMap image
		static void loadBuffer_BMP(Image* img,BYTE *buffer,size_t bfsize);
		static void load_BMP(Image* img,const std::string& path);
		// save image in BitMap format
		static void saveBuffer_BMP(Image* img,BYTE*& buffer,size_t& bfsize);
		static void save_BMP(Image* img,const std::string& path);
		// load Png image
		static void loadBuffer_PNG(Image* img,BYTE *buffer,size_t bfsize);
		static void load_PNG(Image* img,const std::string& path);


};

#include "Image.inl"

#endif
