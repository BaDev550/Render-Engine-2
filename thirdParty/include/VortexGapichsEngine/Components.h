#ifndef COMPONENTS
#define COMPONENTS

#include <string>
#include <glm/glm.hpp>
#include "Model.h"
#include <glm/gtc/quaternion.hpp>

struct Component {
    virtual void renderImGui() = 0;
    virtual ~Component() = default;
};

struct StaticMeshComponent : Component{
    Model staticModel;
    Texture texture;

    StaticMeshComponent() {}
	StaticMeshComponent(const std::string& modelPath, const std::string& texturePath)
    {
        texture.loadTexture(texturePath);
        staticModel.texture = texture;
        staticModel.loadModel(modelPath);
    }

    void Draw(Shader& shader) {
        staticModel.draw(shader);
    }

    void renderImGui() override {
        ImGui::Text("Static Mesh:");
        ImGui::InputText("Mesh Path", &staticModel.path[0], staticModel.path.capacity() + 5);
        if (ImGui::Button("Load")) {
            staticModel.deleteModel();
            staticModel.loadModel(staticModel.path);
        }
    }
};

struct RigidBodyComponent : Component {
    glm::vec3 velocity;
    float mass;
    bool isStatic;

    RigidBodyComponent() {}
    RigidBodyComponent(glm::vec3 vel = glm::vec3(0.0f), float m = 1.0f, bool stat = false)
        : velocity(vel), mass(m), isStatic(stat) {}

    void renderImGui() override {
        ImGui::Text("RigidBody:");
        ImGui::DragFloat3("Velocity", &velocity.x, 0.1f);
        ImGui::DragFloat("Mass", &mass, 0.1f);
        ImGui::Checkbox("Is Static", &isStatic);
    }
};

struct BoxColliderComponent : Component {
    glm::vec3 size;
    glm::vec3 offset;

    BoxColliderComponent(glm::vec3 s = glm::vec3(1.0f), glm::vec3 o = glm::vec3(0.0f))
        : size(s), offset(o) {}
};

struct TransformComponent : Component {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformComponent() {}
    TransformComponent(glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 rot = glm::vec3(0),
        glm::vec3 scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}

    glm::mat4 GetModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model *= glm::mat4_cast(glm::quat(glm::radians(rotation)));
        model = glm::scale(model, scale);
        return model;
    }

    void renderImGui() override {
        ImGui::Text("Transform:");
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    }
};

struct LightComponent : Component{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    enum class LightType { Point, Directional, Spot } type;

    LightComponent() {}
    LightComponent(glm::vec3 position, glm::vec3 color, float intensity, LightType type)
        : position(position), color(color), intensity(intensity), type(type) {}

    void renderImGui() override {
        ImGui::Text("Light:");
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Color", &color.x, 0.1f);
        ImGui::DragFloat("Intensity", &intensity, 0.1f);
    }
};

#endif // !COMPONENTS
