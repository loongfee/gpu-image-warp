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


//############################################################################################
void Filter_Warp_GPU(GpuImageProcess::Image& image, char *outFile, float xRot, float yRot, float zRot)
{
	FBO_BUFFER fbo;		// the fbo object for offscreen rendering
	ImageIO img;		// that image that will be loaded
	GpuImageProcess::Image outImage;
	ticks t1,t2;		// for test purposes
	
	// create dummy argc and argv
	int myargc(1);
	char* myProg = "myProgexe";
	char** progPtr  = &myProg;

	// initialize the glut ( create hidden window )
	glutInit(&myargc, progPtr);
	glutInitWindowSize(0, 0);	
	glutCreateWindow("Filter Warp");

	cout << "Warping "  << endl;	
	
	
	t1 = getticks();			
	fbo.init(image.width, image.height, NULL);
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
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.begin);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

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
	//glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//##########################################################################################
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, img.getImageData());

	outImage = image;
	outImage.begin = malloc(outImage.height*outImage.width* sizeof(GLubyte) * 3);

	glReadPixels(0,0,image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, outImage.begin);	

	t2 = getticks();
	img.root = outImage;
	img.save(outFile);

	// clear all the memory
	fbo.clear();
	glDeleteTextures(1,&tex);

	
	cout << "Warp ticks " << elapsed(t2,t1) << endl;

}
//############################################################################################