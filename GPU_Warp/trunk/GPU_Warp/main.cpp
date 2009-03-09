#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "FilterWarp.h"

#define MAX_FILE_NAME 255

using namespace std;

// file name from arguments
char f_in[MAX_FILE_NAME];
char f_out[MAX_FILE_NAME];

//###################################################
void readArguments(int argc, char **argv);
//###################################################

//###################################################
int main(int argc, char **argv)
{
	readArguments(argc,argv);
	glutInit(&argc, argv);
	glutInitWindowSize(0, 0);	
	glutCreateWindow("Filter Warp");

	Filter_Warp_GPU(f_in,f_out,15,0,0);
	


	exit(EXIT_FAILURE);
}

//############################################################
void readArguments(int argc, char **argv)
{
	if ( argc!=3 )
	{
		cout << "Usage <input1> <output>\n";
		exit(EXIT_FAILURE);
	}
	strcpy(f_in,argv[1]);	
	strcpy(f_out,argv[2]);
}

