#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/atexture.h"
#include "gl_astructs.h"

#include "gl/gl_texture.h"
#include "gl/gl_enums.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	texture->id = gl_texture_create(gl_awrap(wrap), gl_afilter(filter));
	gl_texture_image2d(texture->id, width, height, channels, data);
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