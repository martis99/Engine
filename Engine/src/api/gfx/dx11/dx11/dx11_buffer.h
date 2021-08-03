#pragma once
#ifdef GAPI_DX11
#include <d3d11.h>
#include "api/astructs.h"

ID3D11Buffer* dx11_vb_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size);
ID3D11Buffer* dx11_vb_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size);
void dx11_vb_bind(ID3D11Buffer* vb, ID3D11DeviceContext* context, UINT stride, UINT offset);
void dx11_vb_set_data(ID3D11Buffer* vb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_vb_delete(ID3D11Buffer* vb);

ID3D11Buffer* dx11_ib_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size);
ID3D11Buffer* dx11_ib_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size);
void dx11_ib_bind(ID3D11Buffer* ib, ID3D11DeviceContext* context, DXGI_FORMAT format, UINT offset);
void dx11_ib_set_data(ID3D11Buffer* ib, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_ib_delete(ID3D11Buffer* ib);

ID3D11Buffer* dx11_cb_create_static(ID3D11Device* device, const void* data, UINT data_size);
ID3D11Buffer* dx11_cb_create_dynamic(ID3D11Device* device, UINT data_size);
void dx11_cb_bind_vs(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot);
void dx11_cb_bind_ps(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot);
void dx11_cb_set_data(ID3D11Buffer* cb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size);
void dx11_cb_delete(ID3D11Buffer* cb);

ID3D11InputLayout* dx11_il_create(ID3D11Device* device, AValue* layout, UINT layout_size, AValue* instance, UINT instance_size, const void* shader, SIZE_T shader_size);
void dx11_il_bind(ID3D11InputLayout* il, ID3D11DeviceContext* context);
void dx11_il_delete(ID3D11InputLayout* il);

void dx11_draw(ID3D11DeviceContext* context, UINT vertex_count);
void dx11_draw_indexed(ID3D11DeviceContext* context, UINT index_count);
#endif