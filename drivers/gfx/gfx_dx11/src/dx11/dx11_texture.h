#pragma once
#include "dx11_types.h"

ID3D11Texture2D *dx11_texture_create(DX11Error *error, ID3D11Device *device, AFilter filter, UINT width, UINT height, DXGI_FORMAT format, bool render_target,
				     bool shader_resource, bool readable, const void *data, UINT row_size);
void dx11_texture_update_subresource(ID3D11DeviceContext *context, ID3D11Texture2D *texture, const void *data, UINT row_size, int height);
void dx11_texture_read_pixel(DX11Error *error, ID3D11Texture2D *texture, ID3D11DeviceContext *context, int x, int y, uint pixel_size, void *pixel);
void dx11_texture_delete(ID3D11Texture2D *texture);

ID3D11RenderTargetView *dx11_rtv_create(DX11Error *error, ID3D11Device *device, DXGI_FORMAT format, ID3D11Texture2D *texture);
void dx11_rtv_delete(ID3D11RenderTargetView *rtv);

ID3D11ShaderResourceView *dx11_srv_create(DX11Error *error, ID3D11Device *device, AFilter filter, DXGI_FORMAT format, ID3D11Texture2D *texture);
void dx11_srv_generate_mips(ID3D11ShaderResourceView *srv, ID3D11DeviceContext *context);
void dx11_srv_bind(ID3D11ShaderResourceView *srv, ID3D11DeviceContext *context, UINT slot);
void dx11_srv_delete(ID3D11ShaderResourceView *srv);

ID3D11SamplerState *dx11_ss_create(DX11Error *error, ID3D11Device *device, AFilter filter, D3D11_TEXTURE_ADDRESS_MODE address);
void dx11_ss_bind(ID3D11SamplerState *ss, ID3D11DeviceContext *context, UINT slot);
void dx11_ss_delete(ID3D11SamplerState *ss);
