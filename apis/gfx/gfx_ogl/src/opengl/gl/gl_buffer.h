#pragma once
#include "gl_types.h"

GLuint gl_va_create();
void gl_va_bind(GLuint va);
void gl_va_delete(GLuint va);

GLuint gl_vb_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_vb_create_dynamic(GLsizeiptr data_size);
void gl_vb_bind(GLuint vb);
void gl_vb_set_data(GLuint vb, const void* data, GLsizeiptr data_size);
void gl_vb_delete(GLuint vb);

GLuint gl_ib_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_ib_create_dynamic(GLsizeiptr data_size);
void gl_ib_bind(GLuint ib);
void gl_ib_set_data(GLuint ib, const void* data, GLsizeiptr data_size);
void gl_ib_delete(GLuint ib);

GLuint gl_ub_create_static(const void* data, GLsizeiptr data_size);
GLuint gl_ub_create_dynamic(GLsizeiptr data_size);
void gl_ub_bind(GLuint ub);
void gl_ub_bind_base(GLuint ub, GLuint index);
void gl_ub_set_data(GLuint ub, const void* data, GLsizeiptr data_size);
void gl_ub_delete(GLuint ub);

GLuint gl_fb_create();
void gl_fb_bind(GLuint framebuffer);
bool gl_fb_attach_texture(GLenum target, GLuint texture);
bool gl_fb_check_status(GLuint framebuffer);
void gl_fb_delete(GLuint framebuffer);

GLuint gl_rb_create();
void gl_rb_bind(GLuint renderbuffer);
void gl_rb_storage(GLsizei width, GLsizei height);
void gl_rb_delete(GLuint renderbuffer);

void gl_draw_buffers(GLsizei n, const GLenum* bufs);
void gl_read_pixels(GLenum mode, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data);

void gl_clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLdouble depth);
void gl_dsb_clear(GLfloat depth, GLint stencil);
void gl_cb_clear(GLenum type, GLint index, const void* value);

bool gl_vertex_attrib(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer, GLuint divisor);

void gl_draw_arrays(GLenum mode, GLsizei count);
void gl_draw_arrays_instanced(GLenum mode, GLsizei count, GLsizei instancecount);
void gl_draw_elements(GLenum mode, GLsizei count, GLenum type);
void gl_draw_elements_instanced(GLenum mode, GLsizei count, GLenum type, GLsizei instancecount);