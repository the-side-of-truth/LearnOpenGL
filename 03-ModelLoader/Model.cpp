#include "Model.h"
unordered_map<string, uint32_t> Model::existTextures = unordered_map<string, uint32_t>();

Model::Model(const char* path, std::vector<glm::mat4>&& modelMats) :Model(path) {
    // ����ʵ������buffer
    uint32_t instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, modelMats.size() * sizeof(glm::mat4), (void*)modelMats.data(), GL_STATIC_DRAW);// ���ݴ���
    // Ϊÿ��mesh���������
    for (Mesh& mesh : meshes) {
        uint32_t curVAO = mesh.getVAO();
        glBindVertexArray(curVAO);// ����VAO
        // �����µĶ�������
        // ���� mat4 �ֳ�4�� vec4 �󶨵� attrib location 1~4
        int mat4AttrStart = 3;
        for (int i = 0; i < 4; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
            glEnableVertexAttribArray(mat4AttrStart + i);
            glVertexAttribPointer(mat4AttrStart + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(mat4AttrStart + i, 1);  // ÿʵ��ʹ��һ��
        }

        glBindVertexArray(0);
        mesh.useInstance = true;
        mesh.instanceNum = modelMats.size();
    }
    useInstance = true;
}

Model::Model(std::vector<Vertex> vertices, std::vector<uint32_t> indices, uint32_t diffTexBuf):
    path("temp")
{
    Texture diffTex(diffTexBuf, DIFFUSE, "temp");
    Material mat(diffTex);
    Mesh mesh(std::move(vertices), std::move(indices), std::move(mat));
    meshes.emplace_back(std::move(mesh));
    useInstance = false;
}

void Model::Draw(Shader& shader)
{
    shader.use();
    if (useInstance) {
        shader.setUniformBool("useInstance", true);
    }
    else {
        shader.setUniformBool("useInstance", false);
    }
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
            uint32_t texBufId = loadTexture(absPath.c_str());
            existTextures[absPath.c_str()] = texBufId; // ��¼
            temp.id = texBufId;
            temp.path = absPath;
            temp.type = myType;
            res.emplace_back(temp);
        }
        else { // �Ѽ��ع���ֱ����ȡ��Ϣ
            temp.id = existTextures[absPath.c_str()];
            temp.path = absPath;
            temp.type = myType;
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

    // ���ʶ�ȡ
    std::vector<Texture> textures;
    Material ourMat;
    ourMat.hasDiffuseTex = false;
    ourMat.hasSpecularTex = false;
    ourMat.hasEmitTex = false;
    ourMat.shininess = 32;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // ���ʶ�ȡ
        float Ns = 32;
        material->Get(AI_MATKEY_SHININESS, Ns);
        ourMat.shininess = Ns;
        // �ֱ��ȡ������;��淴������
        std::vector<Texture> diffuse_textures = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        std::vector<Texture> specular_textures = loadMaterialTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
        textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
        if (diffuse_textures.size() > 0){
            ourMat.hasDiffuseTex = true;
            ourMat.diffuseTex0 = diffuse_textures[0];
        }
        if (specular_textures.size() > 0) {
            ourMat.hasSpecularTex = true;
            ourMat.specularTex0 = specular_textures[0];
        }
    }

    Mesh res(
        std::move(vertices), 
        std::move(indices),
        std::move(ourMat)
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