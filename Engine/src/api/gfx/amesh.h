#pragma once
#include "api/astructs.h"
#include "api/aenums.h"

AMesh* amesh_create();
void amesh_delete(AMesh* mesh);

AMesh* amesh_init_static(AMesh* mesh, ARenderer* renderer, AShader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, AValue* layout, uint layout_size, APrimitive primitive);
AMesh* amesh_init_dynamic(AMesh* mesh, ARenderer* renderer, AShader* shader, uint vertices_size, const void* indices, uint indices_size, AValue* layout, uint layout_size, APrimitive primitive);
void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, AValue* layout, uint layout_size);
void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, AValue* layout, uint layout_size);
void amesh_set_vertices(AMesh* mesh, const void* vertices, uint vertices_size);
void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size);
void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size);
void amesh_draw_arrays(AMesh* mesh, ARenderer* renderer);
void amesh_draw_arrays_instanced(AMesh* mesh, int count, ARenderer* renderer);
void amesh_draw_elements(AMesh* mesh, ARenderer* renderer);
void amesh_draw_elements_instanced(AMesh* mesh, int count, ARenderer* renderer);

void amesh_set_count(AMesh* mesh, int count);
void amesh_add_count(AMesh* mesh, int count);