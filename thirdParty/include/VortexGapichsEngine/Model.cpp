#include "Model.h"

bool Model::loadModel(const std::string& file)
{
    path = file;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs |
        aiProcess_GenNormals
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        SEND_MSG(ERROR, "ASSIMP", importer.GetErrorString());
        return false;
    }

    if (file.find(".fbx") != std::string::npos) {
        scaleModel(scene->mRootNode, scene);
    }

    materials.resize(scene->mNumMaterials);
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        materials[i] = processMaterial(scene->mMaterials[i]);
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* mat = scene->mMaterials[i];
        std::cout << "Material " << i << " has:\n";
        std::cout << "  Diffuse: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << " textures\n";
        std::cout << "  Specular: " << mat->GetTextureCount(aiTextureType_SPECULAR) << " textures\n";
        std::cout << "  Normal: " << mat->GetTextureCount(aiTextureType_HEIGHT) << " textures\n";
    }

    processNode(scene->mRootNode, scene);
    SEND_MSG(INFO, "ASSIMP", "Model Loaded");
    return true;
}

void Model::deleteModel()
{
    VAOs.clear();
    VBOs.clear();
    EBOs.clear();
    indexCounts.clear();
    materials.clear();
}

void Model::outline(bool enabled)
{
    bOutlined = enabled;
}

void Model::draw(Shader& shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    for (size_t i = 0; i < VAOs.size(); i++) {
        shader.use();

        if (i < materials.size()) {
            materials[i].bindMaterial(shader);
        }

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, indexCounts[i], GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if (bOutlined) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            single_color_shader.use();
            model = glm::scale(model, glm::vec3(1.51f));
            single_color_shader.setMat4("model", model);

            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, indexCounts[i], GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
        }
    }
}

void Model::scaleModel(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            mesh->mVertices[j].x *= SCALING_FACTOR;
            mesh->mVertices[j].y *= SCALING_FACTOR;
            mesh->mVertices[j].z *= SCALING_FACTOR;
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        scaleModel(node->mChildren[i], scene);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //if (isSkeletalMesh(mesh))
        //{
        //    loadSkeletalData(mesh, scene);
        //}
        //else
        //{
        //    processMesh(mesh, scene);
        //}
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        if (mesh->mNormals) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    int materialIndex = mesh->mMaterialIndex;

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    VAOs.push_back(VAO);
    VBOs.push_back(VBO);
    EBOs.push_back(EBO);
    indexCounts.push_back(indices.size());
}

Material Model::processMaterial(aiMaterial* aiMat)
{
    Material material;

    aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
    aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

    float shininess = 32.0f;
    aiMat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    material.setDiffuseTexture(Texture().LoadFromAssimp(aiMat, aiTextureType_DIFFUSE));
    material.setSpecularMap(Texture().LoadFromAssimp(aiMat, aiTextureType_SPECULAR));
    material.setNormalMap(Texture().LoadFromAssimp(aiMat, aiTextureType_HEIGHT));

    return material;
}

bool Model::isSkeletalMesh(aiMesh* mesh)
{
    return mesh->mNumBones > 0;
}

void Model::loadSkeletalData(aiMesh* mesh, const aiScene* scene)
{
}

void Model::updateBones(const std::vector<aiMatrix4x4>& boneTransforms)
{
}
