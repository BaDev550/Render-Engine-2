#include "Texture.h"
#include <stb/stb_image.h>

void Texture::loadTexture(const std::string& file, const bool gamma)
{
	std::string texFile;
	if (file.empty()) {
		texFile = (ENGINE_PATH + "engine/Textures/NO-TEXTURE.png");
		SEND_MSG(WARN, "TEXTURE", "No texture at path default texture binded! " + file);
	}
	else {
		texFile = file;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	static bool bTextureLoaded = false;
	unsigned char* data{};

	while (!bTextureLoaded) {
		data = stbi_load(texFile.c_str(), &width, &height, &channels, 0);
		if (data) {
			GLenum format{};
			GLenum dataFormat{};
			if (channels == 1) {
				format = dataFormat = GL_RED;
			}
			else if (channels == 3) {
				format = gamma ? GL_SRGB : GL_RGB;
				dataFormat = GL_RGB;
			}
			else if (channels == 4) {
				format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
				dataFormat = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			bTextureLoaded = true;
			break;
		}
		else {
			texFile = (ENGINE_PATH + "engine/Textures/TEXTURE-ERROR.png");
			SEND_MSG(ERROR, "TEXTURE_LOADER", "Failed to load texture from: " + texFile);
			bTextureLoaded = false;
		}
	}
	stbi_image_free(data);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture()
{
	glDeleteTextures(1, &textureID);
}

Texture Texture::DefaultTexture()
{
	Texture tex;
	tex.loadTexture();
	return tex;
}
