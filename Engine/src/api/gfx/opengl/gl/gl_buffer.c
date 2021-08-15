#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_buffer.h"
#include "gl_error.h"

static void buffer_bind(GLenum target, GLuint buffer) {
	GL_ASSERT(glBindBuffer(target, buffer));
}

static GLuint buffer_create_static(GLenum target, const void* data, GLsizeiptr data_size) {
	GLuint buffer = 0;
	if (GL_FAILED("Failed to generate buffer", glGenBuffers(1, &buffer))) {
		return 0;
	}
	buffer_bind(target, buffer);
	if (GL_FAILED("Failed to set buffer data", glBufferData(target, data_size, data, GL_STATIC_DRAW))) {
		return 0;
	}
	return buffer;
}

static GLuint buffer_create_dynamic(GLenum target, GLsizeiptr data_size) {
	GLuint buffer = 0;
	if (GL_FAILED("Failed to generate buffer", glGenBuffers(1, &buffer))) {
		return 0;
	}
	buffer_bind(target, buffer);
	if (GL_FAILED("Failed to set buffer data", glBufferData(target, data_size, NULL, GL_DYNAMIC_DRAW))) {
		return 0;
	}
	return buffer;
}

GLuint gl_va_create() {
	GLuint va = 0;
	if (GL_FAILED("Failed to generate vertex array", glGenVertexArrays(1, &va))) {
		return 0;
	}
	gl_va_bind(va);
	return va;
}

void gl_va_bind(GLuint va) {
	GL_ASSERT(glBindVertexArray(va));
}

void gl_va_delete(GLuint va) {
	GL_ASSERT(glDeleteVertexArrays(1, &va));
}

GLuint gl_vb_create_static(const void* data, GLsizeiptr data_size) {
	return buffer_create_static(GL_ARRAY_BUFFER, data, data_size);
}

GLuint gl_vb_create_dynamic(GLsizeiptr data_size) {
	return buffer_create_dynamic(GL_ARRAY_BUFFER, data_size);
}

void gl_vb_bind(GLuint vb) {
	GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vb));
}

void gl_vb_set_data(GLuint vb, const void* data, GLsizeiptr data_size) {
	gl_vb_bind(vb);
	GL_ASSERT(glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data));
}

void gl_vb_delete(GLuint vb) {
	GL_ASSERT(glDeleteBuffers(1, &vb));
}

GLuint gl_ib_create_static(const void* data, GLsizeiptr data_size) {
	return buffer_create_static(GL_ELEMENT_ARRAY_BUFFER, data, data_size);
}

GLuint gl_ib_create_dynamic(GLsizeiptr data_size) {
	return buffer_create_dynamic(GL_ELEMENT_ARRAY_BUFFER, data_size);
}

void gl_ib_bind(GLuint ib) {
	GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
}

void gl_ib_set_data(GLuint ib, const void* data, GLsizeiptr data_size) {
	gl_ib_bind(ib);
	GL_ASSERT(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, data));
}

void gl_ib_delete(GLuint ib) {
	GL_ASSERT(glDeleteBuffers(1, &ib));
}

GLuint gl_ub_create_static(const void* data, GLsizeiptr data_size) {
	return buffer_create_static(GL_UNIFORM_BUFFER, data, data_size);
}

GLuint gl_ub_create_dynamic(GLsizeiptr data_size) {
	return buffer_create_dynamic(GL_UNIFORM_BUFFER, data_size);
}

void gl_ub_bind(GLuint ub) {
	GL_ASSERT(glBindBuffer(GL_UNIFORM_BUFFER, ub));
}

void gl_ub_bind_base(GLuint ub, GLuint index) {
	GL_ASSERT(glBindBufferBase(GL_UNIFORM_BUFFER, index, ub));
}

void gl_ub_set_data(GLuint ub, const void* data, GLsizeiptr data_size) {
	gl_ub_bind(ub);
	GL_ASSERT(glBufferSubData(GL_UNIFORM_BUFFER, 0, data_size, data));
}

void gl_ub_delete(GLuint ub) {
	GL_ASSERT(glDeleteBuffers(1, &ub));
}

GLuint gl_fb_create() {
	GLuint framebuffer = 0;
	if (GL_FAILED("Failed to generate frame buffer", glGenFramebuffers(1, &framebuffer))) {
		return 0;
	}
	gl_fb_bind(framebuffer);
	return framebuffer;
}

