#include "Cube.h"

GLuint indices[] = {
		0, 1, 2,  // Front face
		2, 3, 0,

		4, 5, 6,  // Right face
		6, 7, 4,

		8, 9, 10, // Back face
		10, 11, 8,

		12, 13, 14, // Left face
		14, 15, 12,

		16, 17, 18, // Bottom face
		18, 19, 16,

		20, 21, 22, // Top face
		22, 23, 20
};

float cubeVertices[] = {
		// Front Face
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		// Right face
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,

		 // Back face
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,

		 // Left face
	    -0.5f, -0.5f, -0.5f,
	    -0.5f, -0.5f,  0.5f,
	    -0.5f,  0.5f,  0.5f,
	    -0.5f,  0.5f, -0.5f,

		 // Bottom face
	    -0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
	    -0.5f, -0.5f,  0.5f,

		 // Top face
	    -0.5f,  0.5f,  0.5f,
	 	 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
	    -0.5f,  0.5f, -0.5f
};

Cube::Cube(std::vector<std::pair<glm::mat4, float>> instanceTransforms, unsigned int shaderID) : currentShader{ shaderID }, instanceTransforms{ instanceTransforms } {
	std::vector<glm::mat4> instanceTransformsVec = {};
	for (auto& _ : instanceTransforms)
		instanceTransformsVec.push_back(_.first);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// instancing //

	glGenBuffers(1, &instanceVbo);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, instanceTransformsVec.size() * sizeof(glm::mat4), &instanceTransformsVec[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0); // mat 4 = 4 separate vec4s
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	////////////////
}

void Cube::draw() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	glUniform3f(glGetUniformLocation(currentShader, "color"), 0.85f, 0.85f, 0.85f);
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0, instanceTransforms.size());

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}