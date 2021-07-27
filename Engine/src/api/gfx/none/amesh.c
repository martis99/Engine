#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/amesh.h"
#include "n_astructs.h"

AMesh* amesh_create() {
	AMesh* mesh = m_malloc(sizeof(AMesh));
	return mesh;
}

void amesh_delete(AMesh* mesh) {
	m_free(mesh, sizeof(AMesh));
}

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive) {
	mesh->primitive = primitive;
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

}

void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer) {

}

void amesh_set_count(AMesh* mesh, int count) {

}

void amesh_add_count(AMesh* mesh, int count) {

}
#endif