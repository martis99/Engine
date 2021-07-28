#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/atexture.h"
#include "dx11_astructs.h"

ATexture* atexture_create(AWrap wrap, AFilter filter) {
	ATexture* texture = m_malloc(sizeof(ATexture));
	return texture;
}

ATexture* atexture_set_data(ATexture* texture, ARenderer* renderer, int width, int height, int channels, void* data) {
	D3D11_TEXTURE2D_DESC td = { 0 };
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = data;
	sd.SysMemPitch = width * 4 * sizeof(unsigned char);

	HRESULT hr = renderer->device->lpVtbl->CreateTexture2D(renderer->device, &td, &sd, &texture->texture);
	if (FAILED(hr)) {
		log_error("Failed to create texture");
		return NULL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = { 0 };
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	renderer->device->lpVtbl->CreateShaderResourceView(renderer->device, (ID3D11Resource*)texture->texture, &srvd, &texture->texture_view);

	D3D11_SAMPLER_DESC sad = { 0 };
	sad.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sad.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sad.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sad.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	renderer->device->lpVtbl->CreateSamplerState(renderer->device, &sad, &texture->sampler);

	return texture;
}

void atexture_bind(ATexture* texture, ARenderer* renderer, uint slot) {
	renderer->context->lpVtbl->PSSetSamplers(renderer->context, 0, 1, &texture->sampler);
	renderer->context->lpVtbl->PSSetShaderResources(renderer->context, 0, 1, &texture->texture_view);
}

void atexture_delete(ATexture* texture) {
	m_free(texture, sizeof(ATexture));
}
#endif