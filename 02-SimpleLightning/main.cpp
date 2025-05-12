// 01-FirstWindow.cpp: 定义应用程序的入口点。
//

#include "02-SimpleLightning.h"
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

    // 数据处理
    unsigned int VBO_Light; // 创建对象缓存
    glGenBuffers(1, &VBO_Light);
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    int stride = 6;
    unsigned int VAO_Light;
    glGenVertexArrays(1, &VAO_Light);
    glBindVertexArray(VAO_Light); // 只记录状态变更，不管数据传输
    // 传输顶点数据VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Light);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// 只会在这里执行一次，不会被VAO记录
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //数据使用说明
    glEnableVertexAttribArray(0);// 启用属性槽
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float))); //数据使用说明
    glEnableVertexAttribArray(1);// 启用属性槽
    glBindVertexArray(0);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::mat4 model_Light = glm::translate(glm::mat4(1.0f), lightPos);
    model_Light = glm::scale(model_Light, glm::vec3(0.2f));

    unsigned int VBO_Object; // 创建对象缓存
    glGenBuffers(1, &VBO_Object);
    unsigned int VAO_Object;
    glGenVertexArrays(1, &VAO_Object);
    glBindVertexArray(VAO_Object); // 只记录状态变更，不管数据传输
    // 传输顶点数据VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// 只会在这里执行一次，不会被VAO记录
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //数据使用说明
    glEnableVertexAttribArray(0);// 启用属性槽
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float))); //数据使用说明
    glEnableVertexAttribArray(1);// 启用属性槽
    glBindVertexArray(0);
    glm::mat4 model_obj(1.0f);

    // 创建着色器
    Shader LightShader("../../../ShaderSrc/simple.vs", "../../../ShaderSrc/lightSource.fs");
    Shader ObjShader("../../../ShaderSrc/simple.vs", "../../../ShaderSrc/obj.fs");
    ObjShader.use();
    glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
    glUniform3fv(glGetUniformLocation(ObjShader.getID(), "objectColor"), 1, glm::value_ptr(toyColor));
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(ObjShader.getID(), "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(ObjShader.getID(), "lightPos"), 1, glm::value_ptr(lightPos));

    // 变换矩阵
    glm::mat4 trans;
    auto f_trans = [](glm::mat4& _Trans, float aspect_ratio, const glm::mat4& model, const glm::mat4& view) {
        _Trans = glm::mat4(1.0f);
        // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        _Trans = projection * view * model;
    };

    // 第一人称
    Observer o1(glm::vec3(0, 0, 10));
    // 渲染循环
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    int win_width, win_height;
    float angle = 0;
    while (!glfwWindowShouldClose(window)) {
        // 用户输入
        o1.processKey(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &win_width, &win_height);
        // 渲染光源
        LightShader.use();
        glBindVertexArray(VAO_Light);
        f_trans(trans, 1.0f * win_width / win_height, model_Light, o1.getTransformer());
        glUniformMatrix4fv(glGetUniformLocation(LightShader.getID(), "MVP"), 1, GL_FALSE, glm::value_ptr(trans));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(vertices[0]) * stride));
        glBindVertexArray(0);
        // 渲染物体
        ObjShader.use();
        glBindVertexArray(VAO_Object);
        f_trans(trans, 1.0f * win_width / win_height, model_obj, o1.getTransformer());
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model_obj)));
        glUniformMatrix4fv(glGetUniformLocation(ObjShader.getID(), "MVP"), 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix3fv(glGetUniformLocation(ObjShader.getID(), "NormalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
        glUniform3fv(glGetUniformLocation(ObjShader.getID(), "CamCenter"), 1, glm::value_ptr(o1.getCamCenter()));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(vertices[0]) * stride));
        glBindVertexArray(0);
        

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
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (alpha < 1.0f) alpha += 0.0001f;
        shader.use();
        shader.setUniformFloat("alpha", alpha);
        std::cout << "alpha:" << alpha << "\n";
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (alpha > 0.0f) alpha -= 0.0001f;
        shader.use();
        shader.setUniformFloat("alpha", alpha);
        std::cout << "alpha:" << alpha << "\n";
    }
}