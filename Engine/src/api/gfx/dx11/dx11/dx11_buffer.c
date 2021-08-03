#include "pch.h"
#ifdef GAPI_DX11
#include "dx11_buffer.h"

static ID3D11Buffer* buffer_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size, D3D11_BIND_FLAG bindFlags, const char* error_message) {
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = { 0 };
	bd.BindFlags = bindFlags;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.ByteWidth = data_size;
	bd.StructureByteStride = element_size;

	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = data;

	HRESULT hr = device->lpVtbl->CreateBuffer(device, &bd, &sd, &buffer);
	if (FAILED(hr)) {
		log_error(error_message);
		return NULL;
	}

	return buffer;
}

static ID3D11Buffer* buffer_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size, D3D11_BIND_FLAG bindFlags, const char* error_message) {
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC bd = { 0 };
	bd.BindFlags = bindFlags;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.ByteWidth = data_size;
	bd.StructureByteStride = element_size;

	HRESULT hr = device->lpVtbl->CreateBuffer(device, &bd, NULL, &buffer);
	if (FAILED(hr)) {
		log_error(error_message);
		return NULL;
	}

	return buffer;
}

static void buffer_set_data(ID3D11Buffer* buffer, ID3D11DeviceContext* context, const void* data, SIZE_T data_size, const char* error_message) {
	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = context->lpVtbl->Map(context, (ID3D11Resource*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (FAILED(hr)) {
		log_error(error_message);
		return;
	}

	memcpy(ms.pData, data, data_size);
	context->lpVtbl->Unmap(context, (ID3D11Resource*)buffer, 0);
}

static void buffer_delete(ID3D11Buffer* buffer) {
	if (buffer != NULL) {
		buffer->lpVtbl->Release(buffer);
	}
}

ID3D11Buffer* dx11_vb_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size) {
	return buffer_create_static(device, data, data_size, element_size, D3D11_BIND_VERTEX_BUFFER, "Failed to create static vertex buffer");
}

ID3D11Buffer* dx11_vb_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size) {
	return buffer_create_dynamic(device, data_size, element_size, D3D11_BIND_VERTEX_BUFFER, "Failed to create dynamic vertex buffer");
}

void dx11_vb_bind(ID3D11Buffer* vb, ID3D11DeviceContext* context, UINT stride, UINT offset) {
	context->lpVtbl->IASetVertexBuffers(context, 0, 1, &vb, &stride, &offset);
}

void dx11_vb_set_data(ID3D11Buffer* vb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(vb, context, data, data_size, "Failed to set vertex buffer data");
}

void dx11_vb_delete(ID3D11Buffer* vb) {
	buffer_delete(vb);
}

ID3D11Buffer* dx11_ib_create_static(ID3D11Device* device, const void* data, UINT data_size, UINT element_size) {
	return buffer_create_static(device, data, data_size, element_size, D3D11_BIND_INDEX_BUFFER, "Failed to create static index buffer");
}

ID3D11Buffer* dx11_ib_create_dynamic(ID3D11Device* device, UINT data_size, UINT element_size) {
	return buffer_create_dynamic(device, data_size, element_size, D3D11_BIND_INDEX_BUFFER, "Failed to create dynamic index buffer");
}

void dx11_ib_bind(ID3D11Buffer* ib, ID3D11DeviceContext* context, DXGI_FORMAT format, UINT offset) {
	context->lpVtbl->IASetIndexBuffer(context, ib, format, offset);
}

void dx11_ib_set_data(ID3D11Buffer* ib, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(ib, context, data, data_size, "Failed to set index buffer data");
}

void dx11_ib_delete(ID3D11Buffer* ib) {
	buffer_delete(ib);
}

ID3D11Buffer* dx11_cb_create_static(ID3D11Device* device, const void* data, UINT data_size) {
	return buffer_create_static(device, data, data_size, 0, D3D11_BIND_CONSTANT_BUFFER, "Failed to create static constant buffer");
}

