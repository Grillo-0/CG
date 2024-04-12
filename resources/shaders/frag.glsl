#version 330 core

in vec3 norm_frac;
in vec2 uv_frac;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
	float mag = dot(normalize(norm_frac), vec3(1.0,1,-1.0));
	FragColor = mix(texture(tex, uv_frac), vec4(0.1, 0.1,0.1,1), mag);
}
