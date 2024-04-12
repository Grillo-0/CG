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
#include "cg_input.h"
#include "cg_math.h"
#include "cg_util.h"

#include "external/bed.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

int main(void) {
	cg_window_create("Camera FPS example", 600 , 400);

	size_t suzzanne_obj_len;
	char *suzzanne_obj = (char*)bed_get("../examples/resources/suzzanne.obj",
					    &suzzanne_obj_len);
	cg_assert(suzzanne_obj != NULL);

	struct cg_mesh mesh = cg_mesh_from_obj_data((char*)suzzanne_obj, suzzanne_obj_len);

	struct cg_model model = cg_model_create(mesh);

	size_t suzzanne_tex_len;
	unsigned char *suzzanne_tex = bed_get("../examples/resources/suzzanne_tex.png",
					      &suzzanne_tex_len);
	cg_assert(suzzanne_tex != NULL);

	int width, height, channels;
	unsigned char *data = stbi_load_from_memory(suzzanne_tex, suzzanne_tex_len,
						    &width, &height, &channels, 3);
	cg_assert(data != NULL);
	cg_assert(channels == 4);

	struct cg_texture tex = cg_texture_create_2d(data, width, height, GL_RGBA, GL_RGB);
	cg_model_put_texture(&model, tex);

	stbi_image_free(data);

	struct cg_vec3f camera_pos = {0, 0, -1};
	float yaw = 3.14;
	float pitch = 0;

	cg_camera_set_perspective(1.5, 4.0/6, 0.1, 10);

	while (!cg_window_should_close()) {
		if (cg_keycode_is_down(CG_KEY_W)) {
			camera_pos.x -= 0.1 * sin(yaw);
			camera_pos.y += 0.1 * sin(pitch);
			camera_pos.z -= 0.1 * cos(yaw);
		}

		if (cg_keycode_is_down(CG_KEY_S)) {
			camera_pos.x += 0.1 * sin(yaw);
			camera_pos.y -= 0.1 * sin(pitch);
			camera_pos.z += 0.1 * cos(yaw);
		}

		if (cg_keycode_is_down(CG_KEY_Q))
			pitch -= 0.04;

		if (cg_keycode_is_down(CG_KEY_E))
			pitch += 0.04;

		if (cg_keycode_is_down(CG_KEY_A))
			yaw += 0.04;

		if (cg_keycode_is_down(CG_KEY_D))
			yaw -= 0.04;

		cg_camera_FPS(camera_pos, pitch, yaw);

		cg_start_render();

		glClearColor(0.1, 0.1, 0.1, 1.0);

		for (int z = 0; z < 10; z++) {
			for (int y = 0; y < 10; y++) {
				for (int x = 0; x < 10; x++) {
					struct cg_mat4f m = cg_mat4f_translate((x - 5) * 5, (y - 5) * 5, (z - 5) * 3);
					cg_model_put_model_matrix(&model, &m);
					cg_model_draw(&model);
				}
			}
		}

		cg_end_render();
	}
}
