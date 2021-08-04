#pragma once
#include "types.h"

Mesh* mesh_create(Mesh* mesh, Renderer* renderer, Shader* shader, AMeshDesc desc, APrimitive primitive);
void mesh_delete(Mesh* mesh);

void mesh_set_vertices(Mesh* mesh, Renderer* renderer, const void* vertices, uint vertices_size);
void mesh_set_instances(Mesh* mesh, Renderer* renderer, const void* instances, uint instances_size);
void mesh_set_indices(Mesh* mesh, Renderer* renderer, const void* indices, uint indices_size);

void mesh_draw(Mesh* mesh, Renderer* renderer, uint indices);

Mesh* mesh_create_cube(Mesh* mesh, Renderer* renderer, Shader* shader);