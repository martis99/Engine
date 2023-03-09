#include "gl_buffer.h"
#include "gl_ctx/gl_error.h"

static void buffer_bind(GLError *error, GLenum target, GLuint buffer)
{
	GL_ASSERT(error, glBindBuffer(target, buffer));
}

static GLuint buffer_create_static(GLError *error, GLenum target, const void *data, GLsizeiptr data_size)
{
	GLuint buffer = 0;
	if (GL_FAILED(error, "Failed to generate buffer", glGenBuffers(1, &buffer))) {
		return 0;
	}
	buffer_bind(error, target, buffer);
	if (GL_FAILED(error, "Failed to set buffer data", glBufferData(target, data_size, data, GL_STATIC_DRAW))) {
		return 0;
	}
	return buffer;
}

static GLuint buffer_create_dynamic(GLError *error, GLenum target, GLsizeiptr data_size)
{
	GLuint buffer = 0;
	if (GL_FAILED(error, "Failed to generate buffer", glGenBuffers(1, &buffer))) {
		return 0;
	}
	buffer_bind(error, target, buffer);
	if (GL_FAILED(error, "Failed to set buffer data", glBufferData(target, data_size, NULL, GL_DYNAMIC_DRAW))) {
		return 0;
	}
	return buffer;
}

GLuint gl_va_create(GLError *error)
{
	GLuint va = 0;
	if (GL_FAILED(error, "Failed to generate vertex array", glGenVertexArrays(1, &va))) {
		return 0;
	}
	gl_va_bind(error, va);
	return va;
}

void gl_va_bind(GLError *error, GLuint va)
{
	GL_ASSERT(error, glBindVertexArray(va));
}

void gl_va_delete(GLError *error, GLuint va)
{
	GL_ASSERT(error, glDeleteVertexArrays(1, &va));
}

GLuint gl_vb_create_static(GLError *error, const void *data, GLsizeiptr data_size)
{
	return buffer_create_static(error, GL_ARRAY_BUFFER, data, data_size);
}

GLuint gl_vb_create_dynamic(GLError *error, GLsizeiptr data_size)
{
	return buffer_create_dynamic(error, GL_ARRAY_BUFFER, data_size);
}

void gl_vb_bind(GLError *error, GLuint vb)
{
	GL_ASSERT(error, glBindBuffer(GL_ARRAY_BUFFER, vb));
}

void gl_vb_set_data(GLError *error, GLuint vb, const void *data, GLsizeiptr data_size)
{
	gl_vb_bind(error, vb);
	GL_ASSERT(error, glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data));
}

void gl_vb_delete(GLError *error, GLuint vb)
{
	GL_ASSERT(error, glDeleteBuffers(1, &vb));
}

GLuint gl_ib_create_static(GLError *error, const void *data, GLsizeiptr data_size)
{
	return buffer_create_static(error, GL_ELEMENT_ARRAY_BUFFER, data, data_size);
}

GLuint gl_ib_create_dynamic(GLError *error, GLsizeiptr data_size)
{
	return buffer_create_dynamic(error, GL_ELEMENT_ARRAY_BUFFER, data_size);
}

void gl_ib_bind(GLError *error, GLuint ib)
{
	GL_ASSERT(error, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
}

void gl_ib_set_data(GLError *error, GLuint ib, const void *data, GLsizeiptr data_size)
{
	gl_ib_bind(error, ib);
	GL_ASSERT(error, glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, data));
}

void gl_ib_delete(GLError *error, GLuint ib)
{
	GL_ASSERT(error, glDeleteBuffers(1, &ib));
}

GLuint gl_ub_create_static(GLError *error, const void *data, GLsizeiptr data_size)
{
	return buffer_create_static(error, GL_UNIFORM_BUFFER, data, data_size);
}

GLuint gl_ub_create_dynamic(GLError *error, GLsizeiptr data_size)
{
	return buffer_create_dynamic(error, GL_UNIFORM_BUFFER, data_size);
}

void gl_ub_bind(GLError *error, GLuint ub)
{
	GL_ASSERT(error, glBindBuffer(GL_UNIFORM_BUFFER, ub));
}

void gl_ub_bind_base(GLError *error, GLuint ub, GLuint index)
{
	GL_ASSERT(error, glBindBufferBase(GL_UNIFORM_BUFFER, index, ub));
}

void gl_ub_set_data(GLError *error, GLuint ub, const void *data, GLsizeiptr data_size)
{
	gl_ub_bind(error, ub);
	GL_ASSERT(error, glBufferSubData(GL_UNIFORM_BUFFER, 0, data_size, data));
}

void gl_ub_delete(GLError *error, GLuint ub)
{
	GL_ASSERT(error, glDeleteBuffers(1, &ub));
}

