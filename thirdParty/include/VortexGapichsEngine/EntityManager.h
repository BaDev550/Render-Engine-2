#ifndef ENTITY_MANAGER_CLASS
#define ENTITY_MANAGER_CLASS

#include <unordered_map>
#include <string>
#include "Entity.h"
#include <glm/gtc/quaternion.hpp>

class EntityManager {
private:
	Entity nextEntity = 0;
public:
	std::unordered_map<Entity, std::string> entities;
	ComponentManager<StaticMeshComponent> staticMeshComponents;
	ComponentManager<TransformComponent> transformComponents;
	ComponentManager<RigidBodyComponent> rigidBodyComponents;
    ComponentManager<LightComponent> lightComponents;

	Entity createEntity(const std::string& name) {
		Entity entity = nextEntity++;
		entities[entity] = name;

		return entity;
	}

	void destroyEntity(Entity entity) {
        entities.erase(entity);
	}

    template<typename T>
    void addComponent(Entity entity, std::unique_ptr<T> component) {
        if constexpr (std::is_same<T, StaticMeshComponent>::value) {
            staticMeshComponents.addComponent(entity, std::move(component));
        }
        else if constexpr (std::is_same<T, TransformComponent>::value) {
            transformComponents.addComponent(entity, std::move(component));
        }
        else if constexpr (std::is_same<T, RigidBodyComponent>::value) {
            rigidBodyComponents.addComponent(entity, std::move(component));
        }
        else if constexpr (std::is_same<T, LightComponent>::value) {
            lightComponents.addComponent(entity, std::move(component));
        }
    }

    template<typename T>
    bool hasComponent(Entity entity) {
        if constexpr (std::is_same<T, StaticMeshComponent>::value) {
            return staticMeshComponents.hasComponent(entity);
        }
        else if constexpr (std::is_same<T, TransformComponent>::value) {
            return transformComponents.hasComponent(entity);
        }
        else if constexpr (std::is_same<T, RigidBodyComponent>::value) {
            return rigidBodyComponents.hasComponent(entity);
        }
        else if constexpr (std::is_same<T, LightComponent>::value) {
            return lightComponents.hasComponent(entity);
        }
        return false;
    }

    template<typename T>
    T* getComponent(Entity entity) {
        if constexpr (std::is_same<T, StaticMeshComponent>::value) {
            auto component = staticMeshComponents.hasComponent(entity)
                ? staticMeshComponents.getComponent(entity)
                : nullptr;
            return dynamic_cast<T*>(component);
        }
        else if constexpr (std::is_same<T, TransformComponent>::value) {
            auto component = transformComponents.hasComponent(entity)
                ? transformComponents.getComponent(entity)
                : nullptr;
            return dynamic_cast<T*>(component);
        }
        else if constexpr (std::is_same<T, RigidBodyComponent>::value) {
            auto component = rigidBodyComponents.hasComponent(entity)
                ? rigidBodyComponents.getComponent(entity)
                : nullptr;
            return dynamic_cast<T*>(component);
        }
        else if constexpr (std::is_same<T, LightComponent>::value) {
            auto component = lightComponents.hasComponent(entity)
                ? lightComponents.getComponent(entity)
                : nullptr;
            return dynamic_cast<T*>(component);
        }
        return nullptr;
    }


    int selectedComp;
    void showEntityInImGui(Entity entity) {
        if (transformComponents.hasComponent(entity)) {
            auto* transform = transformComponents.getComponent(entity);
            if (transform) {
                transform->renderImGui();
            }
        }

        if (staticMeshComponents.hasComponent(entity)) {
            auto* mesh = staticMeshComponents.getComponent(entity);
            if (mesh) {
                mesh->renderImGui();
            }
        }

        if (rigidBodyComponents.hasComponent(entity)) {
            auto* rigidBody = rigidBodyComponents.getComponent(entity);
            if (rigidBody) {
                rigidBody->renderImGui();
            }
        }

        if (lightComponents.hasComponent(entity)) { 
            auto* light = lightComponents.getComponent(entity);
            if (light) {
                light->renderImGui();
            }
        }
        
        if (!hasComponent<StaticMeshComponent>(entity)) {
            if (ImGui::Button("Mesh")) {
                addComponent<StaticMeshComponent>(entity, std::make_unique<StaticMeshComponent>("resources/Models/Cube/cube.obj"));
            }
        }
        if (!hasComponent<RigidBodyComponent>(entity)) {
            ImGui::SameLine();
            if (ImGui::Button("RigidBody")) {
                addComponent<RigidBodyComponent>(entity, std::make_unique<RigidBodyComponent>(glm::vec3(0.0f), 1.0f, true));
            }
        }
        if (!hasComponent<TransformComponent>(entity)) {
            ImGui::SameLine();
            if (ImGui::Button("Transform")) {
                addComponent<TransformComponent>(entity, std::make_unique<TransformComponent>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
            }
        }
    }
};

#endif // !ENTITY_MANAGER_CLASS
