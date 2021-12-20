#include "pch.h"
#include "api/gfx/atexture.h"
#include "gn_atypes.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {

}

void atexture_delete(ATexture* texture) {
	m_free(texture, sizeof(ATexture));
}