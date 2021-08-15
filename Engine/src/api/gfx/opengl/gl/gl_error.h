#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

void gl_error_create();
void gl_error_begin();
bool gl_error_failed(const char* msg, const char* fn, const char* file, int line);
bool gl_error_assert(const char* fn, const char* file, int line);
void gl_error_delete();

#define GL_FAILED(msg, fn) gl_error_begin(), fn, gl_error_failed(msg, #fn, __FILE__, __LINE__) == 0
#define GL_ASSERT(fn) gl_error_begin(); fn; gl_error_assert(#fn, __FILE__, __LINE__)

#endif