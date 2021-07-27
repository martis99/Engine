#include "pch.h"
#ifdef GAPI_OPENGL
#include "api/gfx/amesh.h"

#include "gl/gl_buffer.h"
#include "gl/gl_enums.h"

struct AMesh {
	GLuint va;
	GLuint vb;
	GLuint ivb;
	GLuint ib;
	GLsizei count;

	APrimitive primitive;

	GLuint layout_index;
};

AMesh* amesh_create() {
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

void amesh_init_static(AMesh* mesh, const void* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create();
	mesh->ib = gl_ib_create();

	mesh->layout_index = 0;
	gl_vb_init_static(mesh->vb, mesh->va, vertices, vertices_size, layout, layout_size, &mesh->layout_index);
	gl_ib_init_static(mesh->ib, indices, indices_size);

	mesh->count = indices_size / sizeof(GLuint);
}

void amesh_init_dynamic(AMesh* mesh, uint vertices_size, const void* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create();
	mesh->ib = gl_ib_create();

	mesh->layout_index = 0;
	gl_vb_init_dynamic(mesh->vb, mesh->va, vertices_size, layout, layout_size, &mesh->layout_index);
	gl_ib_init_static(mesh->ib, indices, indices_size);

	mesh->count = vertices_size / sizeof(GLfloat);
}

void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, ADataType* layout, uint layout_size) {
	mesh->ivb = gl_vb_create();
	gl_vb_init_static(mesh->ivb, mesh->va, vertices, vertices_size, layout, layout_size, &mesh->layout_index);
}

void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, ADataType* layout, uint layout_size) {
	mesh->ivb = gl_vb_create();
	gl_vb_init_dynamic(mesh->ivb, mesh->va, vertices_size, layout, layout_size, &mesh->layout_index);
}

void amesh_set_vertices(AMesh* mesh, const void* vertices, uint vertices_size) {
	gl_vb_set_data(mesh->vb, vertices, vertices_size);
}

void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size) {
	gl_vb_set_data(mesh->ivb, vertices, vertices_size);
}

void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size) {
	gl_ib_set_data(mesh->ib, indices, indices_size);
}

void amesh_draw_arrays(AMesh* mesh) {
	gl_va_draw_arrays(mesh->va, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_draw_arrays_instanced(AMesh* mesh, int count) {
	gl_va_draw_arrays_instanced(mesh->va, gl_aprimitive(mesh->primitive), mesh->count, count);
}

void amesh_draw_elements(AMesh* mesh) {
	gl_va_draw_elements(mesh->va, mesh->ib, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_draw_elements_instanced(AMesh* mesh, int count) {
	gl_va_draw_elements_instanced(mesh->va, mesh->ib, gl_aprimitive(mesh->primitive), mesh->count, count);
}

void amesh_set_count(AMesh* mesh, int count) {
	mesh->count = count;
}

void amesh_add_count(AMesh* mesh, int count) {
	mesh->count += count;
}
#endif