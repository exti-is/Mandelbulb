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


class Cube
{
public:
	Cube(glm::vec3 position, unsigned int shaderID);
	~Cube();

	void setModel(glm::mat4 newModel);
	inline glm::mat4 getModel() { return model; }

	void setColor(float r, float g, float b);

	void draw();
	void draw(glm::vec3 newPosition, float r_value);
private:
	unsigned int vao;
	unsigned int vbo;

	glm::mat4 model;
	unsigned int currentShader;

	float r;
	float g;
	float b;

};

