#include "pch.h"
#ifdef GAPI_DX11
#include "api/gfx/amesh.h"

struct AMesh {
	APrimitive primitive;
};

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	m_free(mesh, sizeof(AMesh));
}

void amesh_init_static(AMesh* mesh, const void* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
}

void amesh_init_dynamic(AMesh* mesh, uint vertices_size, const void* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
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

void amesh_draw_arrays(AMesh* mesh) {

}

void amesh_draw_arrays_instanced(AMesh* mesh, int count) {

}

void amesh_draw_elements(AMesh* mesh) {

}

void amesh_draw_elements_instanced(AMesh* mesh, int count) {

}

void amesh_set_count(AMesh* mesh, int count) {

}

void amesh_add_count(AMesh* mesh, int count) {

}
#endif