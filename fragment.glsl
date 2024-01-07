#version 430

in vec3 vertexColor;

out vec4 FragColor;

uniform vec3 u_Color;


void main() {
	FragColor = vec4(vertexColor, 1.0);
}