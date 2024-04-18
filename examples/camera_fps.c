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

int main(void) {
	cg_window_create("Camera FPS example", 600 , 400);

	cg_set_file_read_callback(bed_get);

	cg_disable_cursor();

	struct cg_model model = cg_model_from_obj_file("../examples/resources/suzzanne.obj");

	struct cg_texture tex = cg_texture_from_file_2d("../examples/resources/suzzanne_tex.png");
	model.materials[0].tex_diffuse = tex;

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
