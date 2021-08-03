#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/amesh.h"
#include "dx11_astructs.h"
#include "dx11/dx11_buffer.h"
#include "dx11_aenums.h"

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	mesh->ivb = NULL;
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	dx11_vb_delete(mesh->vb);
	if (mesh->indexed) {
		dx11_ib_delete(mesh->ib);
	}

	if (mesh->ivb != NULL) {
		dx11_vb_delete(mesh->ivb);
	}

	dx11_il_delete(mesh->il);
	m_free(mesh, sizeof(AMesh));
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
	}
	return 0;
}

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, AValue* instance, uint instance_size, APrimitive primitive) {
	mesh->primitive = primitive;
	mesh->vertex_size = vertex_size;

	mesh->vb = dx11_vb_create_static(renderer->device, vertices, vertices_size, vertex_size);
	mesh->ib = dx11_ib_create_static(renderer->device, indices, indices_size, index_size);
	mesh->il = dx11_il_create(renderer->device, layout, layout_size, instance, instance_size, shader->vs_blob->lpVtbl->GetBufferPointer(shader->vs_blob), shader->vs_blob->lpVtbl->GetBufferSize(shader->vs_blob));

	mesh->ivb = dx11_vb_create_dynamic(renderer->device, 100 * sizeof(mat4), sizeof(mat4));

	mesh->indexed = 1;
	mesh->count = indices_size / index_size;
	return mesh;
}

AMesh* amesh_init_dynamic(AMesh* mesh, ARenderer* renderer, AShader* shader, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, AValue* instance, uint instance_size, APrimitive primitive) {
	mesh->primitive = primitive;
	mesh->vertex_size = vertex_size;
	mesh->indexed = 0;
	mesh->vb = dx11_vb_create_dynamic(renderer->device, vertices_size, vertex_size);
	if (indices != NULL) {
		mesh->ib = dx11_ib_create_static(renderer->device, indices, indices_size, index_size);
		mesh->indexed = 1;
	}
	mesh->il = dx11_il_create(renderer->device, layout, layout_size, instance, instance_size, shader->vs_blob->lpVtbl->GetBufferPointer(shader->vs_blob), shader->vs_blob->lpVtbl->GetBufferSize(shader->vs_blob));

	mesh->count = 0;
	return mesh;
}

void amesh_set_vertices(AMesh* mesh, ARenderer* renderer, const void* vertices, uint vertices_size) {
	dx11_vb_set_data(mesh->vb, renderer->context, vertices, vertices_size);
}

void amesh_set_instances(AMesh* mesh, ARenderer* renderer, const void* instances, uint instances_size) {
	dx11_vb_set_data(mesh->ivb, renderer->context, instances, instances_size);
}

void amesh_set_indices(AMesh* mesh, ARenderer* renderer, const void* indices, uint indices_size) {
	dx11_ib_set_data(mesh->vb, renderer->context, indices, indices_size);
}

void amesh_draw_arrays(AMesh* mesh, ARenderer* renderer) {
	dx11_vb_bind(mesh->vb, renderer->context, mesh->vertex_size, 0);
	dx11_il_bind(mesh->il, renderer->context);

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, dx11_aprimitive(mesh->primitive));
	dx11_draw(renderer->context, mesh->count);
}

void amesh_draw_arrays_instanced(AMesh* mesh, int count, ARenderer* renderer) {
	UINT strides[] = {
	sizeof(float) * 5,
	sizeof(mat4)
	};

	UINT offsets[] = {
		0, 0
	};

	ID3D11Buffer* buffers[] = { mesh->vb, mesh->ivb };

	renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 2, buffers, strides, offsets);
	dx11_il_bind(mesh->il, renderer->context);

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, dx11_aprimitive(mesh->primitive));
	renderer->context->lpVtbl->DrawInstanced(renderer->context, mesh->count, 100, 0, 0);
}

void amesh_draw_elements(AMesh* mesh, ARenderer* renderer) {
	dx11_vb_bind(mesh->vb, renderer->context, mesh->vertex_size, 0);
	dx11_ib_bind(mesh->ib, renderer->context, DXGI_FORMAT_R32_UINT, 0);
	dx11_il_bind(mesh->il, renderer->context);

	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, dx11_aprimitive(mesh->primitive));
	dx11_draw_indexed(renderer->context, mesh->count);
}

void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer) {
	UINT strides[] = {
		sizeof(float) * 5,
		sizeof(mat4)
	};

	UINT offsets[] = {
		0, 0
	};

	ID3D11Buffer* buffers[] = { mesh->vb, mesh->ivb };

	renderer->context->lpVtbl->IASetVertexBuffers(renderer->context, 0, 2, buffers, strides, offsets);

	dx11_ib_bind(mesh->ib, renderer->context, DXGI_FORMAT_R32_UINT, 0);
	dx11_il_bind(mesh->il, renderer->context);
	
	renderer->context->lpVtbl->IASetPrimitiveTopology(renderer->context, dx11_aprimitive(mesh->primitive));
	renderer->context->lpVtbl->DrawIndexedInstanced(renderer->context, mesh->count, 100, 0, 0, 0);
}

void amesh_set_count(AMesh* mesh, int count) {
	mesh->count = count;
}

void amesh_add_count(AMesh* mesh, int count) {
	mesh->count += count;
}
#endif