#pragma once

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "MeshLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	bool bOutlined;
public:
	Model() {};
	std::string path;

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	Texture texture; // to-do: make a material class for spec, def norm textures to bind
	Shader single_color_shader;

	void loadModel(const std::string& file) {
		path = file;
		MeshLoader m_loader;
		m_loader.loadOBJ(file, vertices, indices);
		setupModel();
	}

	void deleteModel();
	void outline(bool enabled);
	void draw(Shader& shader);
private:
	void setupModel();
};

