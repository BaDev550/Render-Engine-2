#include "Texture.h"
#include <stb/stb_image.h>

Texture Texture::loadTexture(const std::string& file, const bool gamma, const std::string& textureType)
{
	Texture texture;
	texture.textureType = textureType;
	texture.texturePath = file;
	std::string texFile;

	if (file.empty()) {
		texFile = (ENGINE_PATH + "engine/Textures/NO-TEXTURE.png");
		SEND_MSG(WARN, "TEXTURE", "No texture at path default texture binded! " + file);
	}
	else {
		texFile = file;
	}

	glGenTextures(1, &texture.textureID);
	glBindTexture(GL_TEXTURE_2D, texture.textureID);

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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			bTextureLoaded = true;
			break;
		}
		else {
			SEND_MSG(ERROR, "TEXTURE_LOADER", "Failed to load texture from: " + texFile);
			texFile = (ENGINE_PATH + "engine/Textures/TEXTURE-ERROR.png");
			stbi_image_free(data);
			bTextureLoaded = false;
		}
	}
	stbi_image_free(data);
	return texture;
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
	tex = Texture().loadTexture();
	return tex;
}

std::string GetTextureTypeString(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		return "diffuse";
	case aiTextureType_SPECULAR:
		return "specular";
	case aiTextureType_HEIGHT:
	case aiTextureType_NORMALS:
		return "normal";
	case aiTextureType_AMBIENT:
		return "ambient";
	case aiTextureType_EMISSIVE:
		return "emissive";
	case aiTextureType_OPACITY:
		return "opacity";
	case aiTextureType_DISPLACEMENT:
		return "displacement";
	default:
		return "unknown";
	}
}

Texture Texture::LoadFromAssimp(aiMaterial* mat, aiTextureType type)
{
	Texture texture;
	if (mat->GetTextureCount(type) > 0) {
		aiString path;
		if (mat->GetTexture(type, 0, &path) == AI_SUCCESS) {
			texture.loadTexture(path.C_Str(), true, GetTextureTypeString(type));
		}
	}
	else {
		texture = Texture::DefaultTexture();
	}
	return texture;
}
