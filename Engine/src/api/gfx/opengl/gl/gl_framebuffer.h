#pragma once
#ifdef GAPI_OPENGL
#include "gl.h"

GLuint gl_fb_create();
void gl_fb_bind(GLuint framebuffer);
bool gl_fb_check_status(GLuint framebuffer);
void gl_fb_delete(GLuint framebuffer);

void gl_fb_attachment_bind(GLuint attachment);

GLuint gl_fb_color_attachment(GLuint framebuffer, GLint internalFormat, GLenum format, GLsizei width, GLsizei height, GLuint index);
void gl_fb_color_attachment_clear_i(GLint drawbuffer, const GLint* value);
void gl_fb_color_attachment_clear_f(GLint drawbuffer, const GLfloat* value);
int gl_fb_color_attachment_read_pixel(GLuint index, GLint x, GLint y);

GLuint gl_fb_depth_stencil_attachment(GLuint framebuffer, GLint internalFormat, GLenum format, GLsizei width, GLsizei height);

GLuint gl_rb_create();
void gl_rb_bind(GLuint renderbuffer);
void gl_rb_storage(GLsizei width, GLsizei height);
void gl_rb_delete(GLuint renderbuffer);
#endif