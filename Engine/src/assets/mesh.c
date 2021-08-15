#include "pch.h"
#include "mesh.h"
#include "api/gfx/amesh.h"

#include "app.h"

Mesh* mesh_create(Mesh* mesh, Renderer* renderer, Shader* shader, AMeshData data, APrimitive primitive) {
	mesh->mesh = amesh_create(renderer->renderer, shader->shader, shader->desc, data, primitive);
	if (mesh->mesh == NULL) {
		return NULL;
	}
	return mesh;
}

void mesh_delete(Mesh* mesh) {
	amesh_delete(mesh->mesh);
}

void mesh_set_vertices(Mesh* mesh, Renderer* renderer, const void* vertices, uint vertices_size) {
	amesh_set_vertices(mesh->mesh, renderer->renderer, vertices, vertices_size);
}

void mesh_set_instances(Mesh* mesh, Renderer* renderer, const void* instances, uint instances_size) {
	amesh_set_instances(mesh->mesh, renderer->renderer, instances, instances_size);
}

void mesh_set_indices(Mesh* mesh, Renderer* renderer, const void* indices, uint indices_size) {
	amesh_set_indices(mesh->mesh, renderer->renderer, indices, indices_size);
}

void mesh_draw(Mesh* mesh, Renderer* renderer, uint indices) {
	amesh_draw(mesh->mesh, renderer->renderer, indices);
	app_get_stats()->draw_calls++;
}

Mesh* mesh_create_cube(Mesh* mesh, Renderer* renderer, Shader* shader) {
	float vertices[] = {
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		 0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	};

	uint indices[] = {
		 0,  1,  3,
		 3,  1,  2,
		 4,  5,  7,
		 7,  5,  6,
		 8,  9, 11,
		11,  9, 10,
		12, 13, 15,
		15, 13, 14,
		16, 17, 19,
		19, 17, 18,
		20, 21, 23,
		23, 21, 22
	};

	AMeshData md = { 0 };
	md.vertices.data = vertices;
	md.vertices.size = sizeof(vertices);
	md.indices.data = indices;
	md.indices.size = sizeof(indices);
	return mesh_create(mesh, renderer, shader, md, A_TRIANGLES);
}