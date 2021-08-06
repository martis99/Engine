#include "pch.h"
#ifdef GAPI_OPENGL
#include "gl_texture.h"

GLuint gl_texture_create(GLint wrap, GLint filter, GLint width, GLint height, GLint internal_format, GLenum format, GLenum type, const GLvoid* data) {
	GLuint texture;
	glGenTextures(1, &texture);
	gl_texture_bind(texture, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	gl_texture_bind(texture, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);

	return texture;
}

void gl_texture_bind(GLuint texture, GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void gl_texture_delete(GLuint texture) {
	glDeleteTextures(1, &texture);
}
#endif