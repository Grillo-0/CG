/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __CG_INPUT_H__
#define __CG_INPUT_H__

#include <stdbool.h>

enum cg_keycode {
	CG_KEY_UNKNOWN,

	CG_KEY_A,
	CG_KEY_B,
	CG_KEY_C,
	CG_KEY_D,
	CG_KEY_E,
	CG_KEY_F,
	CG_KEY_G,
	CG_KEY_H,
	CG_KEY_I,
	CG_KEY_J,
	CG_KEY_K,
	CG_KEY_L,
	CG_KEY_M,
	CG_KEY_N,
	CG_KEY_O,
	CG_KEY_P,
	CG_KEY_Q,
	CG_KEY_R,
	CG_KEY_S,
	CG_KEY_T,
	CG_KEY_U,
	CG_KEY_V,
	CG_KEY_W,
	CG_KEY_X,
	CG_KEY_Y,
	CG_KEY_Z,

	CG_KEY_UP,
	CG_KEY_DOWN,
	CG_KEY_LEFT,
	CG_KEY_RIGHT,

	CG_KEY_LEN,
};

bool cg_keycode_is_down(enum cg_keycode code);

#endif // __CG_INPUT_H__
