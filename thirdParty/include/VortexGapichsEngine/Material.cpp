#include "Material.h"

void Material::bindMaterial(Shader& shader)
{
    if (diffuseTexture.textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture.textureID);
        shader.setInt("m_texture.texture_diffuse", 0); 
    }
    if (normalMap.textureID != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap.textureID);
        shader.setInt("m_texture.texture_normal", 1);
    }
    if (specularMap.textureID != 0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularMap.textureID);
        shader.setInt("m_texture.texture_specular", 2);
    }
}

void Material::unbindMaterial()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
