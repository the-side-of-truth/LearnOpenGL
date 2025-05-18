#include "Model.h"
uint32_t Model::textureNumber = 0;
unordered_map<string, uint32_t> Model::existTextures = unordered_map<string, uint32_t>();

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // ��ȡMesh
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // �ݹ��ӽڵ�
    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
    TextureType myType = DIFFUSE;
    float Ns = 0;
    mat->Get(AI_MATKEY_SHININESS, Ns);
    if (type == aiTextureType_DIFFUSE) myType = DIFFUSE;
    else if (type == aiTextureType_SPECULAR) myType = SPECULAR;
    vector<Texture> res;
    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString path;
        mat->GetTexture(type, i, &path);
        // ���ļ���ȡ����
        Texture temp;
        std::string absPath = directory + "/" + path.C_Str();
        if(existTextures.find(absPath) == existTextures.end()){ // δ���ع�
            loadTexture(absPath.c_str(), textureNumber);
            existTextures[absPath.c_str()] = textureNumber; // ��¼
            temp.id = textureNumber++;
            temp.path = absPath;
            temp.type = myType;
            if (myType == SPECULAR) temp.Ns = Ns;
            res.emplace_back(temp);
        }
        else { // �Ѽ��ع���ֱ����ȡ��Ϣ
            temp.id = existTextures[absPath.c_str()];
            temp.path = absPath;
            temp.type = myType;
            if (myType == SPECULAR) temp.Ns = Ns;
            res.emplace_back(temp);
        }
    }
    return res;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // ��������
    if (mesh->mNormals == nullptr) {
        std::cout << std::string(path) << "ȱ�ٷ�����";
        exit(-1);
    }
    if (mesh->mTextureCoords[0] == nullptr) {
        std::cout << std::string(path) << "ȱ����������";
        exit(-1);
    }
    std::vector<Vertex> vertices(mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumVertices; i++) {
        // ����λ��
        aiVector3D& curVertex = mesh->mVertices[i];
        vertices[i].position = glm::vec3(curVertex.x, curVertex.y, curVertex.z);
        // ������
        aiVector3D& curNormal = mesh->mNormals[i];
        vertices[i].normal = glm::vec3(curNormal.x, curNormal.y, curNormal.z);
        // ��������
        aiVector3D& curTexCoords = mesh->mTextureCoords[0][i];
        vertices[i].texCoords = glm::vec2(curTexCoords.x, curTexCoords.y);
    }
    
    // ����
    vector<uint32_t> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    // �����ȡ
    std::vector<Texture> textures;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // �ֱ��ȡ������;��淴������
        std::vector<Texture> diffuse_textures = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        std::vector<Texture> specular_textures = loadMaterialTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
        textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
    }

    Mesh res(
        std::move(vertices), 
        std::move(indices),
        std::move(textures)
    );
    return res;
}

glm::vec3 Model::getCenter() {
    glm::vec3 maxP(-std::numeric_limits<float>::max()), minP(std::numeric_limits<float>::max());
    for (Mesh& m : meshes) {
        glm::vec3 meshCenter = m.getCenter();
        for (int j = 0; j < 3; j++) {
            maxP[j] = std::max(maxP[j], meshCenter[j]);
            minP[j] = std::min(minP[j], meshCenter[j]);
        }
    }
    return (maxP + minP) / 2.0f;
}