/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include "cg_core.h"
#include "cg_input.h"
#include "cg_math.h"

struct cg_contex cg_ctx = {0};

void cg_window_create(const char *window_name, size_t width, size_t height) {
	cg_assert(!SDL_InitSubSystem(SDL_INIT_VIDEO));

	cg_assert(!SDL_VideoInit(NULL));

	cg_info("Creating window...\n");
	SDL_Window *window = SDL_CreateWindow(window_name,
					      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					      width,  height,
					      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	cg_assert(window != NULL);
	cg_ctx.window = (struct cg_window) {
		.base = window,
		.width = width,
		.height = height,
	};

	cg_info("Getting GL context...\n");
	SDL_GLContext gl_ctx = SDL_GL_CreateContext(window);
	cg_assert(gl_ctx != NULL);

	GLenum res = glewInit();
	// Wayland issues
	cg_assert(res == GLEW_OK || res == GLEW_ERROR_NO_GLX_DISPLAY);

	cg_info("Plataform: %s - %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	cg_info("GL version: %s\n", glGetString(GL_VERSION));
	cg_info("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND );

	cg_ctx.view_matrix = cg_mat4f_identity();
	cg_ctx.projection_matrix = cg_mat4f_identity();
	cg_reset_file_read_callback();

	cg_ctx.fill = true;
}

static enum cg_keycode sdl2_to_cg_kc(SDL_Keycode kc) {
	switch (kc) {
		case SDLK_a:
			return CG_KEY_A;
		case SDLK_b:
			return CG_KEY_B;
		case SDLK_c:
			return CG_KEY_C;
		case SDLK_d:
			return CG_KEY_D;
		case SDLK_e:
			return CG_KEY_E;
		case SDLK_f:
			return CG_KEY_F;
		case SDLK_g:
			return CG_KEY_G;
		case SDLK_h:
			return CG_KEY_H;
		case SDLK_i:
			return CG_KEY_I;
		case SDLK_j:
			return CG_KEY_J;
		case SDLK_k:
			return CG_KEY_K;
		case SDLK_l:
			return CG_KEY_L;
		case SDLK_m:
			return CG_KEY_M;
		case SDLK_n:
			return CG_KEY_N;
		case SDLK_o:
			return CG_KEY_O;
		case SDLK_p:
			return CG_KEY_P;
		case SDLK_q:
			return CG_KEY_Q;
		case SDLK_r:
			return CG_KEY_R;
		case SDLK_s:
			return CG_KEY_S;
		case SDLK_t:
			return CG_KEY_T;
		case SDLK_u:
			return CG_KEY_U;
		case SDLK_v:
			return CG_KEY_V;
		case SDLK_w:
			return CG_KEY_W;
		case SDLK_x:
			return CG_KEY_X;
		case SDLK_y:
			return CG_KEY_Y;
		case SDLK_z:
			return CG_KEY_Z;
		case SDLK_UP:
			return CG_KEY_UP;
	}

	return CG_KEY_UNKNOWN;
}

static void check_events(void) {
	SDL_Event e = {0};
	while(SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				cg_ctx.window_should_close = true;
				break;
			case SDL_WINDOWEVENT:;
			SDL_WindowEvent *w = &e.window;
			switch (w->event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					glViewport(0, 0, w->data1, w->data2);
					cg_ctx.window.width = w->data1;
					cg_ctx.window.height = w->data2;
					break;
			}
			break;

			case SDL_KEYDOWN:
				cg_ctx.keys[sdl2_to_cg_kc(e.key.keysym.sym)] = true;
			break;

			case SDL_KEYUP:
				cg_ctx.keys[sdl2_to_cg_kc(e.key.keysym.sym)] = false;
			break;

			case SDL_MOUSEMOTION:;
				SDL_MouseMotionEvent *mouse = &e.motion;
				cg_ctx.mouse_pos = (struct cg_vec2f){mouse->x, mouse->y};
				cg_ctx.mouse_rel_pos = (struct cg_vec2f){mouse->xrel, mouse->yrel};
			break;
		}
	}
}

bool cg_window_should_close(void) {
	check_events();
	return cg_ctx.window_should_close;
}

void cg_enable_cursor(void) {
	cg_assert(!SDL_SetRelativeMouseMode(false));
}

void cg_disable_cursor(void) {
	cg_assert(!SDL_SetRelativeMouseMode(true));
}

void cg_set_file_read_callback(cg_file_reader_callback_t func) {
	cg_ctx.file_read = func;
}

static unsigned char* default_file_read_callback(const char *file_path, size_t *file_size) {
	FILE *fp = fopen(file_path, "r");
	cg_assert(fp != NULL);

	int ret = fseek(fp, 0, SEEK_END);
	cg_assert(ret == 0);
	int size = ftell(fp);
	cg_assert(size != -1);
	ret = fseek(fp, 0, SEEK_SET);
	cg_assert(ret == 0);

	unsigned char *buf = malloc(sizeof(*buf) * size);
	cg_assert(buf != NULL);

	fread(buf, sizeof(*buf) * size, 1, fp);
	cg_assert(ferror(fp) == 0);

	ret = fclose(fp);
	cg_assert(ret == 0);

	*file_size = size;
	return buf;
}

void cg_reset_file_read_callback() {
		cg_ctx.file_read = default_file_read_callback;
}
