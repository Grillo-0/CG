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

	cg_set_file_read_callback(bed_get);

	cg_disable_cursor();

	struct cg_model model = cg_model_from_obj_file("../examples/resources/suzzanne.obj");

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

	struct cg_camera camera = cg_camera_create((struct cg_vec3f){0, 0, -1}, 1.5, 0.1, 100);

	while (!cg_window_should_close()) {
		cg_camera_update_FPS(&camera);

		cg_start_render();

		glClearColor(0.1, 0.1, 0.1, 1.0);

		for (int z = 0; z < 10; z++) {
			for (int y = 0; y < 10; y++) {
				for (int x = 0; x < 10; x++) {
					struct cg_mat4f m = cg_mat4f_translate((x - 5) * 5, (y - 5) * 5, (z - 5) * 3);
					cg_model_put_model_matrix(&model, m);
					cg_model_draw(&model);
				}
			}
		}

		cg_end_render();
	}
}
