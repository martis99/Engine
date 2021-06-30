#include "pch.h"
#ifdef E_OPENGL
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

void gl_program_delete(GLuint program) {
	glDeleteProgram(program);
}
#endif