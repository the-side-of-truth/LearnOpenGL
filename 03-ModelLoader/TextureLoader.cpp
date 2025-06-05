#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t loadTexture(const char* path) {
    // 数据处理
    stbi_set_flip_vertically_on_load(true);
    // 读取纹理，然后将纹理传到GPU
    int width, height, nrChannels; // 读取纹理
    unsigned char* texture_data = stbi_load(path, &width, &height, &nrChannels, 0);
    // 开辟纹理缓冲区
    uint32_t bufferID;
    glGenTextures(1, &bufferID);
    glBindTexture(GL_TEXTURE_2D, bufferID);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    if (texture_data)
    {
        // 根据通道数传输数据
        if (nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        else if (nrChannels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        else {
            std::cout << "路径为 " << path << "的纹理出现错误：" << nrChannels << "通道数的图像不在处理范围内\n";
            exit(-1);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture:" << path << std::endl;
        exit(-1);
    }
    stbi_image_free(texture_data);
    return bufferID;
}