#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/amesh.h"
#include "dx11_astructs.h"
#include "dx11/dx11_buffer.h"

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	dx11_vb_delete(mesh->vb);
	dx11_ib_delete(mesh->ib);
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

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
	mesh->vertex_size = vertex_size;

	mesh->vb = dx11_vb_create_static(renderer->device, vertices, vertices_size, vertex_size);
	mesh->ib = dx11_ib_create_static(renderer->device, indices, indices_size, index_size);
	mesh->il = dx11_il_create(renderer->device, layout, layout_size, shader->vs_blob->lpVtbl->GetBufferPointer(shader->vs_blob), shader->vs_blob->lpVtbl->GetBufferSize(shader->vs_blob));

	mesh->count = indices_size / index_size;
	return mesh;
}

AMesh* amesh_init_dynamic(AMesh* mesh, ARenderer* renderer, AShader* shader, uint vertices_size, const void* indices, uint indices_size, AValue* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
	return mesh;
}

void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, AValue* layout, uint layout_size) {

}

void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, AValue* layout, uint layout_size) {

}

void amesh_set_vertices(AMesh* mesh, const void* vertices, uint vertices_size) {

}

void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size) {

}

void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size) {

}

void amesh_draw_arrays(AMesh* mesh, ARenderer* renderer) {
	dx11_vb_bind(mesh->vb, renderer->context, mesh->vertex_size, 0);
	dx11_ib_bind(mesh->ib, renderer->context, DXGI_FORMAT_R32_UINT, 0);
	dx11_il_bind(mesh->il, renderer->context);
	dx11_draw(renderer->context, mesh->count);
}

void amesh_draw_arrays_instanced(AMesh* mesh, int count, ARenderer* renderer) {

}

void amesh_draw_elements(AMesh* mesh, ARenderer* renderer) {
	dx11_vb_bind(mesh->vb, renderer->context, mesh->vertex_size, 0);
	dx11_ib_bind(mesh->ib, renderer->context, DXGI_FORMAT_R32_UINT, 0);
	dx11_il_bind(mesh->il, renderer->context);
	dx11_draw_indexed(renderer->context, mesh->count);
}

void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer) {

}

void amesh_set_count(AMesh* mesh, int count) {

}

void amesh_add_count(AMesh* mesh, int count) {

}
#endif