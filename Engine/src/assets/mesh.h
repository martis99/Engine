#pragma once
#include "api/gfx/amesh.h"

typedef struct Mesh {
	AMesh* mesh;
} Mesh;

Mesh* mesh_create(Mesh* mesh);
void mesh_delete(Mesh* mesh);

void mesh_init_static(Mesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size);
void mesh_draw(Mesh* mesh);

void mesh_init_quad(Mesh* mesh);