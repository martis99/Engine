#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_ub_create_static(GLsizeiptr size, const void* data);
GLuint gl_ub_create_dynamic(GLsizeiptr size);
void gl_ub_bind(GLuint uniform_buffer);
void gl_ub_unbind(GLuint uniform_buffer);
void gl_ub_bind_base(GLuint uniform_buffer, GLuint index);
void gl_ub_set_data(GLuint uniform_buffer, GLsizeiptr size, const void* data);
void gl_ub_delete(GLuint uniform_buffer);
#endif