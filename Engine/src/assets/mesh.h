#pragma once
#include "api/gfx/amesh.h"

typedef struct Mesh {
	AMesh* mesh;
} Mesh;

Mesh* mesh_create(Mesh* mesh);
void mesh_delete(Mesh* mesh);

void mesh_init_static(Mesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size, APrimitive primitive);
void mesh_init_dynamic(Mesh* mesh, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size, APrimitive primitive);
void mesh_set_vertices(Mesh* mesh, float* vertices, uint vertices_size);
void mesh_set_indices(Mesh* mesh, uint* indices, uint indices_size);
void mesh_draw_arrays(Mesh* mesh);
void mesh_draw(Mesh* mesh);

void mesh_set_count(Mesh* mesh, int count);
void mesh_add_count(Mesh* mesh, int count);

void mesh_init_quad(Mesh* mesh);
void mesh_init_cube(Mesh* mesh);