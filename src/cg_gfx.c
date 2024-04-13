/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#define BED_FUNC_PREFIX cg
#include "external/bed.h"

#include "cg_core.h"
#include "cg_gfx.h"
#include "cg_input.h"
#include "cg_math.h"
#include "cg_util.h"

extern struct cg_contex cg_ctx;

static const char* shader_attrib_names[] = {
	[CG_SATTRIB_LOC_VERTEX_POSITION] = "position",
	[CG_SATTRIB_LOC_VERTEX_UV] = "uv",
	[CG_SATTRIB_LOC_VERTEX_NORMAL] = "normal",
};

static const char* shader_uniform_names[] = {
	[CG_SUNIFORM_MATRIX_MODEL] = "model",
	[CG_SUNIFORM_MATRIX_VIEW] = "view",
	[CG_SUNIFORM_MATRIX_PROJECTION] = "projection",
	[CG_SUNIFORM_SAMPLER_TEXTURE] = "tex",
};

void cg_start_render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cg_assert(!cg_check_gl());
}

void cg_end_render(void) {
	SDL_GL_SwapWindow(cg_ctx.window.base);
}

struct cg_mesh cg_mesh_create(const float *verts, const size_t num_verts,
			      const int *indices, const size_t num_indices,
			      const float *normals, const size_t num_normals,
			      const float *uvs, const size_t num_uvs) {
	cg_assert(verts != NULL);

	struct cg_mesh mesh = {0};

	mesh.verts = malloc(sizeof(*mesh.verts) * num_verts * 3);
	assert(mesh.verts);

	memcpy(mesh.verts, verts, sizeof(*mesh.verts) * num_verts * 3);

	mesh.num_verts = num_verts;

	if (indices != NULL) {
		mesh.indices = malloc(sizeof(*mesh.indices) * num_indices);
		assert(mesh.indices);

		memcpy(mesh.indices, indices, sizeof(*mesh.indices) * num_indices);
		mesh.num_indices = num_indices;
	}

	if (normals != NULL) {
		mesh.normals = malloc(sizeof(*mesh.normals) * num_normals * 3);
		assert(mesh.normals);

		memcpy(mesh.normals, normals, sizeof(*mesh.normals) * num_normals * 3);
		mesh.num_normals = num_normals;
	}

	if (uvs != NULL) {
		mesh.uvs = malloc(sizeof(*mesh.uvs) * num_uvs * 2);
		assert(mesh.uvs);

		memcpy(mesh.uvs, uvs, sizeof(*mesh.uvs) * num_uvs * 2);
		mesh.num_uvs = num_uvs;
	}

	cg_info("Mesh loaded:\n");
	cg_info("\tnumber of vertices: %zu\n", num_verts);
	if (mesh.indices != NULL)
		cg_info("\tnumber of indicies: %zu\n", num_indices);
	if (mesh.normals != NULL)
		cg_info("\tnumber of normals: %zu\n", num_normals);
	if (mesh.uvs != NULL)
		cg_info("\tnumber of uvs: %zu\n", num_uvs);

	glGenVertexArrays(1, &mesh.vao);
	cg_assert(mesh.vao > 0);

	glBindVertexArray(mesh.vao);
	cg_assert(!cg_check_gl());

	glGenBuffers(1, &mesh.vbo);
	cg_assert(mesh.vbo > 0);

	if (mesh.indices != NULL) {
		glGenBuffers(1, &mesh.ebo);
	}

	if (mesh.normals != NULL) {
		glGenBuffers(1, &mesh.nbo);
	}

	if (mesh.uvs != NULL) {
		glGenBuffers(1, &mesh.tbo);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	cg_assert(!cg_check_gl());

	glBufferData(GL_ARRAY_BUFFER, sizeof(*mesh.verts) * mesh.num_verts * 3, mesh.verts,
		     GL_STATIC_DRAW);
	cg_assert(!cg_check_gl());

	glVertexAttribPointer(CG_SATTRIB_LOC_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE,
			      sizeof(*mesh.verts) * 3, 0);
	cg_assert(!cg_check_gl());

	glEnableVertexAttribArray(CG_SATTRIB_LOC_VERTEX_POSITION);
	cg_assert(!cg_check_gl());

	if (mesh.indices != NULL) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
		cg_assert(!cg_check_gl());

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*mesh.indices) * num_indices,
			     mesh.indices, GL_STATIC_DRAW);
		cg_assert(!cg_check_gl());
	}

	if (mesh.uvs != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.tbo);
		cg_assert(!cg_check_gl());

		glBufferData(GL_ARRAY_BUFFER, sizeof(*mesh.uvs) * mesh.num_uvs * 2,
			     mesh.uvs, GL_STATIC_DRAW);
		cg_assert(!cg_check_gl());

		glVertexAttribPointer(CG_SATTRIB_LOC_VERTEX_UV, 2, GL_FLOAT, GL_FALSE,
				      sizeof(*mesh.uvs) * 2, 0);
		cg_assert(!cg_check_gl());

		glEnableVertexAttribArray(CG_SATTRIB_LOC_VERTEX_UV);
		cg_assert(!cg_check_gl());
	}

	if (mesh.normals != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.nbo);
		cg_assert(!cg_check_gl());

		glBufferData(GL_ARRAY_BUFFER, sizeof(*mesh.normals) * mesh.num_normals * 3,
			     mesh.normals, GL_STATIC_DRAW);
		cg_assert(!cg_check_gl());

		glVertexAttribPointer(CG_SATTRIB_LOC_VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE,
				      sizeof(*mesh.normals) * 3, 0);
		cg_assert(!cg_check_gl());

		glEnableVertexAttribArray(CG_SATTRIB_LOC_VERTEX_NORMAL);
		cg_assert(!cg_check_gl());
	}

	return mesh;
}

