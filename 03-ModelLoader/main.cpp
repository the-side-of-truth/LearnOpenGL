#include "03-ModelLoader.h"
#include "shader/Shader.h"
#include "Mesh.h"
#include <vector>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, const Shader& shader);

float alpha = 0.2f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); // 窗口大小
    if (window == NULL) {
        std::cout << "创建窗口失败！\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600); // 成像平面大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义一下项目顶层(LearnOpenGL)相对于可执行文件的路径(仅限visual studio)
    std::string relProjectPath = "../../../../";
    std::string relSubProjectPath = relProjectPath + "03-ModelLoader/";
    // 创建着色器
    Shader ObjShader(
        (relSubProjectPath + "ShaderSrc/simple.vs").c_str(), 
        (relSubProjectPath + "ShaderSrc/obj.fs").c_str()
    );

    ObjShader.use();
    // 定向光源
    ObjShader.setUniformBool("hasDirLight", true);
    ObjShader.setUniformVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    ObjShader.setUniformVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
    ObjShader.setUniformVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
    ObjShader.setUniformVec3("dirLight.direction", 1.0f, 0.0f, -1.0f);
    // 手电筒光源(常量)
    ObjShader.setUniformVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
    ObjShader.setUniformVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
    ObjShader.setUniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    ObjShader.setUniformFloat("spotLight.constant", 1.0f);// 衰减系数
    ObjShader.setUniformFloat("spotLight.linear", 0.09f);
    ObjShader.setUniformFloat("spotLight.quadratic", 0.032f);
    ObjShader.setUniformFloat("spotLight.cutOff_Inner", cosf(glm::radians(10.f))); // 手电筒光切角余弦值
    ObjShader.setUniformFloat("spotLight.cutOff_outer", cosf(glm::radians(15.f))); // 手电筒光切角余弦值

    // 帧缓冲
    uint32_t framebuffer; // 帧缓冲编号
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // 附加纹理
    // 生成颜色纹理
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 将它附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    // 附加渲染缓冲对象
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 创建镜面
    // 顶点数据
    std::vector<Vertex> mirrorVertices = {
        Vertex(glm::vec3(3.0, 3.0, 0.0), glm::vec3(0,0,1), glm::vec2(1, 1)), //右上角
        Vertex(glm::vec3(3.0, 2.0, 0.0), glm::vec3(0,0,1), glm::vec2(1, 0)), //右下角
        Vertex(glm::vec3(2.0, 2.0, 0.0), glm::vec3(0,0,1), glm::vec2(0, 0)), //左下角
        Vertex(glm::vec3(2.0, 3.0, 0.0), glm::vec3(0,0,1), glm::vec2(0, 1))  //左上角
    };
    std::vector<uint32_t> mirrorIndices = {
        2, 1, 0,
        3, 2, 0
    };
    Model mirror(mirrorVertices, mirrorIndices, texColorBuffer);
    glm::mat4 mirrorModelMat(1.0f);
    mirrorModelMat = glm::translate(mirrorModelMat, glm::vec3(-1, -1, 0));
    Observer oMirror(glm::vec3(0, 0, 2));

    // 加载模型
    unsigned int amount = 1000;
    std::vector<glm::mat4> modelMatrices(amount, glm::mat4(1.0f));
    srand(glfwGetTime()); // 初始化随机种子    
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model;
        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. 添加到矩阵的数组中
        modelMatrices[i] = model;
    }
    Model guitar((relProjectPath + "models/cube/cube.obj").c_str(), std::move(modelMatrices));
    glm::mat4 guitarModelMat = glm::mat4(1.0f);

    Model rocks((relProjectPath + "models/rock/rock.obj").c_str(), std::move(modelMatrices));
    glm::mat4 rocksModelMat = glm::mat4(1.0f);

    Model planet((relProjectPath + "models/planet/planet.obj").c_str());
    glm::mat4 planetModelMat = glm::mat4(1.0f);


    // 透视投影矩阵
    glm::mat4 projection;

    // 第一人称观察视角
    Observer o1(glm::vec3(0, 5, 0));
    glEnable(GL_DEPTH_TEST);// 启用深度缓冲
    int win_width, win_height;
    while (!glfwWindowShouldClose(window)) {
        // 处理镜面帧缓冲(把渲染结果存储到纹理)
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
        glEnable(GL_DEPTH_TEST);
        ObjShader.use();
        // 传输顶点着色器所需uniform
        projection = glm::perspective(glm::radians(45.0f), 1.0f * 8 / 6, 0.1f, 1000.0f);
        ObjShader.setUniformMat4("model", guitarModelMat);
        ObjShader.setUniformMat4("projection", projection);
        ObjShader.setUniformMat4("view", oMirror.getTransformer());
        // 传输片段着色器所需uniform并渲染
        ObjShader.setUniformVec3("CamCenter", oMirror.getCamCenter());
        // 手电筒光源(常量)
        ObjShader.setUniformBool("hasSpotLight", true);
        ObjShader.setUniformVec3("spotLight.position", oMirror.getCamCenter());
        ObjShader.setUniformVec3("spotLight.direction", -glm::vec3(glm::transpose(oMirror.getTransformer())[2]));
        // 返回结果类型
        ObjShader.setUniformInt("resClass", 2);
        rocks.Draw(ObjShader);
        planet.Draw(ObjShader);
        ObjShader.setUniformInt("resClass", 0); // 回复正常类型
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
         
        // 处理默认帧缓冲
        // 用户输入
        o1.processKey(window);
        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &win_width, &win_height);
        framebuffer_size_callback(window, win_width, win_height);
        ObjShader.use();
        // 传输顶点着色器所需uniform
        projection = glm::perspective(glm::radians(45.0f), 1.0f * win_width / win_height, 0.1f, 1000.0f);
        ObjShader.setUniformMat4("model", guitarModelMat);
        ObjShader.setUniformMat4("view", o1.getTransformer());
        ObjShader.setUniformMat4("projection", projection);
        // 传输片段着色器所需uniform并渲染
        ObjShader.setUniformBool("hasSpotLight", true);
        ObjShader.setUniformVec3("CamCenter", o1.getCamCenter());
        // 手电筒光源(常量)
        ObjShader.setUniformVec3("spotLight.position", o1.getCamCenter());
        ObjShader.setUniformVec3("spotLight.direction", -glm::vec3(glm::transpose(o1.getTransformer())[2])); // 将光照调暗了一些以搭配场景
        rocks.Draw(ObjShader);
        planet.Draw(ObjShader);
        // 绘制镜子
        ObjShader.setUniformMat4("model", mirrorModelMat);
        // 返回结果类型
        mirror.Draw(ObjShader);
        // 窗口渲染方式与事件处理
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, const Shader& shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}