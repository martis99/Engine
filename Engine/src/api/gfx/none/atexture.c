#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/atexture.h"

struct ATexture {
	uint id;
};

ATexture* atexture_create(AWrap wrap, AFilter filter) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	return texture;
}

ATexture* atexture_set_data(ATexture* texture, int width, int height, int channels, void* data) {
	return texture;
}

void atexture_bind(ATexture* texture, uint slot) {
}

void atexture_delete(ATexture* texture) {
	m_free(texture, sizeof(ATexture));
}
#endif