#include "pch.h"
#ifdef E_OPENGL
#include "api/gfx/amesh.h"

#include "gl/gl_buffer.h"

struct AMesh {
	GLuint va;
	GLuint vb;
	GLuint ib;
	GLsizei count;
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

void amesh_init_static(AMesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size) {
	mesh->va = gl_va_create();
	mesh->vb = gl_vb_create_static(mesh->va, vertices_size, vertices);
	mesh->ib = gl_ib_create_static(mesh->va, indices_size, indices);
	gl_va_layout_create(mesh->va, layout_size, layout);

	mesh->count = indices_size / sizeof(GLuint);
}

void amesh_draw(AMesh* mesh) {
	gl_va_draw_elements(mesh->va, mesh->ib, mesh->count);
}
#endif