GLuint gl_fb_create(GLError *error)
{
	GLuint framebuffer = 0;
	if (GL_FAILED(error, "Failed to generate frame buffer", glGenFramebuffers(1, &framebuffer))) {
		return 0;
	}
	gl_fb_bind(error, framebuffer);
	return framebuffer;
}

void gl_fb_bind(GLError *error, GLuint framebuffer)
{
	GL_ASSERT(error, glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
}

bool gl_fb_attach_texture(GLError *error, GLenum target, GLuint texture)
{
	if (GL_FAILED(error, "Failed to attach texture fo framebuffer", glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, texture, 0))) {
		return A_FAIL;
	}
	return A_SUCCESS;
}

bool gl_fb_check_status(GLError *error, GLuint framebuffer)
{
	GL_ASSERT(error, bool status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	return status;
}

void gl_fb_delete(GLError *error, GLuint framebuffer)
{
	GL_ASSERT(error, glDeleteFramebuffers(1, &framebuffer));
}

GLuint gl_rb_create(GLError *error)
{
	GLuint renderbuffer = 0;
	if (GL_FAILED(error, "Failed to generate render buffer", glGenRenderbuffers(1, &renderbuffer))) {
		return 0;
	}
	gl_rb_bind(error, renderbuffer);
	return renderbuffer;
}

void gl_rb_bind(GLError *error, GLuint renderbuffer)
{
	GL_ASSERT(error, glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer));
}

void gl_rb_storage(GLError *error, GLsizei width, GLsizei height)
{
	GL_ASSERT(error, glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
}

void gl_rb_delete(GLError *error, GLuint renderbuffer)
{
	GL_ASSERT(error, glDeleteRenderbuffers(1, &renderbuffer));
}

void gl_draw_buffers(GLError *error, GLsizei n, const GLenum *bufs)
{
	GL_ASSERT(error, glDrawBuffers(n, bufs));
}

void gl_read_pixels(GLError *error, GLenum mode, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *data)
{
	GL_ASSERT(error, glReadBuffer(mode));
	GL_ASSERT(error, glReadPixels(x, y, width, height, format, type, data));
}

void gl_clear(GLError *error, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLdouble depth)
{
	GL_ASSERT(error, glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL_ASSERT(error, glClearColor(red, green, blue, alpha));
	GL_ASSERT(error, glClearDepth(depth));
}

void gl_dsb_clear(GLError *error, GLfloat depth, GLint stencil)
{
	GL_ASSERT(error, glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil));
}

void gl_cb_clear(GLError *error, GLenum type, GLint index, const void *value)
{
	switch (type) {
	case GL_UNSIGNED_INT: GL_ASSERT(error, glClearBufferuiv(GL_COLOR, index, value)); break;
	case GL_INT: GL_ASSERT(error, glClearBufferiv(GL_COLOR, index, value)); break;
	case GL_FLOAT: GL_ASSERT(error, glClearBufferfv(GL_COLOR, index, value)); break;
	}
}

bool gl_vertex_attrib(GLError *error, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer, GLuint divisor)
{
	if (GL_FAILED(error, "Failed to enable vertex attrib array", glEnableVertexAttribArray(index))) {
		return A_FAIL;
	}

	switch (type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_INT:
	case GL_UNSIGNED_INT:
		if (GL_FAILED(error, "Failed to set vertex attrib pointer", glVertexAttribIPointer(index, size, type, stride, pointer))) {
			return A_FAIL;
		}
		break;
	case GL_FLOAT:
		if (GL_FAILED(error, "Failed to set vertex attrib pointer", glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer))) {
			return A_FAIL;
		}
		break;
	case GL_DOUBLE:
		if (GL_FAILED(error, "Failed to set vertex attrib pointer", glVertexAttribLPointer(index, size, type, stride, pointer))) {
			return A_FAIL;
		}
		break;
	}

	if (GL_FAILED(error, "Failed to set vertex attrib divisor", glVertexAttribDivisor(index, divisor))) {
		return A_FAIL;
	}

	return A_SUCCESS;
}

void gl_draw_arrays(GLError *error, GLenum mode, GLsizei count)
{
	GL_ASSERT(error, glDrawArrays(mode, 0, count));
}

void gl_draw_arrays_instanced(GLError *error, GLenum mode, GLsizei count, GLsizei instancecount)
{
	GL_ASSERT(error, glDrawArraysInstanced(mode, 0, count, instancecount));
}

void gl_draw_elements(GLError *error, GLenum mode, GLsizei count, GLenum type)
{
	GL_ASSERT(error, glDrawElements(mode, count, type, NULL));
}

void gl_draw_elements_instanced(GLError *error, GLenum mode, GLsizei count, GLenum type, GLsizei instancecount)
{
	GL_ASSERT(error, glDrawElementsInstanced(mode, count, type, NULL, instancecount));
}