ID3D11Buffer* dx11_cb_create_dynamic(ID3D11Device* device, UINT data_size) {
	return buffer_create_dynamic(device, data_size, 0, D3D11_BIND_CONSTANT_BUFFER, "Failed to create dynamic constant buffer");
}

void dx11_cb_bind_vs(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->VSSetConstantBuffers(context, slot, 1, &cb);
}

void dx11_cb_bind_ps(ID3D11Buffer* cb, ID3D11DeviceContext* context, UINT slot) {
	context->lpVtbl->PSSetConstantBuffers(context, slot, 1, &cb);
}

void dx11_cb_set_data(ID3D11Buffer* cb, ID3D11DeviceContext* context, const void* data, SIZE_T data_size) {
	buffer_set_data(cb, context, data, data_size, "Failed to set constant buffer data");
}

void dx11_cb_delete(ID3D11Buffer* cb) {
	buffer_delete(cb);
}

static DXGI_FORMAT get_element_format(AType type) {
	switch (type) {
	case VEC1I: return DXGI_FORMAT_R32_SINT;
	case VEC2I: return DXGI_FORMAT_R32G32_SINT;
	case VEC3I: return DXGI_FORMAT_R32G32B32_SINT;
	case VEC4I: return DXGI_FORMAT_R32G32B32A32_SINT;
	case VEC1F: return DXGI_FORMAT_R32_FLOAT;
	case VEC2F: return DXGI_FORMAT_R32G32_FLOAT;
	case VEC3F: return DXGI_FORMAT_R32G32B32_FLOAT;
	case VEC4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case MAT4F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	return 0;
}

ID3D11InputLayout* dx11_il_create(ID3D11Device* device, AValue* layout, UINT layout_size, AValue* instance, UINT instance_size, const void* shader, SIZE_T shader_size) {
	ID3D11InputLayout* il;

	UINT num_layout = layout_size / sizeof(AValue);

	UINT num_instance = 0;
	UINT num_instance_el = 0;
	if (instance != NULL) {
		num_instance = instance_size / sizeof(AValue);
		num_instance_el = 4;
	}
	D3D11_INPUT_ELEMENT_DESC* ied = m_malloc((num_layout + num_instance_el) * sizeof(D3D11_INPUT_ELEMENT_DESC));
	UINT index = 0;
	for (UINT i = 0; i < num_layout; i++) {
		ied[index++] = (D3D11_INPUT_ELEMENT_DESC){ layout[i].name, 0, get_element_format(layout[i].type), 0, i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}

	for (UINT i = 0; i < num_instance; i++) {
		if (instance[i].type == MAT4F) {
			for (int j = 0; j < 4; j++) {
				ied[index++] = (D3D11_INPUT_ELEMENT_DESC){ instance[i].name, j, get_element_format(instance[i].type), 1, (i == 0 && j == 0) ? 0 : D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			}
		} else {
			ied[index++] = (D3D11_INPUT_ELEMENT_DESC){ instance[i].name, i, get_element_format(instance[i].type), 1, i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		}
	}

	HRESULT hr = device->lpVtbl->CreateInputLayout(device, ied, num_layout + num_instance_el, shader, shader_size, &il);
	if (FAILED(hr)) {
		log_error("Failed to create input layout");
		return NULL;
	}

	m_free(ied, (num_layout + num_instance_el) * sizeof(D3D11_INPUT_ELEMENT_DESC));
	return il;
}

void dx11_il_bind(ID3D11InputLayout* il, ID3D11DeviceContext* context) {
	context->lpVtbl->IASetInputLayout(context, il);
}

void dx11_il_delete(ID3D11InputLayout* il) {
	if (il != NULL) {
		il->lpVtbl->Release(il);
	}
}

void dx11_draw(ID3D11DeviceContext* context, UINT vertex_count) {
	context->lpVtbl->Draw(context, vertex_count, 0);
}

void dx11_draw_indexed(ID3D11DeviceContext* context, UINT index_count) {
	context->lpVtbl->DrawIndexed(context, index_count, 0, 0);
}
#endif