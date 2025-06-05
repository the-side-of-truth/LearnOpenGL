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

	Vertex() {}

	Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoords):
		position(_position), normal(_normal), texCoords(_texCoords){}
};

struct Texture {
	unsigned int id; // bufferId,Ψһ
	TextureType type;
	std::string path;
	Texture() {}
	Texture(uint32_t _id, TextureType _type, std::string _path) :
		id(_id), type(_type), path(_path) {}
};

enum TextureSlot {
	diffuseSlot,
	specularSlot
};

struct Material {
	bool hasDiffuseTex;
	bool hasSpecularTex;
	bool hasEmitTex;
	Texture diffuseTex0;
	Texture specularTex0;
	Texture emitTex;
	float shininess; // shininess,���淴��ָ��ϵ��

	Material() {};

	Material(Texture _diffuseTex0) :
		hasDiffuseTex(true),
		diffuseTex0(_diffuseTex0),
		hasSpecularTex(false),
		hasEmitTex(false),
		shininess(32)
	{

	}

};

class Mesh { // һ��meshֻ��һ�ֲ���
private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	Material material;
	uint32_t VAO, VBO, EBO;



	void setupMesh();
public:
	Mesh() {
		useInstance = false;
		instanceNum = 0;
	}

	Mesh(const Mesh& other) noexcept
		: vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		material(std::move(other.material)),
		VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
		instanceNum(other.instanceNum),
		useInstance(other.useInstance)
	{
		// ��������Դ���Ϊ��Ч�������ظ��ͷ�
		
		// std::cout << "�ɹ�����ת�ƹ���\n";
	}

	Mesh(std::vector<Vertex>&& _vertices,
		std::vector<unsigned int>&& _indices,
		Material&& _material)
		: vertices(std::move(_vertices)),
		indices(std::move(_indices)),
		material(std::move(_material)) {
		setupMesh();
		useInstance = false;
		instanceNum = 0;
	}

	Mesh(Mesh&& other) noexcept
		: vertices(std::move(other.vertices)),
		indices(std::move(other.indices)),
		material(std::move(other.material)),
		VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
		instanceNum(other.instanceNum),
		useInstance(other.useInstance) {
		// ��������Դ���Ϊ��Ч�������ظ��ͷ�
		other.VAO = 0;
		other.VBO = 0;
		other.EBO = 0;
		// std::cout << "�ɹ�����ת�ƹ���\n";
	}

	void Draw(Shader& shader);
	glm::vec3 getCenter();

	uint32_t getVAO() { return VAO; }

	bool useInstance;
	uint32_t instanceNum;
};

#endif // !MESH_H
