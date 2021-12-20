#pragma once
#include "dx11.h"

ID3D11VertexShader* dx11_vs_create(ID3D11Device* device, const char* src, ID3DBlob** blob);
void dx11_vs_bind(ID3D11VertexShader* vs, ID3D11DeviceContext* context);
void dx11_vs_delete(ID3D11VertexShader* vs, ID3DBlob* blob);

ID3D11PixelShader* dx11_ps_create(ID3D11Device* device, const char* src, ID3DBlob** blob);
void dx11_ps_bind(ID3D11PixelShader* ps, ID3D11DeviceContext* context);
void dx11_ps_delete(ID3D11PixelShader* ps, ID3DBlob* blob);