#include "Observer.h"

void Observer::processKey(GLFWwindow* window) {
	// 平移
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Z负方向
		center = center - translateSpeed * R[2];
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Z正方向
		center = center + translateSpeed * R[2];
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // X负方向
		center = center - translateSpeed * R[0];
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // X正方向
		center = center + translateSpeed * R[0];
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Y负方向
		center = center - translateSpeed * R[1];
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Y正方向
		center = center + translateSpeed * R[1];
	}
	// 俯仰角 (I,K)，绕Rx轴旋转
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) { // 拉起
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[0]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) { // 俯冲
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[0]));
		R = _key_rotate * R;
	}
	// 翻滚角 (J,L)，绕Ry轴旋转
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) { 
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[1]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { 
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[1]));
		R = _key_rotate * R;
	}
	// 偏航角 (U,O)，绕Rz轴旋转
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotateSpeed), R[2]));
		R = _key_rotate * R;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		glm::mat3 _key_rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotateSpeed), R[2]));
		R = _key_rotate * R;
	}
}