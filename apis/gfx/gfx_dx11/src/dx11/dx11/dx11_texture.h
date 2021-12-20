#pragma once
#include "dx11.h"

ID3D11Texture2D* dx11_texture_create(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool render_target, bool shader_resource, bool readable, const void* data, UINT row_size);
void dx11_texture_read_pixel(ID3D11Texture2D* texture, ID3D11DeviceContext* context, int x, int y, uint pixel_size, void* pixel);
void dx11_texture_delete(ID3D11Texture2D* texture);

ID3D11RenderTargetView* dx11_rtv_create(ID3D11Device* device, DXGI_FORMAT format, ID3D11Texture2D* texture);
void dx11_rtv_delete(ID3D11RenderTargetView* rtv);

ID3D11ShaderResourceView* dx11_srv_create(ID3D11Device* device, DXGI_FORMAT format, ID3D11Texture2D* texture);
void dx11_srv_bind(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* context, UINT slot);
void dx11_srv_delete(ID3D11ShaderResourceView* srv);

ID3D11SamplerState* dx11_ss_create(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE address);
void dx11_ss_bind(ID3D11SamplerState* ss, ID3D11DeviceContext* context, UINT slot);
void dx11_ss_delete(ID3D11SamplerState* ss);