#include "FBO.h"

void check();

//########################################################################
FBO_BUFFER::FBO_BUFFER()
{

}

//########################################################################
bool FBO_BUFFER::init(int width, int height, void *imageData)
{
   // at first Must to initialize!!!
   glewInit();

	//glEnable(GL_DEPTH_TEST);			
	//glDepthFunc(GL_LEQUAL);

   glGenFramebuffersEXT(1, &fbo); 
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); 

   // Init texture 
   glGenTextures(1, &tex); 

   glBindTexture(GL_TEXTURE_2D, tex); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,imageData);
   	   	   	   	

   // Init render buffer (for depth) 
   glGenRenderbuffersEXT(1, &rb); 
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb); 




   //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, width, height);     
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, width, height);     
   
   //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0); 

   //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb); 

	//============================================================================
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
	
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);
	//============================================================================

	
	check();	
	
	//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);		// set main draw color buffer	
	return true;
}
//########################################################################
void check()
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "FBO OK\n";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:			
			std::cerr << "Unsupported extension\n";
			break;
		default:
			std::cerr << "FBO error\n";			
			exit(0);
	}
}
//########################################################################
void FBO_BUFFER::clear()
{
	// free the memory
	glDeleteRenderbuffersEXT(1, &rb);
	glDeleteFramebuffersEXT(1, &fbo);
	glDeleteTextures(1,&tex);
}
//########################################################################
FBO_BUFFER::~FBO_BUFFER()
{

}
//########################################################################