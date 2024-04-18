/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __CG_MATH_H__
#define __CG_MATH_H__

struct cg_vec2f {
	float x, y;
};

struct cg_vec3f {
	float x, y, z;
};

struct cg_mat4f {
	float d[16];
};

#define m(col, row) ((col) + (row) * 4)

struct cg_vec3f cg_vec3f_add(const struct cg_vec3f a, const struct cg_vec3f b);
struct cg_vec3f cg_vec3f_sub(const struct cg_vec3f a, const struct cg_vec3f b);
struct cg_vec3f cg_vec3f_cross(const struct cg_vec3f a, const struct cg_vec3f b);
struct cg_vec3f cg_vec3f_normalize(const struct cg_vec3f a);

inline struct cg_vec3f cg_vec3f_from_array(const float *array) {
	return (struct cg_vec3f) {
		.x = array[0],
		.y = array[1],
		.z = array[2],
	};
}

void cg_mat4f_print(const struct cg_mat4f *m);
struct cg_mat4f cg_mat4f_identity(void);
struct cg_mat4f cg_mat4f_scale(float x_factor, float y_factor, float z_factor);
struct cg_mat4f cg_mat4f_translate(float x, float y, float z);
struct cg_mat4f cg_mat4f_rotate_x(float angle);
struct cg_mat4f cg_mat4f_rotate_y(float angle);
struct cg_mat4f cg_mat4f_rotate_z(float angle);
struct cg_mat4f cg_mat4f_multiply(const struct cg_mat4f a, const struct cg_mat4f b);

struct cg_vec3f cg_vec3f_mat4f_multiply(const struct cg_vec3f vec, const struct cg_mat4f mat);

#endif // __CG_MATH_H__
