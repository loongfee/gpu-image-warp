#ifndef FBO_H
#undef FBO_H



namespace GpuImageProcess
{
	class FBO_BUFFER
	{
		
	public:
		FBO_BUFFER();
		bool init(int width, int height, void *imageData, unsigned char draw_format );
		void clear();

		~FBO_BUFFER();
	private:
		unsigned int fbo;		// FBO handler
		unsigned int rb;		// Render buffer
		unsigned int stencil;
		unsigned int tex;		// texture handler
	};
}
#endif