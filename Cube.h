#ifndef MANDELBULB_SRC_CUBE
#define MANDELBULB_SRC_CUBE

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

class Cube
{
public:
	Cube(std::vector<std::pair<glm::mat4, float>> instanceTransforms, unsigned int shaderID);
	void draw();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo; 
	unsigned int instanceVbo;
	unsigned int currentShader;

	std::vector<std::pair<glm::mat4, float>> instanceTransforms;
};


#endif