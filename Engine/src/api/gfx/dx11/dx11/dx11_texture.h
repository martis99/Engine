#pragma once
#ifdef GAPI_DX11
#include <d3d11.h>

ID3D11Texture2D* dx11_texture_create(ID3D11Device* device, UINT width, UINT height, UINT channels, const void* data);
void dx11_texture_delete(ID3D11Texture2D* texture);

ID3D11ShaderResourceView* dx11_srv_create(ID3D11Device* device, ID3D11Texture2D* texture);
void dx11_srv_bind(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* context, UINT slot);
void dx11_srv_delete(ID3D11ShaderResourceView* srv);

ID3D11SamplerState* dx11_ss_create(ID3D11Device* device);
void dx11_ss_bind(ID3D11SamplerState* ss, ID3D11DeviceContext* context);
void dx11_ss_delete(ID3D11SamplerState* ss);
#endif