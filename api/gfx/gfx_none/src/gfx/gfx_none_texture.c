#include "api/gfx/gfx_api_texture.h"

#include "gfx_none_types.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {

}

void atexture_delete(ATexture* texture, ARenderer* renderer) {
	m_free(texture, sizeof(ATexture));
}