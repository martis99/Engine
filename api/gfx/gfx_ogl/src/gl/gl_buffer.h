#pragma once
#include "gl_types.h"

GLuint gl_va_create(GLError *error);
void gl_va_bind(GLError *error, GLuint va);
void gl_va_delete(GLError *error, GLuint va);

GLuint gl_vb_create_static(GLError *error, const void *data, GLsizeiptr data_size);
GLuint gl_vb_create_dynamic(GLError *error, GLsizeiptr data_size);
void gl_vb_bind(GLError *error, GLuint vb);
void gl_vb_set_data(GLError *error, GLuint vb, const void *data, GLsizeiptr data_size);
void gl_vb_delete(GLError *error, GLuint vb);

GLuint gl_ib_create_static(GLError *error, const void *data, GLsizeiptr data_size);
GLuint gl_ib_create_dynamic(GLError *error, GLsizeiptr data_size);
void gl_ib_bind(GLError *error, GLuint ib);
void gl_ib_set_data(GLError *error, GLuint ib, const void *data, GLsizeiptr data_size);
void gl_ib_delete(GLError *error, GLuint ib);

GLuint gl_ub_create_static(GLError *error, const void *data, GLsizeiptr data_size);
GLuint gl_ub_create_dynamic(GLError *error, GLsizeiptr data_size);
void gl_ub_bind(GLError *error, GLuint ub);
void gl_ub_bind_base(GLError *error, GLuint ub, GLuint index);
void gl_ub_set_data(GLError *error, GLuint ub, const void *data, GLsizeiptr data_size);
void gl_ub_delete(GLError *error, GLuint ub);

GLuint gl_fb_create(GLError *error);
void gl_fb_bind(GLError *error, GLuint framebuffer);
bool gl_fb_attach_texture(GLError *error, GLenum target, GLuint texture);
bool gl_fb_check_status(GLError *error, GLuint framebuffer);
void gl_fb_delete(GLError *error, GLuint framebuffer);

GLuint gl_rb_create(GLError *error);
void gl_rb_bind(GLError *error, GLuint renderbuffer);
void gl_rb_storage(GLError *error, GLsizei width, GLsizei height);
void gl_rb_delete(GLError *error, GLuint renderbuffer);

void gl_draw_buffers(GLError *error, GLsizei n, const GLenum *bufs);
void gl_read_pixels(GLError *error, GLenum mode, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *data);

void gl_clear(GLError *error, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLdouble depth);
void gl_dsb_clear(GLError *error, GLfloat depth, GLint stencil);
void gl_cb_clear(GLError *error, GLenum type, GLint index, const void *value);

bool gl_vertex_attrib(GLError *error, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer, GLuint divisor);

void gl_draw_arrays(GLError *error, GLenum mode, GLsizei count);
void gl_draw_arrays_instanced(GLError *error, GLenum mode, GLsizei count, GLsizei instancecount);
void gl_draw_elements(GLError *error, GLenum mode, GLsizei count, GLenum type);
void gl_draw_elements_instanced(GLError *error, GLenum mode, GLsizei count, GLenum type, GLsizei instancecount);
