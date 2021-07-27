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
	glGetShaderiv(shader, GL_INFO_LONG_LENGTH, length);
}

void gl_shader_info(GLuint shader, GLsizei length, GLchar* info) {
	glGetShaderInfoLog(shader, length, NULL, info);
}

void gl_shader_delete(GLuint shader) {
	glDeleteShader(shader);
}

void gl_shader_bind_uniform_block(GLuint shader, const GLchar* name, GLuint index) {
	glUniformBlockBinding(shader, glGetUniformBlockIndex(shader, name), index);
}
#endif