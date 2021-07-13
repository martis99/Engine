#pragma once
#include "aenums.h"

typedef struct AMesh AMesh;

AMesh* amesh_create();
void amesh_delete(AMesh* mesh);

void amesh_init_static(AMesh* mesh, const void* data, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void amesh_init_dynamic(AMesh* mesh, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void amesh_set_vertices(AMesh* mesh, const void* data, uint vertices_size);
void amesh_set_indices(AMesh* mesh, uint* indices, uint indices_size);
void amesh_draw_arrays(AMesh* mesh);
void amesh_draw(AMesh* mesh);

void amesh_set_count(AMesh* mesh, int count);
void amesh_add_count(AMesh* mesh, int count);