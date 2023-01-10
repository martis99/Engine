#include "gl_texture.h"
#include "gl_error.h"

GLuint gl_texture_create(GLError *error, GLint wrap, AFilter filter, GLint width, GLint height, GLint internal_format, GLenum format, GLenum type, const GLvoid *data)
{
	GLuint texture = 0;
	if (GL_FAILED(error, "Failed to generate texture", glGenTextures(1, &texture))) {
		return 0;
	}
	if (texture == 0) {
		return 0;
	}
	gl_texture_bind(error, texture, 0);

	if (wrap != 0) {
		if (GL_FAILED(error, "Failed to texture wrap", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap))) {
			return 0;
		}
		if (GL_FAILED(error, "Failed to texture wrap", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap))) {
			return 0;
		}
	}

	if (filter != 0) {
		if (GL_FAILED(error, "Failed to texture filter", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_afilter(filter)))) {
			return 0;
		}
		if (GL_FAILED(error, "Failed to texture filter", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_afilter(filter)))) {
			return 0;
		}
	}

	if (GL_FAILED(error, "Failed to set texture image", glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data))) {
		return 0;
	}

	if (gl_mipmap_afilter(filter) == 1) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
		if (GL_FAILED(error, "Failed to generate mipmaps", glGenerateMipmap(GL_TEXTURE_2D))) {
			return 0;
		}
	}

	if (gl_anisotropic_afilter(filter) == 1) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, GL_MAX_TEXTURE_MAX_ANISOTROPY);
	}

	return texture;
}

void gl_texture_bind(GLError *error, GLuint texture, GLuint slot)
{
	GL_ASSERT(error, glActiveTexture(GL_TEXTURE0 + slot));
	GL_ASSERT(error, glBindTexture(GL_TEXTURE_2D, texture));
}

void gl_texture_delete(GLError *error, GLuint texture)
{
	GL_ASSERT(error, glDeleteTextures(1, &texture));
}
