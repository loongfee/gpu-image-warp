#ifndef IMAGEIO_H
#undef IMAGEIO_H
#include "Image.h"


class ImageIO
{
public:
	ImageIO();
	
	static bool save(char *fname, GpuImageProcess::Image& image);
	static bool load(const char *fname, GpuImageProcess::Image& image);
	static bool clamp( GpuImageProcess::Image& floatimage);
	static bool floatToUc(const GpuImageProcess::Image& floatimage, GpuImageProcess::Image& ucimage);

	~ImageIO();

	struct GpuImageProcess::Image root;
};
#endif