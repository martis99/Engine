#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_framebuffer.h"

GLuint gl_fb_create() {
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	gl_fb_bind(framebuffer);
	return framebuffer;
}

void gl_fb_bind(GLuint framebuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

bool gl_fb_check_status(GLuint framebuffer) {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void gl_fb_delete(GLuint framebuffer) {
	glDeleteFramebuffers(1, &framebuffer);
}

void gl_fb_attachment_bind(GLuint attachment) {
	glBindTexture(GL_TEXTURE_2D, attachment);
}

GLuint gl_fb_color_attachment(GLuint framebuffer, GLint internalFormat, GLenum format, GLsizei width, GLsizei height, GLuint index) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture, 0);
	return texture;
}

void gl_fb_color_attachment_clear_i(GLint drawbuffer, const GLint* value) {
	glClearBufferiv(GL_COLOR, drawbuffer, value);
}

void gl_fb_color_attachment_clear_f(GLint drawbuffer, const GLfloat* value) {
	glClearBufferfv(GL_COLOR, drawbuffer, value);
}

int gl_fb_color_attachment_read_pixel(GLuint index, GLint x, GLint y) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
	int data;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
	return data;
}

GLuint gl_fb_depth_stencil_attachment(GLuint framebuffer, GLint internalFormat, GLenum format, GLsizei width, GLsizei height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	return texture;
}

GLuint gl_rb_create() {
	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	gl_rb_bind(renderbuffer);
	return renderbuffer;
}

void gl_rb_bind(GLuint renderbuffer) {
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

void gl_rb_storage(GLsizei width, GLsizei height) {
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void gl_rb_delete(GLuint renderbuffer) {
	glDeleteRenderbuffers(1, &renderbuffer);
}
#endif