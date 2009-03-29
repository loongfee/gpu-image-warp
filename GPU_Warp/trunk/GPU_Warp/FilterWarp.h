#ifndef FILTERWARP_H
#define FILTERWARP_H

namespace GpuImageProcess
{
	struct Image;
}

namespace GpuImageProcess
{
	class FilterWarp
	{
	public:		
		// Call fbo init
		void Init();


		// Do The Warping
		bool Warp(
			const Image& inImage,						// input image
			Image& outImage,							// output image
			float xRot, float yRot, float zRot,			// image rotation
			float xTrans, float yTrans, float zTrans);	// image translation
	};
}
#endif