#include "Observer.h"

void Observer::processKey(GLFWwindow* window) {
	// ƽ��
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Z������
		center = center - translateSpeed * R[2];
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Z������
		center = center + translateSpeed * R[2];
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // X������
		center = center - translateSpeed * R[0];
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // X������
		center = center + translateSpeed * R[0];
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Y������
		center = center - translateSpeed * R[1];
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Y������
		center = center + translateSpeed * R[1];
	}
	// ������ (I,K)����Rx����ת
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) { // ����
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[0]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) { // ����
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[0]));
		R = _key_rotate * R;
	}
	// ������ (J,L)����Ry����ת
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) { 
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[1]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { 
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[1]));
		R = _key_rotate * R;
	}
	// ƫ���� (U,O)����Rz����ת
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[2]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[2]));
		R = _key_rotate * R;
	}
}