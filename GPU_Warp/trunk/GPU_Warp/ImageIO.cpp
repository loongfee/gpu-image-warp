#include <iostream>
#include <string>
#include <GL/glut.h>
#include "FreeImage.h"
#include "ImageIO.h"


using namespace std;
using namespace GpuImageProcess;

/***************************************************/
ImageIO::ImageIO()
{
	root.width = 0;
	root.height = 0;
	root.type = TYPE_UNKNOW;
	root.begin = NULL;
}

/***************************************************
****************************************************/
int ImageIO::getHeight()
{
	return root.height;
}
/***************************************************
****************************************************/
int ImageIO::getWidth()
{
	return root.width;
}
/***************************************************
****************************************************/
int ImageIO::getType()
{
	return root.type;
}

/***************************************************
****************************************************/
void *ImageIO::getImageData()
{
	return root.begin;
}
/***************************************************
****************************************************/
void ImageIO::setType(int t)
{
	switch (t)
	{
	case 1:
		root.type = TYPE_RGB_8BPP;
		break;
	case 2:
		root.type = TYPE_FLOAT_32BPP;
		break;
	case 3:
		root.type =	TYPE_UC_8BPP;
		break;
	default:
		root.type = TYPE_UNKNOW;
		break;
	}	
}
/***************************************************
****************************************************/
void ImageIO::setHeight(int h)
{
	root.height = h;
}
/***************************************************
****************************************************/
void ImageIO::setWidth(int w)
{
	root.width = w;
}
/***************************************************
****************************************************/
bool ImageIO::load(const char *fname,GpuImageProcess::Image& image)
{
	FIBITMAP *img;									// FreeImage type	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;			// by default image format is unknown
	unsigned int pixelSize;							// size of each pixel
	FREE_IMAGE_TYPE imageType;						// type of image
	FREE_IMAGE_COLOR_TYPE colorType;				// color type 
	RGBQUAD aPixel;
	

	// try to define image format
	fif = FreeImage_GetFileType(fname,0);	// second parameter unused !
	if ( fif == FIF_UNKNOWN )
	{
		// try to guess image format from it's file name
		fif = FreeImage_GetFIFFromFilename(fname);
		if (fif == FIF_UNKNOWN)
		{
			cerr <<"Image format unknown\n";
			return false;
		}
	}

	// load image
	if ((img = FreeImage_Load(fif, fname, 0)) == NULL)
	{
		cerr << "Error on image loading from file\n";
		return false;
	}
	// get image INFORMATION	
	image.width = FreeImage_GetWidth(img);
	image.height = FreeImage_GetHeight(img);
	pixelSize = FreeImage_GetBPP(img);			// bytes per pixel
	imageType = FreeImage_GetImageType(img);	// image  type 
	colorType = FreeImage_GetColorType(img);	//page 26
	
	if ( image.width <=0 || image.height <=0 )
	{
		cerr << "Image dimensions are negetive or zero\n";
		return false;
	}
	
	//################################## 8 BIT for pixel ###########################################
	if ( imageType == FIT_BITMAP && pixelSize == 8  )
	{
		image.type = TYPE_UC_8BPP;
		unsigned char *data = (unsigned char*) malloc(image.width * image.height * ( sizeof(unsigned char)));
		unsigned int place=0;
		unsigned char *line;		
		for(int y = 0; y < (image.height); y++) 
		{
			line = (BYTE *)FreeImage_GetScanLine(img, y);								
			for(int x = 0; x < image.width; x++) 
			{					
				data[place] = line[x];
				place++;
			}
		}						
		image.begin = (void*)data;	
		return true;
	}
	//################################## 24 BIT for pixel RGB #######################################
	else if ( imageType == FIT_BITMAP && pixelSize == 24 || pixelSize == 32 )
	{						
		image.type = TYPE_RGB_8BPP;
		unsigned char *data = (unsigned char*) malloc(image.width * image.height * ( sizeof(unsigned char) * 3));
		unsigned int place=0;
		for (int i=0; i<(image.height); i++)
		{
			for (int j=0; j<(image.width); j++)
			{			
				FreeImage_GetPixelColor(img,j,i, &aPixel);			
				data[place++] = (GLubyte) aPixel.rgbRed;			
				data[place++] = (GLubyte) aPixel.rgbGreen;
				data[place++] = (GLubyte) aPixel.rgbBlue;
			}
		}
		image.begin = (void*)data;	
		return true;
	}	
	//################################## 32 BIT for pixel FLOAT #######################################
	else if ( imageType == FIT_FLOAT && pixelSize == 32)
	{		
		image.type = TYPE_FLOAT_32BPP;
		float *data = (float*) malloc(image.width * image.height * (sizeof(float)));
		int place=0;
		BYTE *line;
		for (int i=0; i<(image.height); i++)
		{
			line = FreeImage_GetScanLine(img,i);
			for (int j=0; j<(image.width); j++)
			{			
				data[place] = line[j];
				place++;
			}
		}		
		image.begin = (void*)data;
		return true;
	}
	cerr << "This image type can't be handled\n";
	return false;
}
/***************************************************
****************************************************/
bool ImageIO::save(char *fname, GpuImageProcess::Image& image)
{
	FIBITMAP *img;
	RGBQUAD aPixel;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;			// by default image format is unknown
	int i,j;

	fif = FreeImage_GetFIFFromFilename(fname);
	if (fif == FIF_UNKNOWN)
	{
		cerr <<"Wrong image format or unsupported\n";
		return false;
	}
	if (image.begin== NULL)
	{
		cerr << "Error on allocating memory for image\n";
		return false;
	}		

	
	if ( image.width < 0 || image.height < 0 )
	{
		cerr << "Image dimensions incorrect\n";
		return false;
	}

	unsigned int place=0;
	//################################## 8 BIT for pixel ###########################################
	if ( image.type == TYPE_UC_8BPP)
	{
		unsigned char *data = (unsigned char*)image.begin;
		img = FreeImage_AllocateT(FIT_BITMAP,image.width, image.height,8);

		FIBITMAP* temp = img;
		img = FreeImage_ConvertTo8Bits(img);		
		FreeImage_Unload(temp);	

		BYTE *line;					
		for (i=0; i<image.height; i++)
		{				
			line = FreeImage_GetScanLine(img, i);
			for (j=0; j<image.width; j++)
			{
				line[j] = data[place];
				place++;
			}
		}
	}
	//################################## 24 BIT for pixel RGB #######################################
	else if ( image.type == TYPE_RGB_8BPP)
	{
		unsigned char *data = (unsigned char*)image.begin;
		img = FreeImage_Allocate(image.width, image.height,24);

		for( i=0; i < image.height; i++)
		{
			for (j=0; j< image.width; j++)
			{			
				aPixel.rgbRed   = data[place++]; // red
				aPixel.rgbGreen = data[place++]; // green
				aPixel.rgbBlue  = data[place++]; // blue
				FreeImage_SetPixelColor(img,j,i, &aPixel);
			}
		}
	}
	//################################## 24 BIT for pixel RGB #######################################
	else if ( image.type == TYPE_FLOAT_32BPP)
	{
		float *data = (float*)image.begin;
		img = FreeImage_AllocateT(FIT_BITMAP,image.width, image.height,32);
		
		BYTE *line;
		for( i=0; i < image.height; i++)
		{
			line = FreeImage_GetScanLine(img, i);
			for (j=0; j<image.width; j++)
			{
				line[j] = data[place];
				place++;
			}
		}
	}
	else
	{
		cerr << "image of unknown type\n";
		return false;
	}
	// save the image			
	if (!FreeImage_Save(fif,img,fname,0))
	{
		cerr << "Error on saving image\n";
		return false;
	}
	FreeImage_Unload(img);
	return true;
}
/***************************************************
* Destructor
****************************************************/
ImageIO::~ImageIO()
{
	if ( root.begin != NULL)
	{
		delete(root.begin);
	}
}