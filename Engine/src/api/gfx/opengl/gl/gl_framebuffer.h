#pragma once
#ifdef E_OPENGL
#include "gl.h"

GLuint gl_fb_create();
void gl_fb_bind(GLuint framebuffer);
GLuint gl_fb_attachment_color(GLuint framebuffer, GLsizei width, GLsizei height);
GLuint gl_fb_attachment_depth_stencil(GLuint framebuffer, GLsizei width, GLsizei height);
void gl_fb_attachment_bind_color(GLuint attachment_color);
bool gl_fb_check_status(GLuint framebuffer);
void gl_fb_delete(GLuint framebuffer);

GLuint gl_rb_create();
void gl_rb_bind(GLuint renderbuffer);
void gl_rb_storage(GLsizei width, GLsizei height);
void gl_rb_delete(GLuint renderbuffer);
#endif