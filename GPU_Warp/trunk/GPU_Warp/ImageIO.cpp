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
bool ImageIO::load(char *fname)
{
	FIBITMAP *img;									// FreeImage type	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;			// by default image format is unknown
	unsigned pixelSize;								// size of each pixel
	FREE_IMAGE_TYPE imageType;						// type of image
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
	root.width = FreeImage_GetWidth(img);
	root.height = FreeImage_GetHeight(img);
	pixelSize = FreeImage_GetBPP(img);
	imageType = FreeImage_GetImageType(img);

	if ( root.width <=0 || root.height <=0 )
	{
		cerr << "Image dimensions are negetive or zero\n";
		return false;
	}
	// define image format
	if ( imageType == FIT_BITMAP )
	{
		if ( pixelSize == 24 )
		{						
			root.type = TYPE_RGB_8BPP;
		}
	}		

	GLubyte *data = (GLubyte*) malloc(root.width * root.height * ( sizeof(GLubyte) * 3));
	unsigned int place=0;
	for (int i=0; i<(root.height); i++)
	{
		for (int j=0; j<(root.width); j++)
		{			
			FreeImage_GetPixelColor(img,j,i, &aPixel);			
			data[place++] = (GLubyte) aPixel.rgbRed;			
			data[place++] = (GLubyte) aPixel.rgbGreen;
			data[place++] = (GLubyte) aPixel.rgbBlue;
		}
	}
	root.begin = (void*)data;
	
	return true;
}
/***************************************************
****************************************************/
bool ImageIO::save(char *fname)
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
	if ( root.width < 0 || root.height < 0 )
	{
		cerr << "Image dimensions incorrect\n";
		return false;
	}
	img = FreeImage_Allocate(root.width, root.height,24,0,0,0);
	if (img == NULL || root.begin== NULL)
	{
		cerr << "Error on allocating memory for image\n";
		return false;
	}		
	// create pointer to picture data
	GLubyte *data;
	data = (GLubyte*)root.begin;
	
	unsigned int place=0;
	for( i=0; i < root.height; i++)
	{
		for (j=0; j < root.width; j++)
		{					
			aPixel.rgbRed   = data[place++]; // red
			aPixel.rgbGreen = data[place++]; // green
			aPixel.rgbBlue  = data[place++]; // blue
			FreeImage_SetPixelColor(img,j,i, &aPixel);
		}
	}
	if (!FreeImage_Save(fif,img,fname,0))
	{
		cerr << "Error on saving image\n";
		return false;
	}
	FreeImage_Unload(img);
	return true;
}
/***************************************************
****************************************************/
ImageIO::~ImageIO()
{
	if ( root.begin != NULL)
	{
		delete(root.begin);
	}
}