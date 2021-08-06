#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/atexture.h"
#include "gl_atypes.h"
#include "gl/gl_texture.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));

	GLint internal_format = 0;
	GLenum format = 0;
	GLenum type = GL_UNSIGNED_BYTE;
	if (channels == 4) {
		internal_format = GL_RGBA8;
		format = GL_RGBA;
	} else if (channels == 3) {
		internal_format = GL_RGB8;
		format = GL_RGB;
	} else if (channels == 2) {
		internal_format = GL_RG8;
		format = GL_RG;
	} else if (channels == 1) {
		internal_format = GL_R8;
		format = GL_RED;
	}

	texture->id = gl_texture_create(gl_awrap(wrap), gl_afilter(filter), width, height, internal_format, format, type, data);
	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	gl_texture_bind(texture->id, slot);
}

void atexture_delete(ATexture* texture) {
	gl_texture_delete(texture->id);
	m_free(texture, sizeof(ATexture));
}
#endif