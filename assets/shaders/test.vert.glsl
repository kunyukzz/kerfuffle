// VERTEX SHADER
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

uniform mat4 model;

layout(std140) uniform camera_block {
	mat4 proj;
	mat4 view;
};

out vec3 out_frag;
out vec3 out_normal;

void main() {
	out_frag = vec3(model * vec4(a_pos, 1.0));
	out_normal = mat3(transpose(inverse(model))) * a_normal;

	gl_Position = proj * view * model * vec4(out_frag, 1.0);
	//gl_Position = proj * view * model * vec4(a_pos, 1.0);
	//gl_Position = vec4(a_pos, 1.0);
}
