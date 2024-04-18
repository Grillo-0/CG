/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "cg_core.h"
#include "cg_gfx.h"
#include "cg_math.h"
#include "cg_util.h"

#include "external/bed.h"

float angle_x, angle_y;

int main(void) {
	cg_window_create("OBJ Loading Example", 400 , 400);

	cg_set_file_read_callback(bed_get);

	struct cg_model model = cg_model_from_obj_file("../examples/resources/suzzanne.obj");

	while (!cg_window_should_close()) {
		cg_start_render();

		glClearColor(0.1, 0.1, 0.1, 1.0);

		cg_model_draw(&model);

		cg_end_render();
	}
}
