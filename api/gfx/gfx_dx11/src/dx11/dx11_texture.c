#include "dx11_texture.h"
#include "dx11_error.h"

ID3D11Texture2D *dx11_texture_create(DX11Error *error, ID3D11Device *device, AFilter filter, UINT width, UINT height, DXGI_FORMAT format, bool render_target,
				     bool shader_resource, bool readable, const void *data, UINT row_size)
{
	ID3D11Texture2D *texture = NULL;

	D3D11_TEXTURE2D_DESC desc = {
		.Width		    = width,
		.Height		    = height,
		.ArraySize	    = 1,
		.Format		    = format,
		.SampleDesc.Count   = 1,
		.SampleDesc.Quality = 0,
	};

	if (dx11_mipmap_afilter(filter) == 0) {
		desc.MipLevels	    = 1;
		desc.Usage	    = readable == 0 ? D3D11_USAGE_DEFAULT : D3D11_USAGE_STAGING;
		desc.BindFlags	    = (render_target == 0 ? 0 : D3D11_BIND_RENDER_TARGET) | (shader_resource == 0 ? 0 : D3D11_BIND_SHADER_RESOURCE);
		desc.CPUAccessFlags = readable == 0 ? 0 : D3D11_CPU_ACCESS_READ;
		desc.MiscFlags	    = 0;
	} else {
		desc.MipLevels	    = 0;
		desc.Usage	    = D3D11_USAGE_DEFAULT;
		desc.BindFlags	    = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags	    = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (data == NULL || dx11_mipmap_afilter(filter) == 1) {
		if (DX11_FAILED(error, "Failed to create texture", device->lpVtbl->CreateTexture2D(device, &desc, NULL, &texture))) {
			return NULL;
		}
	} else {
		D3D11_SUBRESOURCE_DATA sd = {
			.pSysMem     = data,
			.SysMemPitch = row_size,
		};

		if (DX11_FAILED(error, "Failed to create texture", device->lpVtbl->CreateTexture2D(device, &desc, &sd, &texture))) {
			return NULL;
		}
	}

	return texture;
}

void dx11_texture_update_subresource(ID3D11DeviceContext *context, ID3D11Texture2D *texture, const void *data, UINT row_size, int height)
{
	context->lpVtbl->UpdateSubresource(context, (ID3D11Resource *)texture, 0, NULL, data, row_size, 0);
}

void dx11_texture_read_pixel(DX11Error *error, ID3D11Texture2D *texture, ID3D11DeviceContext *context, int x, int y, uint pixel_size, void *pixel)
{
	D3D11_MAPPED_SUBRESOURCE ms = { 0 };
	DX11_ASSERT(error, context->lpVtbl->Map(context, (ID3D11Resource *)texture, 0, D3D11_MAP_READ, 0, &ms));

	if (ms.pData != NULL) {
		memcpy(pixel, (byte *)ms.pData + (size_t)y * ms.RowPitch + (size_t)x * pixel_size, pixel_size);
	}
	context->lpVtbl->Unmap(context, (ID3D11Resource *)texture, 0);
}

void dx11_texture_delete(ID3D11Texture2D *texture)
{
	texture->lpVtbl->Release(texture);
}

ID3D11RenderTargetView *dx11_rtv_create(DX11Error *error, ID3D11Device *device, DXGI_FORMAT format, ID3D11Texture2D *texture)
{
	ID3D11RenderTargetView *rtv = NULL;

	D3D11_RENDER_TARGET_VIEW_DESC desc = {
		.Format		    = format,
		.ViewDimension	    = D3D11_RTV_DIMENSION_TEXTURE2D,
		.Texture2D.MipSlice = 0,
	};

	if (DX11_FAILED(error, "Failed to create render target view", device->lpVtbl->CreateRenderTargetView(device, (ID3D11Resource *)texture, &desc, &rtv))) {
		return NULL;
	}

	return rtv;
}

void dx11_rtv_delete(ID3D11RenderTargetView *rtv)
{
	rtv->lpVtbl->Release(rtv);
}

ID3D11ShaderResourceView *dx11_srv_create(DX11Error *error, ID3D11Device *device, AFilter filter, DXGI_FORMAT format, ID3D11Texture2D *texture)
{
	ID3D11ShaderResourceView *srv = NULL;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
		.Format			   = format,
		.ViewDimension		   = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D.MostDetailedMip = 0,
	};

	if (dx11_mipmap_afilter(filter) == 0) {
		desc.Texture2D.MipLevels = 1;
	} else {
		desc.Texture2D.MipLevels = -1;
	}

	if (DX11_FAILED(error, "Failed to create shader resource view", device->lpVtbl->CreateShaderResourceView(device, (ID3D11Resource *)texture, &desc, &srv))) {
		return NULL;
	}

	return srv;
}

void dx11_srv_generate_mips(ID3D11ShaderResourceView *srv, ID3D11DeviceContext *context)
{
	context->lpVtbl->GenerateMips(context, srv);
}

void dx11_srv_bind(ID3D11ShaderResourceView *srv, ID3D11DeviceContext *context, UINT slot)
{
	context->lpVtbl->VSSetShaderResources(context, slot, 1, &srv);
	context->lpVtbl->PSSetShaderResources(context, slot, 1, &srv);
}

void dx11_srv_delete(ID3D11ShaderResourceView *srv)
{
	srv->lpVtbl->Release(srv);
}

ID3D11SamplerState *dx11_ss_create(DX11Error *error, ID3D11Device *device, AFilter filter, D3D11_TEXTURE_ADDRESS_MODE address)
{
	ID3D11SamplerState *ss = NULL;

	D3D11_SAMPLER_DESC desc = {
		.Filter	  = dx11_afilter(filter),
		.AddressU = address,
		.AddressV = address,
		.AddressW = address,
	};

	if (dx11_mipmap_afilter(filter) == 1) {
		desc.MipLODBias = 0;
		desc.MinLOD	= 0;
		desc.MaxLOD	= D3D11_FLOAT32_MAX;
	}

	if (dx11_anisotropic_afilter(filter) == 1) {
		desc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	}

	if (DX11_FAILED(error, "Failed to create sampler state", device->lpVtbl->CreateSamplerState(device, &desc, &ss))) {
		return NULL;
	}

	return ss;
}

void dx11_ss_bind(ID3D11SamplerState *ss, ID3D11DeviceContext *context, UINT slot)
{
	context->lpVtbl->VSSetSamplers(context, slot, 1, &ss);
	context->lpVtbl->PSSetSamplers(context, slot, 1, &ss);
}

void dx11_ss_delete(ID3D11SamplerState *ss)
{
	ss->lpVtbl->Release(ss);
}
