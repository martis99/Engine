#include "pch.h"
#ifdef E_OPENGL
#include "gl_uniform_buffer.h"

GLuint gl_ub_create_static(GLsizeiptr size, const void* data) {
	GLuint uniform_buffer;
	glGenBuffers(1, &uniform_buffer);
	gl_ub_bind(uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
	gl_ub_unbind(uniform_buffer);
	return uniform_buffer;
}

GLuint gl_ub_create_dynamic(GLsizeiptr size) {
	GLuint uniform_buffer;
	glGenBuffers(1, &uniform_buffer);
	gl_ub_bind(uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	gl_ub_unbind(uniform_buffer);
	return uniform_buffer;
}

void gl_ub_bind(GLuint uniform_buffer) {
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
}

void gl_ub_unbind(GLuint uniform_buffer) {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void gl_ub_bind_base(GLuint uniform_buffer, GLuint index) {
	glBindBufferBase(GL_UNIFORM_BUFFER, index, uniform_buffer);
}

void gl_ub_set_data(GLuint uniform_buffer, GLsizeiptr size, const void* data) {
	gl_ub_bind(uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	gl_ub_unbind(uniform_buffer);
}

void gl_ub_delete(GLuint uniform_buffer) {
	glDeleteBuffers(1, &uniform_buffer);
}

#endif