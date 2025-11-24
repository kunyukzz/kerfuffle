// VERTEX SHADER
#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 model;

layout(std140) uniform camera_block {
	mat4 proj;
	mat4 view;
};

//uniform mat4 proj;
//uniform mat4 view;

void main() {
	gl_Position = proj * view * model * vec4(a_pos, 1.0);
	//gl_Position = vec4(a_pos, 1.0);
}
