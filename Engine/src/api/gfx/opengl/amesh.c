#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/amesh.h"

#include "gl/gl_buffer.h"
#include "gl/gl_enums.h"

struct AMesh {
	GLuint va;
	GLuint vb;
	GLuint ib;
	GLsizei count;

	APrimitive primitive;
};

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	gl_ib_delete(mesh->ib);
	gl_vb_delete(mesh->vb);
	gl_va_delete(mesh->va);
	m_free(mesh, sizeof(AMesh));
}

void amesh_init_static(AMesh* mesh, const void* data, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create_static(mesh->va, vertices_size, data);
	mesh->ib = gl_ib_create_static(mesh->va, indices_size, indices);
	gl_va_layout_create(mesh->va, layout_size, layout);

	mesh->count = indices_size / sizeof(GLuint);
}

void amesh_init_dynamic(AMesh* mesh, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;

	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create_dynamic(mesh->va, vertices_size);
	mesh->ib = gl_ib_create_static(mesh->va, indices_size, indices);
	gl_va_layout_create(mesh->va, layout_size, layout);

	mesh->count = vertices_size / sizeof(GLfloat);
}

void amesh_set_vertices(AMesh* mesh, const void* data, uint vertices_size) {
	gl_vb_set_data(mesh->va, mesh->vb, vertices_size, data);
}

void amesh_set_indices(AMesh* mesh, uint* indices, uint indices_size) {
	gl_ib_set_data(mesh->va, mesh->ib, indices_size, indices);
}

void amesh_draw_arrays(AMesh* mesh) {
	gl_va_draw_arrays(mesh->va, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_draw(AMesh* mesh) {
	gl_va_draw_elements(mesh->va, mesh->ib, gl_aprimitive(mesh->primitive), mesh->count);
}

void amesh_set_count(AMesh* mesh, int count) {
	mesh->count = count;
}

void amesh_add_count(AMesh* mesh, int count) {
	mesh->count += count;
}
#endif