#ifndef SYSTEMS_CLASS
#define SYSTEMS_CLASS

#include "ComponentManager.h"
#include "Model.h"

class PhysicsSystem {
public:
    void Update(const std::vector<Component*>& components, float deltaTime) {
        RigidBodyComponent* rb = nullptr;
        TransformComponent* transform = nullptr;

        for (auto* component : components) {
            if (auto* tempRb = dynamic_cast<RigidBodyComponent*>(component)) {
                rb = tempRb;
            }
            else if (auto* tempTransform = dynamic_cast<TransformComponent*>(component)) {
                transform = tempTransform;
            }
        }

        if (rb && transform) {
            if (rb->isStatic) {
                rb->velocity = glm::vec3(0.0f);
            }
            else {
                rb->velocity.y -= 1.0f;
            }

            transform->position += rb->velocity * deltaTime;
        }
    }
};

class LightManager {
public:
    std::vector<LightComponent> lights;

    void addLight(const LightComponent& light) {
        lights.push_back(light);
    }

    const std::vector<LightComponent>& getLights() const {
        return lights;
    }

    void renderLightImGui(LightComponent light) {
        light.renderImGui();
    }
};

class RenderingSystem {
private:
    LightManager* lightManager;
public:
    RenderingSystem() {}
    
    void setLightManager(LightManager* manager) { lightManager = manager; }
    void Render(const std::vector<Component*>& components, Shader& shader) {
        StaticMeshComponent* mesh = nullptr;
        TransformComponent* transform = nullptr;

        for (auto* component : components) {
            if (auto* tempMesh = dynamic_cast<StaticMeshComponent*>(component)) {
                mesh = tempMesh;
            }
            else if (auto* tempTransform = dynamic_cast<TransformComponent*>(component)) {
                transform = tempTransform;
            }
        }

        shader.use();
        passLightsToShader(shader);

        if (mesh && transform) {
            glm::mat4 modelMatrix = transform->GetModelMatrix();
            shader.setMat4("model", modelMatrix);

            mesh->staticModel.outline(false);
            mesh->Draw(shader);
        }
    }

    void passLightsToShader(Shader& shader) {
        const auto& lights = lightManager->getLights();

        for (size_t i = 0; i < lights.size(); ++i) {
            const auto& light = lights[i];

            std::string uniformBase = "lights[" + std::to_string(i) + "]";

            shader.setVec3(uniformBase + ".position", light.position);
            shader.setVec3(uniformBase + ".color", light.color);
            shader.setFloat(uniformBase + ".intensity", light.intensity);
            shader.setInt(uniformBase + ".type", static_cast<int>(light.type));
        }

        shader.setInt("numLights", static_cast<int>(lights.size()));
    }
};


#endif // !SYSTEMS_CLASS
