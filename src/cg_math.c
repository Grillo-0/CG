/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "cg_math.h"
#include "cg_util.h"

struct cg_vec3f cg_vec3f_add(const struct cg_vec3f a, const struct cg_vec3f b) {
	return (struct cg_vec3f) {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
	};
}

struct cg_vec3f cg_vec3f_sub(const struct cg_vec3f a, const struct cg_vec3f b) {
	return (struct cg_vec3f) {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z,
	};
}

struct cg_vec3f cg_vec3f_cross(const struct cg_vec3f a, const struct cg_vec3f b) {
	return (struct cg_vec3f) {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x,
	};
}

struct cg_vec3f cg_vec3f_normal(const struct cg_vec3f a) {
	float magnitude = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);

	return (struct cg_vec3f){
		.x = a.x / magnitude,
		.y = a.y / magnitude,
		.z = a.z / magnitude,
	};
}

void cg_mat4f_print(const struct cg_mat4f *m) {
	cg_debug("[\n");
	for (size_t row = 0; row < 4; row++) {
		cg_debug("\t");
		for (size_t col = 0; col < 4; col++) {
			fprintf(stderr, "%f, ", m->d[m(col, row)]);
		}
		fprintf(stderr, "\n");
	}
	cg_debug("]\n");
}

struct cg_mat4f cg_mat4f_identity(void) {
	static const struct cg_mat4f ret = {
		.d[m(0, 0)] = 1.0f,
		.d[m(1, 1)] = 1.0f,
		.d[m(2, 2)] = 1.0f,
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_scale(float x_factor, float y_factor, float z_factor) {
	struct cg_mat4f ret = {
		.d[m(0, 0)] = x_factor,
		.d[m(1, 1)] = y_factor,
		.d[m(2, 2)] = z_factor,
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_translate(float x, float y, float z) {
	struct cg_mat4f ret = {
		.d[m(0, 0)] = 1.0f,
		.d[m(3, 0)] = x,
		.d[m(1, 1)] = 1.0f,
		.d[m(3, 1)] = y,
		.d[m(2, 2)] = 1.0f,
		.d[m(3, 2)] = z,
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_rotate_x(float angle) {
	struct cg_mat4f ret = {
		.d[m(0, 0)] = 1.0f,
		.d[m(1, 1)] = cos(angle),
		.d[m(2, 1)] = -sin(angle),
		.d[m(1, 2)] = sin(angle),
		.d[m(2, 2)] = cos(angle),
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_rotate_y(float angle) {
	struct cg_mat4f ret = {
		.d[m(0, 0)] = cos(angle),
		.d[m(2, 0)] = sin(angle),
		.d[m(1, 1)] = 1.0f,
		.d[m(0, 2)] = -sin(angle),
		.d[m(2, 2)] = cos(angle),
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_rotate_z(float angle) {
	struct cg_mat4f ret = {
		.d[m(0, 0)] = cos(angle),
		.d[m(1, 0)] = -sin(angle),
		.d[m(0, 1)] = sin(angle),
		.d[m(1, 1)] = cos(angle),
		.d[m(2, 2)] = 1.0f,
		.d[m(3, 3)] = 1.0f
	};

	return ret;
}

struct cg_mat4f cg_mat4f_multiply(const struct cg_mat4f a, const struct cg_mat4f b) {
	struct cg_mat4f ret = { 0 };

	for (size_t row = 0; row < 4; row++) {
		for (size_t col = 0; col < 4; col++) {
			for (size_t i  = 0; i < 4; i++) {
				ret.d[m(col, row)] += a.d[m(col, i)] * b.d[m(i, row)];
			}
		}
	}

	return ret;
}

