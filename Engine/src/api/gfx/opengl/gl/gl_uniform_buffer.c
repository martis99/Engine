#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_uniform_buffer.h"

static void gl_ub_bind(GLuint ub) {
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
}

static void gl_ub_unbind(GLuint ub) {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint gl_ub_create() {
	GLuint uniform_buffer;
	glGenBuffers(1, &uniform_buffer);
	return uniform_buffer;
}

void gl_ub_create_static(GLuint ub, const void* uniforms, GLsizeiptr uniforms_size) {
	gl_ub_bind(ub);
	glBufferData(GL_UNIFORM_BUFFER, uniforms_size, uniforms, GL_STATIC_DRAW);
	gl_ub_unbind(ub);
}

void gl_ub_create_dynamic(GLuint ub, GLsizeiptr uniforms_size) {
	gl_ub_bind(ub);
	glBufferData(GL_UNIFORM_BUFFER, uniforms_size, NULL, GL_DYNAMIC_DRAW);
	gl_ub_unbind(ub);
}

void gl_ub_bind_base(GLuint ub, GLuint index) {
	glBindBufferBase(GL_UNIFORM_BUFFER, index, ub);
}

void gl_ub_set_data(GLuint ub, const void* uniforms, GLsizeiptr uniforms_size) {
	gl_ub_bind(ub);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, uniforms_size, uniforms);
	gl_ub_unbind(ub);
}

void gl_ub_delete(GLuint ub) {
	glDeleteBuffers(1, &ub);
}

#endif