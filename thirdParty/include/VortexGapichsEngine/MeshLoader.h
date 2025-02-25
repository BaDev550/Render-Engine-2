#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "VECore.h"

class MeshLoader
{
public:
	MeshLoader() {};

	bool loadOBJ(const std::string& file, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);
};

