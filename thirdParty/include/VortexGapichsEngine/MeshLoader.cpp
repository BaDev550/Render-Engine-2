#include "MeshLoader.h"

bool MeshLoader::loadOBJ(const std::string& file, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
{
	std::ifstream m_file(file);
	if (!m_file.is_open()) {
		SEND_MSG(ERROR, "MODEL_LOADER", "Failed to open file: " + file);
		return false;
	}

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_texCoords;
	std::vector<glm::vec3> temp_normals;
	std::unordered_map<Vertex, unsigned int> vertex_map;

	std::string line;
	while (std::getline(m_file, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;

		if (type == "v") {
			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			temp_positions.push_back(pos);
		}
		else if (type == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			uv.y = 1.0f - uv.y;
			temp_texCoords.push_back(uv);
		}
		else if (type == "vn") {
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (type == "f") {
			std::string v1, v2, v3;
			ss >> v1 >> v2 >> v3;

			std::vector<std::string> vertices = { v1, v2, v3 };

			for (const auto& vert : vertices) {
				std::stringstream vertexStream(vert);
				std::string pIndex, tIndex, nIndex;

				std::getline(vertexStream, pIndex, '/');
				std::getline(vertexStream, tIndex, '/');
				std::getline(vertexStream, nIndex, '/');

				int posIdx = std::stoi(pIndex) - 1;
				int texIdx = tIndex.empty() ? -1 : std::stoi(tIndex) - 1;
				int normIdx = nIndex.empty() ? -1 : std::stoi(nIndex) - 1;

				Vertex vertex = 
				{ 
					temp_positions[posIdx],
					texIdx >= 0 ? temp_texCoords[texIdx] : glm::vec2(0.0f),
					temp_normals[normIdx]
				};

				if (vertex_map.find(vertex) == vertex_map.end()) {
					vertex_map[vertex] = static_cast<unsigned int>(outVertices.size());
					outVertices.push_back(vertex);
				}
				outIndices.push_back(vertex_map[vertex]);
			}
		}
	}
	SEND_MSG(INFO, "MESH_LOADER", "Mesh loaded: " + file);
	return true;
}
