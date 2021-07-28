#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/atexture.h"
#include "dx11_astructs.h"
#include "dx11/dx11_texture.h"

ATexture* atexture_create(AWrap wrap, AFilter filter) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	return texture;
}

ATexture* atexture_set_data(ATexture* texture, ARenderer* renderer, int width, int height, int channels, void* data) {
	texture->texture = dx11_texture_create(renderer->device, width, height, channels, data);
	texture->srv = dx11_srv_create(renderer->device, texture->texture);
	texture->ss = dx11_ss_create(renderer->device);
	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	dx11_srv_bind(texture->srv, renderer->context);
	dx11_ss_bind(texture->ss, renderer->context);
}

void atexture_delete(ATexture* texture) {
	dx11_texture_delete(texture->texture);
	dx11_srv_delete(texture->srv);
	dx11_ss_delete(texture->ss);
	m_free(texture, sizeof(ATexture));
}
#endif