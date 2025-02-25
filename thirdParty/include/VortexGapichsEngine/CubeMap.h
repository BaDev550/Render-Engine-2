#ifndef CUBE_MAP_CLASS
#define CUBE_MAP_CLASS

#include <glad/glad.h>
#include <string>
#include <vector>
#include "VECore.h"
#include "Shader.h"
#include <glm/glm.hpp>

class CubeMap
{
private:
	std::vector<std::string> faces{
		"engine/Textures/skybox_1/right.jpg",
		"engine/Textures/skybox_1/left.jpg",
		"engine/Textures/skybox_1/top.jpg",
		"engine/Textures/skybox_1/bottom.jpg",
		"engine/Textures/skybox_1/front.jpg",
		"engine/Textures/skybox_1/back.jpg"
	};

	unsigned int sb_textureID;
	unsigned int cm_textureID;

	Shader skybox_shader;
	unsigned int skyboxVAO, skyboxVBO;
public:
	CubeMap() {}
	~CubeMap() {
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);
	}

	unsigned int createSkyBox(std::vector<std::string> faces);
	void createCubemap();

	void render(glm::mat4& cameraViewMatrix);

	int getSkyBoxTexture() { return sb_textureID; }
	int getCubeMapTexture() { return cm_textureID; }
};

#endif // !CUBE_MAP_CLASS