struct lex {
	char *base;
	char *it;
	size_t size;
};

static const char *used_chars = "\n/";

static bool is_finished(struct lex *l) {
	return l->it > l->base + l->size;
}

static void next_word(struct lex *l, char **word, size_t *len) {
	while (!is_finished(l) && isblank(*l->it))
		l->it++;

	*word = l->it;

	if (strchr(used_chars,*l->it) != NULL) {
		*len = 1;
		l->it++;
		return;
	}

	while (!is_finished(l) && !isblank(*l->it) && strchr(used_chars, *l->it) == NULL)
		l->it++;

	*len =  l->it - *word;
}

struct da_float CG_DA(float);

static void parse_coord(struct da_float *coords, struct lex *l) {
	char *word;
	size_t len;
	while (next_word(l, &word, &len), *word != '\n') {
		char *end = word + len;
		float num = strtof(word, &end);
		cg_assert(end != word);

		cg_da_append(coords, num);
	}
}

struct index {
	int v;
	int vt;
	int vn;
};
struct da_index CG_DA(struct index);

static void parse_indices(struct da_index *indices, struct lex *l) {
	char *word;
	size_t len;
	struct index idx = {-1, -1, -1};
	size_t slash_count = 0;
	bool last_was_slash = false;
	bool first = true;
	while (next_word(l, &word, &len), *word != '\n') {
		if (*word == '/') {
			slash_count++;
			last_was_slash = true;
			continue;
		}

		char *end = word + len;
		int num = strtol(word, &end, 10);
		cg_assert(end != word);

		num--;

		if (!first && !last_was_slash) {
			cg_da_append(indices, idx);
			idx = (struct index){-1, -1, -1};
			slash_count = 0;
		}

		if (slash_count == 0)
			idx.v = num;
		else if (slash_count == 1)
			idx.vt = num;
		else if (slash_count == 2)
			idx.vn = num;
		else
			cg_assert(0);

		last_was_slash = false;
		first = false;
	}
	cg_da_append(indices, idx);
}

enum coord {
	COORD_X = 0,
	COORD_Y,
	COORD_Z,
};

static void find_coord_min_max(const struct da_float da, float *min, float *max, enum coord c) {
	*min = da.items[c];
	*max = *min;
	for (size_t i = c + 3; i < da.len; i += 3) {
		*min = CG_MIN(da.items[i], *min);
		*max = CG_MAX(da.items[i], *max);
	}
}

struct cg_mesh cg_mesh_from_obj_data(char *data, size_t size) {
	cg_assert(data != NULL);

	struct lex l = {
		.base = data,
		.it = data,
		.size = size,
	};

	struct da_float verts = { 0 };
	struct da_float uvs = { 0 };
	struct da_float norms = { 0 };
	struct da_index indices = { 0 };

