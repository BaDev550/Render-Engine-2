#ifndef POST_PROCESS_CLASS
#define POST_PROCESS_CLASS

#include <glad/glad.h>
#include "VECore.h"
#include "Shader.h"

class PostProcessing
{
private:
	unsigned int FrameBufferTextureColor;
	unsigned int MSAA_Texture;
	unsigned int quadVAO, quadVBO;
	unsigned int FBO;

	int width, height;
	unsigned int shadow_height = 1024, shadow_width = 1024;
public:
	Shader screen_shader;
	unsigned int depthMap;

	PostProcessing() {}
	~PostProcessing() {
		glDeleteFramebuffers(1, &FBO);
	}
	void start(int width, int height);
	
	void bindFBO();

	void unbindFBO();

	void render();
	void clearScreen();
	void clearFBO();
};


#endif // !POST_PROCESS_CLASS