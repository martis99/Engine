#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_shader.h"

GLuint gl_shader_create(GLenum type, const GLchar* source, GLint* status) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, status);
	return shader;
}

void gl_shader_info_length(GLuint shader, GLint* length) {
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, length);
}

void gl_shader_info(GLuint shader, GLsizei length, GLchar* info) {
	glGetShaderInfoLog(shader, length, NULL, info);
}

void gl_shader_delete(GLuint shader) {
	glDeleteShader(shader);
}

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
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, length);
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
#endif