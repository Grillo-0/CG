#version 330 core

in vec3 norm_frac;
in vec2 uv_frac;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
	FragColor = texture(tex, uv_frac);
}
