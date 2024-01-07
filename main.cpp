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
#include <thread>
#include <mutex>

#include "Camera.h"
#include "Cube.h"

#define DIMENSIONS 384
#define ORDER 8

float map(float var, float min1, float max1, float min2, float max2) {
	return min2 + (max2 - min2) * ((var - min1) / (max1 - min1));
}

struct ShaderFiles {
	unsigned int vertex;
	unsigned int fragment;
};

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderID;

Camera camera;
void generateMandelbulbSetThreads(int thread_count, int dim, std::vector<std::pair<glm::mat4, float>>& mandelbulb_positions);

struct Spherical {
	float r;
	float theta;
	float phi;
	Spherical(float x, float y, float z) {
		r = sqrt(x*x + y*y + z*z);
		theta = atan2(sqrt(x*x + y*y), z);
		phi = atan2(y, x);
	}
};

void initShaders() {
	std::string line;

	std::ifstream vertex_shader("vertex.glsl");
	std::string vertexSourceStr;
	
	while (getline(vertex_shader, line))
		vertexSourceStr += line + "\n";
	const char* vertexSource = vertexSourceStr.c_str();

	std::ifstream fragment_shader("fragment.glsl");
	std::string fragmentSourceStr;
	while (getline(fragment_shader, line))
		fragmentSourceStr += line += "\n";
	const char* fragmentSource = fragmentSourceStr.c_str();

	// -- //
	
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
	}


	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
	}

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	glValidateProgram(shaderID);

	glUseProgram(shaderID);
}



float fov = 50;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset < 0) {
		fov -= 1;
	}
	else {
		fov += 1;
	}
}

double last_xpos = 0;
double last_ypos = 0;

float Y_VIEW_ANGLE = 0.f;
float X_VIEW_ANGLE = 0.f;
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	float cameraSpeed = 1;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		if (ypos < last_ypos) {
			X_VIEW_ANGLE -= cameraSpeed;
		} else if (ypos > last_ypos) {
			X_VIEW_ANGLE += cameraSpeed;
		}

		if (xpos < last_xpos) {
			Y_VIEW_ANGLE -= cameraSpeed;
		}
		else if (xpos > last_xpos) {
			Y_VIEW_ANGLE += cameraSpeed;
		}
	} 
	
	last_xpos = xpos;
	last_ypos = ypos;
}

float PosX = 0;
float PosY = 0;
float PosZ = -18;
void processInput(GLFWwindow* window) {
	float Speed = 0.1;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		PosY -= Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		PosY += Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		PosX += Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		PosX -= Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		PosZ -= Speed;
	} if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		PosZ += Speed;
	}
}


int
main(void) {
	//gl initialization 
	
	srand(time(NULL));

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "Mandelbulb Simulation", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error initializing GLEW";

	initShaders();

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//////////////

	std::vector<std::pair<glm::mat4, float>> mandelbulb_positions;
	generateMandelbulbSetThreads(13, DIMENSIONS, mandelbulb_positions); // capped at 16 threads for safety
	Cube cube(mandelbulb_positions, shaderID);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1, 0.1, 0.1, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderID);

		processInput(window);

		glm::mat4 projection;
		glm::mat4 view = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), 1280 / 720.f, 0.1f, 100.f);
		view = glm::translate(view, glm::vec3(PosX, PosY, PosZ)); 
		view = glm::rotate(view, glm::radians(X_VIEW_ANGLE), glm::vec3(1.f, 0.f, 0.f));
		view = glm::rotate(view, glm::radians(Y_VIEW_ANGLE), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view)); 
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		cube.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void generateMandelbulbMt(int start, int dimensions, int max_dimensions, float x, float y, float z, std::vector<std::pair<glm::vec3, float>>& mandelbulb_positions) {
	std::vector<std::pair<glm::vec3, float>> positions;
	for (int i = start; i < dimensions; i++) {
		for (int j = 0; j < max_dimensions; j++) {
			bool edge = false;
			for (int k = 0; k < max_dimensions; k++) {
				x = map(i, 0, max_dimensions, -1, 1);
				y = map(j, 0, max_dimensions, -1, 1);
				z = map(k, 0, max_dimensions, -1, 1);

				glm::vec3 v = glm::vec3(0, 0, 0);
				int n = ORDER;
				int iteration = 0;
				int maxIt = 20;
				while (true) {
					Spherical Coord = Spherical(v.x, v.y, v.z);

					// v = v^n + c // 

					// v^n = r^2<sinθn cosφn, sinθn sinφn, cosφn>
					float newx = pow(Coord.r, n) * sin(Coord.theta * n) * cos(Coord.phi * n);
					float newy = pow(Coord.r, n) * sin(Coord.theta * n) * sin(Coord.phi * n);
					float newz = pow(Coord.r, n) * cos(Coord.theta * n);

					//   v    v^n    c
					v.x = newx + x;
					v.y = newy + y;
					v.z = newz + z;

					// r is distance from center. if it is too far, the value is not bounded
					// meaning that it isn't part of the mandelbulb set, so you would break out
					// of the loop and not add to mandelbulb points.

					iteration++;
					if (Coord.r > 2) {
						if (edge) edge = false;
						break;
					}

					if (iteration > maxIt) { // only points where r is less than 2 will make it to max iterations, meaning that
						if (!edge) {         // it should be in the mandelbulb.
							edge = true;
							positions.push_back(std::pair<glm::vec3, float>(glm::vec3(x * 7, y * 7, z * 7), Coord.r));
						}
						break;
					}
				}
			}
		}
	}

	mandelbulb_positions = positions;
	printf("Finished thread #%i->#%i\n", start,dimensions);
}

void generateMandelbulbSetThreads(int thread_count, int dim, std::vector<std::pair<glm::mat4, float>>& mandelbulb_positions) {
	if (thread_count > 16) thread_count = 16; // capped at 16 for safety - 100% cpu usage
	std::vector<std::vector<std::pair<glm::vec3, float>>> thread_positions = {}; 
	std::vector<std::thread> threads;
	
	for (int i = 0; i < 128; i++) 
		thread_positions.push_back({});

	for (int i = 0; i < thread_count; i++) 
		threads.push_back(std::thread(generateMandelbulbMt, ((float)i/thread_count)*dim, (((float)i + 1) / thread_count) * dim, dim, 0, 0, 0, std::ref(thread_positions[i])));

	for (int i = 0; i < thread_count; i++) 
		threads[i].join();

	std::pair<glm::mat4, float> pos;
	glm::mat4 transform(0.01f);

	for (auto& vec : thread_positions) {
		for (auto& pair : vec) {
			//glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::mat4(0.01f);
			transform = glm::translate(transform, pair.first);
			transform = glm::scale(transform, glm::vec3(0.01));
			pos = {transform, pair.second};
			mandelbulb_positions.emplace_back(pos);			
		}
	}
	printf("Size: %d", mandelbulb_positions.size());


}