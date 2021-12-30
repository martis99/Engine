#pragma once
#include "dx11.h"

ID3D11BlendState* dx11_blend_create(DX11Error* error, ID3D11Device* device, BOOL enabled);
void dx11_blend_bind(ID3D11BlendState* blend, ID3D11DeviceContext* context);
void dx11_blend_delete(ID3D11BlendState* blend);

ID3D11RasterizerState* dx11_resterizer_create(DX11Error* error, ID3D11Device* device, D3D11_FILL_MODE fill_mode, D3D11_CULL_MODE cull_mode);
void dx11_rasterizer_bind(ID3D11RasterizerState* raterizer, ID3D11DeviceContext* context);
void dx11_rasterizer_delete(ID3D11RasterizerState* rasterizer);

ID3D11DepthStencilState* dx11_depth_stencil_create(DX11Error* error, ID3D11Device* device, BOOL depth_enable, BOOL stencil_enable);
void dx11_depth_stencil_bind(ID3D11DepthStencilState* depth_stencil, ID3D11DeviceContext* context);
void dx11_depth_stencil_delete(ID3D11DepthStencilState* depth_stencil);