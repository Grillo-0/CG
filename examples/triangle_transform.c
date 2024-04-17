/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <math.h>

#include "cg_core.h"
#include "cg_gfx.h"
#include "cg_math.h"
#include "cg_util.h"

#define PI (acos(-1))

float verts[] = {
	-1.0, -1.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, -1.0, 0.0,
};

const char *vert_shader_src =
	"#version 330 core\n"
	"in vec3 position;\n"
	"uniform mat4 model;\n"
	"out vec3 pos;\n"
	"void main() {\n"
		"pos = position;\n"
		"vec3 temp = (vec4(position, 1.0) * model).xyz;\n"
		"gl_Position = vec4(temp, 1.0);\n"
	"};";

const char *frag_shader_src =
	"#version 330 core\n"
	"in vec3 pos;\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
		"vec3 temp_pos = (pos + 1) / 2;\n"
		"FragColor = vec4(temp_pos, 1.0);\n"
	"}";

int main(void) {
	cg_window_create("Triangle Transform Example", 400 , 400);

	struct cg_mesh mesh = cg_mesh_create(verts, CG_ARRAY_LEN(verts) / 3,
					     NULL, 0,
					     NULL, 0,
					     NULL, 0);

	struct cg_shader_prg_builder builder = {0};

	cg_shader_prg_builder_add_shader(&builder, vert_shader_src, -1, GL_VERTEX_SHADER);
	cg_shader_prg_builder_add_shader(&builder, frag_shader_src, -1, GL_FRAGMENT_SHADER);

	struct cg_shader_prg shader_prog = cg_shader_prg_builder_build(&builder);

	struct cg_model model = cg_model_create(&mesh, 1);
	cg_model_put_shader_prg(&model, shader_prog);

	struct cg_mat4f transform = cg_mat4f_identity();
	transform = cg_mat4f_multiply(transform, cg_mat4f_rotate_z(PI/4));
	transform = cg_mat4f_multiply(transform, cg_mat4f_scale(0.25, 0.25, 1.0));
	transform = cg_mat4f_multiply(transform, cg_mat4f_translate(0.5, 0.5, 0.0));

	cg_model_put_model_matrix(&model, transform);

	while (!cg_window_should_close()) {
		cg_start_render();

		glClearColor(0.1, 0.1, 0.1, 1.0);

		cg_model_draw(&model);

		cg_end_render();
	}

}
