/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __CG_GFX_H__
#define __CG_GFX_H__

#include <GL/glew.h>

#include "cg_math.h"
#include "cg_util.h"

enum cg_shader_attrib_loc {
	CG_SATTRIB_LOC_VERTEX_POSITION,
	CG_SATTRIB_LOC_VERTEX_NORMAL,
	CG_SATTRIB_LOC_VERTEX_UV,
};

enum cg_shader_uniform {
	CG_SUNIFORM_MATRIX_MODEL,
	CG_SUNIFORM_MATRIX_VIEW,
	CG_SUNIFORM_MATRIX_PROJECTION,
	CG_SUNIFORM_SAMPLER_TEXTURE,
	CG_SUNIFORM_SIZE,
};

enum cg_texture_type {
	CG_TEXTURE_2D,
};

struct cg_mesh {
	size_t num_verts;
	float *verts;

	size_t num_indices;
	int *indices;

	size_t num_uvs;
	float *uvs;

	size_t num_normals;
	float *normals;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int nbo;
	unsigned int tbo;
};

struct cg_shader_prg_builder {
	struct CG_DA(unsigned int) shaders;
};

struct cg_shader_prg {
	unsigned int id;
	int uniform_locs[CG_SUNIFORM_SIZE];
};

struct cg_texture {
	enum cg_texture_type type;
	unsigned int gl_tex;
};

struct cg_model {
	struct cg_shader_prg prg;
	struct cg_mesh mesh;
	struct cg_mat4f model_matrix;
	struct cg_texture texture;
};

struct cg_camera {
	struct cg_vec3f pos;
	struct cg_mat4f rotation;

	float fov;
	float far_plane;
	float near_plane;
};

void cg_start_render(void);
void cg_end_render(void);

struct cg_mesh cg_mesh_create(const float *verts, const size_t num_verts,
			      const int *indices, const size_t num_indices,
			      const float *normals, const size_t num_normals,
			      const float *uvs, const size_t num_uvs);
struct cg_mesh cg_mesh_from_obj_file(char *path);

void cg_shader_prg_builder_add_shader(struct cg_shader_prg_builder *builder, const char *src,
				      int length,
				      GLenum type);
struct cg_shader_prg cg_shader_prg_builder_build(struct cg_shader_prg_builder *builder);

struct cg_texture cg_texture_create_2d(const unsigned char *data, size_t width, size_t height,
				       int internal_format, int format);

struct cg_model cg_model_create(struct cg_mesh mesh);
void cg_model_put_shader_prg(struct cg_model *model, struct cg_shader_prg prg);
void cg_model_put_model_matrix(struct cg_model *model, struct cg_mat4f model_matrix);
void cg_model_put_texture(struct cg_model *model, struct cg_texture texture);
void cg_model_draw(struct cg_model *model);

struct cg_camera cg_camera_create(const struct cg_vec3f pos,
				  const float fov,
				  const float near_plane,
				  const float far_plane);

void cg_camera_look_at(struct cg_camera *camera, const struct cg_vec3f target);

void cg_camera_update_FPS(struct cg_camera *camera);

#endif // __CG_GFX_H__
