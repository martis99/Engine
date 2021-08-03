#pragma once
#include "structs.h"

Mesh* mesh_create(Mesh* mesh);
void mesh_delete(Mesh* mesh);

void mesh_init_static(Mesh* mesh, Renderer* renderer, Shader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, APrimitive primitive);
void mesh_init_dynamic(Mesh* mesh, Renderer* renderer, Shader* shader, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, APrimitive primitive);
void mesh_set_vertices(Mesh* mesh, Renderer* renderer, const void* vertices, uint vertices_size);
void mesh_set_instances(Mesh* mesh, Renderer* renderer, const void* instances, uint instances_size);
void mesh_set_indices(Mesh* mesh, Renderer* renderer, const void* indices, uint indices_size);
void mesh_draw_arrays(Mesh* mesh, Renderer* renderer);
void mesh_draw_elements(Mesh* mesh, Renderer* renderer);
void mesh_draw_arrays_instanced(Mesh* mesh, int count, Renderer* renderer);
void mesh_draw_elements_instanced(Mesh* mesh, int count, Renderer* renderer);

void mesh_set_count(Mesh* mesh, int count);
void mesh_add_count(Mesh* mesh, int count);

void mesh_init_quad(Mesh* mesh, Renderer* renderer, Shader* shader);
void mesh_init_cube(Mesh* mesh, Renderer* renderer, Shader* shader);