	while (!is_finished(&l)) {
		char *word;
		size_t w_len;
		next_word(&l, &word, &w_len);

		if (w_len == 1 && word[0] == 'v') {
			parse_coord(&verts, &l);
		}

		if (w_len == 2 && word[0] == 'v' && word[1] == 't') {
			parse_coord(&uvs, &l);
		}

		if (w_len == 2 && word[0] == 'v' && word[1] == 'n') {
			parse_coord(&norms, &l);
		}

		if (*word == 'f') {
			parse_indices(&indices, &l);
		}
	}

	float x_min, x_max;
	find_coord_min_max(verts, &x_min, &x_max, COORD_X);
	float y_min, y_max;
	find_coord_min_max(verts, &y_min, &y_max, COORD_Y);
	float z_min, z_max;
	find_coord_min_max(verts, &z_min, &z_max, COORD_Z);

	float x_size = x_max - x_min;
	float y_size = y_max - y_min;
	float z_size = z_max - z_min;

	for (size_t i = 0; i < verts.len; i += 3) {
		float *x = &verts.items[i + 0];
		float *y = &verts.items[i + 1];
		float *z = &verts.items[i + 2];

		*x = (*x - x_size / 2 - x_min) / x_size;
		*y = (*y - y_size / 2 - y_min) / x_size;
		*z = (*z - z_size / 2 - z_min) / x_size;
	}

	float *ex_verts = verts.len == 0 ? NULL : malloc(sizeof(*ex_verts) * indices.len * 3);
	float *ex_uvs = uvs.len == 0 ? NULL : malloc(sizeof(*ex_uvs) * indices.len * 2);
	float *ex_norms = norms.len == 0 ? NULL : malloc(sizeof(*ex_uvs) * indices.len * 3);

	for (size_t i = 0; i < indices.len; i++) {
		if (ex_verts != NULL) {
			ex_verts[i * 3 + 0] = verts.items[indices.items[i].v * 3 + 0];
			ex_verts[i * 3 + 1] = verts.items[indices.items[i].v * 3 + 1];
			ex_verts[i * 3 + 2] = verts.items[indices.items[i].v * 3 + 2];
		}

		if (ex_uvs != NULL) {
			ex_uvs[i * 2 + 0] = uvs.items[indices.items[i].vt * 2 + 0];
			ex_uvs[i * 2 + 1] = uvs.items[indices.items[i].vt * 2 + 1];
		}

		if (ex_norms != NULL) {
			ex_norms[i * 3 + 0] = norms.items[indices.items[i].vn * 3 + 0];
			ex_norms[i * 3 + 1] = norms.items[indices.items[i].vn * 3 + 1];
			ex_norms[i * 3 + 2] = norms.items[indices.items[i].vn * 3 + 2];
		}
	}

	struct cg_mesh mesh = cg_mesh_create(ex_verts, indices.len,
					     NULL, 0,
					     ex_norms, indices.len,
					     ex_uvs, indices.len);
	free(verts.items);
	free(uvs.items);
	free(norms.items);
	free(ex_verts);
	free(ex_uvs);
	free(ex_norms);
	free(indices.items);

	return mesh;
}

static unsigned int create_shader(const char *src, int length, GLenum type) {
	unsigned int shader = glCreateShader(type);
	cg_assert(shader != 0);

	if (length < 1)
		glShaderSource(shader, 1, &src, NULL);
	else
		glShaderSource(shader, 1, &src, &length);
	cg_assert(!cg_check_gl());

	glCompileShader(shader);
	cg_assert(!cg_check_gl());

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	cg_assert(!cg_check_gl());

	char info_log[1024];
	if (status == false) {
		glGetShaderInfoLog(shader, 1024, NULL, info_log);
		cg_error("Shader compilation error: %s", info_log);
		cg_assert(0);
	}

	return shader;
}

void cg_shader_prg_builder_add_shader(struct cg_shader_prg_builder *builder, const char *src,
				      int length,
				      GLenum type) {
	unsigned int shader = create_shader(src, length, type);
	cg_da_append(&builder->shaders, shader);
}

static void bind_loc(unsigned int prg, enum cg_shader_attrib_loc loc) {
	glBindAttribLocation(prg, loc, shader_attrib_names[loc]);
	cg_assert(!cg_check_gl());
}

