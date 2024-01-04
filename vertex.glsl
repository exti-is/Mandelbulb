#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 color;
//uniform int mandelbulb;
//uniform float r;

//uniform mat4 u_transform;

//float map(float var, float min1, float max1, float min2, float max2) {
//	return min2 + (max2 - min2) * ((var - min1) / (max1 - min1));
//}

void main() {
	//gl_Position = u_transform * vec4(aPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertexColor = color;

//	vertexColor = vec3(0.0, 0.0, 0.0);

	
	//if (mandelbulb == 1) {
	//	vertexColor = vec3(map(r, 0.f, 2.f, 0.f, 1.f), map(r, 0.f, 2.f, 0.f, 1.f), map(r, 0.f, 2.f, 0.f, 1.f));
	//}
	
}