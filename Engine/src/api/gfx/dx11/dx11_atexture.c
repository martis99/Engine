#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/atexture.h"
#include "dx11_atypes.h"
#include "dx11/dx11_texture.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	switch (channels) {
	case 1: format = DXGI_FORMAT_R8_UNORM; break;
	case 2: format = DXGI_FORMAT_R8G8_UNORM; break;
	case 4: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	}

	texture->texture = dx11_texture_create(renderer->device, width, height, format, 0, 1, 0, data, width * channels * sizeof(unsigned char));
	if (texture->texture == NULL) {
		return NULL;
	}

	texture->srv = dx11_srv_create(renderer->device, format, texture->texture);
	if (texture->srv == NULL) {
		return NULL;
	}

	texture->ss = dx11_ss_create(renderer->device, dx11_afilter(filter), dx11_awrap(wrap));
	if (texture->ss == NULL) {
		return NULL;
	}

	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	dx11_srv_bind(texture->srv, renderer->context, slot);
	dx11_ss_bind(texture->ss, renderer->context, slot);
}

void atexture_delete(ATexture* texture) {
	dx11_texture_delete(texture->texture);
	dx11_srv_delete(texture->srv);
	dx11_ss_delete(texture->ss);
	m_free(texture, sizeof(ATexture));
}
#endif