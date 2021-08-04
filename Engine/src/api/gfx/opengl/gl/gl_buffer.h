#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

GLuint gl_va_create();
void gl_va_draw_arrays(GLuint va, GLenum mode, GLsizei count);
void gl_va_draw_arrays_instanced(GLuint va, GLenum mode, GLsizei count, GLsizei instancecount);
void gl_va_draw_elements(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLenum type);
void gl_va_draw_elements_instanced(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLenum type, GLsizei instancecount);
void gl_va_delete(GLuint va);

GLuint gl_vb_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_vb_create_dynamic(GLsizeiptr data_size);
void gl_vb_set_data(GLuint vb, const void* data, GLsizeiptr data_size);
void gl_vb_delete(GLuint vb);

GLuint gl_ib_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_ib_create_dynamic(GLsizeiptr data_size);
void gl_ib_set_data(GLuint ib, const void* data, GLsizeiptr data_size);
void gl_ib_delete(GLuint ib);

GLuint gl_ub_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_ub_create_dynamic(GLsizeiptr data_size);
void gl_ub_bind_base(GLuint ub, GLuint index);
void gl_ub_set_data(GLuint ub, const void* data, GLsizeiptr data_size);
void gl_ub_delete(GLuint ub);

#endif