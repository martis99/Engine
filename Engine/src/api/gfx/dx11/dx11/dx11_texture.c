#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_texture.h"

ID3D11Texture2D* dx11_texture_create(ID3D11Device* device, UINT width, UINT height, UINT channels, const void* data) {
	ID3D11Texture2D* texture;

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

	HRESULT hr = device->lpVtbl->CreateTexture2D(device, &td, &sd, &texture);
	if (FAILED(hr)) {
		log_error("Failed to create texture");
		return NULL;
	}

	return texture;
}

void dx11_texture_delete(ID3D11Texture2D* texture) {
	if (texture != NULL) {
		texture->lpVtbl->Release(texture);
	}
}

ID3D11ShaderResourceView* dx11_srv_create(ID3D11Device* device, ID3D11Texture2D* texture) {
	ID3D11ShaderResourceView* srv;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = { 0 };
	srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	HRESULT hr = device->lpVtbl->CreateShaderResourceView(device, (ID3D11Resource*)texture, &srvd, &srv);
	if (FAILED(hr)) {
		log_error("Failed to create shader resource view");
		return NULL;
	}

	return srv;
}

void dx11_srv_bind(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* context) {
	context->lpVtbl->PSSetShaderResources(context, 0, 1, &srv);
}

void dx11_srv_delete(ID3D11ShaderResourceView* srv) {
	if (srv != NULL) {
		srv->lpVtbl->Release(srv);
	}
}

ID3D11SamplerState* dx11_ss_create(ID3D11Device* device) {
	ID3D11SamplerState* ss;

	D3D11_SAMPLER_DESC sd = { 0 };
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	HRESULT hr = device->lpVtbl->CreateSamplerState(device, &sd, &ss);
	if (FAILED(hr)) {
		log_error("Failed to create sampler state");
		return NULL;
	}

	return ss;
}

void dx11_ss_bind(ID3D11SamplerState* ss, ID3D11DeviceContext* context) {
	context->lpVtbl->PSSetSamplers(context, 0, 1, &ss);
}

void dx11_ss_delete(ID3D11SamplerState* ss) {
	if (ss != NULL) {
		ss->lpVtbl->Release(ss);
	}
}
#endif