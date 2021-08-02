#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/atexture.h"
#include "dx11_astructs.h"
#include "dx11/dx11_texture.h"
#include "dx11_aenums.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	texture->texture = dx11_texture_create(renderer->device, width, height, channels, data);
	texture->srv = dx11_srv_create(renderer->device, texture->texture, channels);
	texture->ss = dx11_ss_create(renderer->device, dx11_afilter(filter), dx11_awrap(wrap));
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