struct cg_shader_prg cg_shader_prg_builder_build(struct cg_shader_prg_builder *builder) {
	struct cg_shader_prg prg = {0};

	prg.id = glCreateProgram();
	cg_assert(prg.id != 0);

	for (size_t i = 0; i < builder->shaders.len; i++) {
		unsigned int shader = builder->shaders.items[i];

		glAttachShader(prg.id, shader);
		cg_assert(!cg_check_gl());
	}

	bind_loc(prg.id, CG_SATTRIB_LOC_VERTEX_POSITION);
	bind_loc(prg.id, CG_SATTRIB_LOC_VERTEX_UV);

	glLinkProgram(prg.id);
	cg_assert(!cg_check_gl());

	int status;
	char info_log[1024];
	glGetProgramiv(prg.id, GL_LINK_STATUS, &status);
	cg_assert(!cg_check_gl());

	if (status == false) {
		glGetProgramInfoLog(prg.id, 1024, NULL, info_log);
		cg_error("Shader program linking error: %s", info_log);
		cg_assert(0);
	}

	for (size_t i = 0; i < CG_SUNIFORM_SIZE; i++) {
		prg.uniform_locs[i] = glGetUniformLocation(prg.id, shader_uniform_names[i]);
		cg_assert(!cg_check_gl());
	}

	for (size_t i = 0; i < builder->shaders.len; i++) {
		unsigned int shader = builder->shaders.items[i];

		glDeleteShader(shader);
		cg_assert(!cg_check_gl());
	}

	return prg;
}

struct cg_texture cg_texture_create_2d(const unsigned char *data, size_t width, size_t height,
				       int internal_format, int format) {
	struct cg_texture tex = { .type = CG_TEXTURE_2D };

	glGenTextures(1, &tex.gl_tex);
	cg_assert(!cg_check_gl());

	glBindTexture(GL_TEXTURE_2D, tex.gl_tex);
	cg_assert(!cg_check_gl());

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE,
		     data);
	cg_assert(!cg_check_gl());

	glGenerateMipmap(GL_TEXTURE_2D);
	cg_assert(!cg_check_gl());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	cg_assert(!cg_check_gl());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	cg_assert(!cg_check_gl());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	cg_assert(!cg_check_gl());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cg_assert(!cg_check_gl());

	return tex;
}

struct cg_model cg_model_create(struct cg_mesh mesh) {
	static struct cg_shader_prg default_shader_prg = {0};
	static struct cg_texture default_tex = {0};

	if (default_shader_prg.id == 0) {
		struct cg_shader_prg_builder builder = {0};

		size_t vert_shader_len;
		const char *vert_shader_src = (char*)cg_bed_get("../resources/shaders/vert.glsl",
								   &vert_shader_len);
		cg_assert(vert_shader_src != NULL);

		cg_shader_prg_builder_add_shader(&builder,
						 vert_shader_src,
						 vert_shader_len,
						 GL_VERTEX_SHADER);

		size_t frag_shader_len;
		const char *frag_shader_src = (char*)cg_bed_get("../resources/shaders/frag.glsl",
								&frag_shader_len);
		cg_assert(frag_shader_src != NULL);

		cg_shader_prg_builder_add_shader(&builder,
						 frag_shader_src,
						 frag_shader_len,
						 GL_FRAGMENT_SHADER);

		default_shader_prg = cg_shader_prg_builder_build(&builder);
	}

	if (default_tex.gl_tex == 0) {
		const unsigned char default_tex_data[] ={
			0xff, 0x00, 0xff,
			0x00, 0x00, 0x00,
			0xff, 0x00, 0xff,
			0x00, 0x00, 0x00,
		};

		default_tex = cg_texture_create_2d(default_tex_data, 4, 1,
						   GL_RGB, GL_RGB);
		glBindTexture(GL_TEXTURE_2D, default_tex.gl_tex);
		cg_assert(!cg_check_gl());
		cg_assert(!cg_check_gl());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		cg_assert(!cg_check_gl());
	}

	return (struct cg_model) {
		.mesh = mesh,
		.prg = default_shader_prg,
		.texture = default_tex,
		.model_matrix = cg_mat4f_identity(),
	};
}

