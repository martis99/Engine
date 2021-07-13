#include "pch.h"
#ifdef E_OPENGL
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

GLuint gl_fb_attachment_color(GLuint framebuffer, GLsizei width, GLsizei height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	return texture;
}

GLuint gl_fb_attachment_depth_stencil(GLuint framebuffer, GLsizei width, GLsizei height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	return texture;
}

void gl_fb_attachment_bind_color(GLuint attachment_color) {
	glBindTexture(GL_TEXTURE_2D, attachment_color);
}

bool gl_fb_check_status(GLuint framebuffer) {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void gl_fb_delete(GLuint framebuffer) {
	glDeleteFramebuffers(1, &framebuffer);
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