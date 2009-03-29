#include "FBO.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

bool check_FBO_errors();

//########################################################################
GpuImageProcess::FBO_BUFFER::FBO_BUFFER()
{

}
//########################################################################
bool GpuImageProcess::FBO_BUFFER::init(int width, int height, void *imageData, unsigned char draw_format)
{		
	glewInit();
	glGenFramebuffersEXT(1, &fbo); 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); 

	// Init texture 
	glGenTextures(1, &tex); 

	glBindTexture(GL_TEXTURE_2D, tex); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 

	if ( draw_format == 8)
		glTexImage2D(GL_TEXTURE_2D,0,1,width,height,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,imageData);
	else if ( draw_format == 24)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,imageData);
	else if ( draw_format == 32)
		glTexImage2D(GL_TEXTURE_2D,0,1,width,height,0,GL_LUMINANCE,GL_FLOAT,imageData);
 	   	   	   	

	 // Init render buffer (for depth) 
	glGenRenderbuffersEXT(1, &rb); 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);    
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, width, height);        
   	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);	
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);	
	if (!check_FBO_errors())
		return false;
	return true;
}
//########################################################################
bool check_FBO_errors()
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:			
			std::cerr << "Unsupported extension\n";
			return false;
		default:
			std::cerr << "FBO error\n";			
			return false;
	}
	return false;
}
//########################################################################
void GpuImageProcess::FBO_BUFFER::clear()
{
	// free the memory
	glDeleteRenderbuffersEXT(1, &rb);
	glDeleteFramebuffersEXT(1, &fbo);
	glDeleteTextures(1,&tex);
}
//########################################################################
GpuImageProcess::FBO_BUFFER::~FBO_BUFFER()
{

}
//########################################################################