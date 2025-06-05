#include "Mesh.h"
#include <limits>

void Mesh::setupMesh() {
	// ���뻺��
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// ���ݴ���
	glBindVertexArray(VAO);
	// �������Դ���
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (void*)vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);// ��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);// ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);// ��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	// �������ݴ���
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), (void*)indices.data(), GL_STATIC_DRAW);
	// �������
	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
	shader.use();
	// ���������
	glBindVertexArray(VAO);

	// ���ݲ���
	std::string matPrefix = "m.";
	// ������
	glActiveTexture(GL_TEXTURE0 + TextureSlot::diffuseSlot);
	glBindTexture(GL_TEXTURE_2D, material.diffuseTex0.id);
	shader.setUniformBool(matPrefix + "hasDiffuseTex", material.hasDiffuseTex);
	shader.setUniformInt(matPrefix + "diffuseTex0", (int)diffuseSlot);
	// ���淴��
	glActiveTexture(GL_TEXTURE0 + TextureSlot::specularSlot);
	glBindTexture(GL_TEXTURE_2D, material.specularTex0.id);
	shader.setUniformBool(matPrefix + "hasSpecularTex", material.hasSpecularTex);
	shader.setUniformInt(matPrefix + "specularTex0", TextureSlot::specularSlot);
	// ����
	shader.setUniformBool(matPrefix + "hasEmitTex", material.hasEmitTex);
	shader.setUniformFloat(matPrefix + "shininess", material.shininess);
	//  ��������uniform

	// ��Ⱦ
	if (useInstance) {
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceNum);
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

glm::vec3 Mesh::getCenter() {
	// �������ж���
	int n = vertices.size();
	glm::vec3 maxP(-std::numeric_limits<float>::max()), minP(std::numeric_limits<float>::max());
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) {
			maxP[j] = std::max(maxP[j], vertices[i].position[j]);
			minP[j] = std::min(minP[j], vertices[i].position[j]);
		}
	}
	return (maxP + minP) / 2.0f;
}