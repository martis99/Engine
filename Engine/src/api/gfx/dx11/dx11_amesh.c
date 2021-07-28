#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/amesh.h"
#include "dx11_astructs.h"

#include <d3dcompiler.h>

#pragma comment (lib, "D3DCompiler.lib")

typedef struct Vertex {
	float x;
	float y;
	float z;
	float u;
	float v;
} Vertex;

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	m_free(mesh, sizeof(AMesh));
}

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	Vertex verts[] = {
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
	};

	unsigned int indes[] = {
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.ByteWidth = sizeof(verts);
	vbd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vsd = { 0 };
	vsd.pSysMem = verts;
	HRESULT hr;
	hr = renderer->device->lpVtbl->CreateBuffer(renderer->device, &vbd, &vsd, &mesh->vb);
	if (FAILED(hr)) {
		log_error("Failed to create vertex buffer");
		return NULL;
	}

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 1, &mesh->vb, &stride, &offset);

	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = sizeof(indes);
	ibd.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA isd = { 0 };
	isd.pSysMem = indes;
	hr = renderer->device->lpVtbl->CreateBuffer(renderer->device, &ibd, &isd, &mesh->ib);
	if (FAILED(hr)) {
		log_error("Failed to create index buffer");
		return NULL;
	}

	renderer->context->lpVtbl->IASetIndexBuffer(renderer->context, mesh->ib, DXGI_FORMAT_R32_UINT, 0);

	ID3D11InputLayout* input_layout;
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = renderer->device->lpVtbl->CreateInputLayout(renderer->device, ied, 2, shader->vs_blob->lpVtbl->GetBufferPointer(shader->vs_blob), shader->vs_blob->lpVtbl->GetBufferSize(shader->vs_blob), &input_layout);
	if (FAILED(hr)) {
		log_error("Failed to create input layout");
		return NULL;
	}
	renderer->context->lpVtbl->IASetInputLayout(renderer->context, input_layout);

	return mesh;
}

AMesh* amesh_init_dynamic(AMesh* mesh, ARenderer* renderer, AShader* shader, uint vertices_size, const void* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
	return mesh;
}

void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, ADataType* layout, uint layout_size) {

}

void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, ADataType* layout, uint layout_size) {

}

void amesh_set_vertices(AMesh* mesh, const void* vertices, uint vertices_size) {

}

void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size) {

}

void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size) {

}

void amesh_draw_arrays(AMesh* mesh, ARenderer* renderer) {

}

void amesh_draw_arrays_instanced(AMesh* mesh, int count, ARenderer* renderer) {

}

void amesh_draw_elements(AMesh* mesh, ARenderer* renderer) {
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 1, &mesh->vb, &stride, &offset);
	//renderer->context->lpVtbl->Draw(renderer->context, 3, 0);
	renderer->context->lpVtbl->DrawIndexed(renderer->context, 6, 0, 0);
}

void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer) {

}

void amesh_set_count(AMesh* mesh, int count) {

}

void amesh_add_count(AMesh* mesh, int count) {

}
#endif