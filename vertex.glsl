#version 430

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in mat4 instanceTransform;

out vec3 vertexColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main() {
	gl_Position = projection * view * instanceTransform * vec4(aPos, 1.0);
	vertexColor = color;
}