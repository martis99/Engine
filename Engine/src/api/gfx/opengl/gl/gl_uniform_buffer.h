#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_ub_create();
void gl_ub_create_static(GLuint ub, const void* uniforms, GLsizeiptr uniforms_size);
void gl_ub_create_dynamic(GLuint ub, GLsizeiptr uniforms_size);
void gl_ub_bind_base(GLuint ub, GLuint index);
void gl_ub_set_data(GLuint ub, const void* uniforms, GLsizeiptr uniforms_size);
void gl_ub_delete(GLuint ub);
#endif