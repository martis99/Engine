#pragma once
#include "aenums.h"

typedef struct AMesh AMesh;

AMesh* amesh_create();
void amesh_delete(AMesh* mesh);

void amesh_init_static(AMesh* mesh, const void* vertices, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void amesh_init_dynamic(AMesh* mesh, uint vertices_size, const void* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void amesh_add_instance_buffer_static(AMesh* mesh, const void* vertices, uint vertices_size, ADataType* layout, uint layout_size);
void amesh_add_instance_buffer_dynamic(AMesh* mesh, uint vertices_size, ADataType* layout, uint layout_size);
void amesh_set_vertices(AMesh* mesh, const void* vertices, uint vertices_size);
void amesh_set_instance_data(AMesh* mesh, const void* vertices, uint vertices_size);
void amesh_set_indices(AMesh* mesh, const void* indices, uint indices_size);
void amesh_draw_arrays(AMesh* mesh);
void amesh_draw_arrays_instanced(AMesh* mesh, int count);
void amesh_draw_elements(AMesh* mesh);
void amesh_draw_elements_instanced(AMesh* mesh, int count);

void amesh_set_count(AMesh* mesh, int count);
void amesh_add_count(AMesh* mesh, int count);