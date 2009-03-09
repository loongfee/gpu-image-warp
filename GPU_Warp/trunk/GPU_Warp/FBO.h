#ifndef FBO_H
#undef FBO_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


class FBO_BUFFER
{
public:
	FBO_BUFFER();
	bool init(int width, int height, void *imageData);
	void clear();

	~FBO_BUFFER();
private:
	GLuint fbo;		// FBO handler
	GLuint rb;		// Render buffer
	GLuint stencil;
	GLuint tex;		// texture handler
};



#endif