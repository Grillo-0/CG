#version 330 core
in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 norm_frac;
out vec2 uv_frac;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	norm_frac = normal;
	uv_frac = uv;
	uv_frac.y *= -1;
	gl_Position =  vec4(position, 1.0) * model * view * projection;
}
