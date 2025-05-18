#ifndef MESH_H
#define MESH_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/Shader.h"
#include <vector>
#include <string>

enum TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	TextureType type;
	std::string path;
	float Ns; // shininess,镜面反射指数系数
};



class Mesh {
private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;
	uint32_t VAO, VBO, EBO;

	void setupMesh();
public:
	Mesh(std::vector<Vertex>&& _vertices,
		std::vector<unsigned int>&& _indices,
		std::vector<Texture>&& _textures)
		: vertices(std::move(_vertices)),
		indices(std::move(_indices)),
		textures(std::move(_textures)) {
		setupMesh();
	}

	Mesh(Mesh&& other) noexcept
		: vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		textures(std::move(other.textures)),
		VAO(other.VAO), VBO(other.VBO), EBO(other.EBO) {
		// 将其他资源标记为无效，避免重复释放
		other.VAO = 0;
		other.VBO = 0;
		other.EBO = 0;
		// std::cout << "成功调用转移构造\n";
	}

	void Draw(Shader& shader);
	glm::vec3 getCenter();
};

#endif // !MESH_H
