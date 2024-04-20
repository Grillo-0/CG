#version 330 core

in vec3 norm_frac;
in vec2 uv_frac;

out vec4 frag_color;

uniform vec3 diffuse_color;
uniform sampler2D diffuse_tex;
uniform bool diffuse_tex_provided;

void main() {
	if (diffuse_tex_provided) {
		frag_color = texture(diffuse_tex, uv_frac);
	} else {
		frag_color = vec4(diffuse_color, 1.0);
	}
}
