#pragma once

#include <vector>
#include "Shader.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define SCALING_FACTOR 0.01f

class Model
{
private:
	std::vector<unsigned int> VAOs, VBOs, EBOs;
	std::vector<unsigned int> indexCounts;
	std::vector<Material> materials;

	bool bOutlined;
public:
	Model() {};
	std::string path;

	Shader single_color_shader;

	bool loadModel(const std::string& file);
	void deleteModel();

	void outline(bool enabled);
	void draw(Shader& shader);
private:
	void scaleModel(aiNode* node, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	Material processMaterial(aiMaterial* aiMat);

	bool isSkeletalMesh(aiMesh* mesh);
	void loadSkeletalData(aiMesh* mesh, const aiScene* scene);
	void updateBones(const std::vector<aiMatrix4x4>& boneTransforms);
};

