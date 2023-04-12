#include "gfx_mesh.h"

#include "math/maths.h"

#include <float.h>

Mesh *mesh_create(Mesh *mesh, Renderer *renderer, Shader *shader, AMeshData data, APrimitive primitive)
{
	renderer->driver->mesh_create(mesh->mesh, renderer->renderer, shader->shader, shader->desc, data, primitive);
	if (mesh->mesh == NULL) {
		return NULL;
	}
	return mesh;
}

void mesh_delete(Mesh *mesh, Renderer *renderer)
{
	renderer->driver->mesh_delete(mesh->mesh, renderer->renderer);
}

void mesh_set_vertices(Mesh *mesh, Renderer *renderer, const void *vertices, uint vertices_size)
{
	renderer->driver->mesh_set_vertices(mesh->mesh, renderer->renderer, vertices, vertices_size);
}

void mesh_set_instances(Mesh *mesh, Renderer *renderer, const void *instances, uint instances_size)
{
	renderer->driver->mesh_set_instances(mesh->mesh, renderer->renderer, instances, instances_size);
}

void mesh_set_indices(Mesh *mesh, Renderer *renderer, const void *indices, uint indices_size)
{
	renderer->driver->mesh_set_indices(mesh->mesh, renderer->renderer, indices, indices_size);
}

void mesh_draw(Mesh *mesh, Renderer *renderer, uint indices)
{
	renderer->driver->mesh_draw(mesh->mesh, renderer->renderer, indices);
	renderer->draw_calls++;
}

Mesh *mesh_create_cube(Mesh *mesh, Renderer *renderer, Shader *shader)
{
	float F = -1;
	float B = 1;
	float L = -1;
	float R = 1;
	float U = 1;
	float D = -1;

	float TL = 0;
	float TR = 1;
	float TU = 1;
	float TD = 0;

	float CR = 1.0f;
	float CG = 0.5f;
	float CB = 0.2f;
	float CA = 1.0f;

	// clang-format off
	float vertices[] = {
		L, D, F,  0, 0, F,  TL, TD,  CR, CG, CB,
		L, U, F,  0, 0, F,  TL, TU,  CR, CG, CB,
		R, U, F,  0, 0, F,  TR, TU,  CR, CG, CB,
		R, D, F,  0, 0, F,  TR, TD,  CR, CG, CB,

		R, D, B,  0, 0, B,  TL, TD,  CR, CG, CB,
		R, U, B,  0, 0, B,  TL, TU,  CR, CG, CB,
		L, U, B,  0, 0, B,  TR, TU,  CR, CG, CB,
		L, D, B,  0, 0, B,  TR, TD,  CR, CG, CB,

		L, D, B,  L, 0, 0,  TL, TD,  CR, CG, CB,
		L, U, B,  L, 0, 0,  TL, TU,  CR, CG, CB,
		L, U, F,  L, 0, 0,  TR, TU,  CR, CG, CB,
		L, D, F,  L, 0, 0,  TR, TD,  CR, CG, CB,

		R, D, F,  R, 0, 0,  TL, TD,  CR, CG, CB,
		R, U, F,  R, 0, 0,  TL, TU,  CR, CG, CB,
		R, U, B,  R, 0, 0,  TR, TU,  CR, CG, CB,
		R, D, B,  R, 0, 0,  TR, TD,  CR, CG, CB,

		L, U, F,  0, U, 0,  TL, TD,  CR, CG, CB,
		L, U, B,  0, U, 0,  TL, TU,  CR, CG, CB,
		R, U, B,  0, U, 0,  TR, TU,  CR, CG, CB,
		R, U, F,  0, U, 0,  TR, TD,  CR, CG, CB,

		L, D, B,  0, D, 0,  TL, TD,  CR, CG, CB,
		L, D, F,  0, D, 0,  TL, TU,  CR, CG, CB,
		R, D, F,  0, D, 0,  TR, TU,  CR, CG, CB,
		R, D, B,  0, D, 0,  TR, TD,  CR, CG, CB,
	};

	uint indices[] = {
		 0,  1,  2,
		 0,  2,  3,

		 4,  5,  6,
		 4,  6,  7,

		 8,  9, 10,
		 8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,
	};
	// clang-format on

	AMeshData md = {
		.vertices.data = vertices,
		.vertices.size = sizeof(vertices),
		.indices.data  = indices,
		.indices.size  = sizeof(indices),
	};
	return mesh_create(mesh, renderer, shader, md, A_TRIANGLES);
}
