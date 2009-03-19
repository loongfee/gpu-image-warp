#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "FilterWarp.h"
#include "ImageIO.h"

#define MAX_FILE_NAME 255

using namespace std;

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
	GpuImageProcess::Image image;
	ImageIO::load(f_in,image);		

	Filter_Warp_GPU(image,f_out,xRot,yRot,zRot);

	exit(EXIT_SUCCESS);
}
//############################################################
void readArguments(int argc, char **argv)
{
	if ( argc!=6 )
	{
		cout << "Usage <input1> <output> <x-rotation> <y-rotation> <z-rotation>\n";
		exit(EXIT_FAILURE);
	}
	
	strcpy_s(f_in,argv[1]);
	strcpy_s(f_out,argv[2]);	
	xRot = (float) atof(argv[3]);
	yRot = (float) atof(argv[4]);
	zRot = (float) atof(argv[5]);	
}
//############################################################
