#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_program.h"

GLuint gl_program_create() {
	return glCreateProgram();
}

void gl_program_attach_shader(GLuint program, GLuint shader) {
	glAttachShader(program, shader);
}

void gl_program_link(GLuint program, GLint* status) {
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, status);
}

void gl_program_info_length(GLuint program, GLint* length) {
	glGetProgramiv(program, GL_INFO_LONG_LENGTH, length);
}

void gl_program_info(GLuint program, GLsizei length, GLchar* info) {
	glGetProgramInfoLog(program, length, NULL, info);
}

void gl_program_use(GLuint program) {
	glUseProgram(program);
}

GLint gl_program_get_uniform_location(GLuint program, const GLchar* name) {
	return glGetUniformLocation(program, name);
}

void gl_program_delete(GLuint program) {
	glDeleteProgram(program);
}

void gl_uniform_vec1i(GLint location, GLsizei count, const GLint* value) {
	glUniform1iv(location, count, value);
}

void gl_uniform_vec2i(GLint location, GLsizei count, const GLint* value) {
	glUniform2iv(location, count, value);
}

void gl_uniform_vec3i(GLint location, GLsizei count, const GLint* value) {
	glUniform3iv(location, count, value);
}

void gl_uniform_vec4i(GLint location, GLsizei count, const GLint* value) {
	glUniform4iv(location, count, value);
}

void gl_uniform_vec1f(GLint location, GLsizei count, const GLfloat* value) {
	glUniform1fv(location, count, value);
}

void gl_uniform_vec2f(GLint location, GLsizei count, const GLfloat* value) {
	glUniform2fv(location, count, value);
}

void gl_uniform_vec3f(GLint location, GLsizei count, const GLfloat* value) {
	glUniform3fv(location, count, value);
}

void gl_uniform_vec4f(GLint location, GLsizei count, const GLfloat* value) {
	glUniform4fv(location, count, value);
}

void gl_uniform_mat4f(GLint location, GLsizei count, const GLfloat* value) {
	glUniformMatrix4fv(location, count, GL_FALSE, value);
}
#endif