#ifndef MODEL_H
#define MODEL_H

#include "shader/Shader.h"
// #include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
#include "Mesh.h"
#include "TextureLoader.h"

class Model
{
public:
	/*  ����   */
	Model(const char* path): path(path)
	{
		loadModel(string(path));
	}
	void Draw(Shader& shader);
	glm::vec3 getCenter(); // ��ȡģ������

private:
	const char* path;
	/*  ģ������  */
	vector<Mesh> meshes;
	string directory;
	/*  ����   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

	// �����Ź���
	static uint32_t textureNumber;
	static unordered_map<string, uint32_t> existTextures;
};

#endif // !MODEL_H
