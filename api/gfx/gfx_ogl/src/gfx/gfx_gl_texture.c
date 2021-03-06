#include "api/gfx/gfx_api_texture.h"
#include "gl/gl_defines.h"
#include "gl/gl_texture.h"

#include "gfx_gl_types.h"

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

	texture->id = gl_texture_create(renderer->error, gl_awrap(wrap), filter, width, height, internal_format, format, type, data);
	if (texture->id == 0) {
		log_msg(renderer->log, "Failed to create texture");
		return NULL;
	}
	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	gl_texture_bind(renderer->error, texture->id, slot);
}

void atexture_delete(ATexture* texture, ARenderer* renderer) {
	if (texture->id != 0) {
		gl_texture_delete(renderer->error, texture->id);
		texture->id = 0;
	}
	m_free(texture, sizeof(ATexture));
}