#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
    Texture diffuseTexture;
    Texture normalMap;
    Texture specularMap;
    float shininess;

    Material()
        : shininess(32.0f) {
        diffuseTexture.textureID = 0;
        normalMap.textureID = 0;
        specularMap.textureID = 0;
    }

    void bindMaterial(Shader& shader);
    void unbindMaterial();

    void setDiffuseTexture(const Texture& tex) { diffuseTexture = tex; }
    void setNormalMap(const Texture& tex) { normalMap = tex; }
    void setSpecularMap(const Texture& tex) { specularMap = tex; }
};

#endif // !MATERIAL_CLASS
