#include "pch.h"
#ifdef E_OPENGL
#include "gl_texture.h"

GLuint gl_texture_generate() {
	GLuint texture;
	glGenTextures(1, &texture);
	return texture;
}

GLuint gl_texture_create(TextureWrap wrap, AFilter filter) {
	GLuint texture = gl_texture_generate();
	gl_texture_bind(texture, 0);

	GLint texture_wrap;
	switch (wrap) {
	case W_REPEAT:
		texture_wrap = GL_REPEAT;
		break;
	default:
		texture_wrap = GL_REPEAT;
		break;
	}

	GLint texture_filter;
	switch (filter) {
	case F_LINEAR:
		texture_filter = GL_LINEAR;
		break;
	case F_NEAREST:
		texture_filter = GL_NEAREST;
		break;
	default:
		texture_filter = GL_LINEAR;
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);

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