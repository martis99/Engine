#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_buffer.h"

static void va_bind(GLuint va) {
	glBindVertexArray(va);
}

static void va_unbind(GLuint va) {
	glBindVertexArray(0);
}

static void vb_bind(GLuint vb) {
	glBindBuffer(GL_ARRAY_BUFFER, vb);
}

static void vb_unbind(GLuint vb) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void ib_bind(GLuint ib) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

static void ib_unbind(GLuint ib) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void gl_ub_bind(GLuint ub) {
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
}

static void gl_ub_unbind(GLuint ub) {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint gl_va_create() {
	GLuint va = 0;
	glGenVertexArrays(1, &va);
	va_bind(va);
	return va;
}

void gl_va_draw_arrays(GLuint va, GLenum mode, GLsizei count) {
	va_bind(va);
	glDrawArrays(mode, 0, count);
}

void gl_va_draw_arrays_instanced(GLuint va, GLenum mode, GLsizei count, GLsizei instancecount) {
	va_bind(va);
	glDrawArraysInstanced(mode, 0, count, instancecount);
}

void gl_va_draw_elements(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLenum type) {
	va_bind(va);
	ib_bind(ib);
	glDrawElements(mode, count, type, NULL);
}

void gl_va_draw_elements_instanced(GLuint va, GLuint ib, GLenum mode, GLsizei count, GLenum type, GLsizei instancecount) {
	va_bind(va);
	ib_bind(ib);
	glDrawElementsInstanced(mode, count, type, NULL, instancecount);
}

void gl_va_delete(GLuint va) {
	glDeleteVertexArrays(1, &va);
}

GLuint gl_vb_create_static(const void* data, GLsizeiptr data_size) {
	GLuint vb = 0;
	glGenBuffers(1, &vb);
	vb_bind(vb);
	glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
	return vb;
}

GLuint gl_vb_create_dynamic(GLsizeiptr data_size) {
	GLuint vb = 0;
	glGenBuffers(1, &vb);
	vb_bind(vb);
	glBufferData(GL_ARRAY_BUFFER, data_size, NULL, GL_DYNAMIC_DRAW);
	return vb;
}

void gl_vb_set_data(GLuint vb, const void* data, GLsizeiptr data_size) {
	vb_bind(vb);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data);
}

void gl_vb_delete(GLuint vb) {
	glDeleteBuffers(1, &vb);
}

GLuint gl_ib_create_static(const void* data, GLsizeiptr data_size) {
	GLuint ib = 0;
	glGenBuffers(1, &ib);
	ib_bind(ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
	return ib;
}

GLuint gl_ib_create_dynamic(GLsizeiptr data_size) {
	GLuint ib = 0;
	glGenBuffers(1, &ib);
	ib_bind(ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, NULL, GL_DYNAMIC_DRAW);
	return ib;
}

void gl_ib_set_data(GLuint ib, const void* data, GLsizeiptr data_size) {
	ib_bind(ib);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, data);
}

void gl_ib_delete(GLuint ib) {
	glDeleteBuffers(1, &ib);
}

GLuint gl_ub_create_static(const void* data, GLsizeiptr data_size) {
	GLuint ub = 0;
	glGenBuffers(1, &ub);
	gl_ub_bind(ub);
	glBufferData(GL_UNIFORM_BUFFER, data_size, data, GL_STATIC_DRAW);
	gl_ub_unbind(ub);
	return ub;
}

GLuint gl_ub_create_dynamic(GLsizeiptr data_size) {
	GLuint ub = 0;
	glGenBuffers(1, &ub);
	gl_ub_bind(ub);
	glBufferData(GL_UNIFORM_BUFFER, data_size, NULL, GL_DYNAMIC_DRAW);
	gl_ub_unbind(ub);
	return ub;
}

void gl_ub_bind_base(GLuint ub, GLuint index) {
	glBindBufferBase(GL_UNIFORM_BUFFER, index, ub);
}

void gl_ub_set_data(GLuint ub, const void* data, GLsizeiptr data_size) {
	gl_ub_bind(ub);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, data_size, data);
	gl_ub_unbind(ub);
}

void gl_ub_delete(GLuint ub) {
	glDeleteBuffers(1, &ub);
}

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