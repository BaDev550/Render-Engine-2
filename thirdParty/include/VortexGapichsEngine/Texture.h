#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS

#include <iostream>
#include <string>
#include <glad/glad.h>
#include "VECore.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Texture
{
public:
	Texture() {}
	Texture(const std::string& file, const std::string& textureType) {
		loadTexture(file, true, textureType);
	}

	unsigned int textureID;
	std::string textureType;
	std::string texturePath;

	Texture loadTexture(const std::string& file = "", const bool gamma = true, const std::string& textureType = "diffuse");

	void bind() const;
	void unbind() const;

	void deleteTexture();
	Texture LoadFromAssimp(aiMaterial* mat, aiTextureType type);

	static Texture DefaultTexture();
};

#endif // !TEXTURE_CLASS
