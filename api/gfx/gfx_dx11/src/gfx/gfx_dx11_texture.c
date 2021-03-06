#include "api/gfx/gfx_api_texture.h"
#include "dx11/dx11_texture.h"

#include "gfx_dx11_types.h"

ATexture* atexture_create(ARenderer* renderer, AWrap wrap, AFilter filter, int width, int height, int channels, void* data) {
	ATexture* texture = m_malloc(sizeof(ATexture));

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	switch (channels) {
	case 1: format = DXGI_FORMAT_R8_UNORM; break;
	case 2: format = DXGI_FORMAT_R8G8_UNORM; break;
	case 4: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	}

	texture->texture = dx11_texture_create(renderer->error, renderer->device, filter, width, height, format, 0, 1, 0, data, width * channels * sizeof(unsigned char));
	if (texture->texture == NULL) {
		log_msg(renderer->log, "Failed to create texture");
		return NULL;
	}

	if (dx11_mipmap_afilter(filter) == 1) {
		dx11_texture_update_subresource(renderer->context, texture->texture, data, width * channels * sizeof(unsigned char), height);
	}

	texture->srv = dx11_srv_create(renderer->error, renderer->device, filter, format, texture->texture);
	if (texture->srv == NULL) {
		log_msg(renderer->log, "Failed to create shader resource view");
		return NULL;
	}

	if (dx11_mipmap_afilter(filter) == 1) {
		dx11_srv_generate_mips(texture->srv, renderer->context);
	}

	texture->ss = dx11_ss_create(renderer->error, renderer->device, filter, dx11_awrap(wrap));
	if (texture->ss == NULL) {
		log_msg(renderer->log, "Failed to create sampler state");
		return NULL;
	}

	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	dx11_srv_bind(texture->srv, renderer->context, slot);
	dx11_ss_bind(texture->ss, renderer->context, slot);
}

void atexture_delete(ATexture* texture, ARenderer* renderer) {
	if (texture->texture != NULL) {
		dx11_texture_delete(texture->texture);
		texture->texture = NULL;
	}
	if (texture->srv != NULL) {
		dx11_srv_delete(texture->srv);
		texture->srv = NULL;
	}
	if (texture->ss != NULL) {
		dx11_ss_delete(texture->ss);
		texture->ss = NULL;
	}
	m_free(texture, sizeof(ATexture));
}