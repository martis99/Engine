#include "gl_shader.h"
#include "gl_error.h"

GLuint gl_shader_create(GLError* error, GLenum type, const GLchar* source, GLint* status) {
	GLuint shader = 0;
	if (GL_FAILED(error, "Failed to create shader", shader = glCreateShader(type))) {
		return 0;
	}
	if (shader == 0) {
		return 0;
	}
	if (GL_FAILED(error, "Failed to set shader source", glShaderSource(shader, 1, &source, NULL))) {
		return 0;
	}
	if (GL_FAILED(error, "Failed to compile shader", glCompileShader(shader))) {
		printf(source);
		error->callbacks.on_error(source, NULL);
		return 0;
	}
	if (GL_FAILED(error, "Failed to get compile status", glGetShaderiv(shader, GL_COMPILE_STATUS, status))) {
		return 0;
	}
	return shader;
}

void gl_shader_info_length(GLError* error, GLuint shader, GLint* length) {
	GL_ASSERT(error, glGetShaderiv(shader, GL_INFO_LOG_LENGTH, length));
}

void gl_shader_info(GLError* error, GLuint shader, GLsizei length, GLchar* info) {
	GL_ASSERT(error, glGetShaderInfoLog(shader, length, NULL, info));
}

void gl_shader_delete(GLError* error, GLuint shader) {
	GL_ASSERT(error, glDeleteShader(shader));
}

GLuint gl_program_create(GLError* error) {
	GLuint program = 0;
	if (GL_FAILED(error, "Failed to create program", program = glCreateProgram())) {
		return 0;
	}
	return program;
}

bool gl_program_attach_shader(GLError* error, GLuint program, GLuint shader) {
	if (GL_FAILED(error, "Failed to attach shader", glAttachShader(program, shader))) {
		return A_FAIL;
	}
	return A_SUCCESS;
}

bool gl_program_link(GLError* error, GLuint program, GLint* status) {
	if (GL_FAILED(error, "Failed to link program", glLinkProgram(program))) {
		return A_FAIL;
	}
	GL_ASSERT(error, glGetProgramiv(program, GL_LINK_STATUS, status));
	return A_SUCCESS;
}

void gl_program_info_length(GLError* error, GLuint program, GLint* length) {
	GL_ASSERT(error, glGetProgramiv(program, GL_INFO_LOG_LENGTH, length));
}

void gl_program_info(GLError* error, GLuint program, GLsizei length, GLchar* info) {
	GL_ASSERT(error, glGetProgramInfoLog(program, length, NULL, info));
}

void gl_program_use(GLError* error, GLuint program) {
	GL_ASSERT(error, glUseProgram(program));
}

GLint gl_program_get_uniform_location(GLError* error, GLuint program, const GLchar* name) {
	GLint location = -1;
	if (GL_FAILED(error, "Failed to get uniform location", location = glGetUniformLocation(program, name))) {
		return 0;
	}
	return location;
}

void gl_program_delete(GLError* error, GLuint program) {
	GL_ASSERT(error, glDeleteProgram(program));
}

void gl_uniform_vec1i(GLError* error, GLint location, GLsizei count, const GLint* value) {
	GL_ASSERT(error, glUniform1iv(location, count, value));
}