#include <iostream>
//#include <cstdlib> // для system
using namespace std;
// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
double xpos, ypos;
void computeMatricesFromInputs() {
	
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	// Получить позицию мыши
	// Get mouse position
	
	glfwGetCursorPos(window, &xpos, &ypos);
	//cout << xpos << " " << ypos << endl;
	int xw, yw;
	glfwGetWindowSize(window, &xw, &yw);
	// Сбросить позицию мыши для следующего кадра
	glfwSetCursorPos(window, xw / 2, yw / 2);
	
	// Вычисляем углы
	horizontalAngle += mouseSpeed * deltaTime * float(xw / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(yw / 2 - ypos);
	cout << verticalAngle << " " << horizontalAngle << endl;
	
}