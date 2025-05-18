# include "shader/Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	/*---------------------------着色器文件读取---------------------------*/
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "着色器文件无法打开" << std::endl;
		exit(-1);
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	/*---------------------------着色器文件读取(完)---------------------------*/

	/*---------------------------着色器编译---------------------------*/
	// 编译检测标志与log缓冲
	int  success;
	char infoLog[512];
    // 创建并编译顶点着色器
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vShaderCode, NULL);
    glCompileShader(VertexShader);
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "顶点着色器编译失败\n" << infoLog << "\n";
        exit(-1);
    }
    // 创建片段着色器
    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "片段着色器编译失败\n" << infoLog << "\n";
        exit(-1);
    }
	/*---------------------------着色器编译(完)---------------------------*/
	// 创建着色器程序，链接并删除着色器
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	ID = shaderProgram;
	glAttachShader(shaderProgram, VertexShader);
	glAttachShader(shaderProgram, FragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "着色器程序链接失败\n" << infoLog << "\n";
		exit(-1);
	}
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::use() const{
	glUseProgram(ID);
}

void Shader::setUniformBool(const std::string& name, const bool value) const {
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniformInt(const std::string& name, const int value) const {
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniformFloat(const std::string& name, const float value) const {
	use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value) const {
	use();
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setUniformVec3(const std::string& name, const float v1, const float v2, const float v3) const {
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setUniformMat3(const std::string& name, const glm::mat3& value) const {
	use();
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& value) const {
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}