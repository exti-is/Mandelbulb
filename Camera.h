#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Camera {
public:
	Camera();

	glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	
	glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	void processInput(GLFWwindow* window);

	glm::mat4 GetViewMatrix() { return glm::lookAt(cameraPos, cameraPos + cameraFront, up); }

};