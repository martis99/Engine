#pragma once
#include "api/gfx/amesh.h"

typedef struct Mesh {
	AMesh* mesh;
} Mesh;

Mesh* mesh_create(Mesh* mesh);
void mesh_delete(Mesh* mesh);

void mesh_init_static(Mesh* mesh, const void* data, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void mesh_init_dynamic(Mesh* mesh, uint vertices_size, uint* indices, uint indices_size, ADataType* layout, uint layout_size, APrimitive primitive);
void mesh_add_instance_buffer_static(Mesh* mesh, const void* vertices, uint vertices_size, ADataType* layout, uint layout_size);
void mesh_add_instance_buffer_dynamic(Mesh* mesh, uint vertices_size, ADataType* layout, uint layout_size);
void mesh_set_vertices(Mesh* mesh, const void* vertices, uint vertices_size);
void mesh_set_instance_data(Mesh* mesh, const void* vertices, uint vertices_size);
void mesh_set_indices(Mesh* mesh, const void* indices, uint indices_size);
void mesh_draw_arrays(Mesh* mesh);
void mesh_draw_elements(Mesh* mesh);
void mesh_draw_arrays_instanced(Mesh* mesh, int count);
void mesh_draw_elements_instanced(Mesh* mesh, int count);

void mesh_set_count(Mesh* mesh, int count);
void mesh_add_count(Mesh* mesh, int count);

void mesh_init_quad(Mesh* mesh);
void mesh_init_cube(Mesh* mesh);