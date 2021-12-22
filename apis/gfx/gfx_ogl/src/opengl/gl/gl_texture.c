#include "gl_texture.h"
#include "gl_error.h"

GLuint gl_texture_create(GLint wrap, GLint filter, GLint width, GLint height, GLint internal_format, GLenum format, GLenum type, const GLvoid* data, bool generate_mipmaps) {
	GLuint texture = 0;
	if (GL_FAILED("Failed to generate texture", glGenTextures(1, &texture))) {
		return 0;
	}
	if (texture == 0) {
		return 0;
	}
	gl_texture_bind(texture, 0);

	if (wrap != 0) {
		if (GL_FAILED("Failed to texture wrap", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap))) {
			return 0;
		}
		if (GL_FAILED("Failed to texture wrap", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap))) {
			return 0;
		}
	}
	if (filter != 0) {
		if (GL_FAILED("Failed to texture filter", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter))) {
			return 0;
		}
		if (GL_FAILED("Failed to texture filter", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter))) {
			return 0;
		}
	}

	if (generate_mipmaps) {
		if (GL_FAILED("Failed to generate mipmaps", glGenerateMipmap(GL_TEXTURE_2D))) {
			return 0;
		}
	}
	if (GL_FAILED("Failed to set texture image", glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data))) {
		return 0;
	}

	return texture;
}

void gl_texture_bind(GLuint texture, GLuint slot) {
	GL_ASSERT(glActiveTexture(GL_TEXTURE0 + slot));
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, texture));
}

void gl_texture_delete(GLuint texture) {
	GL_ASSERT(glDeleteTextures(1, &texture));
}