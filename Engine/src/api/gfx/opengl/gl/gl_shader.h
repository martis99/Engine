#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

GLuint gl_shader_create(GLenum type, const GLchar* source, GLint* status);
void gl_shader_info_length(GLuint shader, GLint* length);
void gl_shader_info(GLuint shader, GLsizei length, GLchar* info);
void gl_shader_delete(GLuint shader);

void gl_shader_bind_uniform_block(GLuint shader, const GLchar* name, GLuint index);
#endif