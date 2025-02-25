#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS

#include <iostream>
#include <string>
#include <glad/glad.h>
#include "VECore.h"

class Texture
{
public:
	Texture() {}

	unsigned int textureID;
	std::string textureType;

	void loadTexture(const std::string& file = "", const bool gamma = true);

	void bind() const;
	void unbind() const;

	void deleteTexture();

	static Texture DefaultTexture();
};

#endif // !TEXTURE_CLASS
