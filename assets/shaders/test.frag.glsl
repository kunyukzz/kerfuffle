// FRAGMENT SHADER
#version 330 core

out vec4 frag_color;

in vec3 out_frag;
in vec3 out_normal;

uniform vec3 light_pos; 
uniform vec3 view_pos; 
uniform vec3 light_color;
uniform vec3 object_color;

void main() {
	// ambient
	float strength = 0.1;
	vec3 ambient = strength * light_color;

	// diffuse
	vec3 norm = normalize(out_normal);
	vec3 light_dir = normalize(light_pos - out_frag);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	// specular
	float spec_str = 0.5;
	vec3 view_dir = normalize(view_pos - out_frag);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
	vec3 specular = spec_str * spec * light_color;

	vec3 result = (ambient + diffuse + specular) * object_color;
	frag_color = vec4(result, 1.0);
}
