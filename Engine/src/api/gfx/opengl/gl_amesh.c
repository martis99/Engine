#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amesh.h"
#include "gl_astructs.h"

#include "gl/gl_buffer.h"
#include "gl/gl_enums.h"

AMesh* amesh_create(ARenderer* renderer) {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	gl_ib_delete(mesh->ib);
	gl_vb_delete(mesh->vb);
	gl_vb_delete(mesh->ivb);
	gl_va_delete(mesh->va);
	m_free(mesh, sizeof(AMesh));
}

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create();
	mesh->ib = gl_ib_create();

	mesh->layout_index = 0;
	gl_vb_init_static(mesh->vb, mesh->va, vertices, vertices_size, layout, layout_size, &mesh->layout_index);
	gl_ib_init_static(mesh->ib, indices, indices_size);

	mesh->count = indices_size / sizeof(GLuint);

	return mesh;
}

AMesh* amesh_init_dynamic(AMesh* mesh, ARenderer* renderer, AShader* shader, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create();
	mesh->ib = gl_ib_create();

	mesh->layout_index = 0;
	gl_vb_init_dynamic(mesh->vb, mesh->va, vertices_size, layout, layout_size, &mesh->layout_index);
	gl_ib_init_static(mesh->ib, indices, indices_size);

	mesh->count = vertices_size / sizeof(GLfloat);

	return mesh;
}

void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, AValue* layout, uint layout_size) {
	mesh->ivb = gl_vb_create();
	gl_vb_init_static(mesh->ivb, mesh->va, vertices, vertices_size, layout, layout_size, &mesh->layout_index);
}

void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, AValue* layout, uint layout_size) {
	mesh->ivb = gl_vb_create();
	gl_vb_init_dynamic(mesh->ivb, mesh->va, vertices_size, layout, layout_size, &mesh->layout_index);
}

void amesh_set_vertices(AMesh* mesh, ARenderer* renderer, const void* vertices, uint vertices_size) {
	gl_vb_set_data(mesh->vb, vertices, vertices_size);
}

void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size) {
	gl_vb_set_data(mesh->ivb, vertices, vertices_size);
}

void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size) {
	gl_ib_set_data(mesh->ib, indices, indices_size);
}

void amesh_draw_arrays(AMesh* mesh, ARenderer* renderer) {
	gl_va_draw_arrays(mesh->va, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_draw_arrays_instanced(AMesh* mesh, int count, ARenderer* renderer) {
	gl_va_draw_arrays_instanced(mesh->va, gl_aprimitive(mesh->primitive), mesh->count, count);
}

void amesh_draw_elements(AMesh* mesh, ARenderer* renderer) {
	gl_va_draw_elements(mesh->va, mesh->ib, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer) {
	gl_va_draw_elements_instanced(mesh->va, mesh->ib, gl_aprimitive(mesh->primitive), mesh->count, count);
}

void amesh_set_count(AMesh* mesh, int count) {
	mesh->count = count;
}

void amesh_add_count(AMesh* mesh, int count) {
	mesh->count += count;
}
#endif