#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void loadTexture(const char* path, int textureID) {
    // ���ݴ���
    stbi_set_flip_vertically_on_load(true);
    // ��ȡ����Ȼ��������GPU
    int width, height, nrChannels; // ��ȡ����
    unsigned char* texture_data = stbi_load(path, &width, &height, &nrChannels, 0);
    // ������������
    uint32_t bufferID;
    glGenTextures(1, &bufferID);
    glActiveTexture(GL_TEXTURE0 + textureID);
    glBindTexture(GL_TEXTURE_2D, bufferID);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    if (texture_data)
    {
        // ����ͨ������������
        if (nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        else if (nrChannels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        else {
            std::cout << "·��Ϊ " << path << "��������ִ���" << nrChannels << "ͨ������ͼ���ڴ���Χ��\n";
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture:" << path << std::endl;
    }
    stbi_image_free(texture_data);
}