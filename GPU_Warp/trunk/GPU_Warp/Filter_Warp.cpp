#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string>
#include "FilterWarp.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "ImageIO.h"
#include "FBO.h"
#include "cycle.h"


using namespace std;
using namespace GpuImageProcess;

//############################################################################################
void checkErrors();
//############################################################################################
void Filter_Warp_GPU(GpuImageProcess::Image& image, char *outFile, float xRot, float yRot, float zRot)
{
	FBO_BUFFER fbo;		// the fbo object for offscreen rendering
	ImageIO img;		// that image that will be loaded
	
	ticks t1,t2;		// for test purposes
	
	// create dummy argc and argv
	int myargc(1);
	char* myProg = "myProgexe";
	char** progPtr  = &myProg;

	// initialize the glut ( create hidden window )
	glutInit(&myargc, progPtr);
	glutInitWindowSize(0, 0);	
	glutCreateWindow("Filter Warp");

	
	// get the image type and send it to fbo
	if ( image.type == TYPE_UNKNOW)
	{
		cerr << "Recieved unknown image type\tTerminating\n";
		exit(EXIT_FAILURE);
	}
	cout << "Warping "  << endl;	
	
	
	t1 = getticks();			
	if ( image.type == TYPE_FLOAT_32BPP)	//32
		fbo.init(image.width, image.height, NULL, 32);
	else if ( image.type == TYPE_RGB_8BPP)	//24
		fbo.init(image.width, image.height, NULL, 24);
	else if ( image.type == TYPE_UC_8BPP) // 8
		fbo.init(image.width, image.height, NULL, 8);	
	checkErrors();
	//##########################################################################################
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,image.width, image.height);
	gluPerspective(45.0f,(GLfloat)image.width/(GLfloat)image.height,0.1f,100.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	
	glTranslatef(0.0f,0.0f,0.0f);
	glRotatef(xRot,1.0f,0.0f,0.0f);		//	x
	glRotatef(yRot,0.0f,1.0f,0.0f);		//	y
	glRotatef(zRot,0.0f,0.0f,1.0f);		//  z

	// create texture and load data to it
	GLuint tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	if ( image.type == TYPE_UC_8BPP)			// 8
		glTexImage2D(GL_TEXTURE_2D, 0, 1, image.width, image.height, 0, GL_COLOR_INDEX , GL_UNSIGNED_BYTE, image.begin);
	else if ( image.type == TYPE_RGB_8BPP)		//24	
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.begin);		
	else if ( image.type == TYPE_FLOAT_32BPP)	//32
		glTexImage2D(GL_TEXTURE_2D, 0, 1, image.width, image.height, 0, GL_RED , GL_FLOAT, image.begin);
	checkErrors();
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	checkErrors();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	// create quad and put the texture on it
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//##########################################################################################	
	if ( image.type == TYPE_FLOAT_32BPP)	//32
		glReadPixels(0,0,image.width, image.height, GL_RED, GL_FLOAT, image.begin);
	if ( image.type == TYPE_RGB_8BPP)	//24
		glReadPixels(0,0,image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.begin);
	if ( image.type == TYPE_UC_8BPP) // 8
		glReadPixels(0,0,image.width, image.height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, image.begin);
	checkErrors();
	//##########################################################################################
	t2 = getticks();
	img.save(outFile,image);

	// clear all the memory
	fbo.clear();
	glDeleteTextures(1,&tex);
	cout << "Warp ticks " << elapsed(t2,t1) << endl;

}
/*******************************************
 * Check for OpenGL errors
 *******************************************/
void checkErrors()
{
	GLenum err;

	err = glGetError();

	switch ( err )
	{
		case GL_NO_ERROR:
		{
			break;
		}
		case GL_INVALID_ENUM:
		{
			cerr << "OpenGL Error: GL_INVALID_ENUM\n";
			break;
		}
		case GL_INVALID_VALUE:
		{
			cerr << "OpenGL Error: GL_INVALID_VALUE\n";
			break;
		}
		case GL_INVALID_OPERATION:
		{
			cerr << "OpenGL Error: GL_INVALID_OPERATION\n";
			break;
		}
		case GL_STACK_OVERFLOW:
		{
			cerr << "OpenGL Error: GL_STACK_OVERFLOW\n";
			break;
		}
		case GL_STACK_UNDERFLOW:
		{
			cerr << "OpenGL Error: GL_STACK_UNDERFLOW\n";
			break;
		}
		case GL_OUT_OF_MEMORY:
		{
			cerr << "OpenGL Error: GL_OUT_OF_MEMORY\n";
			break;
		}
	}
}