void cg_model_put_shader_prg(struct cg_model *model, struct cg_shader_prg prg) {
	model->prg = prg;
}

void cg_model_put_model_matrix(struct cg_model *model, struct cg_mat4f model_matrix) {
	memcpy(model->model_matrix.d, model_matrix.d, sizeof(model_matrix.d));
}

void cg_model_put_texture(struct cg_model *model, struct cg_texture texture) {
	model->texture = texture;
}

void cg_model_draw(struct cg_model *model) {
	glUseProgram(model->prg.id);
	cg_assert(!cg_check_gl());

	glUniformMatrix4fv(model->prg.uniform_locs[CG_SUNIFORM_MATRIX_MODEL],
			   1, false, model->model_matrix.d);
	cg_assert(!cg_check_gl());

	glUniformMatrix4fv(model->prg.uniform_locs[CG_SUNIFORM_MATRIX_VIEW],
			   1, false, cg_ctx.view_matrix.d);
	cg_assert(!cg_check_gl());

	glUniformMatrix4fv(model->prg.uniform_locs[CG_SUNIFORM_MATRIX_PROJECTION],
			   1, false, cg_ctx.projection_matrix.d);
	cg_assert(!cg_check_gl());

	if (model->texture.gl_tex != 0) {
		glActiveTexture(GL_TEXTURE0);
		cg_assert(!cg_check_gl());
		glBindTexture(GL_TEXTURE_2D, model->texture.gl_tex);
		cg_assert(!cg_check_gl());
	}

	glBindVertexArray(model->mesh.vao);
	cg_assert(!cg_check_gl());

	if (model->mesh.indices == NULL)
		glDrawArrays(GL_TRIANGLES, 0, model->mesh.num_verts);
	else
		glDrawElements(GL_TRIANGLES, model->mesh.num_indices, GL_UNSIGNED_INT, 0);

	cg_assert(!cg_check_gl());
}

struct cg_camera cg_camera_create(const struct cg_vec3f pos,
				  const float fov,
				  const float near_plane,
				  const float far_plane) {

	float aspect = cg_ctx.window.width / cg_ctx.window.height;

	float_t depth = near_plane - far_plane;

	cg_ctx.projection_matrix = (struct cg_mat4f) {{
		1.0 / (tanf(fov / 2) * aspect), 0.0, 0.0, 0.0,
		0.0, 1.0 / tanf(fov / 2), 0.0 , 0.0,
		0.0, 0.0, (near_plane + far_plane) / depth, 2 * near_plane * far_plane / depth,
		0.0, 0.0, -1.0, 0.0,
	}};

	return (struct cg_camera) {
		.pos = pos,
		.rotation = cg_mat4f_identity(),
		.fov = fov,
		.near_plane = near_plane,
		.far_plane = far_plane,
	};
}

void cg_camera_update_FPS(struct cg_camera *camera) {
	struct cg_vec3f ds = {0};

	if (cg_keycode_is_down(CG_KEY_W))
		ds.z -= 0.1;

	if (cg_keycode_is_down(CG_KEY_S))
		ds.z += 0.1;

	if (cg_keycode_is_down(CG_KEY_A))
		ds.x -= 0.1;

	if (cg_keycode_is_down(CG_KEY_D))
		ds.x += 0.1;

	struct cg_vec2f rel_pos = cg_mouse_rel_pos();

	rel_pos.x /= cg_ctx.window.width;
	rel_pos.y /= cg_ctx.window.height;

	rel_pos.x *= 10.0;
	rel_pos.y *= 10.0;

	camera->rotation = cg_mat4f_multiply(cg_mat4f_rotate_y(rel_pos.x), camera->rotation);
	camera->rotation = cg_mat4f_multiply(camera->rotation, cg_mat4f_rotate_x(rel_pos.y));

	float pitch, yaw, roll;
	cg_mat4f_rotation_to_angles(camera->rotation, &pitch, &yaw, &roll);
	ds = cg_vec3f_mat4f_multiply(ds, cg_mat4f_rotate_y(yaw));
	camera->pos = cg_vec3f_add(camera->pos, ds);

	struct cg_mat4f translation = cg_mat4f_translate(-camera->pos.x,
							 -camera->pos.y,
							 -camera->pos.z);

	cg_ctx.view_matrix = cg_mat4f_multiply(translation, camera->rotation);
}
