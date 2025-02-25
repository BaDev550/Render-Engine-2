#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;

    bool operator==(const Vertex& other) const {
        return position == other.position && texCoords == other.texCoords && normal == other.normal;
    }
};

namespace std {
    template <>
    struct hash<Vertex> {
        size_t operator()(const Vertex& v) const {
            return hash<float>()(v.position.x) ^ hash<float>()(v.position.y) ^ hash<float>()(v.position.z) ^
                hash<float>()(v.texCoords.x) ^ hash<float>()(v.texCoords.y) ^
                hash<float>()(v.normal.x) ^ hash<float>()(v.normal.y) ^ hash<float>()(v.normal.z);
        }
    };
}