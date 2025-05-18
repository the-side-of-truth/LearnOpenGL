#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>

class Shader {
public:
	Shader() {};
	Shader(const char* vertexPath, const char* fragmentPath);
	void use() const;// 启用Shader
	uint32_t getID() { return ID; }
	// 设置uniform变量的值
	void setUniformBool(const std::string& name, const bool value) const;
	void setUniformInt(const std::string& name, const int value) const;
	void setUniformFloat(const std::string& name, const float value) const;
	void setUniformVec3(const std::string& name, const glm::vec3& value) const;
	void setUniformVec3(const std::string& name, const float v1, const float v2, const float v3) const;
	void setUniformMat3(const std::string& name, const glm::mat3& value) const;
	void setUniformMat4(const std::string& name, const glm::mat4& value) const;
private:
	uint32_t ID; // 着色器ID(句柄)
	
};

#endif // !SHADER_H

