#pragma once
#include "dx11_types.h"

ID3D11Buffer* dx11_vb_create_static(DX11Error* error, ID3D11Device* device, const void* data, UINT data_size, UINT element_size);
ID3D11Buffer* dx11_vb_create_dynamic(DX11Error* error, ID3D11Device* device, UINT data_size, UINT element_size);
void dx11_vb_bind(ID3D11Buffer* vb, ID3D11DeviceContext* context, UINT stride, UINT offset);
void dx11_vb_set_data(DX11Error* error, ID3D11Buffer* vb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_vb_delete(ID3D11Buffer* vb);

ID3D11Buffer* dx11_ib_create_static(DX11Error* error, ID3D11Device* device, const void* data, UINT data_size, UINT element_size);
ID3D11Buffer* dx11_ib_create_dynamic(DX11Error* error, ID3D11Device* device, UINT data_size, UINT element_size);
void dx11_ib_bind(ID3D11Buffer* ib, ID3D11DeviceContext* context, DXGI_FORMAT format, UINT offset);
void dx11_ib_set_data(DX11Error* error, ID3D11Buffer* ib, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_ib_delete(ID3D11Buffer* ib);

ID3D11Buffer* dx11_cb_create_static(DX11Error* error, ID3D11Device* device, const void* data, UINT data_size);
ID3D11Buffer* dx11_cb_create_dynamic(DX11Error* error, ID3D11Device* device, UINT data_size);
void dx11_cb_bind_vs(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot);
void dx11_cb_bind_ps(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot);
void dx11_cb_set_data(DX11Error* error, ID3D11Buffer* cb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_cb_delete(ID3D11Buffer* cb);

ID3D11InputLayout* dx11_il_create(DX11Error* error, ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* ied, UINT num_elements, ID3DBlob* blob);
void dx11_il_bind(ID3D11InputLayout* il, ID3D11DeviceContext* context);
void dx11_il_delete(ID3D11InputLayout* il);