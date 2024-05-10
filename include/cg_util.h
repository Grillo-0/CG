/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __CG_UTIL_H__
#define __CG_UTIL_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define cg_error(...) fprintf(stderr, "[ERROR]: "  __VA_ARGS__)
#define cg_warn(...) fprintf(stderr, "[WARNING]: " __VA_ARGS__)
#define cg_info(...) fprintf(stdout, "[INFO]: " __VA_ARGS__)
#define cg_debug( ...) fprintf(stderr, "[DEBUG]: " __VA_ARGS__)

#define cg_assert(val) __cg_assert((val), #val, __FILE__, __LINE__)

bool cg_check_gl(void);
bool cg_check_sdl(void);

static inline void __cg_assert(int val, const char* expr_str, const char *file_name, int line) {
	if (!val) {
		cg_error("%s:%d: cg_assert(%s) failed\n", file_name, line, expr_str);
		cg_check_sdl();
		cg_check_gl();
		exit(EXIT_FAILURE);
	}
}


#define CG_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CG_MIN(a, b) (((a) < (b)) ? (a) : (b))

#define CG_ARRAY_LEN(a) ( sizeof( (a) ) / sizeof( (a)[0] ) )

#define CG_DA(item_type) { \
	item_type *items; \
	size_t len; \
	size_t capacity; \
}

#define CG_DA_EXPAND_FACTOR 2

#define cg_da_append(da, item) do { \
	if ((da)->len == (da)->capacity) { \
		if ((da)->capacity == 0) \
			(da)->capacity = 1; \
		(da)->capacity *= CG_DA_EXPAND_FACTOR; \
		(da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->capacity); \
	} \
	(da)->items[(da)->len] = (item); \
	(da)->len++; \
} while (0)

#endif // __CG_UTIL_H__
