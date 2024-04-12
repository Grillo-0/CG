/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "cg_util.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>

static const char* gl_error_strs[] = {
	[GL_NO_ERROR] = "No error has been recorded",
	[GL_INVALID_ENUM] = "An unacceptable value is specified for an enumerated argument",
	[GL_INVALID_VALUE] = "A numeric argument is out of range",
	[GL_INVALID_OPERATION] = "The specified operation is not allowed in the current state",
	[GL_INVALID_FRAMEBUFFER_OPERATION] = "The framebuffer object is not complete",
	[GL_OUT_OF_MEMORY] = "There is not enough memory left to execute the command",
	[GL_STACK_UNDERFLOW] = "An attempt has been made to perform an operation that would cause an internal stack to underflow",
	[GL_STACK_OVERFLOW] = "An attempt has been made to perform an operation that would cause an internal stack to overflow",
};

bool cg_check_sdl(void) {
	const char *erro_str = SDL_GetError();
	if (erro_str[0] != '\0') {
		cg_error("SDL2 error: %s", erro_str);
		return true;
	}

	return false;
}

bool cg_check_gl(void) {
	GLenum err;
	bool has_error = false;

	while (err = glGetError(), err != GL_NO_ERROR) {
		cg_error("gl_error: %s\n",  gl_error_strs[err]);
		has_error =  true;
	}

	return has_error;
}

char* cg_file_to_str(const char* path) {
	int ret = 0;

	FILE *fp = fopen(path, "r");
	cg_assert(fp != NULL);

	ret = fseek(fp, 0, SEEK_END);
	cg_assert(ret == 0);

	long len = ftell(fp);
	cg_assert(len >= 0);

	rewind(fp);

	char *str = calloc(len, sizeof(*str));
	cg_assert(str != NULL);

	fread(str, 1, len, fp);
	cg_assert(ferror(fp) == 0);

	ret = fclose(fp);
	cg_assert(ret == 0);

	return str;
}

