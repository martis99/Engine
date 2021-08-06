#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_texture.h"

ID3D11Texture2D* dx11_texture_create(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool render_target, bool shader_resource, bool readable, const void* data, UINT row_size) {
	ID3D11Texture2D* texture = NULL;

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = readable == 0 ? D3D11_USAGE_DEFAULT : D3D11_USAGE_STAGING;
	desc.BindFlags = (render_target == 0 ? 0 : D3D11_BIND_RENDER_TARGET) | (shader_resource == 0 ? 0 : D3D11_BIND_SHADER_RESOURCE);
	desc.CPUAccessFlags = readable == 0 ? 0 : D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;

	if (data == NULL) {
		HRESULT hr = device->lpVtbl->CreateTexture2D(device, &desc, NULL, &texture);
		if (FAILED(hr)) {
			log_error("Failed to create texture");
			return NULL;
		}
	} else {
		D3D11_SUBRESOURCE_DATA sd = { 0 };
		sd.pSysMem = data;
		sd.SysMemPitch = row_size;

		HRESULT hr = device->lpVtbl->CreateTexture2D(device, &desc, &sd, &texture);
		if (FAILED(hr)) {
			log_error("Failed to create texture");
			return NULL;
		}
	}

	return texture;
}

void dx11_texture_delete(ID3D11Texture2D* texture) {
	if (texture != NULL) {
		texture->lpVtbl->Release(texture);
	}
}

ID3D11RenderTargetView* dx11_rtv_create(ID3D11Device* device, DXGI_FORMAT format, ID3D11Texture2D* texture) {
	ID3D11RenderTargetView* rtv = NULL;

	D3D11_RENDER_TARGET_VIEW_DESC desc = { 0 };
	desc.Format = format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	HRESULT hr = device->lpVtbl->CreateRenderTargetView(device, (ID3D11Resource*)texture, &desc, &rtv);
	if (FAILED(hr)) {
		log_error("Failed to create render target view");
		return NULL;
	}

	return rtv;
}

void dx11_rtv_delete(ID3D11RenderTargetView* rtv) {
	if (rtv != NULL) {
		rtv->lpVtbl->Release(rtv);
	}
}

ID3D11ShaderResourceView* dx11_srv_create(ID3D11Device* device, DXGI_FORMAT format, ID3D11Texture2D* texture) {
	ID3D11ShaderResourceView* srv = NULL;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = { 0 };
	desc.Format = format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;

	HRESULT hr = device->lpVtbl->CreateShaderResourceView(device, (ID3D11Resource*)texture, &desc, &srv);
	if (FAILED(hr)) {
		log_error("Failed to create shader resource view");
		return NULL;
	}

	return srv;
}

void dx11_srv_bind(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->PSSetShaderResources(context, slot, 1, &srv);
}

void dx11_srv_delete(ID3D11ShaderResourceView* srv) {
	if (srv != NULL) {
		srv->lpVtbl->Release(srv);
	}
}

ID3D11SamplerState* dx11_ss_create(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE address) {
	ID3D11SamplerState* ss = NULL;

	D3D11_SAMPLER_DESC desc = { 0 };
	desc.Filter = filter;
	desc.AddressU = address;
	desc.AddressV = address;
	desc.AddressW = address;

	HRESULT hr = device->lpVtbl->CreateSamplerState(device, &desc, &ss);
	if (FAILED(hr)) {
		log_error("Failed to create sampler state");
		return NULL;
	}

	return ss;
}

void dx11_ss_bind(ID3D11SamplerState* ss, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->PSSetSamplers(context, slot, 1, &ss);
}

void dx11_ss_delete(ID3D11SamplerState* ss) {
	if (ss != NULL) {
		ss->lpVtbl->Release(ss);
	}
}
#endif