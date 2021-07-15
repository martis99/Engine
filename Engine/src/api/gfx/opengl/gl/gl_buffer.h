#pragma once
#ifdef E_OPENGL
#include "gl.h"
#include "api/gfx/aenums.h"

GLuint gl_va_create();
void gl_va_draw_arrays(GLuint va, GLenum mode, GLsizei count);
void gl_va_draw_arrays_instanced(GLuint va, GLenum mode, GLsizei count, GLsizei instancecount);
void gl_va_draw_elements(GLuint va, GLuint ib, GLenum mode, GLsizei count);
void gl_va_draw_elements_instanced(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLsizei instancecount);
void gl_va_delete(GLuint va);

GLuint gl_vb_create();
void gl_vb_init_static(GLuint vb, GLuint va, const void* vertices, GLsizeiptr vertices_size, ADataType* layout, GLuint layout_size, GLuint* index);
void gl_vb_init_dynamic(GLuint vb, GLuint va, GLsizeiptr vertices_size, ADataType* layout, GLuint layout_size, GLuint* index);
void gl_vb_set_data(GLuint vb, const void* vertices, GLsizeiptr vertices_size);
void gl_vb_delete(GLuint vb);

GLuint gl_ib_create();
void gl_ib_init_static(GLuint ib, const void* indices, GLsizeiptr indices_size);
void gl_ib_init_dynamic(GLuint ib, GLsizeiptr indices_size);
void gl_ib_set_data(GLuint ib, const void* indices, GLsizeiptr indices_size);
void gl_ib_delete(GLuint ib);

#endif