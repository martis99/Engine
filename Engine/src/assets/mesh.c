#include "pch.h"
#include "mesh.h"

#include "app.h"

Mesh* mesh_create(Mesh* mesh) {
	mesh->mesh = amesh_create();
	return mesh;
}

void mesh_delete(Mesh* mesh) {
	amesh_delete(mesh->mesh);
}

void mesh_init_static(Mesh* mesh, Renderer* renderer, Shader* shader, const void* vertices, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, APrimitive primitive) {
	amesh_init_static(mesh->mesh, renderer->renderer, shader->shader, vertices, vertices_size, vertex_size, indices, indices_size, index_size, shader->layout, shader->layout_size, primitive);
}

void mesh_init_dynamic(Mesh* mesh, Renderer* renderer, Shader* shader, uint vertices_size, uint vertex_size, uint* indices, uint indices_size, uint index_size, APrimitive primitive) {
	amesh_init_dynamic(mesh->mesh, renderer->renderer, shader->shader, vertices_size, vertex_size, indices, indices_size, index_size, shader->layout, shader->layout_size, primitive);
}

void mesh_add_instance_buffer_static(Mesh* mesh, const void* vertices, uint vertices_size, AValue* values, uint values_size) {
	amesh_add_instance_buffer_static(mesh->mesh, vertices, vertices_size, values, values_size);
}

void mesh_add_instance_buffer_dynamic(Mesh* mesh, uint vertices_size, AValue* values, uint values_size) {
	amesh_add_instance_buffer_dynamic(mesh->mesh, vertices_size, values, values_size);
}

void mesh_set_vertices(Mesh* mesh, Renderer* renderer, const void* vertices, uint vertices_size) {
	amesh_set_vertices(mesh->mesh, renderer->renderer, vertices, vertices_size);
}

void mesh_set_instance_data(Mesh* mesh, const void* vertices, uint vertices_size) {
	amesh_set_instance_data(mesh->mesh, vertices, vertices_size);
}

void mesh_set_indices(Mesh* mesh, const void* indices, uint indices_size) {
	amesh_set_indices(mesh->mesh, indices, indices_size);
}

void mesh_draw_arrays(Mesh* mesh, Renderer* renderer) {
	amesh_draw_arrays(mesh->mesh, renderer->renderer);
	app.stats.draw_calls++;
}

void mesh_draw_arrays_instanced(Mesh* mesh, int count, Renderer* renderer) {
	amesh_draw_arrays_instanced(mesh->mesh, count, renderer->renderer);
	app.stats.draw_calls++;
}

void mesh_draw_elements(Mesh* mesh, Renderer* renderer) {
	amesh_draw_elements(mesh->mesh, renderer->renderer);
	app.stats.draw_calls++;
}

void mesh_draw_elements_instanced(Mesh* mesh, int count, Renderer* renderer) {
	amesh_draw_elements_instanced(mesh->mesh, count, renderer->renderer);
	app.stats.draw_calls++;
}

void mesh_set_count(Mesh* mesh, int count) {
	amesh_set_count(mesh->mesh, count);
}

void mesh_add_count(Mesh* mesh, int count) {
	amesh_add_count(mesh->mesh, count);
}

void mesh_init_quad(Mesh* mesh, Renderer* renderer, Shader* shader) {
	float vertices[] = {
		0.0f, 100.0f, 0.0f, 0.0f, 1.0f,
		100.0f, 100.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	uint indices[] = {
		0, 3, 1,
		3, 2, 1
	};

	mesh_init_static(mesh, renderer, shader, vertices, sizeof(vertices), 5 * sizeof(float), indices, sizeof(indices), sizeof(uint), A_TRIANGLES);
}

void mesh_init_cube(Mesh* mesh, Renderer* renderer, Shader* shader) {
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

	mesh_init_static(mesh, renderer, shader, vertices, sizeof(vertices), 5 * sizeof(float), indices, sizeof(indices), sizeof(uint), A_TRIANGLES);
}