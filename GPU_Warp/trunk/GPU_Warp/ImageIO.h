#ifndef IMAGEIO_H
#undef IMAGEIO_H
#include "Image.h"


class ImageIO
{
public:
	ImageIO();
	int getWidth();				// get image width
	int getHeight();			
	int getType();
	void *getImageData();		// return pointer to image data

	void setType(int t);
	void setWidth(int w);
	void setHeight(int h);
	bool load(char *fname);
	bool save(char *fname);

	~ImageIO();
private:
	struct GpuImageProcess::Image root;
};
#endif