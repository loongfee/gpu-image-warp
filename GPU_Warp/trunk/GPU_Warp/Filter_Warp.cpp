#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "FilterWarp.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "ImageIO.h"
#include "FBO.h"
#include "cycle.h"

bool checkErrors();

/* Initialization of the GLUT 
 * Must be run at first */
void GpuImageProcess::FilterWarp::Init()
{
	// create dummy argc and argv
	int myargc(1);
	char* myProg = "myProgexe";
	char** progPtr  = &myProg;

	// initialize the glut ( create hidden window )
	glutInit(&myargc, progPtr);
	glutInitWindowSize(0, 0);	
	glutCreateWindow("Filter Warp");
}

//############################################################################################
/* Warps an image
 * inImage - image for input
 * outImage - image for out put
 * xRot,yRot,zRot - rotation on angles
 * xTrans,yTrans,zTrans
 */
//############################################################################################
bool GpuImageProcess::FilterWarp::Warp(const Image& inImage,	
		Image& outImage,
		float xRot, float yRot, float zRot,
		float xTrans, float yTrans, float zTrans)
{
	FBO_BUFFER fbo;		// the fbo object for offscreen rendering
	ImageIO img;		// that image that will be loaded
	
	ticks t1,t2;		// for test purposes
		
	// get the image type and send it to fbo
	if ( inImage.type == TYPE_UNKNOW)
	{
		std::cerr << "Recieved unknown image type\tTerminating\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Warping "  << std::endl;	
	
	
	t1 = getticks();			
	if ( inImage.type == TYPE_FLOAT_32BPP)	//32
	{
		if (!fbo.init(inImage.width, inImage.height, NULL, 32))
			return false;
	}
	else if ( inImage.type == TYPE_RGB_8BPP)	//24
	{
		if (!fbo.init(inImage.width, inImage.height, NULL, 24))
			return false;
	}
	else if ( inImage.type == TYPE_UC_8BPP) // 8
	{
		if (!fbo.init(inImage.width, inImage.height, NULL, 8))
			return false;
	}
	else
		return false;
	if (!checkErrors())
		return false;
	//##########################################################################################
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,inImage.width, inImage.height);
	gluPerspective(45.0f,(GLfloat)inImage.width/(GLfloat)inImage.height,0.1f,100.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	
	glTranslatef(xTrans,yTrans,zTrans);
	glRotatef(xRot,1.0f,0.0f,0.0f);		//	x
	glRotatef(yRot,0.0f,1.0f,0.0f);		//	y
	glRotatef(zRot,0.0f,0.0f,1.0f);		//  z

	// create texture and load data to it
	GLuint tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	if ( inImage.type == TYPE_UC_8BPP)			// 8
		glTexImage2D(GL_TEXTURE_2D, 0, 1, inImage.width, inImage.height, 0, GL_LUMINANCE , GL_UNSIGNED_BYTE, inImage.begin);
	else if ( inImage.type == TYPE_RGB_8BPP)		//24	
		glTexImage2D(GL_TEXTURE_2D, 0, 3, inImage.width, inImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, inImage.begin);		
	else if ( inImage.type == TYPE_FLOAT_32BPP)	//32
		glTexImage2D(GL_TEXTURE_2D, 0, 1, inImage.width, inImage.height, 0, GL_LUMINANCE , GL_FLOAT, inImage.begin);
	if (!checkErrors())
		return false;
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	if (!checkErrors())
		return false;

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
	if ( inImage.type == TYPE_FLOAT_32BPP)	//32		
		glReadPixels(0,0,inImage.width, inImage.height, GL_LUMINANCE, GL_FLOAT, inImage.begin);
	if ( inImage.type == TYPE_RGB_8BPP)	//24
		glReadPixels(0,0,inImage.width, inImage.height, GL_RGB, GL_UNSIGNED_BYTE, inImage.begin);
	if ( inImage.type == TYPE_UC_8BPP) // 8
		glReadPixels(0,0,inImage.width, inImage.height, GL_LUMINANCE , GL_UNSIGNED_BYTE, inImage.begin);
	if (!checkErrors())
		return false;
	//##########################################################################################
	t2 = getticks();	
	outImage = inImage;
	// clear all the memory
	fbo.clear();
	glDeleteTextures(1,&tex);
	std::cout << "Warp ticks " << elapsed(t2,t1) << std::endl;
	return true;
}
/*******************************************
 * Check for OpenGL errors
 *******************************************/
bool checkErrors()
{
	GLenum err;

	err = glGetError();

	switch ( err )
	{
		case GL_NO_ERROR:
		{
			return true;
		}
		case GL_INVALID_ENUM:
		{
			std::cerr << "OpenGL Error: GL_INVALID_ENUM\n";
			return false;
		}
		case GL_INVALID_VALUE:
		{
			std::cerr << "OpenGL Error: GL_INVALID_VALUE\n";
			return false;
		}
		case GL_INVALID_OPERATION:
		{
			std::cerr << "OpenGL Error: GL_INVALID_OPERATION\n";
			return false;
		}
		case GL_STACK_OVERFLOW:
		{
			std::cerr << "OpenGL Error: GL_STACK_OVERFLOW\n";
			return false;
		}
		case GL_STACK_UNDERFLOW:
		{
			std::cerr << "OpenGL Error: GL_STACK_UNDERFLOW\n";
			return false;
		}
		case GL_OUT_OF_MEMORY:
		{
			std::cerr << "OpenGL Error: GL_OUT_OF_MEMORY\n";
			return false;
		}
	}
	return false;
}