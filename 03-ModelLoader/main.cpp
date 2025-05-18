#include "03-ModelLoader.h"
#include "shader/Shader.h"



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

    // 创建着色器
    Shader ObjShader("../../../ShaderSrc/simple.vs", "../../../ShaderSrc/obj.fs");
    ObjShader.use();
    // 定向光源
    ObjShader.setUniformVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    ObjShader.setUniformVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
    ObjShader.setUniformVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
    ObjShader.setUniformVec3("dirLight.direction", 1.0f, 0.0f, -1.0f);

    // 加载模型
    Model guitar("D:/Data/Meshs/backpack/backpack.obj");
    glm::mat4 guitarModelMat = glm::translate(glm::mat4(1.0f), -guitar.getCenter());

    // 变换矩阵
    glm::mat4 trans;
    auto f_trans = [](glm::mat4& _Trans, float aspect_ratio, const glm::mat4& model, const glm::mat4& view) {
        _Trans = glm::mat4(1.0f);
        // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        _Trans = projection * view * model;
    };

    // 第一人称观察视角
    Observer o1(glm::vec3(0, 0, 5));
    glEnable(GL_DEPTH_TEST);
    int win_width, win_height;

    while (!glfwWindowShouldClose(window)) {
        // 用户输入
        o1.processKey(window);
        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &win_width, &win_height);
        framebuffer_size_callback(window, win_width, win_height);
        ObjShader.use();
        // 传输顶点着色器所需uniform
        f_trans(trans, 1.0f * win_width / win_height, guitarModelMat, o1.getTransformer()); // 生成MVP
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(guitarModelMat)));
        ObjShader.setUniformMat4("model", guitarModelMat);
        ObjShader.setUniformMat4("MVP", trans);
        ObjShader.setUniformMat4("view", o1.getTransformer());
        ObjShader.setUniformMat3("NormalMat", normalMat);
        // 传输片段着色器所需uniform并渲染
        guitar.Draw(ObjShader);
        
        
        

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