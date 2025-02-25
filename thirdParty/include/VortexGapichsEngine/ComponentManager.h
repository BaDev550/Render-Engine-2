#ifndef COMPONENT_MANAGER_CLASS
#define COMPONENT_MANAGER_CLASS

#include <unordered_map>
#include "Components.h"
#include "Entity.h"

template <typename T>
class ComponentManager {
private:
    std::unordered_map<Entity, std::unique_ptr<Component>> components;
public:
    void addComponent(Entity entity, std::unique_ptr<T> component) {
        components[entity] = std::move(component);
    }

    bool hasComponent(Entity entity) {
        return components.find(entity) != components.end();
    }

    T* getComponent(Entity entity) {
        auto it = components.find(entity);
        return it != components.end() ? dynamic_cast<T*>(it->second.get()) : nullptr;
    }
};

#endif // !COMPONENT_MANAGER_CLASS