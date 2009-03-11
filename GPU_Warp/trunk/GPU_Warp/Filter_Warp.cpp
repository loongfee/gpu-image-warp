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
void Filter_Warp_GPU(int* argc, char** argv,char *fname1, char *outFile, float xRot, float yRot, float zRot)
{
	FBO_BUFFER fbo;		// the fbo object for offscreen rendering
	ImageIO img;		// that image that will be loaded
	ticks t1,t2;		// for test purposes
	
	// initialize the glut ( create hidden window )
	glutInit(argc, argv);
	glutInitWindowSize(0, 0);	
	glutCreateWindow("Filter Warp");

	cout << "Warping " << fname1 << endl;	
	img.load(fname1);		
	
	t1 = getticks();			
	fbo.init(img.getWidth(), img.getHeight(), NULL);
	//##########################################################################################
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,img.getWidth(), img.getHeight());
	gluPerspective(45.0f,(GLfloat)img.getWidth()/(GLfloat)img.getHeight(),0.1f,100.0f);
	
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
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getImageData());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	// create quad and put the texture on it
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();	
	//glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//##########################################################################################
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, img.getImageData());
	glReadPixels(0,0,img.getWidth(), img.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, img.getImageData());	

	t2 = getticks();
	img.save(outFile);

	// clear all the memory
	fbo.clear();
	glDeleteTextures(1,&tex);

	
	cout << "Warp ticks " << elapsed(t2,t1) << endl;

}
//############################################################################################