#pragma once
#ifdef E_OPENGL
#include "gl.h"
#include "api/gfx/aprimitive.h"

GLuint gl_va_create();
void gl_va_layout_create(GLuint vertex_array, GLuint layout_size, GLuint* layout);
void gl_va_bind(GLuint vertex_array);
void gl_va_draw_arrays(GLuint vertex_array, APrimitive primitive, GLsizei count);
void gl_va_draw_elements(GLuint vertex_array, GLuint index_buffer, APrimitive primitive, GLsizei count);
void gl_va_delete(GLuint vertex_array);

GLuint gl_vb_create_static(GLuint vertex_array, GLsizeiptr size, const void* data);
GLuint gl_vb_create_dynamic(GLuint vertex_array, GLsizeiptr size);
void gl_vb_bind(GLuint vertex_array, GLuint vertex_buffer);
void gl_vb_set_data(GLuint vertex_array, GLuint vertex_buffer, GLsizeiptr size, const void* data);
void gl_vb_delete(GLuint vertex_buffer);

GLuint gl_ib_create_static(GLuint vertex_array, GLsizeiptr size, const void* data);
GLuint gl_ib_create_dynamic(GLuint vertex_array, GLsizeiptr size);
void gl_ib_bind(GLuint vertex_array, GLuint index_buffer);
void gl_ib_set_data(GLuint vertex_array, GLuint index_buffer, GLsizeiptr size, const void* data);
void gl_ib_delete(GLuint index_buffer);
#endif