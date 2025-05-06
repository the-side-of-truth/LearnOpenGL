// 01-FirstWindow.cpp: 定义应用程序的入口点。
//

#include "01-FirstWindow.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
    unsigned int VBO; // 创建对象缓存
    glGenBuffers(1, &VBO);
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, 0.0f, 0.0f,  // 右下角
        0.0f, -0.0f, 0.0f, // 左下角
    };
    unsigned int EBO; // 元素索引
    unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    glGenBuffers(1, &EBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);// 绑定VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// 指定输出到哪个缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 传递数据
    int stride = 3;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //数据使用说明
    glEnableVertexAttribArray(0);// 启用属性槽
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0); // 解绑VAO

    unsigned int VBO1; // 创建对象缓存
    glGenBuffers(1, &VBO1);
    float vertices1[] = {
        -0.0f, 0.0f, 0.0f,   // 左上角
        -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //数据使用说明
    glEnableVertexAttribArray(0);// 启用属性槽
    glBindVertexArray(0);

    // 着色器处理
    // 创建并编译顶点着色器
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* VerterShaderSrc = "#version 330 core\n"
        "layout (location=0) in vec3 aPos; \n"
        "void main(){ \n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f); \n"
        "}\0"
        ;
    glShaderSource(VertexShader, 1, &VerterShaderSrc, NULL);
    glCompileShader(VertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "顶点着色器编译失败\n" << infoLog << "\n";
        exit(-1);
    }
    // 创建片段着色器
    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* FragmentShaderSrc = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { \n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
        "}\0"
        ;
    glShaderSource(FragmentShader, 1, &FragmentShaderSrc, NULL);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "片段着色器编译失败\n" << infoLog << "\n";
        exit(-1);
    }

    unsigned int FragmentShader1;
    FragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    const char* FragmentShaderSrc1 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { \n"
        "FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);"
        "}\0"
        ;
    glShaderSource(FragmentShader1, 1, &FragmentShaderSrc1, NULL);
    glCompileShader(FragmentShader1);
    glGetShaderiv(FragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader1, 512, NULL, infoLog);
        std::cout << "片段着色器1编译失败\n" << infoLog << "\n";
        exit(-1);
    }

    // 链接
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram ,VertexShader);
    glAttachShader(shaderProgram, FragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "着色器程序链接失败\n" << infoLog << "\n";
        exit(-1);
    }
    glDeleteShader(FragmentShader);

    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, VertexShader);
    glAttachShader(shaderProgram1, FragmentShader1);
    glLinkProgram(shaderProgram1);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "着色器程序1链接失败\n" << infoLog << "\n";
        exit(-1);
    }
    // glUseProgram(shaderProgram);
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader1);

    // 渲染循环
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        // 用户输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        // glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));
        // glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
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

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}