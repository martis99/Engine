#pragma once
#include "gl.h"
#include "gl_ctx_types.h"

GLError *gl_error_create(GLError *error);
void gl_error_begin(GLError *error);
bool gl_error_failed(GLError *error, const char *msg, const char *fn, const char *func, int line);
bool gl_error_assert(GLError *error, const char *fn, const char *func, int line);
void gl_error_delete(GLError *error);

#define GL_FAILED(error, msg, fn) gl_error_begin(error), fn, gl_error_failed(error, msg, #fn, __func__, __LINE__) == 0

#define GL_ASSERT(error, fn)   \
	gl_error_begin(error); \
	fn;                    \
	gl_error_assert(error, #fn, __func__, __LINE__)
