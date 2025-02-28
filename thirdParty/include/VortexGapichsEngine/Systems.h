#ifndef SYSTEMS_CLASS
#define SYSTEMS_CLASS

#include "ComponentManager.h"
#include "EntityManager.h"
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
                if (rb->applyGravity) {
                    rb->velocity.y -= 1.0f;
                }
            }


            transform->position += rb->velocity * deltaTime;
        }
    }
};

class RenderingSystem {
private:
    EntityManager* entityManager;
public:
    RenderingSystem() {}
    
    void setLightManager(EntityManager* manager) { entityManager = manager; }
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

    int lightCount = 0;
    void passLightsToShader(Shader& shader) {
        for (const auto& [entity, name] : entityManager->entities) {
            if (entityManager->hasComponent<LightComponent>(entity)) {
                LightComponent* light = entityManager->getComponent<LightComponent>(entity);
                TransformComponent* light_transform = entityManager->getComponent<TransformComponent>(entity);
                int lightType = light->type;

                std::string uniformBase = "lights[" + std::to_string(lightCount) + "]";
                shader.setVec3(uniformBase + ".position", light_transform->position);
                shader.setVec3(uniformBase + ".color", light->color);
                shader.setFloat(uniformBase + ".intensity", light->intensity);
                shader.setInt(uniformBase + ".type", lightType);

                if (light->type == LightType::Spot) {
                    shader.setVec3(uniformBase + ".direction", light_transform->getForwardVector());
                    shader.setFloat(uniformBase + ".cutoff", glm::cos(glm::radians(12.5f)));
                    shader.setFloat(uniformBase + ".outerCutoff", glm::cos(glm::radians(17.5f)));
                }

                lightCount++;
            }
        }

        shader.setInt("numLights", lightCount);
    }
};


#endif // !SYSTEMS_CLASS
