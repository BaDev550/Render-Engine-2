#ifndef COMPONENTS
#define COMPONENTS

#include <string>
#include <glm/glm.hpp>
#include "Model.h"
#include <glm/gtc/quaternion.hpp>

struct Component {
public:
    virtual void renderImGui() = 0;
    virtual ~Component() = default;
};

struct StaticMeshComponent : Component{
public:
    Model staticModel;

    StaticMeshComponent() {}
	StaticMeshComponent(const std::string& modelPath)
    {
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
public:
    glm::vec3 velocity;
    bool isStatic;
    bool applyGravity = false;
    float mass;

    RigidBodyComponent() {}
    RigidBodyComponent(glm::vec3 vel = glm::vec3(0.0f), float m = 1.0f, bool stat = false)
        : velocity(vel), mass(m), isStatic(stat) {}

    void renderImGui() override {
        ImGui::Text("RigidBody:");
        ImGui::DragFloat3("Velocity", &velocity.x, 0.1f);
        ImGui::DragFloat("Mass", &mass, 0.1f);
        ImGui::Checkbox("Is Static", &isStatic);
        ImGui::Checkbox("Use Gravity", &applyGravity);
    }

    void addForce(glm::vec3 force) {
        velocity += force;
    }
};

struct BoxColliderComponent : Component {
public:
    glm::vec3 size;
    glm::vec3 offset;

    BoxColliderComponent(glm::vec3 s = glm::vec3(1.0f), glm::vec3 o = glm::vec3(0.0f))
        : size(s), offset(o) {}
};

struct TransformComponent : Component {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

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

    glm::vec3 getForwardVector() {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 getRightVector() {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 getUpVector() {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void renderImGui() override {
        ImGui::Text("Transform:");
        ImGui::DragFloat3("Position", &position.x, 0.1f);
        ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);
        ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    }
};

enum LightType {
    Point = 0,
    Spot = 1,
    Directional = 2
};

struct LightComponent : Component{
public:
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    LightType type;

    LightComponent() {}
    LightComponent(glm::vec3 color, float intensity, LightType type)
        : color(color), intensity(intensity), type(type) {}

    void renderImGui() override {
        ImGui::Text("Light:");
        ImGui::ColorEdit3("Color", &color.x, 0.1f);
        ImGui::DragFloat("Intensity", &intensity, 0.1f);
    }
};

#endif // !COMPONENTS