void gl_fb_bind(GLuint framebuffer) {
	GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
}

bool gl_fb_attach_texture(GLenum target, GLuint texture) {
	if (GL_FAILED("Failed to attach texture fo framebuffer", glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, texture, 0))) {
		return A_FAIL;
	}
	return A_SUCCESS;
}

bool gl_fb_check_status(GLuint framebuffer) {
	GL_ASSERT(bool status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	return status;
}

void gl_fb_delete(GLuint framebuffer) {
	GL_ASSERT(glDeleteFramebuffers(1, &framebuffer));
}

GLuint gl_rb_create() {
	GLuint renderbuffer = 0;
	if (GL_FAILED("Failed to generate render buffer", glGenRenderbuffers(1, &renderbuffer))) {
		return 0;
	}
	gl_rb_bind(renderbuffer);
	return renderbuffer;
}

void gl_rb_bind(GLuint renderbuffer) {
	GL_ASSERT(glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer));
}

void gl_rb_storage(GLsizei width, GLsizei height) {
	GL_ASSERT(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
}

void gl_rb_delete(GLuint renderbuffer) {
	GL_ASSERT(glDeleteRenderbuffers(1, &renderbuffer));
}

void gl_draw_buffers(GLsizei n, const GLenum* bufs) {
	GL_ASSERT(glDrawBuffers(n, bufs));
}

void gl_read_pixels(GLenum mode, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data) {
	GL_ASSERT(glReadBuffer(mode));
	GL_ASSERT(glReadPixels(x, y, width, height, format, type, data));
}

void gl_clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLdouble depth) {
	GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_ASSERT(glClearColor(red, green, blue, alpha));
	GL_ASSERT(glClearDepth(depth));
}

void gl_dsb_clear(GLfloat depth, GLint stencil) {
	GL_ASSERT(glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil));
}

void gl_cb_clear(GLenum type, GLint index, const void* value) {
	switch (type) {
	case GL_UNSIGNED_INT: GL_ASSERT(glClearBufferuiv(GL_COLOR, index, value)); break;
	case GL_INT: GL_ASSERT(glClearBufferiv(GL_COLOR, index, value)); break;
	case GL_FLOAT: GL_ASSERT(glClearBufferfv(GL_COLOR, index, value)); break;
	}
}

bool gl_vertex_attrib(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer, GLuint divisor) {
	if (GL_FAILED("Failed to enable vertex attrib array", glEnableVertexAttribArray(index))) {
		return A_FAIL;
	}

	switch (type) {
	case GL_BYTE: case GL_UNSIGNED_BYTE: case GL_SHORT: case GL_UNSIGNED_SHORT: case GL_INT: case GL_UNSIGNED_INT:
		if (GL_FAILED("Failed to set vertex attrib pointer", glVertexAttribIPointer(index, size, type, stride, pointer))) {
			return A_FAIL;
		}
		break;
	case GL_FLOAT:
		if (GL_FAILED("Failed to set vertex attrib pointer", glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer))) {
			return A_FAIL;
		}
		break;
	case GL_DOUBLE:
		if (GL_FAILED("Failed to set vertex attrib pointer", glVertexAttribLPointer(index, size, type, stride, pointer))) {
			return A_FAIL;
		}
		break;
	}

	if (GL_FAILED("Failed to set vertex attrib divisor", glVertexAttribDivisor(index, divisor))) {
		return A_FAIL;
	}

	return A_SUCCESS;
}

void gl_draw_arrays(GLenum mode, GLsizei count) {
	GL_ASSERT(glDrawArrays(mode, 0, count));
}

void gl_draw_arrays_instanced(GLenum mode, GLsizei count, GLsizei instancecount) {
	GL_ASSERT(glDrawArraysInstanced(mode, 0, count, instancecount));
}

void gl_draw_elements(GLenum mode, GLsizei count, GLenum type) {
	GL_ASSERT(glDrawElements(mode, count, type, NULL));
}

void gl_draw_elements_instanced(GLenum mode, GLsizei count, GLenum type, GLsizei instancecount) {
	GL_ASSERT(glDrawElementsInstanced(mode, count, type, NULL, instancecount));
}

#endif