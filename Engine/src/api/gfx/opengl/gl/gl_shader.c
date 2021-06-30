#include "pch.h"
#ifdef E_OPENGL
#include "gl_shader.h"

GLuint gl_shader_create(ShaderType type, const GLchar* source, GLint* status) {
	GLenum shader_type;
	switch (type) {
	case S_VERTEX:
		shader_type = GL_VERTEX_SHADER;
		break;
	case S_FRAGMENT:
		shader_type = GL_FRAGMENT_SHADER;
		break;
	default:
		shader_type = GL_VERTEX_SHADER;
		break;
	}

	GLuint shader = glCreateShader(shader_type);
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
#endif