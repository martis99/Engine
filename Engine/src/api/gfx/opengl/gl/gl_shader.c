#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_shader.h"
#include "gl_error.h"

GLuint gl_shader_create(GLenum type, const GLchar* source, GLint* status) {
	GLuint shader = 0;
	if (GL_FAILED("Failed to create shader", shader = glCreateShader(type))) {
		return 0;
	}
	if (shader == 0) {
		return 0;
	}
	if (GL_FAILED("Failed to set shader source", glShaderSource(shader, 1, &source, NULL))) {
		return 0;
	}
	if (GL_FAILED("Failed to compile shader", glCompileShader(shader))) {
		return 0;
	}
	if (GL_FAILED("Failed to get compile status", glGetShaderiv(shader, GL_COMPILE_STATUS, status))) {
		return 0;
	}
	return shader;
}

void gl_shader_info_length(GLuint shader, GLint* length) {
	GL_ASSERT(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, length));
}

void gl_shader_info(GLuint shader, GLsizei length, GLchar* info) {
	GL_ASSERT(glGetShaderInfoLog(shader, length, NULL, info));
}

void gl_shader_delete(GLuint shader) {
	GL_ASSERT(glDeleteShader(shader));
}

GLuint gl_program_create() {
	GLuint program = 0;
	if (GL_FAILED("Failed to create program", program = glCreateProgram())) {
		return 0;
	}
	return program;
}

bool gl_program_attach_shader(GLuint program, GLuint shader) {
	if (GL_FAILED("Failed to attach shader", glAttachShader(program, shader))) {
		return A_FAIL;
	}
	return A_SUCCESS;
}

bool gl_program_link(GLuint program, GLint* status) {
	if (GL_FAILED("Failed to link program", glLinkProgram(program))) {
		return A_FAIL;
	}
	GL_ASSERT(glGetProgramiv(program, GL_LINK_STATUS, status));
	return A_SUCCESS;
}

void gl_program_info_length(GLuint program, GLint* length) {
	GL_ASSERT(glGetProgramiv(program, GL_INFO_LOG_LENGTH, length));
}

void gl_program_info(GLuint program, GLsizei length, GLchar* info) {
	GL_ASSERT(glGetProgramInfoLog(program, length, NULL, info));
}

void gl_program_use(GLuint program) {
	GL_ASSERT(glUseProgram(program));
}

GLint gl_program_get_uniform_location(GLuint program, const GLchar* name) {
	GLint location = -1;
	if (GL_FAILED("Failed to get uniform location", location = glGetUniformLocation(program, name))) {
		return 0;
	}
	return location;
}

void gl_program_delete(GLuint program) {
	GL_ASSERT(glDeleteProgram(program));
}

void gl_uniform_vec1i(GLint location, GLsizei count, const GLint* value) {
	GL_ASSERT(glUniform1iv(location, count, value));
}
#endif