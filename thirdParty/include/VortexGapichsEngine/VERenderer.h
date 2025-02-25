#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#include "VEWindow.h"
#include "Camera.h"
#include "VEInput.h"

#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "PostProcess.h"
#include "CubeMap.h"

#include "ComponentManager.h"
#include "Systems.h"

class VERenderer
{
public:
	VERenderer() {}
	~VERenderer() {}

	Shader core_shader;

	VEInput input;
	PostProcessing post_process;
	CubeMap cube_map;

	void start(VEWindow& window);
	void render(VEWindow& window, RenderingSystem system, const std::vector<Component*>& components);

	void processInput(float dt);
};

#endif // !RENDERER_CLASS