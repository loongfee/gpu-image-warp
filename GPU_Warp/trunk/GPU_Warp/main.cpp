#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "FilterWarp.h"
#include "ImageIO.h"

#define MAX_FILE_NAME 255


// file name from arguments
char f_in[MAX_FILE_NAME];
char f_out[MAX_FILE_NAME];
float xRot, yRot, zRot;

//###################################################
void readArguments(int argc, char **argv);
//###################################################

//###################################################
int main(int argc, char **argv)
{
	
	readArguments(argc,argv);	
	GpuImageProcess::Image image, outImage;
	GpuImageProcess::FilterWarp warp;
	ImageIO::load(f_in,image);	

	if (image.type == GpuImageProcess::TYPE_FLOAT_32BPP)
		ImageIO::clamp(image);
	
	warp.Init();
	warp.Warp(image,outImage,xRot,yRot,zRot,0.0f,0.0f,0.0f);

	if (outImage.type == GpuImageProcess::TYPE_FLOAT_32BPP)
	{
		GpuImageProcess::Image ucImage;
		ImageIO::floatToUc(outImage,ucImage);
		ImageIO::save(f_out,ucImage);
	}
	else
	{
		ImageIO::save(f_out,outImage);
	}
	exit(EXIT_SUCCESS);
}
//############################################################
void readArguments(int argc, char **argv)
{
	if ( argc!=6 )
	{
		std::cout << "Usage <input1> <output> <x-rotation> <y-rotation> <z-rotation>\n";
		exit(EXIT_FAILURE);
	}	
	strcpy_s(f_in,argv[1]);
	strcpy_s(f_out,argv[2]);	
	xRot = (float) atof(argv[3]);
	yRot = (float) atof(argv[4]);
	zRot = (float) atof(argv[5]);	
}
//############################################################
