#ifndef IMAGE_H
#define IMAGE_H


namespace GpuImageProcess
{	
	/* These are three types of images that we 
	 * are working with
	 */
	enum IMAGE_TYPE
	{
		TYPE_UNKNOW = -1,			// unknown image type
		TYPE_RGB_8BPP = 1,			// 8 bytes per r,g,b (8*3)
		TYPE_FLOAT_32BPP = 2,		// 32 bytes per RED
		TYPE_UC_8BPP = 3			// 8 bytes unsigned char
	};

	/* Structure of our image
	*/
	struct Image
	{
		void *begin;
		int width;
		int height;
		IMAGE_TYPE type;
	};
} // end of name space


#endif