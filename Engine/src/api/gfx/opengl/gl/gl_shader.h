#pragma once
#ifdef E_OPENGL
#include "gl.h"

typedef enum {
	S_VERTEX,
	S_FRAGMENT
} ShaderType;

GLuint gl_shader_create(ShaderType type, const GLchar* source, GLint* status);
void gl_shader_info_length(GLuint shader, GLint* length);
void gl_shader_info(GLuint shader, GLsizei length, GLchar* info);
void gl_shader_delete(GLuint shader);
#endif