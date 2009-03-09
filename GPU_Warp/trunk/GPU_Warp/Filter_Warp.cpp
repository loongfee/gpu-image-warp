#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string>
#include "FilterWarp.h"
#include "ImageIO.h"
#include "FBO.h"
#include "cycle.h"

using namespace std;

void Filter_Warp_GPU(char *fname1, char *outFile, float xRot, float yRot, float zRot)
{
	FBO_BUFFER fbo;
	ImageIO img, img2;
	ticks t1,t2;
	int width,height;
	
	
	cout << "Warping " << fname1 << endl;	
	img.load(fname1);
	img2.load("2.jpg");

	width = img.getWidth();
	height = img.getHeight();
	float vp[16]; 
	glGetFloatv(GL_VIEWPORT, vp); 

	
	t1 = getticks();		
	//fbo.init(img.getWidth(), img.getHeight(), img.getImageData());
	fbo.init(img.getWidth(), img.getHeight(), NULL);
	//##########################################################################################
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,width,height);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();

	
	glTranslatef(0.0f,0.0f,0.0f);
	glRotatef(45.0f,1.0f,0.0f,0.0f);		//x
	glRotatef(45.0f,0.0f,1.0f,0.0f);		//y
	glRotatef(45.0f,0.0f,0.0f,1.0f);		//z

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
	// Restore old view port and set rendering back to default frame buffer
	//glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//##########################################################################################
	
	glReadPixels(0,0,img.getWidth(), img.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, img.getImageData());	

	t2 = getticks();
	img.save(outFile);
	fbo.clear();
	cout << "Warp ticks " << elapsed(t2,t1) << endl;

}