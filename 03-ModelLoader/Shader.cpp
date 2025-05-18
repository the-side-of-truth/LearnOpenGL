# include "shader/Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	/*---------------------------��ɫ���ļ���ȡ---------------------------*/
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "��ɫ���ļ��޷���" << std::endl;
		exit(-1);
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	/*---------------------------��ɫ���ļ���ȡ(��)---------------------------*/

	/*---------------------------��ɫ������---------------------------*/
	// �������־��log����
	int  success;
	char infoLog[512];
    // ���������붥����ɫ��
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vShaderCode, NULL);
    glCompileShader(VertexShader);
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "������ɫ������ʧ��\n" << infoLog << "\n";
        exit(-1);
    }
    // ����Ƭ����ɫ��
    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "Ƭ����ɫ������ʧ��\n" << infoLog << "\n";
        exit(-1);
    }
	/*---------------------------��ɫ������(��)---------------------------*/
	// ������ɫ���������Ӳ�ɾ����ɫ��
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	ID = shaderProgram;
	glAttachShader(shaderProgram, VertexShader);
	glAttachShader(shaderProgram, FragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "��ɫ����������ʧ��\n" << infoLog << "\n";
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