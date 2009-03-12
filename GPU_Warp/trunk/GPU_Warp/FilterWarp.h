#ifndef FILTERWARP_H
#define FILTERWARP_H

namespace GpuImageProcess
{
	struct Image;
}

/* Warps an image with given rotation */
void Filter_Warp_GPU(GpuImageProcess::Image& image, char *outFile, float xRot, float yRot, float zRot);

#endif