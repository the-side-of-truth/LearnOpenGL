#ifndef OBSERVER_H
#define OBSERVER_H
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Observer {
public:
	Observer() {
		R = glm::mat3(1.0f);
		center = glm::vec3(0.0f, 0.0f, 3.0f);
	}

	Observer(const glm::vec3& _center) {
		R = glm::mat3(1.0f);
		center = _center;
		rotateSpeed = 0.01;
		translateSpeed = 0.0004;
	}

	glm::mat4 getTransformer() const{
		glm::mat4 _transformer(0.0f);
		glm::mat3 Rt = glm::transpose(R);
		// һ��һ�е�����
		_transformer[0] = glm::vec4(Rt[0], 0.0f);
		_transformer[1] = glm::vec4(Rt[1], 0.0f);
		_transformer[2] = glm::vec4(Rt[2], 0.0f);
		_transformer[3] = glm::vec4(-Rt * center, 1.0f);
		return _transformer;
	}

	glm::vec3 getCamCenter() const {
		return center;
	}

	glm::vec3 getEyeDirection() const {
		return -glm::vec3(R[2].x, R[2].y, R[2].z);
	}
	
	// ����������
	void processKey(GLFWwindow* window);

private:
	glm::mat3 R;// c2w��ת����(Rx, Ry, Rz),����������
	glm::vec3 center;// �������
	
	float rotateSpeed;// �Ƕ�ת��
	float translateSpeed;// ƽ���ٶ�

};
#endif // OBSERVER_H
