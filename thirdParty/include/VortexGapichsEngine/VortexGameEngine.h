#ifndef VORTEX_GAME_ENGINE
#define VORTEX_GAME_ENGINE

#include "VEngine.h"

#include "Camera.h"
#include "VECore.h"
#include "VEInput.h"
#include "VELogger.h"
#include "VEWindow.h"

#include "CubeMap.h"
#include "Model.h"
#include "PostProcess.h"
#include "Shader.h"
#include "Texture.h"
#include "VERenderer.h"
#include "Vertex.h"

#include "Entity.h"
#include "Components.h"
#include "Systems.h"

int main() {
	VEngine engine;
	engine.engineInit();

	return 0;
}

#endif // !VORTEX_GAME_ENGINE