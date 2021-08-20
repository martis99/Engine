#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_buffer.h"
#include "dx11_error.h"

static ID3D11Buffer* buffer_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size, D3D11_BIND_FLAG bindFlags) {
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC bd = { 0 };
	bd.BindFlags = bindFlags;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = data_size;
	bd.StructureByteStride = element_size;

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = data;

	if (DX11_FAILED("Failed to create static buffer", device->lpVtbl->CreateBuffer(device, &bd, &sd, &buffer))) {
		return NULL;
	}

	return buffer;
}

static ID3D11Buffer* buffer_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size, D3D11_BIND_FLAG bindFlags) {
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC bd = { 0 };
	bd.BindFlags = bindFlags;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.ByteWidth = data_size;
	bd.StructureByteStride = element_size;

	if (DX11_FAILED("Failed to create dynamic buffer", device->lpVtbl->CreateBuffer(device, &bd, NULL, &buffer))) {
		return NULL;
	}

	return buffer;
}

static void buffer_set_data(ID3D11Buffer* buffer, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	D3D11_MAPPED_SUBRESOURCE ms = { 0 };

	DX11_ASSERT(context->lpVtbl->Map(context, (ID3D11Resource*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));

	if (ms.pData != NULL) {
		memcpy(ms.pData, data, data_size);
	}

	context->lpVtbl->Unmap(context, (ID3D11Resource*)buffer, 0);
}

static void buffer_delete(ID3D11Buffer* buffer) {
	buffer->lpVtbl->Release(buffer);
}

ID3D11Buffer* dx11_vb_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size) {
	return buffer_create_static(device, data, data_size, element_size, D3D11_BIND_VERTEX_BUFFER);
}

ID3D11Buffer* dx11_vb_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size) {
	return buffer_create_dynamic(device, data_size, element_size, D3D11_BIND_VERTEX_BUFFER);
}

void dx11_vb_bind(ID3D11Buffer* vb, ID3D11DeviceContext* context, UINT stride, UINT offset) {
	context->lpVtbl->IASetVertexBuffers(context, 0, 1, &vb, &stride, &offset);
}

void dx11_vb_set_data(ID3D11Buffer* vb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(vb, context, data, data_size);
}

void dx11_vb_delete(ID3D11Buffer* vb) {
	buffer_delete(vb);
}

ID3D11Buffer* dx11_ib_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size) {
	return buffer_create_static(device, data, data_size, element_size, D3D11_BIND_INDEX_BUFFER);
}

ID3D11Buffer* dx11_ib_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size) {
	return buffer_create_dynamic(device, data_size, element_size, D3D11_BIND_INDEX_BUFFER);
}

void dx11_ib_bind(ID3D11Buffer* ib, ID3D11DeviceContext* context, DXGI_FORMAT format, UINT offset) {
	context->lpVtbl->IASetIndexBuffer(context, ib, format, offset);
}

void dx11_ib_set_data(ID3D11Buffer* ib, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(ib, context, data, data_size);
}

void dx11_ib_delete(ID3D11Buffer* ib) {
	buffer_delete(ib);
}

ID3D11Buffer* dx11_cb_create_static(ID3D11Device* device, const void* data, UINT data_size) {
	return buffer_create_static(device, data, data_size, 0, D3D11_BIND_CONSTANT_BUFFER);
}

ID3D11Buffer* dx11_cb_create_dynamic(ID3D11Device* device, UINT data_size) {
	return buffer_create_dynamic(device, data_size, 0, D3D11_BIND_CONSTANT_BUFFER);
}

void dx11_cb_bind_vs(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->VSSetConstantBuffers(context, slot, 1, &cb);
}

void dx11_cb_bind_ps(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->PSSetConstantBuffers(context, slot, 1, &cb);
}

void dx11_cb_set_data(ID3D11Buffer* cb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(cb, context, data, data_size);
}

void dx11_cb_delete(ID3D11Buffer* cb) {
	buffer_delete(cb);
}

ID3D11InputLayout* dx11_il_create(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* ied, UINT num_elements, ID3DBlob* blob) {
	ID3D11InputLayout* layout = NULL;
	if (DX11_FAILED("Failed to create input layour", device->lpVtbl->CreateInputLayout(device, ied, num_elements, blob->lpVtbl->GetBufferPointer(blob), blob->lpVtbl->GetBufferSize(blob), &layout))) {
		return NULL;
	}
	return layout;
}

void dx11_il_bind(ID3D11InputLayout* il, ID3D11DeviceContext* context) {
	context->lpVtbl->IASetInputLayout(context, il);
}

void dx11_il_delete(ID3D11InputLayout* il) {
	il->lpVtbl->Release(il);
}
#endif