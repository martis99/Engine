#include "pch.h"
#ifdef E_OPENGL
#include "gl_texture.h"

GLuint gl_texture_generate() {
	GLuint texture;
	glGenTextures(1, &texture);
	return texture;
}

GLuint gl_texture_create(GLint wrap, GLint filter) {
	GLuint texture = gl_texture_generate();
	gl_texture_bind(texture, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	return texture;
}

void gl_texture_bind(GLuint texture, GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void gl_texture_generate_mipmap() {
	glGenerateMipmap(GL_TEXTURE_2D);
}

void gl_texture_image2d(GLuint texture, GLint width, GLint height, GLint channels, const GLvoid* data) {
	GLint format_internal = 0;
	GLenum format_data = 0;
	if (channels == 4) {
		format_internal = GL_RGBA8;
		format_data = GL_RGBA;
	} else if (channels == 3) {
		format_internal = GL_RGB8;
		format_data = GL_RGB;
	} else if (channels == 1) {
		format_internal = GL_R8;
		format_data = GL_RED;
	}

	gl_texture_bind(texture, 0);
	gl_texture_generate_mipmap();
	glTexImage2D(GL_TEXTURE_2D, 0, format_internal, width, height, 0, format_data, GL_UNSIGNED_BYTE, data);
}

void gl_texture_delete(GLuint texture) {
	glDeleteTextures(1, &texture);
}
#endif